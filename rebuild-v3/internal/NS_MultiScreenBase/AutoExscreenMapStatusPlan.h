#pragma once
#include "AutoExscreenUiConfig.h"

namespace NS_MultiScreenBase {

// Control flow of V21 sub_2c070 after GetMapViewHandle succeeds. The caller
// must gate this plan on the map-view handle: the original returns immediately
// when that handle is null. The configuration snapshot is copied before use.
struct MapStatusPlan {
    bool writeLevel;
    bool setMapLevel;
    bool writeMode;
    // Calls MapView::OpenDynamicLevel(false, bl::DynamicLevelType(0)).
    bool disableDynamicLevel;
    bool attemptTimerStart;
    bool setMapViewMode;
    bool resetMapBindingList;
};

inline MapStatusPlan AutoExscreenUtil_PlanMapStatus(
    int configuredLevel, int requestedLevel, int requestedMode) {
    const bool level = requestedLevel != 99;
    const bool mode = requestedMode != 99;
    const bool changeMode = mode && requestedLevel != -1;
    // 0x2c114 invokes the predicate even when either requested field is 99.
    const bool requestedDynamic = AutoExscreenUtil_IsDynamicLevelRequest(
        configuredLevel, requestedLevel);
    const bool dynamic = changeMode && requestedDynamic;
    const MapStatusPlan result = {
        level, level, mode, dynamic, dynamic, changeMode, true
    };
    return result;
}

} // namespace NS_MultiScreenBase
