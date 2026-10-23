#include "AutoMultiScreenConfigBinding.h"
#include "AutoMultiScreenBindingState.h"
#include "AutoExscreenUiConfig.h"
#include <hsl/HSL.h>
#include <NS_MultiScreenBase/AutoMultiScreenConfig.h>
#include <cstdint>
#include <cstring>
#include <initializer_list>

namespace NS_MultiScreenBase {
namespace {
// The V21 host vtables consist of four-byte ARM32 function pointers. Keep
// their logical indices separate in 64-bit host contract tests as well.
const unsigned armVtableWord = 4;
int32_t wordAt(const unsigned char* object, unsigned offset) {
    int32_t value;
    std::memcpy(&value, object + offset, sizeof(value));
    return value;
}
void putWord(unsigned char* object, unsigned offset, uint32_t value) {
    std::memcpy(object + offset, &value, sizeof(value));
}
int engineForScreen(int screen) {
    return screen == 0 ? 5 : screen == 1 ? 3 : screen == 2 ? 7 : -1;
}
}

void AutoMultiScreenConfig_InitSceneRecord(void* record) {
    unsigned char* bytes = static_cast<unsigned char*>(record);
    // sub_1c210 writes only these ranges. Its untouched bytes are preserved.
    bytes[0] = 0;
    std::memset(bytes + 4, 0xff, 0x20);
    std::memset(bytes + 0x24, 0, 2);
    bytes[0x26] = 1;
    for (unsigned offset : {0x28u, 0x2cu, 0x30u, 0x34u})
        putWord(bytes, offset, UINT32_MAX);
    putWord(bytes, 0x38, 6);
    putWord(bytes, 0x3c, 240);
    for (unsigned offset : {0x40u, 0x44u, 0x48u})
        putWord(bytes, offset, 0xbf800000u); // -1.0f
    std::memset(bytes + 0x4c, 0, 8);
    for (unsigned offset : {0x54u, 0x60u, 0x6cu})
        std::memset(bytes + offset, 0, 12);
    putWord(bytes, 0x78, 0x3f3851ecu); // 0.72f
    putWord(bytes, 0x7c, 0xbf800000u);
    putWord(bytes, 0x80, 0x3f000000u); // 0.5f
    std::memset(bytes + 0x84, 0, 0x11);
    bytes[0x95] = bytes[0x96] = 1;
    std::memset(bytes + 0x97, 0, 5);
    putWord(bytes, 0x9c, 7);
    bytes[0xa0] = 0;
    std::memset(bytes + 0xa4, 0, 0x25);
    std::memset(bytes + 0xcc, 0, 8);
    putWord(bytes, 0xd4, 0xbf800000u);
    std::memset(bytes + 0xd8, 0, 2);
    bytes[0xda] = 0;
    putWord(bytes, 0xdc, 0);
}

void AutoMultiScreenConfig_InitBindingRecord(void* record, int screen) {
    unsigned char* bytes = static_cast<unsigned char*>(record);
    // sub_3bec0 clears 25 bytes, then sets the mapped engine at word zero.
    std::memset(bytes + 4, 0, 0x19);
    putWord(bytes, 0, static_cast<uint32_t>(engineForScreen(screen)));
}

hsl::MapView* AutoMultiScreenConfig_GetMapViewHandle(const void* instance) {
    const unsigned char* object = static_cast<const unsigned char*>(instance);
    const int screen = wordAt(object, 0x30c);
    // sub_16788 maps Large/Small/Widget to 5/3/7; sub_1ddd8 rejects
    // unexpected engine IDs before calling HSL::GetMapViewHandle.
    const int engine = engineForScreen(screen);
    if (engine < 0) return nullptr;
    return hsl::HSL::GetMapViewHandle(static_cast<bl::MapEngineID>(engine));
}

void AutoMultiScreenConfig_ResetMapBindingList(void* instance, int scene) {
    unsigned char* object = static_cast<unsigned char*>(instance);
    // sub_1c5c8 and sub_1e790 fall back to scene zero for unknown values.
    const unsigned sceneIndex = scene == 2 ? 2 : scene == 1 ? 1 : 0;
    const unsigned char* sceneRecord = object + 0xe4 * sceneIndex;
    AutoMapBindingState* state = reinterpret_cast<AutoMapBindingState*>(
        object + 0x2ac + 0x20 * sceneIndex);
    const ScreenType screen = static_cast<ScreenType>(wordAt(object, 0x30c));
    const int engine = state->words[0];
    // sub_16868 maps engine ID 3 to Small, 7 to Widget, and all other
    // values to Large before consulting the per-screen cross-image override.
    const ScreenType engineScreen = static_cast<ScreenType>(
        engine == 7 ? 2 : engine == 3 ? 1 : 0);
    const int onlyCross = AutoExscreenUtil_IsOnlyShowCrossImage(
        engineScreen, sceneRecord[0xa0]);
    const int uiDayNight = AutoExscreenUtil_GetMapDayNightModeFromUiConfig(screen);
    const int uiLevel = AutoExscreenUtil_GetMapViewLevelFromUiConfig(screen);
    const int uiMode = AutoExscreenUtil_GetMapViewModeFromUiConfig(screen);
    const AutoMapBindingState calculated = AutoExscreenUtil_ResolveMapBindingState(
        sceneRecord, uiLevel, uiMode, uiDayNight, onlyCross, *state);
    std::memcpy(state, &calculated, sizeof(calculated));

    void* service = hsl::HSL::GetMapServiceHandle();
    if (!service) return;
    typedef void* (*Lookup)(void*, int);
    void** serviceVtable = *static_cast<void***>(service);
    Lookup lookup = reinterpret_cast<Lookup>(serviceVtable[0x3c / armVtableWord]);
    void* first = lookup(service, 1);
    void* second = lookup(service, state->words[0]);
    if (!first || !second) return;
    state->words[7] = 1;
    typedef void (*Bind)(void*, void*, int32_t*);
    void** firstVtable = *static_cast<void***>(first);
    Bind bind = reinterpret_cast<Bind>(firstVtable[0x3c / armVtableWord]);
    bind(first, second, &state->words[1]);
}

void AutoMultiScreenConfig_SetMapViewMode(void* instance, int fallbackMode,
                                         int requestedMode) {
    hsl::MapView* own = AutoMultiScreenConfig_GetMapViewHandle(instance);
    hsl::MapView* reference = hsl::HSL::GetMapViewHandle(
        static_cast<bl::MapEngineID>(1));
    if (!own || !reference) return;

    const int effective = requestedMode == 99 ? fallbackMode : requestedMode;
    putWord(static_cast<unsigned char*>(instance), 0x314,
            static_cast<uint32_t>(effective));
    if (static_cast<unsigned>(effective) > 3u) return;

    typedef int (*GetMode)(void*);
    typedef void (*SetMode)(void*, int, bool, bool, bool, bool);
    void** ownVtable = *reinterpret_cast<void***>(own);
    const SetMode setMode = reinterpret_cast<SetMode>(
        ownVtable[0x90 / armVtableWord]);
    int mapMode = 2 - effective;
    if (effective == 3) {
        void** referenceVtable = *reinterpret_cast<void***>(reference);
        const GetMode getMode = reinterpret_cast<GetMode>(
            referenceVtable[0x98 / armVtableWord]);
        mapMode = getMode(reference);
    }
    setMode(own, mapMode, false, false, false, false);
}

void AutoMultiScreenConfig_CloseSceneDynamicLevel(int screen, int scene) {
    const int engine = engineForScreen(screen);
    if (engine < 0) return;
    hsl::MapView* view = hsl::HSL::GetMapViewHandle(
        static_cast<bl::MapEngineID>(engine));
    if (!view || (scene != 0 && scene != 1)) return;

    const int dynamicType = scene == 0 ? 1 : 0;
    void** vtable = *reinterpret_cast<void***>(view);
    typedef bool (*IsOpenDynamicLevel)(void*, int);
    typedef void (*OpenDynamicLevel)(void*, bool, int);
    const IsOpenDynamicLevel isOpen = reinterpret_cast<IsOpenDynamicLevel>(
        vtable[0x3ec / armVtableWord]);
    if (!isOpen(view, dynamicType)) return;
    const OpenDynamicLevel open = reinterpret_cast<OpenDynamicLevel>(
        (*reinterpret_cast<void***>(view))[0x3e8 / armVtableWord]);
    open(view, false, dynamicType);
}

void AutoMultiScreenConfig_InitSceneDynamicLevel(void* instance, int scene) {
    hsl::MapView* view = AutoMultiScreenConfig_GetMapViewHandle(instance);
    if (!view) return;
    unsigned char* object = static_cast<unsigned char*>(instance);
    const unsigned sceneIndex = scene == 2 ? 2 : scene == 1 ? 1 : 0;
    const unsigned char* sceneRecord = object + 0xe4 * sceneIndex;
    const int32_t quarter = wordAt(sceneRecord, 8) / 4;

    // sub_176dc..17732 writes every word of a 0x88-byte parameter object.
    // Float fields are kept as their observed IEEE-754 words, avoiding an
    // accidental change in rounding of the constants when compiled on ARM.
    alignas(8) const uint32_t params[34] = {
        10, 20, 20, 10, 20, 10, static_cast<uint32_t>(quarter),
        0x3f000000u, 0x3f30e560u, 0x41880000u, 0x41700000u,
        0x41880000u, 0x41600000u, 0x41880000u, 0x41600000u,
        0x41880000u, 0x41600000u, 0x42340000u, 0x42480000u,
        0x41880000u, 0x41f00000u, 0x41700000u, 0x41600000u,
        0x41600000u, 600, 0x41880000u, 0x3f333333u, 0x41700000u,
        0x42b40000u, 200, 0, 0, 0, 0
    };
    static_assert(sizeof(params) == 0x88, "V21 DynamicLevelParam extent");
    typedef void (*InitDynamicLevel)(void*, const void*);
    typedef void (*OpenDynamicLevel)(void*, bool, int);
    typedef void (*OpenDynamicCenter)(void*, bool);
    const auto slot = [&](unsigned offset) {
        return (*reinterpret_cast<void***>(view))[offset / armVtableWord];
    };
    reinterpret_cast<InitDynamicLevel>(slot(0x3cc))(view, params);
    reinterpret_cast<OpenDynamicLevel>(slot(0x3e8))(
        view, true, scene == 0 ? 1 : 0);
    reinterpret_cast<OpenDynamicCenter>(slot(0x3f0))(
        view, false);
}

void AutoMultiScreenConfig_SetMapLevel(void* instance, int fallbackLevel,
                                       int requestedLevel) {
    hsl::MapView* own = AutoMultiScreenConfig_GetMapViewHandle(instance);
    if (!own) return;
    hsl::MapView* reference = hsl::HSL::GetMapViewHandle(
        static_cast<bl::MapEngineID>(1));
    if (!reference) return;

    unsigned char* object = static_cast<unsigned char*>(instance);
    const int screen = wordAt(object, 0x30c);
    const int scene = *AutoExscreenUtil_GetCurrentSceneSlot(
        static_cast<ScreenType>(screen));
    const auto ownSlot = [&](unsigned offset) {
        return (*reinterpret_cast<void***>(own))[offset / armVtableWord];
    };
    typedef void (*ZoomStep)(void*, bool, bool, bool, bool);
    typedef float (*GetZoomLevel)(void*);
    typedef void (*SetZoomLevel)(void*, float, bool, bool, bool, bool);
    typedef void (*OpenDynamicLevel)(void*, bool, int);
    typedef void (*ResetDynamicLevel)(void*);

    // sub_1e22c/1e2f0/1e30c/1e338 close the previously recorded type.
    // Only the fallback absolute-level branch also resets the host's
    // dynamic-level parameters at virtual slot +0x3d4.
    const auto closeRecorded = [&](bool resetParameters) {
        const int activeType = wordAt(object, 0x338);
        if (activeType == -1) return;
        reinterpret_cast<OpenDynamicLevel>(ownSlot(0x3e8))(
            own, false, activeType);
        if (resetParameters)
            reinterpret_cast<ResetDynamicLevel>(ownSlot(0x3d4))(own);
        putWord(object, 0x338, UINT32_MAX);
    };
    const auto preserveReferenceZoom = [&]() {
        AutoMultiScreenConfig_CloseSceneDynamicLevel(screen, scene);
        void** referenceVtable = *reinterpret_cast<void***>(reference);
        const float level = reinterpret_cast<GetZoomLevel>(
            referenceVtable[0x198 / armVtableWord])(reference);
        reinterpret_cast<SetZoomLevel>(ownSlot(0x1b0))(
            own, level, false, false, true, false);
        closeRecorded(false);
    };
    const auto startDynamic = [&]() {
        const int activeType = wordAt(object, 0x338);
        if (activeType != -1)
            reinterpret_cast<OpenDynamicLevel>(ownSlot(0x3e8))(
                own, false, activeType);
        AutoMultiScreenConfig_InitSceneDynamicLevel(instance, scene);
        putWord(object, 0x338, scene == 0 ? 1u : 0u);
    };

    switch (requestedLevel) {
    case -3:
    case -2:
        AutoMultiScreenConfig_CloseSceneDynamicLevel(screen, scene);
        reinterpret_cast<ZoomStep>(ownSlot(
            requestedLevel == -3 ? 0x8c : 0x88))(
            own, true, false, true, false);
        return;
    case -1:
        startDynamic();
        return;
    case 0:
        preserveReferenceZoom();
        return;
    default:
        break;
    }
    if (requestedLevel >= 3 && requestedLevel <= 19) {
        closeRecorded(false);
        AutoMultiScreenConfig_CloseSceneDynamicLevel(screen, scene);
        reinterpret_cast<SetZoomLevel>(ownSlot(0x1b0))(
            own, static_cast<float>(requestedLevel), false, true, false, false);
        return;
    }
    if (requestedLevel != 99) return;
    if (fallbackLevel == -1) {
        startDynamic();
        return;
    }
    if (fallbackLevel == 0) {
        preserveReferenceZoom();
        return;
    }
    closeRecorded(true);
    AutoMultiScreenConfig_CloseSceneDynamicLevel(screen, scene);
    const int clampedLevel = fallbackLevel < 3 ? 3 :
                             fallbackLevel > 19 ? 19 : fallbackLevel;
    reinterpret_cast<SetZoomLevel>(ownSlot(0x1b0))(
        own, static_cast<float>(clampedLevel), false, true, true, false);
}

void AutoMultiScreenConfig::SetMapLevel(int fallbackLevel,
                                        int requestedLevel) {
    AutoMultiScreenConfig_SetMapLevel(this, fallbackLevel, requestedLevel);
}

void AutoMultiScreenConfig::SetMapViewMode(int fallbackMode,
                                          int requestedMode) {
    AutoMultiScreenConfig_SetMapViewMode(this, fallbackMode, requestedMode);
}

} // namespace NS_MultiScreenBase
