#include "../internal/NS_MultiScreenBase/AutoExscreenControlStatus.h"
#include <NS_MultiScreenBase/AutoConfigScreenMode.h>
#include <NS_MultiScreenBase/AutoMultiScreenConfig.h>
#include <hsl/HSL.h>
#include <hsl/MultiScreenService.h>
#include <cassert>
#include <cstdint>
#include <cstring>

namespace {
void* vtable[0x19c / 4] = {};
struct FakeView { void** functions; } view = {vtable};
unsigned char configBytes[0x340] = {};
bool haveView = false;
int scene = 0; // UI slot differs from the singleton's internal scene +0x310.
int cross = 99;
int eagleEye = 99;
int preview = 99;
int requestedEngine = -1;
int readCount = 0;
float zoomValue = 8.9f;
int mapMode = 1;
float zoom(void* self) { assert(self == &view); return zoomValue; }
int getMode(void* self) { assert(self == &view); return mapMode; }
void writeWord(unsigned offset, int value) {
    std::memcpy(configBytes + offset, &value, sizeof(value));
}
}
namespace hsl {
MapView* HSL::GetMapViewHandle(bl::MapEngineID id) {
    requestedEngine = static_cast<int>(id);
    return haveView ? reinterpret_cast<MapView*>(&view) : nullptr;
}
MultiScreenService* MultiScreenService::GetInstance() {
    return reinterpret_cast<MultiScreenService*>(configBytes);
}
int MultiScreenService::GetWindowId(bl::MapEngineID id) {
    assert(static_cast<int>(id) == 5);
    return 42;
}
}
namespace NS_MultiScreenBase {
AutoMultiScreenConfig* AutoMultiScreenConfig::GetInstance(ScreenType screen) {
    assert(screen == SCREEN_TYPE_MAIN);
    return reinterpret_cast<AutoMultiScreenConfig*>(configBytes);
}
AutoConfigScreenMode* AutoConfigScreenMode::GetInstance() {
    return reinterpret_cast<AutoConfigScreenMode*>(configBytes);
}
int AutoConfigScreenMode::GetCurrentScreenMode(ScreenType screen) {
    assert(screen == SCREEN_TYPE_MAIN);
    return 2;
}
int* AutoExscreenUtil_GetCurrentSceneSlot(ScreenType) { return &scene; }
int AutoExscreenUtil_GetMapParamsFromUiConfig(ScreenType, int mode,
                                               int selected, int field) {
    assert(mode == 2 && selected == scene);
    ++readCount;
    if (field == 2) return cross;
    if (field == 3) return eagleEye;
    if (field == 4) return preview;
    assert(false);
    return 99;
}
}
int main() {
    vtable[0x198 / 4] = reinterpret_cast<void*>(&zoom);
    vtable[0x98 / 4] = reinterpret_cast<void*>(&getMode);
    writeWord(0x30c, 0);
    writeWord(0x310, 1);
    writeWord(0x314, 3);
    writeWord(0xe4 + 0x9c, 7);
    configBytes[0xe4 + 0xc7] = 1;
    tagSTExtScreenControl status{};
    status.words[7] = 123;
    status.words[8] = 456;
    const auto unchanged = status;
    using NS_MultiScreenBase::AutoExscreenUtil_FillControlStatus;
    AutoExscreenUtil_FillControlStatus(NS_MultiScreenBase::SCREEN_TYPE_MAIN,
                                        5, status);
    assert(status == unchanged && requestedEngine == 5 && readCount == 0);

    haveView = true;
    AutoExscreenUtil_FillControlStatus(NS_MultiScreenBase::SCREEN_TYPE_MAIN,
                                        5, status);
    const int expected[] = {42, 3, 8, 1, 1, 99, 3, 123, 456};
    for (int i = 0; i < 9; ++i) assert(status.words[i] == expected[i]);
    assert(readCount == 3);
    cross = 0;
    eagleEye = 0;
    preview = 1;
    AutoExscreenUtil_FillControlStatus(NS_MultiScreenBase::SCREEN_TYPE_MAIN,
                                        5, status);
    assert(status.words[3] == 0 && status.words[4] == 0 && status.words[5] == 1);
    writeWord(0xe4 + 0x9c, 0);
    cross = 1;
    AutoExscreenUtil_FillControlStatus(NS_MultiScreenBase::SCREEN_TYPE_MAIN,
                                        5, status);
    assert(status.words[3] == 0);

    using NS_MultiScreenBase::AutoExscreenUtil_ProvisionalControlStatus;
    haveView = false;
    status.words[1] = 99;
    status.words[2] = -3;
    AutoExscreenUtil_ProvisionalControlStatus(status);
    assert(status.words[1] == 99 && status.words[2] == -3);
    haveView = true;
    AutoExscreenUtil_ProvisionalControlStatus(status);
    assert(requestedEngine == 1 && status.words[1] == 1 && status.words[2] == 7);
    status.words[1] = 3;
    status.words[2] = -2;
    zoomValue = 19.2f;
    mapMode = 2;
    AutoExscreenUtil_ProvisionalControlStatus(status);
    assert(status.words[1] == 2 && status.words[2] == -2);
    status.words[2] = 0;
    AutoExscreenUtil_ProvisionalControlStatus(status);
    assert(status.words[2] == 19);
}
