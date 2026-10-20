#pragma once
#include <GFrame/GPIClosure.h>
namespace GNS_FRAME {
class CGAsyncObject {
public:
    // V21 libGFrame.so 0x1fee2d; instance method, 64-bit source token.
    void scheduleTaskHelper(GPI::Closure::Callable<void()>&, int, int, int, void*, unsigned long long);
    void cancelScheduleTaskHelper(void*);
};
}
