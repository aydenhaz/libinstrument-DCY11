#pragma once
#include <GFrame/GPIBroadcastCallable.h>
namespace IPL_ABI {
enum class CruiseBroadcastSlot { First, Second };
GPI::BroadcastCallable makeCruiseBroadcastCallable(void* fragment, CruiseBroadcastSlot slot);
}
