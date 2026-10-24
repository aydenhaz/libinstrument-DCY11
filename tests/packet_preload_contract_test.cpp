#include <cassert>
#include <cstring>
#include <memory>
#include <stdexcept>
#include <GFrame/CGAsyncObject.h>
#include <GFrame/CGWorkStation.h>
#include <GFrame/CGFrameSimulation.h>
#include <hsl/UITaskThreadProxy.h>
#include "NS_InstrumentPannelLarge/CGIPLPacket.h"
using NS_InstrumentPannelLarge::CGIPLPacket;
static void* packet;
static bool locked, fail;
static int paints, topCalls;
static std::unique_ptr<GPI::Closure::Callable<void()>> retained;
namespace hmi { UITaskThreadProxy::UITaskThreadProxy(int) {} UITaskThreadProxy::~UITaskThreadProxy() {} }
extern "C" void* I_GetAlcPtr() { return nullptr; }
static void lock(void* p) { assert(p==static_cast<char*>(packet)+8&&!locked); locked=true; }
static void unlock(void* p) { assert(p==static_cast<char*>(packet)+8&&locked); locked=false; }
namespace NS_MultiScreenBase { void DispatchTopRenderCallback(GNS_FRAME::CGCanvas*) {} }
namespace GNS_FRAME {
CGWorkStation* CGWorkStation::ownWorkStation() { static CGWorkStation s; return &s; }
void CGWorkStation::setTopRenderCallback(void (*p)(CGCanvas*)) {
    assert(p==&CGIPLPacket::TopRenderCallback&&!locked); ++topCalls;
}
void CGAsyncObject::scheduleTaskHelper(GPI::Closure::Callable<void()>& task,
        int delay,int zero,int repeat,void* identity,unsigned long long token) {
    assert(static_cast<void*>(this)==packet&&locked);
    assert(delay==1000&&zero==0&&repeat==-1&&(token>>32)==0x110);
    void (CGIPLPacket::*method)()=&CGIPLPacket::refreshGFramePaint;
    uintptr_t entry; std::memcpy(&entry,&method,sizeof(entry));
    assert(identity==reinterpret_cast<void*>(entry));
    if(fail) throw std::runtime_error("schedule failure");
    retained.reset(new GPI::Closure::Callable<void()>(task));
}
CGFrameSimulation* CGFrameSimulation::ownWorkStation() { static CGFrameSimulation s; return &s; }
void CGFrameSimulation::sendMsg(int code,int a,int b,unsigned long long token,GPI::ScopedRefptr<GPI::MessageCtrl>* p) {
    assert(code==0x30&&!a&&!b&&!p&&(token>>32)==0xe0); ++paints;
}
}
int main() {
    CGIPLPacket object; packet=&object;
    void* entries[]={nullptr,nullptr,reinterpret_cast<void*>(&lock),reinterpret_cast<void*>(&unlock)};
    void** table=entries; std::memcpy(static_cast<char*>(packet)+8,&table,sizeof(table));
    object.onPreLoad(); assert(!locked&&retained&&topCalls==1);
    retained->invoke(); assert(paints==1); retained.reset();
    fail=true;
    try { object.onPreLoad(); assert(false); } catch(const std::runtime_error&) {}
    assert(!locked&&topCalls==2);
}
