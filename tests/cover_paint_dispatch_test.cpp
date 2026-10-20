#include "../internal/NS_MultiScreenBase/CoverUtil.h"
#include <hsl/StringUtils.h>
#include <CAutoUtil.h>
#include <cassert>
#include <climits>
#include <vector>
CGString::CGString(){}
CGString::~CGString(){}
dice::String16 hsl::StringUtils::Format(const unsigned short*,...){assert(false);return dice::String16();}
void CAutoUtil::setSMConfigValue(const dice::String16&,const dice::String16&){assert(false);}
namespace NS_MultiScreenBase {
struct CoverUtilTestAccess {
 static void prepare(CoverUtil& c,void* device,int counter,bool state=true,bool inhibit=false,bool requested=false){
  c.image_=&c;c.enabled_=1;c.unknown10_=device;c.counter_=counter;c.state_=state;c.inhibitCapture_=inhibit;c.requested_=requested;
  c.surface_.scalar[0]=640;c.surface_.scalar[1]=480;
 }
 static int count(CoverUtil& c){return c.counter_;}
 static void count(CoverUtil& c,int n){c.counter_=n;}
 static bool requested(CoverUtil& c){return c.requested_;}
 static bl::ScreenShotInfo* info(CoverUtil& c){return &c.screenshot_;}
};
}
using Cover=NS_MultiScreenBase::CoverUtil;
using Access=NS_MultiScreenBase::CoverUtilTestAccess;
struct Fake {void** table;};
static Cover* current;
static Fake* expectedCanvas;
static Fake* expectedDevice;
static std::vector<int> calls;
static bool mutateDraw=false,mutateCapture=false;
static void draw(GNS_FRAME::CGCanvas* canvas,GNS_FRAME::CGImage* image,const GPI::CGRect& rect,GNS_FRAME::EGDrawImageType_ mode,void* style){
 assert(static_cast<void*>(canvas)==expectedCanvas&&static_cast<void*>(image)==current);
 const int expected[]={0,640,0,480};
 for(int i=0;i<4;++i)assert(rect.coordinates[i]==expected[i]);
 assert(static_cast<int>(mode)==0&&style==&Access::info(*current)->tail);
 calls.push_back(1);
 if(mutateDraw)Access::count(*current,10);
}
static bool capture(void* device,const dice::Rect<int32_t>& rect,bl::ScreenShotInfo& info,bl::IBLMapBusinessDataObserver* observer){
 assert(device==expectedDevice&&&info==Access::info(*current));
 assert(observer==static_cast<bl::IBLMapBusinessDataObserver*>(current));
 assert(Access::requested(*current));
 const int expected[]={0,640,0,480};
 for(int i=0;i<4;++i)assert(rect.coordinates[i]==expected[i]);
 calls.push_back(2);
 if(mutateCapture)Access::count(*current,201);
 return false; // Original ignores a false service result and still increments.
}
int main(){
 void* canvasTable[52]{};canvasTable[51]=reinterpret_cast<void*>(&draw);
 void* deviceTable[75]{};deviceTable[74]=reinterpret_cast<void*>(&capture);
 Fake canvas{canvasTable},device{deviceTable};expectedCanvas=&canvas;expectedDevice=&device;
 Cover c;current=&c;
 c.TopRenderCoverCallback(nullptr);assert(calls.empty());
 auto paint=[&](){calls.clear();c.TopRenderCoverCallback(reinterpret_cast<GNS_FRAME::CGCanvas*>(&canvas));};
 Access::prepare(c,&device,2);paint();assert((calls==std::vector<int>{1})&&Access::count(c)==3);
 Access::prepare(c,&device,3);paint();assert(calls.empty()&&Access::count(c)==4);
 Access::prepare(c,&device,10);paint();assert((calls==std::vector<int>{2})&&Access::count(c)==11);
 Access::prepare(c,&device,10,true,true);paint();assert(calls.empty()&&Access::count(c)==11);
 Access::prepare(c,&device,10,true,false,true);paint();assert(calls.empty()&&Access::count(c)==11);
 Access::prepare(c,&device,INT_MAX);paint();assert(calls.empty()&&Access::count(c)==INT_MIN);
 mutateCapture=true;Access::prepare(c,&device,10);paint();assert(Access::count(c)==202);
 mutateCapture=false;mutateDraw=true;Access::prepare(c,&device,2);paint();
 assert((calls==std::vector<int>{1,2})&&Access::count(c)==11);
}
