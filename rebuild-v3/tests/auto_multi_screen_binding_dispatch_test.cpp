#include "../internal/NS_MultiScreenBase/AutoMultiScreenConfigBinding.h"
#include "../internal/NS_MultiScreenBase/AutoMultiScreenBindingState.h"
#include <AbiTypes.h>
#include <hsl/HSL.h>
#include <cassert>
#include <cstdint>
#include <cstring>
#include <initializer_list>
#include <new>

namespace {
void* serviceVtable[16] = {};
void* mapVtable[16] = {};
struct Object { void** vtable; } service = {serviceVtable}, first = {mapVtable}, second = {mapVtable};
void* selectedService = &service;
int lookupCount = 0;
int bindCount = 0;
int queriedScreen = -1;
int32_t* boundState = nullptr;
bool allowSecond = true;
int viewEngine = -1;
unsigned char fakeView;

void* lookup(void* self, int engine) {
    assert(self == &service);
    assert(engine == (lookupCount == 0 ? 1 : 7));
    ++lookupCount;
    return engine == 1 ? &first : allowSecond ? &second : nullptr;
}
void bind(void* self, void* map, int32_t* parameters) {
    assert(self == &first && map == &second);
    ++bindCount;
    boundState = parameters;
}
void put(unsigned char* scene, unsigned offset, int32_t value) {
    std::memcpy(scene + offset, &value, 4);
}
uint32_t word(const unsigned char* scene, unsigned offset) {
    uint32_t value;
    std::memcpy(&value, scene + offset, 4);
    return value;
}
}

namespace hsl {
void* HSL::GetMapServiceHandle() { return selectedService; }
MapView* HSL::GetMapViewHandle(bl::MapEngineID engine) {
    viewEngine = static_cast<int>(engine);
    return reinterpret_cast<MapView*>(&fakeView);
}
}
namespace NS_MultiScreenBase {
int AutoExscreenUtil_GetMapViewLevelFromUiConfig(ScreenType) { return -1; }
int AutoExscreenUtil_GetMapViewModeFromUiConfig(ScreenType) { return 1; }
int AutoExscreenUtil_GetMapDayNightModeFromUiConfig(ScreenType) { return 99; }
int AutoExscreenUtil_IsOnlyShowCrossImage(ScreenType screen, int fallback) {
    queriedScreen = static_cast<int>(screen);
    return fallback;
}
}

int main() {
    const unsigned slot = 0x3c / sizeof(void*);
    serviceVtable[slot] = reinterpret_cast<void*>(&lookup);
    mapVtable[slot] = reinterpret_cast<void*>(&bind);
    alignas(8) unsigned char object[0x340] = {};
    const int32_t screen = 0;
    std::memcpy(object + 0x30c, &screen, 4);
    alignas(4) unsigned char initializedScene[0xe4];
    std::memset(initializedScene, 0xa5, sizeof(initializedScene));
    NS_MultiScreenBase::AutoMultiScreenConfig_InitSceneRecord(initializedScene);
    for (unsigned i = 4; i < 0x24; ++i) assert(initializedScene[i] == 0xff);
    assert(initializedScene[0] == 0 && initializedScene[0x26] == 1);
    assert(word(initializedScene, 0x28) == UINT32_MAX);
    assert(word(initializedScene, 0x38) == 6 && word(initializedScene, 0x3c) == 240);
    assert(word(initializedScene, 0x40) == 0xbf800000u);
    assert(word(initializedScene, 0x78) == 0x3f3851ecu);
    assert(word(initializedScene, 0x7c) == 0xbf800000u);
    assert(word(initializedScene, 0x80) == 0x3f000000u);
    assert(initializedScene[0x95] == 1 && initializedScene[0x96] == 1);
    assert(word(initializedScene, 0x9c) == 7 && word(initializedScene, 0xd4) == 0xbf800000u);
    assert(initializedScene[0x27] == 0xa5 && initializedScene[0xc9] == 0xa5 &&
           initializedScene[0xe3] == 0xa5);
    unsigned char initializedBinding[0x20];
    for (int index = 0; index < 3; ++index) {
        std::memset(initializedBinding, 0xa5, sizeof(initializedBinding));
        NS_MultiScreenBase::AutoMultiScreenConfig_InitBindingRecord(initializedBinding, index);
        assert(word(initializedBinding, 0) == static_cast<uint32_t>(index == 0 ? 5 : index == 1 ? 3 : 7));
        for (unsigned byte = 4; byte < 0x1d; ++byte) assert(initializedBinding[byte] == 0);
        for (unsigned byte = 0x1d; byte < 0x20; ++byte) assert(initializedBinding[byte] == 0xa5);
    }
    NS_MultiScreenBase::AutoMultiScreenConfig_InitBindingRecord(initializedBinding, 99);
    assert(word(initializedBinding, 0) == UINT32_MAX);
    using NS_MultiScreenBase::AutoMultiScreenConfig_GetMapViewHandle;
    const int engines[] = {5, 3, 7};
    for (int index = 0; index < 3; ++index) {
        std::memcpy(object + 0x30c, &index, 4);
        viewEngine = -1;
        assert(AutoMultiScreenConfig_GetMapViewHandle(object) ==
               reinterpret_cast<hsl::MapView*>(&fakeView));
        assert(viewEngine == engines[index]);
    }
    for (int invalid : {-1, 3, 99}) {
        std::memcpy(object + 0x30c, &invalid, 4);
        viewEngine = -1;
        assert(AutoMultiScreenConfig_GetMapViewHandle(object) == nullptr);
        assert(viewEngine == -1);
    }
    std::memcpy(object + 0x30c, &screen, 4);
    unsigned char* scene = object + 0xe4;
    put(scene, 0x4c, 2);
    put(scene, 0x50, 0);
    put(scene, 0x88, 12);
    put(scene, 0x8c, 3);
    put(scene, 0x90, 1);
    auto* state = new (object + 0x2cc) NS_MultiScreenBase::AutoMapBindingState{};
    state->words[0] = 7;

    NS_MultiScreenBase::AutoMultiScreenConfig_ResetMapBindingList(object, 1);
    assert(queriedScreen == 2 && lookupCount == 2 && bindCount == 1);
    assert(state->words[1] == 1 && state->words[2] == 12);
    assert(state->words[5] == 2 && state->words[7] == 1);
    assert(boundState == &state->words[1]);

    lookupCount = bindCount = 0;
    state->words[7] = 0;
    allowSecond = false;
    NS_MultiScreenBase::AutoMultiScreenConfig_ResetMapBindingList(object, 1);
    assert(lookupCount == 2 && bindCount == 0 && state->words[7] == 0);

    lookupCount = bindCount = 0;
    selectedService = nullptr;
    NS_MultiScreenBase::AutoMultiScreenConfig_ResetMapBindingList(object, 1);
    assert(lookupCount == 0 && bindCount == 0);
}
