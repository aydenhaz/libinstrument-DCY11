// Tests only corrected host-call boundaries; teardown is still incomplete.
#include <cassert>
#include <cstdarg>
#include <cstdint>
#include <cstring>
#include <vector>
#include <GFrame/CGAsyncObject.h>
#include <GFrame/CGPackage.h>
#include <GFrame/CGWorkStation.h>
#include <GFrame/CGFrameSimulation.h>
#include <GFrame/CGString.h>
#include <hsl/HSL.h>
#include <hsl/ThirdManager.h>
#include <hsl/MapViewServiceImpl.h>
#include <hsl/UITaskThreadProxy.h>
#include "NS_InstrumentPannelLarge/CGIPLPacket.h"
using NS_InstrumentPannelLarge::CGIPLPacket;
namespace NS_InstrumentPannelLarge {
// The live factory is intentionally absent. This test supplies only the
// publication step after preparing its mocked host lock.
class PacketFactory {
public:
 static void publishForShutdownTest(CGIPLPacket* packet) {
  CGIPLPacket::publishConstructed(packet);
 }
};
}
static void* packet;
static std::vector<int> events;
static void* mapService;
static bool locked;
static bool traceEnabled=true;
static void lockTask(void* self){assert(self==static_cast<unsigned char*>(packet)+8&&!locked);locked=true;events.push_back(11);}
static void unlockTask(void* self){assert(self==static_cast<unsigned char*>(packet)+8&&locked);locked=false;events.push_back(12);}
static void* lockSlots[4]={nullptr,nullptr,reinterpret_cast<void*>(&lockTask),reinterpret_cast<void*>(&unlockTask)};
static bool hasFacility;
static void* serviceSlots[16];
static void* facilitySlots[43];
struct HostObject {void** vtable;};
static HostObject serviceObject={serviceSlots},facilityObject={facilitySlots};
static hsl::MapViewBase* lookup(void* service,int engine){
 assert(service==&serviceObject&&engine==5);events.push_back(7);
 return hasFacility?reinterpret_cast<hsl::MapViewBase*>(&facilityObject):nullptr;
}
static int deviceId(hsl::MapViewBase* facility){assert(static_cast<void*>(facility)==&facilityObject);events.push_back(8);return 17;}

static void emit(void*,int n,int zero,uintptr_t target,uint32_t flag,uint32_t reserved,const char* module,const char* aux,const char* sig,int line,const char* name,...){
 assert(n==8&&zero==0&&target==reinterpret_cast<uintptr_t>(&emit));
 assert(flag==0x08000000&&reserved==0&&!std::strcmp(module,"HMI_PANNELLARGE")&&!aux[0]);
 assert(sig[0]);
 if(!std::strcmp(name,"CGIPLPacket destructor")){assert(line==0xea);events.push_back(0);}
 else{assert(!std::strcmp(name,"deviceId:%d")&&line==0xff);va_list values;va_start(values,name);assert(va_arg(values,int)==17);va_end(values);events.push_back(9);}
}
extern "C" void* I_GetAlcPtr(){if(!traceEnabled)return nullptr;static void* table[]={reinterpret_cast<void*>(&emit)};static void** object=table;return &object;}
CGString::CGString(){}
CGString::~CGString(){}
namespace GNS_FRAME {
CGWorkStation* CGWorkStation::ownWorkStation(){static CGWorkStation ws;return &ws;}
void CGWorkStation::setMapRenderCallback(void (*callback)()){assert(!callback);events.push_back(1);}
void CGWorkStation::setTopRenderCallback(void (*callback)(CGCanvas*)){assert(!callback);events.push_back(2);}
CGFrameSimulation* CGFrameSimulation::ownWorkStation(){static CGFrameSimulation ws;return &ws;}
void CGFrameSimulation::showFpsInfo(bool enabled,CGString&,int size){assert(!enabled&&size==24);events.push_back(3);}
void CGFrameSimulation::sendMsg(int,int,int,unsigned long long,GPI::ScopedRefptr<GPI::MessageCtrl>*){}
void CGAsyncObject::cancelScheduleTaskHelper(void* identity){
 assert(static_cast<void*>(this)==packet&&locked);
 void (CGIPLPacket::*method)()=&CGIPLPacket::refreshGFramePaint;uintptr_t entry;std::memcpy(&entry,&method,sizeof(entry));
 assert(identity==reinterpret_cast<void*>(entry));events.push_back(4);
}
void CGPackage::closeAllFragments(){assert(static_cast<void*>(this)==packet&&!locked);events.push_back(5);}
}
namespace hmi {
UITaskThreadProxy::UITaskThreadProxy(int){}
UITaskThreadProxy::~UITaskThreadProxy(){events.push_back(14);}
}
struct TrackedProxy:hmi::UITaskThreadProxy {
 TrackedProxy():hmi::UITaskThreadProxy(0){}
 ~TrackedProxy() override {
  assert(CGIPLPacket::activePacket()==packet);events.push_back(13);
 }
};
namespace hsl {
void* HSL::GetMapServiceHandle(){return mapService;}
ThirdManager* HSL::Get3rdHandle(){static ThirdManager service;return &service;}
void MapViewServiceImpl::DestroyDisplayFacility(MapViewBase* facility){assert(static_cast<void*>(this)==&serviceObject&&static_cast<void*>(facility)==&facilityObject);events.push_back(10);}
void ThirdManager::RemoveUITaskThread(UI_TASK_DEVICE_ID device){assert(device==UI_TASK_DEVICE_INSTRUMENT);events.push_back(6);}
}
static void prepareLock(CGIPLPacket& object){
 packet=&object;void** table=lockSlots;
 std::memcpy(static_cast<unsigned char*>(packet)+8,&table,sizeof(table));
}
int main(){
 CGIPLPacket object;assert(CGIPLPacket::activePacket()==nullptr);
 prepareLock(object);
 NS_InstrumentPannelLarge::PacketFactory::publishForShutdownTest(&object);
 assert(CGIPLPacket::activePacket()==&object);
 serviceSlots[15]=reinterpret_cast<void*>(&lookup);facilitySlots[42]=reinterpret_cast<void*>(&deviceId);
 mapService=nullptr;object.onDestroy();assert((events==std::vector<int>{0,1,2,3,11,4,12,5,6}));
 events.clear();mapService=&serviceObject;hasFacility=false;object.onDestroy();assert((events==std::vector<int>{0,1,2,3,11,4,12,5,7,6}));
 events.clear();hasFacility=true;object.onDestroy();assert((events==std::vector<int>{0,1,2,3,11,4,12,5,7,8,9,10,6}));
 events.clear();traceEnabled=false;object.onDestroy();assert((events==std::vector<int>{1,2,3,11,4,12,5,7,8,10,6}));
 assert(CGIPLPacket::activePacket()==nullptr);
 CGIPLPacket withProxy;assert(CGIPLPacket::activePacket()==nullptr);
 prepareLock(withProxy);
 NS_InstrumentPannelLarge::PacketFactory::publishForShutdownTest(&withProxy);
 assert(CGIPLPacket::activePacket()==&withProxy);
 hmi::UITaskThreadProxy* proxy=new TrackedProxy;
 auto* slot=static_cast<unsigned char*>(packet)+CGIPLPacket::taskProxyOffset();
 std::memcpy(slot,&proxy,sizeof(proxy));
 events.clear();traceEnabled=true;mapService=nullptr;withProxy.onDestroy();
 assert((events==std::vector<int>{0,1,2,3,11,4,12,5,6,13,14}));
 std::memcpy(&proxy,slot,sizeof(proxy));assert(!proxy&&CGIPLPacket::activePacket()==nullptr);
 events.clear();withProxy.onDestroy();assert((events==std::vector<int>{0,1,2,3,11,4,12,5,6}));

}
