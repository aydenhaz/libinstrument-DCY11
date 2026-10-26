#include "BroadcastList.h"
#if defined(__arm__) && !defined(__aarch64__)
#include <cstring>
extern "C" void* v21PublisherGet(void*, void*(*)(), void(*)(void*))
    __asm__("_ZN3GPI9Broadcast8internal9Publisher3getEPvPFS3_vEPFvS3_E");
namespace IPL_ABI {
namespace {
void* createState() { return createBroadcastListState(); }
void destroyState(void* state) { destroyBroadcastListState(static_cast<BroadcastListState*>(state)); }
}
BroadcastListState* resolveBroadcastList(void* broadcaster) {
    void** table;
    std::memcpy(&table,broadcaster,sizeof table);
    void* publisher = reinterpret_cast<void*(*)(void*)>(table[0])(broadcaster);
    return static_cast<BroadcastListState*>(v21PublisherGet(publisher,createState,destroyState));
}
}
#endif
