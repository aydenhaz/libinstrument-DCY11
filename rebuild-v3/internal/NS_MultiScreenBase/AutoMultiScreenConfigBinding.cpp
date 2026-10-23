#include "AutoMultiScreenConfigBinding.h"
#include "AutoMultiScreenBindingState.h"
#include "AutoExscreenUiConfig.h"
#include <hsl/HSL.h>
#include <cstdint>
#include <cstring>
#include <initializer_list>

namespace NS_MultiScreenBase {
namespace {
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
    Lookup lookup = reinterpret_cast<Lookup>(serviceVtable[0x3c / sizeof(void*)]);
    void* first = lookup(service, 1);
    void* second = lookup(service, state->words[0]);
    if (!first || !second) return;
    state->words[7] = 1;
    typedef void (*Bind)(void*, void*, int32_t*);
    void** firstVtable = *static_cast<void***>(first);
    Bind bind = reinterpret_cast<Bind>(firstVtable[0x3c / sizeof(void*)]);
    bind(first, second, &state->words[1]);
}

} // namespace NS_MultiScreenBase
