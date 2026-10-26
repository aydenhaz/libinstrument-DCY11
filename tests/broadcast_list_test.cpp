#include "abi/BroadcastList.h"
#include <cassert>
#include <thread>
#include <vector>
using namespace IPL_ABI;
int main() {
    BroadcastListState state{{nullptr,nullptr},0,{}};
    state.sentinel.next=state.sentinel.previous=&state.sentinel;
    int firstTarget=0,secondTarget=0;
    GPI::BroadcastCallable first(&firstTarget,{0xe4,1}),second(&firstTarget,{0xec,1}),other(&secondTarget,{0xe4,1});
    addBroadcastCallable(state,16,first);assert(state.count==1);
    auto* oldest=state.sentinel.next;
    addBroadcastCallable(state,16,first);assert(state.count==1 && state.sentinel.next==oldest);
    addBroadcastCallable(state,17,first);addBroadcastCallable(state,16,second);addBroadcastCallable(state,16,other);
    assert(state.count==4 && state.sentinel.previous==oldest);
    removeBroadcastCallable(state,99,first);assert(state.count==4);
    removeBroadcastCallable(state,16,first);assert(state.count==3);
    removeBroadcastCallable(state,16,first);assert(state.count==3);
    removeBroadcastCallable(state,17,first);removeBroadcastCallable(state,16,second);removeBroadcastCallable(state,16,other);
    assert(state.count==0 && state.sentinel.next==&state.sentinel && state.sentinel.previous==&state.sentinel);
    {GPI::BroadcastCallable transient(&firstTarget,{0xe4,1});addBroadcastCallable(state,16,transient);}
    removeBroadcastCallable(state,16,first);assert(state.count==0);
    std::vector<std::thread> threads;
    for(unsigned channel=0;channel<4;++channel)threads.emplace_back([&,channel] {
        for(int i=0;i<1000;++i) {addBroadcastCallable(state,channel,first);addBroadcastCallable(state,channel,first);removeBroadcastCallable(state,channel,first);}
    });
    for(auto& thread:threads)thread.join();
    assert(state.count==0 && state.sentinel.next==&state.sentinel && state.sentinel.previous==&state.sentinel);
    // Publisher disposer must also clean up still-registered nodes.
    auto* owned=createBroadcastListState();
    assert(owned->count==0 && owned->sentinel.next==&owned->sentinel);
    addBroadcastCallable(*owned,16,first);
    addBroadcastCallable(*owned,17,second);
    destroyBroadcastListState(owned);
    destroyBroadcastListState(createBroadcastListState());
    destroyBroadcastListState(nullptr);
    assert(!first.empty() && !second.empty());

}
