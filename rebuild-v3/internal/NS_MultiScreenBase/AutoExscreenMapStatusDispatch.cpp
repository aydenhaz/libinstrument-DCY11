#include "AutoExscreenMapStatusDispatch.h"
#include "AutoExscreenMapStatusPlan.h"
#include "AutoExscreenMapTimer.h"
#include "AutoMultiScreenConfigBinding.h"
#include <hsl/HSL.h>
#include <cstdint>

namespace NS_MultiScreenBase {

bool AutoExscreenUtil_ApplyMapStatus(void* instance, ScreenType screen,
                                     void* timerState, int mapEngineId,
                                     int requestedLevel, int requestedMode) {
    hsl::MapView* view = hsl::HSL::GetMapViewHandle(
        static_cast<bl::MapEngineID>(mapEngineId));
    if (!view) return false;

    AutoExscreenUtil_ApplyMapStatusWithView(
        instance, screen, timerState, view, requestedLevel, requestedMode);
    return true;
}

void AutoExscreenUtil_ApplyMapStatusWithView(void* instance, ScreenType screen,
                                              void* timerState, hsl::MapView* view,
                                              int requestedLevel, int requestedMode) {
    const int scene = *AutoExscreenUtil_GetCurrentSceneSlot(screen);
    int configuredLevel, configuredMode;
    AutoMultiScreenConfig_SnapshotMapState(
        instance, scene, &configuredLevel, &configuredMode);
    const MapStatusPlan plan = AutoExscreenUtil_PlanMapStatus(
        configuredLevel, requestedLevel, requestedMode);

    if (plan.writeLevel)
        AutoExscreenUtil_SetMapViewLevelToUiConfig(screen, requestedLevel);
    if (plan.setMapLevel)
        AutoMultiScreenConfig_SetMapLevel(instance, configuredLevel,
                                           requestedLevel);
    if (plan.writeMode)
        AutoExscreenUtil_SetMapViewModeToUiConfig(screen, requestedMode);

    if (plan.disableDynamicLevel) {
        typedef void (*OpenDynamicLevel)(void*, bool, int);
        void** vtable = *reinterpret_cast<void***>(view);
        reinterpret_cast<OpenDynamicLevel>(vtable[0x3e8 / 4])(
            view, false, 0);
        AutoExscreenUtil_TryStartMapStatusTimer(timerState);
    }
    if (plan.setMapViewMode)
        AutoMultiScreenConfig_SetMapViewMode(instance, configuredMode,
                                             requestedMode);
    if (plan.resetMapBindingList)
        AutoMultiScreenConfig_ResetMapBindingList(instance, scene);
}

}
