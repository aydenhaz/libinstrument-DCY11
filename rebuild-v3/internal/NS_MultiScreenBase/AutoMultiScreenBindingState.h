#pragma once
#include <cstdint>
#include <cstring>

namespace NS_MultiScreenBase {

// V21 AutoMultiScreenConfig holds three 0xe4-byte scene records at +0,
// +0xe4, +0x1c8 and three 0x20-byte binding records at +0x2ac, +0x2cc,
// +0x2ec. The semantics of every binding word are not yet established;
// retain their original offsets rather than giving uncertain field names.
struct AutoMapBindingState {
    int32_t words[8];
};
static_assert(sizeof(AutoMapBindingState) == 0x20, "V21 map binding record");

inline int32_t AutoBindingSceneWord(const unsigned char* scene, unsigned offset) {
    int32_t value;
    std::memcpy(&value, scene + offset, sizeof(value));
    return value;
}

// Scalar state transition from V21 sub_1e790. The caller supplies the
// current UI fields 0/1/5 and the effective OnlyShowCrossImage override.
// It preserves word 0 (engine selector) and word 7 (binding status); the
// original updates word 7 only after successful map-service registration.
inline AutoMapBindingState AutoExscreenUtil_ResolveMapBindingState(
    const unsigned char* scene, int uiLevel, int uiMode, int uiDayNight,
    int onlyCrossImage, AutoMapBindingState state) {
    const int32_t jsonLevel = AutoBindingSceneWord(scene, 0x88);
    const int32_t jsonViewMode = AutoBindingSceneWord(scene, 0x8c);
    const int32_t jsonCrossMode = AutoBindingSceneWord(scene, 0x90);
    const int32_t jsonDayNight = AutoBindingSceneWord(scene, 0x4c);
    const int32_t jsonPreview = AutoBindingSceneWord(scene, 0x50);

    int32_t dayNight = uiDayNight == 99 ? jsonDayNight : uiDayNight;
    if (static_cast<uint32_t>(dayNight) >= 3) dayNight = 1;
    state.words[5] = dayNight;
    state.words[3] = static_cast<uint32_t>(jsonCrossMode) >= 2 ? 2 : jsonCrossMode;

    state.words[2] = -1;
    if (uiLevel != 0 && (uiLevel != 99 || jsonLevel != 0)) {
        int32_t level = jsonLevel;
        if (static_cast<uint32_t>(level) <= 3) level = 3;
        if (static_cast<uint32_t>(level) >= 19) level = 19;
        state.words[2] = level;
    } else if (onlyCrossImage == 0) {
        state.words[2] = 0;
    }

    state.words[1] = -1;
    const int32_t requestedMode = uiMode == 99 ? jsonViewMode :
                                  jsonViewMode == 3 ? uiMode : jsonViewMode;
    if (requestedMode >= 0 && requestedMode <= 2) {
        state.words[1] = requestedMode;
    } else if (requestedMode == 3 && onlyCrossImage == 0 &&
               uiLevel == -1 && jsonLevel != -1) {
        state.words[1] = 3;
    } else if (requestedMode != 3) {
        state.words[1] = 0;
    }

    state.words[6] = jsonPreview == 1 || jsonPreview == 2 ? jsonPreview :
                     jsonPreview == 0 ? onlyCrossImage : 1;
    state.words[4] = scene[0x99] ? 1 : 2;
    return state;
}

} // namespace NS_MultiScreenBase
