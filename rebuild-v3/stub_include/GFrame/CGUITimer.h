#pragma once

namespace GNS_FRAME {
class CGUITimer {
public:
    // V21 libGFrame, called at 0x2c1c0 with the embedded timer at state+0x10.
    bool start(int interval, unsigned long long deadline);
};
}
