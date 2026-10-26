#include "CruiseBroadcastBindings.h"
#if defined(__arm__) && !defined(__aarch64__)
#include "BroadcastList.h"
#include "CruiseBroadcastCallable.h"
#include <GFrame/CGBroadcaster.h>
#include <cstring>
namespace IPL_ABI {
namespace {
void bind(void* fragment, uint32_t id, CruiseBroadcastSlot slot, bool add) {
    void* broadcaster = GNS_FRAME::CGBroadcaster::getInstance(id);
    auto callable = makeCruiseBroadcastCallable(fragment,slot);
    BroadcastListState* state = resolveBroadcastList(broadcaster);
    uint32_t channel;
    std::memcpy(&channel,static_cast<unsigned char*>(broadcaster)+4,4);
    if (add) addBroadcastCallable(*state,channel,callable);
    else removeBroadcastCallable(*state,channel,callable);
}
}
void attachCruiseBroadcasts(void* fragment) {
    bind(fragment,0x10,CruiseBroadcastSlot::First,true);
    bind(fragment,0x10004,CruiseBroadcastSlot::Second,true);
}
void detachCruiseBroadcasts(void* fragment) {
    bind(fragment,0x10,CruiseBroadcastSlot::First,false);
    bind(fragment,0x10004,CruiseBroadcastSlot::Second,false);
}
}
#endif
