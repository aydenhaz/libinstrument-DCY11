#pragma once

#include <cstdint>

namespace IPL_ABI {

struct OpeningActivityGateResult {
    std::uintptr_t secondVirtualResult;
    bool hasAttachedActivityState;
};

// Original onInitEnd 0x2ecdc..0x2ecf8. Requires an actual constructed V21
// CGFragmentActivity. The second virtual return value is preserved for the
// later Fragment stage; the +8 state is checked after both calls.
OpeningActivityGateResult inspectOpeningActivity(void* activity);

} // namespace IPL_ABI
