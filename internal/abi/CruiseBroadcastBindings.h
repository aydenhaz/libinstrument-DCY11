#pragma once
#if defined(__arm__) && !defined(__aarch64__)
namespace IPL_ABI {
// Caller must supply a constructed Cruise Fragment with the original slots.
void attachCruiseBroadcasts(void* fragment);
void detachCruiseBroadcasts(void* fragment);
}
#endif
