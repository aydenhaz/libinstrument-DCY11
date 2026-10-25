#include <cassert>
#include <cstring>
#include <cstdint>
#include <GFrame/CGFrameSimulation.h>
#include "NS_InstrumentPannelLarge/CGIPLPacket.h"
static int calls,traceCalls,loaded;
static void emit(void*,int count,int reserved,uintptr_t r3,uint32_t flag,uint32_t zero,const char* module,const char* aux,const char* sig,int line,const char* name){
 assert(count==8&&reserved==0&&flag==0x08000000&&zero==0);
 assert(!std::strcmp(module,"HMI_PANNELLARGE")&&!aux[0]&&sig[0]);
 if(!std::strcmp(name,"refreshGFramePaint")){assert(line==0xdf&&r3==reinterpret_cast<uintptr_t>(&emit));}
 else if(!std::strcmp(name,"onPostLoadRes")){assert(line==0x11a&&r3==reinterpret_cast<uintptr_t>(&emit));assert(!std::strcmp(sig,"virtual void NS_InstrumentPannelLarge::CGIPLPacket::onPostLoadRes()"));}
 else{assert(!std::strcmp(name,"onPostAlreadyLoadRes")&&line==0x115&&r3==0x08000000);}
 ++traceCalls;
}
extern "C" void* I_GetAlcPtr(){static void* table[]={reinterpret_cast<void*>(&emit)};static void** obj=table;return &obj;}
namespace GNS_FRAME {
CGFrameSimulation* CGFrameSimulation::ownWorkStation(){static char storage;return reinterpret_cast<CGFrameSimulation*>(&storage);}
void CGFrameSimulation::sendMsg(int code,int a,int b,unsigned long long token,GPI::ScopedRefptr<GPI::MessageCtrl>* ctrl){assert(code==0x30&&a==0&&b==0&&!ctrl);assert((token>>32)==0xe0);++calls;}
}
static void post(NS_InstrumentPannelLarge::CGIPLPacket*){++loaded;}
int main(){
 using NS_InstrumentPannelLarge::CGIPLPacket;
 CGIPLPacket object;
 void* slots[16]={};slots[14]=reinterpret_cast<void*>(&post);void** table=slots;std::memcpy(&object,&table,sizeof(table));
 auto* p=&object;p->refreshGFramePaint();p->onPostAlreadyLoadRes();p->onPostLoadRes();
 assert(calls==1&&loaded==1&&traceCalls==3);
}
