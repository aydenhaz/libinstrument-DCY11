#pragma once
#include <GFrame/GPIBroadcastCallable.h>
#include <mutex>
#include <cstdint>
#include <cstddef>
namespace IPL_ABI {
struct BroadcastLink { BroadcastLink* next; BroadcastLink* previous; };
// Existing host list state, not the broadcaster object. Its sentinel and
// mutex must already be initialized by the actual host constructor.
struct BroadcastListState {
    BroadcastLink sentinel;
    uint32_t count;
    std::mutex mutex;
};
BroadcastListState* createBroadcastListState();
void destroyBroadcastListState(BroadcastListState*);
#if defined(__arm__) && !defined(__aarch64__)
BroadcastListState* resolveBroadcastList(void* broadcaster);
#endif
void addBroadcastCallable(BroadcastListState&, uint32_t channel, const GPI::BroadcastCallable&);
void removeBroadcastCallable(BroadcastListState&, uint32_t channel, const GPI::BroadcastCallable&);
#if defined(__arm__) && !defined(__aarch64__)
static_assert(sizeof(BroadcastListState)==16,"V21 broadcast list state");
static_assert(offsetof(BroadcastListState,mutex)==12,"V21 broadcast mutex offset");
#endif
}
