#include "../internal/NS_MultiScreenBase/OpeningCoverBinding.h"
#include "../internal/NS_MultiScreenBase/CoverUtil.h"
#include <GFrame/CGFragment.h>
#include <hsl/HSL.h>
#include <cassert>
#include <cstring>
#include <new>
#include <vector>
using namespace NS_MultiScreenBase;
CoverUtil::CoverUtil()=default;
CoverUtil::~CoverUtil()=default;
void CoverUtil::MapSurfaceViewStatusChange(int,MAPSURFACE,void*,int,int,int){assert(false);}
bool CoverUtil::onBusinessDataObserver(int,const void*,void*){assert(false);return false;}
static int copies;
CGString::CGString(){} CGString::~CGString(){}
CGString::CGString(const CGString&){++copies;}
alignas(16) static unsigned char storage[0x440];
static std::vector<int> calls;
static void* service;
static void* replacement;
static CoverUtil* target;
static bool mutate;
static void put(unsigned offset,void* value){std::memcpy(storage+offset,&value,sizeof(value));}
static void* get(unsigned offset){void* value;std::memcpy(&value,storage+offset,sizeof(value));return value;}
void* hsl::HSL::GetMapServiceHandle(){calls.push_back(1);return service;}
GNS_FRAME::CGContext* GNS_FRAME::CGFragment::getContext(){
 assert(reinterpret_cast<void*>(this)==storage);calls.push_back(4);
 if(mutate){put(0x388,replacement);put(0x40c,nullptr);}
 return reinterpret_cast<CGContext*>(0x1230);
}
static uint32_t deviceId(void*){calls.push_back(2);return 0xfedcba98;}
static bl::IMapDevice* device(void*,uint32_t id){assert(id==0xfedcba98);calls.push_back(3);return nullptr;}
static bl::IMapView* view(void* receiver){assert(!mutate||receiver==replacement);calls.push_back(5);return reinterpret_cast<bl::IMapView*>(0x4560);}
void CoverUtil::setConfig(GNS_FRAME::CGContext* context,bl::IMapDevice* device,bl::IMapView* view,SurfaceConfig config){
 assert(this==target&&context==reinterpret_cast<void*>(0x1230)&&!device&&view==reinterpret_cast<void*>(0x4560));
 assert(config.scalar[0]==800&&config.scalar[1]==480&&config.scalar[7]==7);calls.push_back(6);
}
int main(){
 void* facilityTable[43]={};facilityTable[42]=reinterpret_cast<void*>(&deviceId);facilityTable[9]=reinterpret_cast<void*>(&view);
 void* serviceTable[9]={};serviceTable[8]=reinterpret_cast<void*>(&device);
 void* facility=facilityTable;void* facility2=facilityTable;void* serviceObject=serviceTable;replacement=&facility2;
 auto* config=new(storage+0x3a0) SurfaceConfig;config->scalar[0]=800;config->scalar[1]=480;config->scalar[7]=7;
 CoverUtil receiver;target=&receiver;
 bindOpeningCover(storage,nullptr);assert(calls.empty()&&!get(0x40c));
 bindOpeningCover(storage,target);assert((calls==std::vector<int>{1})&&get(0x40c)==target);
 calls.clear();service=&serviceObject;bindOpeningCover(storage,target);assert((calls==std::vector<int>{1}));
 for(bool change:{false,true}){
  mutate=change;put(0x388,&facility);calls.clear();int before=copies;
  bindOpeningCover(storage,target);assert((calls==std::vector<int>{1,2,3,4,5,6}));assert(copies==before+1);
 }
 config->~SurfaceConfig();
}
