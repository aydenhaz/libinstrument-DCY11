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
void* ownViewVtable[0x400 / 4] = {};
void* referenceViewVtable[0x1a0 / 4] = {};
struct FakeView { void** vtable; } ownView = {ownViewVtable},
                              referenceView = {referenceViewVtable};
bool allowOwnView = true;
bool allowReferenceView = true;
int referenceMode = 1;
int selectedMode = -1;
int setModeCount = 0;
int getModeCount = 0;
int isOpenType = -1;
int openType = -1;
int dynamicOpenCount = 0;
bool isOpenValue = true;
bool lastDynamicEnabled = false;
uint32_t capturedDynamicParams[34] = {};
char dynamicEvents[4] = {};
unsigned dynamicEventCount = 0;
bool dynamicCenterEnabled = true;
int sceneSlot = 0;
int sceneScreen = -1;
int zoomInCount = 0;
int zoomOutCount = 0;
int getZoomCount = 0;
int setZoomCount = 0;
int resetDynamicCount = 0;
float referenceZoom = 8.5f;
float selectedZoom = -1.0f;
bool selectedZoomFlags[4] = {};
struct DynamicAction { bool enabled; int type; };
DynamicAction dynamicActions[32] = {};
unsigned dynamicActionCount = 0;

void zoomIn(void* self, bool a, bool b, bool c, bool d) {
    assert(self == &ownView && a && !b && c && !d);
    ++zoomInCount;
}
void zoomOut(void* self, bool a, bool b, bool c, bool d) {
    assert(self == &ownView && a && !b && c && !d);
    ++zoomOutCount;
}
float getZoom(void* self) {
    assert(self == &referenceView);
    ++getZoomCount;
    return referenceZoom;
}
void setZoom(void* self, float level, bool a, bool b, bool c, bool d) {
    assert(self == &ownView);
    selectedZoom = level;
    selectedZoomFlags[0] = a;
    selectedZoomFlags[1] = b;
    selectedZoomFlags[2] = c;
    selectedZoomFlags[3] = d;
    ++setZoomCount;
}
void resetDynamic(void* self) {
    assert(self == &ownView);
    ++resetDynamicCount;
}

void initDynamic(void* self, const void* params) {
    assert(self == &ownView);
    dynamicEventCount = 0;
    assert(dynamicEventCount < sizeof(dynamicEvents));
    dynamicEvents[dynamicEventCount++] = 'I';
    std::memcpy(capturedDynamicParams, params, sizeof(capturedDynamicParams));
}
void openDynamicCenter(void* self, bool enabled) {
    assert(self == &ownView);
    assert(dynamicEventCount < sizeof(dynamicEvents));
    dynamicEvents[dynamicEventCount++] = 'C';
    dynamicCenterEnabled = enabled;
}

bool isDynamicOpen(void* self, int type) {
    assert(self == &ownView);
    isOpenType = type;
    return isOpenValue;
}
void openDynamic(void* self, bool enabled, int type) {
    assert(self == &ownView);
    assert(dynamicActionCount < sizeof(dynamicActions) / sizeof(dynamicActions[0]));
    dynamicActions[dynamicActionCount++] = {enabled, type};
    lastDynamicEnabled = enabled;
    if (dynamicEventCount == 1 && dynamicEvents[0] == 'I') {
        assert(dynamicEventCount < sizeof(dynamicEvents));
        dynamicEvents[dynamicEventCount++] = 'O';
    }
    openType = type;
    ++dynamicOpenCount;
}

int getMapMode(void* self) {
    assert(self == &referenceView);
    ++getModeCount;
    return referenceMode;
}
void setMapMode(void* self, int mode, bool a, bool b, bool c, bool d) {
    assert(self == &ownView && !a && !b && !c && !d);
    selectedMode = mode;
    ++setModeCount;
}

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
    if (viewEngine == 1)
        return allowReferenceView ? reinterpret_cast<MapView*>(&referenceView) : nullptr;
    return allowOwnView ? reinterpret_cast<MapView*>(&ownView) : nullptr;
}
}
namespace NS_MultiScreenBase {
int* AutoExscreenUtil_GetCurrentSceneSlot(ScreenType screen) {
    sceneScreen = static_cast<int>(screen);
    return &sceneSlot;
}
int AutoExscreenUtil_GetMapViewLevelFromUiConfig(ScreenType) { return -1; }
int AutoExscreenUtil_GetMapViewModeFromUiConfig(ScreenType) { return 1; }
int AutoExscreenUtil_GetMapDayNightModeFromUiConfig(ScreenType) { return 99; }
int AutoExscreenUtil_IsOnlyShowCrossImage(ScreenType screen, int fallback) {
    queriedScreen = static_cast<int>(screen);
    return fallback;
}
}

int main() {
    const unsigned slot = 0x3c / 4;
    serviceVtable[slot] = reinterpret_cast<void*>(&lookup);
    mapVtable[slot] = reinterpret_cast<void*>(&bind);
    ownViewVtable[0x90 / 4] = reinterpret_cast<void*>(&setMapMode);
    ownViewVtable[0x88 / 4] = reinterpret_cast<void*>(&zoomIn);
    ownViewVtable[0x8c / 4] = reinterpret_cast<void*>(&zoomOut);
    ownViewVtable[0x1b0 / 4] = reinterpret_cast<void*>(&setZoom);
    ownViewVtable[0x3d4 / 4] = reinterpret_cast<void*>(&resetDynamic);
    ownViewVtable[0x3ec / 4] = reinterpret_cast<void*>(&isDynamicOpen);
    ownViewVtable[0x3e8 / 4] = reinterpret_cast<void*>(&openDynamic);
    ownViewVtable[0x3cc / 4] = reinterpret_cast<void*>(&initDynamic);
    ownViewVtable[0x3f0 / 4] = reinterpret_cast<void*>(&openDynamicCenter);
    referenceViewVtable[0x98 / 4] = reinterpret_cast<void*>(&getMapMode);
    referenceViewVtable[0x198 / 4] = reinterpret_cast<void*>(&getZoom);
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
               reinterpret_cast<hsl::MapView*>(&ownView));
        assert(viewEngine == engines[index]);
    }
    for (int invalid : {-1, 3, 99}) {
        std::memcpy(object + 0x30c, &invalid, 4);
        viewEngine = -1;
        assert(AutoMultiScreenConfig_GetMapViewHandle(object) == nullptr);
        assert(viewEngine == -1);
    }
    std::memcpy(object + 0x30c, &screen, 4);
    using NS_MultiScreenBase::AutoMultiScreenConfig_SnapshotMapState;
    int snapshotLevel = 0, snapshotMode = 0;
    put(object, 0x88, -1);
    put(object, 0x8c, 3);
    put(object + 0xe4, 0x88, 12);
    put(object + 0xe4, 0x8c, 99);
    AutoMultiScreenConfig_SnapshotMapState(object, 0, &snapshotLevel, &snapshotMode);
    assert(snapshotLevel == -1 && snapshotMode == 3);
    AutoMultiScreenConfig_SnapshotMapState(object, 1, &snapshotLevel, &snapshotMode);
    assert(snapshotLevel == 12 && snapshotMode == 99);
    AutoMultiScreenConfig_SnapshotMapState(object, 99, &snapshotLevel, &snapshotMode);
    assert(snapshotLevel == -1 && snapshotMode == 3);
    using NS_MultiScreenBase::AutoMultiScreenConfig_SetMapViewMode;
    for (int requested = 0; requested < 3; ++requested) {
        AutoMultiScreenConfig_SetMapViewMode(object, 99, requested);
        assert(word(object, 0x314) == static_cast<uint32_t>(requested));
        assert(selectedMode == 2 - requested && setModeCount == requested + 1);
        assert(getModeCount == 0 && viewEngine == 1);
    }
    referenceMode = 2;
    AutoMultiScreenConfig_SetMapViewMode(object, 99, 3);
    assert(word(object, 0x314) == 3 && selectedMode == 2);
    assert(setModeCount == 4 && getModeCount == 1);
    AutoMultiScreenConfig_SetMapViewMode(object, 1, 99);
    assert(word(object, 0x314) == 1 && selectedMode == 1 && setModeCount == 5);
    AutoMultiScreenConfig_SetMapViewMode(object, 0, -1);
    assert(word(object, 0x314) == UINT32_MAX && setModeCount == 5);
    allowReferenceView = false;
    AutoMultiScreenConfig_SetMapViewMode(object, 99, 2);
    assert(word(object, 0x314) == UINT32_MAX && setModeCount == 5);
    allowReferenceView = true;
    allowOwnView = false;
    AutoMultiScreenConfig_SetMapViewMode(object, 99, 2);
    assert(word(object, 0x314) == UINT32_MAX && setModeCount == 5);
    allowOwnView = true;
    using NS_MultiScreenBase::AutoMultiScreenConfig_SetMapLevel;
    put(object, 0x338, -1);
    put(object, 8, 41);
    put(object + 0xe4, 8, -5);
    sceneSlot = 0;
    isOpenValue = true;
    dynamicEventCount = 0;
    AutoMultiScreenConfig_SetMapLevel(object, 0, -3);
    AutoMultiScreenConfig_SetMapLevel(object, 0, -2);
    assert(sceneScreen == 0 && zoomOutCount == 1 && zoomInCount == 1);
    assert(isOpenType == 1 && word(object, 0x338) == UINT32_MAX);
    AutoMultiScreenConfig_SetMapLevel(object, 0, -1);
    assert(word(object, 0x338) == 1 && lastDynamicEnabled && openType == 1);
    assert(dynamicEventCount == 3 && std::memcmp(dynamicEvents, "IOC", 3) == 0);
    sceneSlot = 1;
    dynamicEventCount = 0;
    unsigned actionBase = dynamicActionCount;
    AutoMultiScreenConfig_SetMapLevel(object, 0, -1);
    assert(dynamicActionCount == actionBase + 2);
    assert(!dynamicActions[actionBase].enabled && dynamicActions[actionBase].type == 1);
    assert(dynamicActions[actionBase + 1].enabled && dynamicActions[actionBase + 1].type == 0);
    assert(word(object, 0x338) == 0 && capturedDynamicParams[6] == UINT32_MAX);
    actionBase = dynamicActionCount;
    AutoMultiScreenConfig_SetMapLevel(object, 0, 0);
    assert(getZoomCount == 1 && setZoomCount == 1 && selectedZoom == referenceZoom);
    assert(!selectedZoomFlags[0] && !selectedZoomFlags[1] &&
           selectedZoomFlags[2] && !selectedZoomFlags[3]);
    assert(dynamicActionCount == actionBase + 2 && word(object, 0x338) == UINT32_MAX);
    put(object, 0x338, 1);
    sceneSlot = 0;
    AutoMultiScreenConfig_SetMapLevel(object, 0, 8);
    assert(selectedZoom == 8.0f && setZoomCount == 2);
    assert(!selectedZoomFlags[0] && selectedZoomFlags[1] &&
           !selectedZoomFlags[2] && !selectedZoomFlags[3]);
    assert(word(object, 0x338) == UINT32_MAX && resetDynamicCount == 0);
    dynamicEventCount = 0;
    AutoMultiScreenConfig_SetMapLevel(object, -1, 99);
    assert(word(object, 0x338) == 1 && lastDynamicEnabled);
    AutoMultiScreenConfig_SetMapLevel(object, 0, 99);
    assert(word(object, 0x338) == UINT32_MAX && selectedZoom == referenceZoom);
    put(object, 0x338, 0);
    AutoMultiScreenConfig_SetMapLevel(object, -10, 99);
    assert(selectedZoom == 3.0f && resetDynamicCount == 1);
    assert(!selectedZoomFlags[0] && selectedZoomFlags[1] &&
           selectedZoomFlags[2] && !selectedZoomFlags[3]);
    assert(word(object, 0x338) == UINT32_MAX);
    AutoMultiScreenConfig_SetMapLevel(object, 100, 99);
    assert(selectedZoom == 19.0f && resetDynamicCount == 1);
    const int zoomBeforeInvalid = setZoomCount;
    for (int invalid : {-4, 1, 2, 20, 98})
        AutoMultiScreenConfig_SetMapLevel(object, 7, invalid);
    assert(setZoomCount == zoomBeforeInvalid && word(object, 0x338) == UINT32_MAX);
    allowReferenceView = false;
    AutoMultiScreenConfig_SetMapLevel(object, 7, 99);
    assert(setZoomCount == zoomBeforeInvalid);
    allowReferenceView = true;
    dynamicOpenCount = 0;
    dynamicActionCount = 0;
    using NS_MultiScreenBase::AutoMultiScreenConfig_CloseSceneDynamicLevel;
    AutoMultiScreenConfig_CloseSceneDynamicLevel(0, 0);
    assert(viewEngine == 5 && isOpenType == 1 && openType == 1 && dynamicOpenCount == 1);
    assert(!lastDynamicEnabled);
    AutoMultiScreenConfig_CloseSceneDynamicLevel(1, 1);
    assert(viewEngine == 3 && isOpenType == 0 && openType == 0 && dynamicOpenCount == 2);
    isOpenValue = false;
    AutoMultiScreenConfig_CloseSceneDynamicLevel(2, 1);
    assert(viewEngine == 7 && isOpenType == 0 && dynamicOpenCount == 2);
    AutoMultiScreenConfig_CloseSceneDynamicLevel(0, 2);
    assert(viewEngine == 5 && isOpenType == 0 && dynamicOpenCount == 2);
    using NS_MultiScreenBase::AutoMultiScreenConfig_InitSceneDynamicLevel;
    put(object, 8, 41);
    put(object + 0xe4, 8, -5);
    dynamicEventCount = 0;
    AutoMultiScreenConfig_InitSceneDynamicLevel(object, 0);
    assert(viewEngine == 5 && dynamicOpenCount == 3 && lastDynamicEnabled && openType == 1);
    assert(dynamicEventCount == 3 && std::memcmp(dynamicEvents, "IOC", 3) == 0);
    assert(!dynamicCenterEnabled && capturedDynamicParams[6] == 10);
    assert(capturedDynamicParams[0] == 10 && capturedDynamicParams[1] == 20);
    assert(capturedDynamicParams[17] == 0x42340000u &&
           capturedDynamicParams[18] == 0x42480000u);
    assert(capturedDynamicParams[24] == 600 &&
           capturedDynamicParams[28] == 0x42b40000u &&
           capturedDynamicParams[29] == 200);
    for (unsigned i = 30; i < 34; ++i) assert(capturedDynamicParams[i] == 0);
    dynamicEventCount = 0;
    AutoMultiScreenConfig_InitSceneDynamicLevel(object, 1);
    assert(dynamicOpenCount == 4 && lastDynamicEnabled && openType == 0);
    assert(capturedDynamicParams[6] == UINT32_MAX);
    assert(dynamicEventCount == 3 && std::memcmp(dynamicEvents, "IOC", 3) == 0);
    allowOwnView = false;
    dynamicEventCount = 0;
    AutoMultiScreenConfig_InitSceneDynamicLevel(object, 0);
    assert(dynamicOpenCount == 4 && dynamicEventCount == 0);
    allowOwnView = true;
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
