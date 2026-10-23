#include "AutoMultiScreenConfigBinding.h"
#include "AutoMultiScreenBindingState.h"
#include "AutoExscreenUiConfig.h"
#include <hsl/HSL.h>
#include <cstdint>
#include <cstring>

namespace NS_MultiScreenBase {
namespace {
int32_t wordAt(const unsigned char* object, unsigned offset) {
    int32_t value;
    std::memcpy(&value, object + offset, sizeof(value));
    return value;
}
}

hsl::MapView* AutoMultiScreenConfig_GetMapViewHandle(const void* instance) {
    const unsigned char* object = static_cast<const unsigned char*>(instance);
    const int screen = wordAt(object, 0x30c);
    // sub_16788 maps Large/Small/Widget to 5/3/7; sub_1ddd8 rejects
    // unexpected engine IDs before calling HSL::GetMapViewHandle.
    const int engine = screen == 0 ? 5 : screen == 1 ? 3 : screen == 2 ? 7 : -1;
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
