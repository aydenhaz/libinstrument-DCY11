// Tests only corrected host-call boundaries; teardown is still incomplete.
#include <cassert>
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
#include "NS_InstrumentPannelLarge/CGIPLPacket.h"
using NS_InstrumentPannelLarge::CGIPLPacket;
static void* packet;
static std::vector<int> events;
static void emit(void*,int n,int zero,uintptr_t target,uint32_t flag,uint32_t reserved,const char* module,const char* aux,const char* sig,int line,const char* name){
 assert(n==8&&zero==0&&target==reinterpret_cast<uintptr_t>(&emit));
 assert(flag==0x08000000&&reserved==0&&!std::strcmp(module,"HMI_PANNELLARGE")&&!aux[0]);
 assert(!std::strcmp(name,"onDestroy")&&line==0xea&&sig[0]);events.push_back(0);
}
extern "C" void* I_GetAlcPtr(){static void* table[]={reinterpret_cast<void*>(&emit)};static void** object=table;return &object;}
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
 assert(static_cast<void*>(this)==packet);
 void (CGIPLPacket::*method)()=&CGIPLPacket::refreshGFramePaint;uintptr_t entry;std::memcpy(&entry,&method,sizeof(entry));
 assert(identity==reinterpret_cast<void*>(entry));events.push_back(4);
}
void CGPackage::closeAllFragments(){assert(static_cast<void*>(this)==packet);events.push_back(5);}
}
namespace hsl {
HSL* HSL::Get3rdHandle(){static HSL service;return &service;}
void ThirdManager::RemoveUITaskThread(UI_TASK_DEVICE_ID device){assert(device==UI_TASK_DEVICE_INSTRUMENT);events.push_back(6);}
}
int main(){CGIPLPacket object;packet=&object;object.onDestroy();assert((events==std::vector<int>{0,1,2,3,4,5,6}));}
