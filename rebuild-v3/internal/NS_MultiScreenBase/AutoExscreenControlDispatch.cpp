#include "Observers.h"
#include "AutoExscreenControlStatus.h"
#include "AutoExscreenUiConfig.h"
#include "AutoExscreenMapStyle.h"
#include "AutoMultiScreenConfigBinding.h"
#include <NS_MultiScreenBase/AutoConfigScreenMode.h>
#include <NS_MultiScreenBase/AutoMultiScreenConfig.h>
#include <hsl/GOTHERProtocolHmiToAl.h>
#include <hsl/HSL.h>
#include <hsl/MultiScreenService.h>
#include <cstdint>
#include <cstring>
#include <string>

namespace NS_MultiScreenBase {
namespace {
int32_t readWord(const void* address) {
    int32_t value;
    std::memcpy(&value, address, sizeof(value));
    return value;
}
void notifyCollection(ObserverCollectionAbi& collection, unsigned slot,
                      ScreenType screen, bool enabled) {
    void** mutexVtable = *reinterpret_cast<void***>(&collection);
    typedef void (*MutexOp)(void*);
    reinterpret_cast<MutexOp>(mutexVtable[2])(&collection);
    typedef void (*ObserverOp)(void*, ScreenType, bool);
    for (uint32_t index = 0; index < collection.count; ++index) {
        void* observer = collection.observers[index];
        void** methods = *reinterpret_cast<void***>(observer);
        reinterpret_cast<ObserverOp>(methods[slot / 4])(observer, screen,
                                                         enabled);
    }
    reinterpret_cast<MutexOp>(mutexVtable[3])(&collection);
}
float zoomLevel(void* view) {
    typedef float (*GetZoomLevel)(void*);
    return reinterpret_cast<GetZoomLevel>(
        (*reinterpret_cast<void***>(view))[0x198 / 4])(view);
}

// V21 0x2b3ac..0x2b86c. The scene's two style names are copied at the
// callback entry, before any configuration writes. The reference view is
// required even for the explicit 1/2 selectors in the original.
void applyMapStyle(AutoMultiScreenConfig* config, int selector,
                   const std::string& dayName,
                   const std::string& nightName) {
    if (selector != 0 && selector != 1 && selector != 2) return;
    void* own = AutoMultiScreenConfig_GetMapViewHandle(config);
    if (!own) return;
    void* reference = hsl::HSL::GetMapViewHandle(static_cast<bl::MapEngineID>(1));
    if (!reference) return;
    AutoExscreenUtil_ApplyMapStyleWithViews(
        own, reference, selector, dayName, nightName);
}
}

// V21 0x2ace0: 36-byte control record. Diagnostic logging is omitted; its
// gates and all status/UI dispatches are preserved as separate operations.
void AutoExscreenNotifyObserver::OnSetExScreenMapStatusUi(
    const tagSTExtScreenControl& control) {
    hsl::MultiScreenService* service = hsl::MultiScreenService::GetInstance();
    const int engine = static_cast<int>(
        service->GetMapEngineId(control.words[kExtScreenSelector]));
    const int currentMode = AutoConfigScreenMode::GetInstance()->
        GetCurrentScreenMode(screenType_);
    const int scene = *AutoExscreenUtil_GetCurrentSceneSlot(screenType_);
    hsl::MapView* view = hsl::HSL::GetMapViewHandle(
        static_cast<bl::MapEngineID>(engine));
    if (!view) return;

    const int observerEngine = readWord(
        reinterpret_cast<const unsigned char*>(this) + 0xe0);
    if (engine != observerEngine ||
        (control.words[kExtScreenModeCondition] != currentMode + 1 &&
         control.words[kExtScreenModeCondition] != 99) ||
        (engine != 3 && engine != 5 && engine != 7)) return;

    AutoMultiScreenConfig* config = AutoMultiScreenConfig::GetInstance(screenType_);
    const unsigned sceneIndex = scene == 2 ? 2 : scene == 1 ? 1 : 0;
    const unsigned char* record = reinterpret_cast<const unsigned char*>(config) +
                                  sceneIndex * 0xe4;
    const int fallbackLevel = readWord(record + 0x88);
    const int fallbackMode = readWord(record + 0x8c);
    const std::string dayName = *reinterpret_cast<const std::string*>(record + 0x54);
    const std::string nightName = *reinterpret_cast<const std::string*>(record + 0x60);
    const std::string thirdName = *reinterpret_cast<const std::string*>(record + 0x6c);
    (void)thirdName;

    if (control.words[kExtRequestedViewMode] != 99) {
        const int requested = control.words[kExtRequestedViewMode];
        AutoExscreenUtil_SetMapViewModeToUiConfig(screenType_, requested);
        config->SetMapViewMode(fallbackMode, requested);
    }
    const int requestedLevel = control.words[kExtRequestedLevel];
    if (requestedLevel != 99) {
        int writtenLevel = requestedLevel;
        bool apply = true;
        if (requestedLevel == -2 || requestedLevel == -3) {
            const int currentLevel = static_cast<int>(zoomLevel(view));
            apply = requestedLevel == -2 ? currentLevel <= 18 : currentLevel >= 4;
            writtenLevel = currentLevel + (requestedLevel == -2 ? 1 : -1);
        }
        if (apply) {
            AutoExscreenUtil_SetMapViewLevelToUiConfig(screenType_, writtenLevel);
            config->SetMapLevel(fallbackLevel, requestedLevel);
        }
    }
    if (control.words[kExtCrossControl] != 99)
        OnSetExternalScreenCrossControlUi(engine,
                                          control.words[kExtCrossControl] == 1);

    const int eagleEye = control.words[kExtEagleEye];
    if (eagleEye == 0 || eagleEye == 1) {
        AutoExscreenUtil_SetEagleEyeEnabledToUiConfig(screenType_, eagleEye == 1);
        notifyCollection(renderObservers_, 8, screenType_, eagleEye == 1);
    }
    const int preview = control.words[kExtPreview];
    if (preview == 0 || preview == 1) {
        const int mode = AutoConfigScreenMode::GetInstance()->
            GetCurrentScreenMode(screenType_);
        for (int selectedScene = 0; selectedScene < 3; ++selectedScene)
            AutoExscreenUtil_SetMapParamsToUiConfig(
                screenType_, mode, selectedScene, 4, preview);
        notifyCollection(renderObservers_, 4, screenType_, preview == 1);
    }
    config->ResetMapBindingList(scene);
    if (control.words[kExtViewStateRequest] != 99)
        applyMapStyle(config, control.words[kExtViewStateRequest],
                      dayName, nightName);

    tagSTExtScreenControl status = control;
    AutoExscreenUtil_ProvisionalControlStatus(status);
    AutoExscreenUtil_FillControlStatus(screenType_, observerEngine, status);
    hsl::GOTHERProtocolHmiToAl::MultiScreenStatusHmiToAl(status);
}

}
