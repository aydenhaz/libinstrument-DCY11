#include "AutoExscreenControlStatus.h"
#include "AutoExscreenUiConfig.h"
#include <NS_MultiScreenBase/AutoConfigScreenMode.h>
#include <NS_MultiScreenBase/AutoMultiScreenConfig.h>
#include <hsl/HSL.h>
#include <hsl/MultiScreenService.h>
#include <cstdint>
#include <cstring>

namespace NS_MultiScreenBase {
namespace {
int32_t readWord(const unsigned char* at) {
    int32_t result;
    std::memcpy(&result, at, sizeof(result));
    return result;
}
int normalizedPreview(ScreenType screen) {
    const int mode = AutoConfigScreenMode::GetInstance()->GetCurrentScreenMode(screen);
    const int scene = *AutoExscreenUtil_GetCurrentSceneSlot(screen);
    const int preview = AutoExscreenUtil_GetMapParamsFromUiConfig(
        screen, mode, scene, 4);
    return preview == 0 || preview == 1 ? preview : 99;
}
}

void AutoExscreenUtil_ProvisionalControlStatus(tagSTExtScreenControl& status) {
    hsl::MapView* reference = hsl::HSL::GetMapViewHandle(
        static_cast<bl::MapEngineID>(1));
    if (reference) {
        typedef float (*GetZoomLevel)(void*);
        void** vtable = *reinterpret_cast<void***>(reference);
        const int level = static_cast<int>(
            reinterpret_cast<GetZoomLevel>(vtable[0x198 / 4])(reference));
        switch (status.words[2]) {
            case -3: if (level >= 4) status.words[2] = level - 1; break;
            case -2: if (level <= 18) status.words[2] = level + 1; break;
            case 0: status.words[2] = level; break;
        }
    }
    reference = hsl::HSL::GetMapViewHandle(static_cast<bl::MapEngineID>(1));
    if (!reference || (status.words[1] != 3 && status.words[1] != 99)) return;
    typedef int (*GetMapMode)(void*);
    void** vtable = *reinterpret_cast<void***>(reference);
    const int mode = reinterpret_cast<GetMapMode>(vtable[0x98 / 4])(reference);
    status.words[1] = status.words[1] == 3 ? mode :
                      static_cast<unsigned>(mode) < 3u ? 2 - mode : 99;
}

void AutoExscreenUtil_FillControlStatus(ScreenType screen, int mapEngineId,
                                        tagSTExtScreenControl& status) {
    hsl::MapView* view = hsl::HSL::GetMapViewHandle(
        static_cast<bl::MapEngineID>(mapEngineId));
    if (!view) return;

    status.words[0] = hsl::MultiScreenService::GetInstance()->GetWindowId(
        static_cast<bl::MapEngineID>(mapEngineId));
    AutoMultiScreenConfig* config = AutoMultiScreenConfig::GetInstance(screen);
    const unsigned char* bytes = reinterpret_cast<const unsigned char*>(config);
    status.words[1] = readWord(bytes + 0x314);
    typedef float (*GetZoomLevel)(void*);
    void** vtable = *reinterpret_cast<void***>(view);
    status.words[2] = static_cast<int>(
        reinterpret_cast<GetZoomLevel>(vtable[0x198 / 4])(view));

    const ScreenType instanceScreen = static_cast<ScreenType>(
        readWord(bytes + 0x30c));
    const int scene = *AutoExscreenUtil_GetCurrentSceneSlot(instanceScreen);
    const int internalScene = readWord(bytes + 0x310);
    const unsigned sceneIndex = internalScene == 2 ? 2 : internalScene == 1 ? 1 : 0;
    const unsigned char* record = bytes + sceneIndex * 0xe4;
    if (readWord(record + 0x9c)) {
        const int mode = AutoConfigScreenMode::GetInstance()->GetCurrentScreenMode(
            instanceScreen);
        status.words[3] = AutoExscreenUtil_GetMapParamsFromUiConfig(
            instanceScreen, mode, scene, 2) != 0;
    } else {
        status.words[3] = 0;
    }

    const int mode = AutoConfigScreenMode::GetInstance()->GetCurrentScreenMode(
        instanceScreen);
    const int eagleEye = AutoExscreenUtil_GetMapParamsFromUiConfig(
        instanceScreen, mode, *AutoExscreenUtil_GetCurrentSceneSlot(instanceScreen), 3);
    status.words[4] = eagleEye == 99 ? record[0xc7] != 0 : eagleEye == 1;
    status.words[5] = normalizedPreview(instanceScreen);
    status.words[6] = AutoConfigScreenMode::GetInstance()->GetCurrentScreenMode(
        screen) + 1;
}

}
