#include "CruiseBroadcastCallable.h"
namespace IPL_ABI {
GPI::BroadcastCallable makeCruiseBroadcastCallable(void* fragment, CruiseBroadcastSlot slot) {
    return GPI::BroadcastCallable(fragment, {slot == CruiseBroadcastSlot::First ? 0xe4u : 0xecu, 1});
}
}
