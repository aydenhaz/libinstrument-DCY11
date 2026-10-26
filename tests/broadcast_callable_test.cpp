#include "abi/CruiseBroadcastCallable.h"
#include <cassert>
#include <cstring>
namespace GNS_FRAME { struct STGBroadcastMsg_ {int value;}; }
struct Receiver {uintptr_t* table; int calls; const GNS_FRAME::STGBroadcastMsg_* seen;};
static void receive(void* self,const GNS_FRAME::STGBroadcastMsg_& message) {
    auto* r=static_cast<Receiver*>(self);++r->calls;r->seen=&message;
}
int main() {
    uintptr_t table[]={reinterpret_cast<uintptr_t>(&receive),reinterpret_cast<uintptr_t>(&receive)};
    Receiver r{table,0,nullptr},other{table,0,nullptr}; GNS_FRAME::STGBroadcastMsg_ msg{123};
    GPI::BroadcastCallable direct(&r,{reinterpret_cast<uintptr_t>(&receive),0});
    GPI::BroadcastCallable virt(&r,{0,1});
    auto copy=virt; assert(copy.equals(virt) && !copy.equals(direct));
    copy.invoke(msg);assert(r.calls==1 && r.seen==&msg);
    virt.reset();assert(virt.empty() && !copy.empty());
    copy.invoker()->invoke(msg);assert(r.calls==2);
    GPI::BroadcastCallable wrongTarget(&other,{0,1}),wrongSlot(&r,{sizeof(void*),1});
    assert(!copy.equals(wrongTarget) && !copy.equals(wrongSlot));
    auto moved=std::move(copy);assert(copy.empty() && !moved.empty());
    direct=direct;direct.invoke(msg);assert(r.calls==3);
    direct=moved;direct.invoke(msg);assert(r.calls==4);
    auto* wrapper=new GPI::detail::BroadcastMemberWrapper(&r,{0,1});
    auto* cloned=wrapper->clone();delete wrapper;cloned->invoke(msg);delete cloned;
    assert(r.calls==5);
    unsigned char* interior=reinterpret_cast<unsigned char*>(&r)+sizeof(void*);
    GPI::BroadcastCallable adjusted(interior,{reinterpret_cast<uintptr_t>(&receive),-intptr_t(sizeof(void*))*2});
    adjusted.invoke(msg);assert(r.calls==6);
    GPI::BroadcastMemberAddress nullA{0,0},nullB{0,8},virtualZero{0,1};
    assert(nullA==nullB && !(nullA==virtualZero));
    auto first=IPL_ABI::makeCruiseBroadcastCallable(&r,IPL_ABI::CruiseBroadcastSlot::First);
    auto second=IPL_ABI::makeCruiseBroadcastCallable(&r,IPL_ABI::CruiseBroadcastSlot::Second);
    auto again=IPL_ABI::makeCruiseBroadcastCallable(&r,IPL_ABI::CruiseBroadcastSlot::First);
    assert(first.equals(again) && !first.equals(second));
    GPI::BroadcastMemberAddress descriptor;
    std::memcpy(&descriptor,first.invoker()->methodAddress(),sizeof descriptor);
    assert(descriptor.function==0xe4 && descriptor.adjustment==1);
    moved.reset();direct.reset();assert(direct.empty());
}
