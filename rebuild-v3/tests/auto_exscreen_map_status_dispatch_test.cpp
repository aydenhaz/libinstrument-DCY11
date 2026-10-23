#include "../internal/NS_MultiScreenBase/AutoExscreenMapStatusDispatch.h"
#include <hsl/HSL.h>
#include <GFrame/CGUITimer.h>
#include <cassert>
#include <cstdint>
#include <cstring>
#include <new>

namespace {
void* vtable[0x400 / 4] = {};
struct FakeView { void** functions; } view = {vtable};
alignas(8) unsigned char instance[0x340] = {};
alignas(8) unsigned char timerState[0x40] = {};
int scene = 1;
int configuredLevel = -1;
int configuredMode = 3;
bool haveView = true;
bool timerResult = true;
int lookupEngine = -1;
int timerCalls = 0;
char actions[64] = {};
unsigned actionCount = 0;

void event(char id) {
    assert(actionCount < sizeof(actions));
    actions[actionCount++] = id;
}
void expect(const char* expected) {
    assert(actionCount == std::strlen(expected));
    assert(std::memcmp(actions, expected, actionCount) == 0);
    actionCount = 0;
}
void openDynamic(void* self, bool enable, int type) {
    assert(self == &view && !enable && type == 0);
    event('D');
}
}

namespace hsl {
MapView* HSL::GetMapViewHandle(bl::MapEngineID engine) {
    lookupEngine = static_cast<int>(engine);
    return haveView ? reinterpret_cast<MapView*>(&view) : nullptr;
}
}
namespace GNS_FRAME {
bool CGUITimer::start(int interval, unsigned long long deadline) {
    assert(this == reinterpret_cast<CGUITimer*>(timerState + 0x10));
    assert(interval == 250 && deadline == 0x1122334455667788ULL);
    ++timerCalls;
    event('T');
    return timerResult;
}
}
namespace NS_MultiScreenBase {
int* AutoExscreenUtil_GetCurrentSceneSlot(ScreenType screen) {
    assert(static_cast<int>(screen) == 2);
    event('C');
    return &scene;
}
bool AutoExscreenUtil_IsDynamicLevelRequest(int current, int requested) {
    assert(current == configuredLevel);
    event('P');
    return requested == -1 || (current == -1 && requested == 99);
}
void AutoMultiScreenConfig_SnapshotMapState(const void* owner, int selected,
                                             int* level, int* mode) {
    assert(owner == instance && selected == scene);
    event('S');
    *level = configuredLevel;
    *mode = configuredMode;
}
void AutoExscreenUtil_SetMapViewLevelToUiConfig(ScreenType screen, int level) {
    assert(static_cast<int>(screen) == 2 && level != 99);
    event('L');
}
void AutoExscreenUtil_SetMapViewModeToUiConfig(ScreenType screen, int mode) {
    assert(static_cast<int>(screen) == 2 && mode != 99);
    event('M');
}
void AutoMultiScreenConfig_SetMapLevel(void* owner, int fallback, int requested) {
    assert(owner == instance && fallback == configuredLevel && requested != 99);
    event('l');
}
void AutoMultiScreenConfig_SetMapViewMode(void* owner, int fallback, int requested) {
    assert(owner == instance && fallback == configuredMode && requested != 99);
    event('m');
}
void AutoMultiScreenConfig_ResetMapBindingList(void* owner, int selected) {
    assert(owner == instance && selected == scene);
    event('R');
}
}

int main() {
    vtable[0x3e8 / 4] = reinterpret_cast<void*>(&openDynamic);
    new (timerState + 0x10) GNS_FRAME::CGUITimer();
    const int32_t interval = 250;
    const uint64_t deadline = 0x1122334455667788ULL;
    std::memcpy(timerState + 4, &interval, sizeof(interval));
    std::memcpy(timerState + 8, &deadline, sizeof(deadline));
    using NS_MultiScreenBase::AutoExscreenUtil_ApplyMapStatus;
    const NS_MultiScreenBase::ScreenType screen =
        static_cast<NS_MultiScreenBase::ScreenType>(2);

    haveView = false;
    assert(!AutoExscreenUtil_ApplyMapStatus(instance, screen, timerState, 7, 99, 99));
    assert(lookupEngine == 7);
    expect("");
    haveView = true;
    assert(AutoExscreenUtil_ApplyMapStatus(instance, screen, timerState, 7, 99, 99));
    expect("CSPR");
    lookupEngine = -1;
    NS_MultiScreenBase::AutoExscreenUtil_ApplyMapStatusWithView(
        instance, screen, timerState, reinterpret_cast<hsl::MapView*>(&view),
        99, 99);
    assert(lookupEngine == -1);
    expect("CSPR");
    assert(AutoExscreenUtil_ApplyMapStatus(instance, screen, timerState, 7, 10, 99));
    expect("CSPLlR");
    assert(AutoExscreenUtil_ApplyMapStatus(instance, screen, timerState, 7, -1, 2));
    expect("CSPLlMR");
    assert(timerCalls == 0);
    assert(AutoExscreenUtil_ApplyMapStatus(instance, screen, timerState, 7, 99, 2));
    expect("CSPMDTmR");
    assert(timerCalls == 1 && timerState[0] == 1);
    assert(AutoExscreenUtil_ApplyMapStatus(instance, screen, timerState, 7, 99, 2));
    expect("CSPMDmR");
    configuredLevel = 12;
    configuredMode = 1;
    assert(AutoExscreenUtil_ApplyMapStatus(instance, screen, timerState, 7, 10, 2));
    expect("CSPLlMmR");
}
