#include "../internal/NS_MultiScreenBase/AutoExscreenMapTimer.h"
#include <GFrame/CGUITimer.h>
#include <cassert>
#include <cstdint>
#include <cstring>
#include <new>

namespace {
alignas(8) unsigned char state[0x40] = {};
int callCount = 0;
int actualInterval = 0;
unsigned long long actualDeadline = 0;
bool startResult = false;

void setInterval(int32_t value) {
    std::memcpy(state + 4, &value, sizeof(value));
}
void setDeadline(uint64_t value) {
    std::memcpy(state + 8, &value, sizeof(value));
}
}

namespace GNS_FRAME {
bool CGUITimer::start(int interval, unsigned long long deadline) {
    assert(this == reinterpret_cast<CGUITimer*>(state + 0x10));
    ++callCount;
    actualInterval = interval;
    actualDeadline = deadline;
    return startResult;
}
}

int main() {
    new (state + 0x10) GNS_FRAME::CGUITimer();
    using NS_MultiScreenBase::AutoExscreenUtil_TryStartMapStatusTimer;
    assert(!AutoExscreenUtil_TryStartMapStatusTimer(nullptr));
    setInterval(0);
    assert(!AutoExscreenUtil_TryStartMapStatusTimer(state));
    setInterval(-1);
    assert(!AutoExscreenUtil_TryStartMapStatusTimer(state));
    setInterval(250);
    state[0] = 1;
    assert(!AutoExscreenUtil_TryStartMapStatusTimer(state));
    state[0] = 0;
    state[1] = 1;
    assert(!AutoExscreenUtil_TryStartMapStatusTimer(state));
    state[1] = 0;
    assert(callCount == 0);

    setDeadline(0x1122334455667788ULL);
    assert(!AutoExscreenUtil_TryStartMapStatusTimer(state));
    assert(callCount == 1 && actualInterval == 250 &&
           actualDeadline == 0x1122334455667788ULL && state[0] == 0);
    startResult = true;
    assert(AutoExscreenUtil_TryStartMapStatusTimer(state));
    assert(callCount == 2 && state[0] == 1 && state[1] == 0);
    assert(!AutoExscreenUtil_TryStartMapStatusTimer(state));
    assert(callCount == 2);
}
