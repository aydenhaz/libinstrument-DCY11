#include "AutoExscreenMapTimer.h"
#include <GFrame/CGUITimer.h>
#include <cstdint>
#include <cstring>

namespace NS_MultiScreenBase {

bool AutoExscreenUtil_TryStartMapStatusTimer(void* state) {
    if (!state) return false;
    unsigned char* bytes = static_cast<unsigned char*>(state);
    int32_t interval;
    std::memcpy(&interval, bytes + 4, sizeof(interval));
    if (interval < 1 || bytes[0] || bytes[1]) return false;

    uint64_t deadline;
    std::memcpy(&deadline, bytes + 8, sizeof(deadline));
    GNS_FRAME::CGUITimer* timer =
        reinterpret_cast<GNS_FRAME::CGUITimer*>(bytes + 0x10);
    if (!timer->start(interval, deadline)) return false;
    bytes[0] = 1;
    return true;
}

}
