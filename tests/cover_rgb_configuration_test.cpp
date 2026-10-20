#include "../internal/abi/CoverCaptureResult.h"
#include "../internal/NS_MultiScreenBase/CoverUtil.h"
#include <cassert>
#include <cstdarg>
#include <string>
#include <vector>
CGString::CGString() {}
CGString::~CGString() {}
namespace NS_MultiScreenBase {
struct CoverUtilTestAccess {
 static void prepare(CoverUtil& c,uint8_t* data){c.bytes_=data;c.width_=2;c.height_=1;c.length_=8;c.stride_=4;c.image_=&c;c.enabled_=1;c.requested_=1;c.state_=0;c.counter_=9;c.surface_.scalar[7]=7;}
 static bool defaults(const CoverUtil& c){return c.threshold_==2&&!c.image_&&!c.enabled_&&!c.requested_&&!c.state_&&!c.counter_&&c.unknown94_==1&&c.surface_.scalar[7]==0xffffffffu;}
 static bool requested(const CoverUtil& c){return c.requested_;}
 static bool ready(const CoverUtil& c){return c.state_;}
 static int count(const CoverUtil& c){return c.counter_;}
};
}
static std::vector<std::string> calls;
static std::string text(const dice::String16& value){
    std::string out;
    for(unsigned i=0;i<value.size();++i)out+=char(value.c_str()[i]);
    return out;
}
dice::String16 hsl::StringUtils::Format(const unsigned short* format,...){
    assert(format[0]=='%'&&format[1]=='d'&&format[2]==0);
    va_list args;va_start(args,format);int value=va_arg(args,int);va_end(args);
    calls.push_back("format:"+std::to_string(value));
    return dice::String16(std::to_string(value).c_str());
}
void CAutoUtil::setSMConfigValue(const dice::String16& key,const dice::String16& value){
    calls.push_back(text(key)+"="+text(value));
}
int main(){
    IPL_ABI::writeCoverRgbConfiguration({0,127,255});
    const std::vector<std::string> expected={"format:0","format:127","format:255","widgetCoverR=0","widgetCoverG=127","widgetCoverB=255"};
    assert(calls==expected);
    calls.clear();
    const uint8_t image[]={0,127,255,33,0,127,255,44};
    bool requested=true,ready=false;
    assert(IPL_ABI::applyCoverCaptureResult(22,image,8,2,1,4,requested,ready)==IPL_ABI::CoverCaptureApply::IgnoredEvent);
    assert(calls.empty() && requested && !ready);
    assert(IPL_ABI::applyCoverCaptureResult(23,image,8,2,1,4,requested,ready)==IPL_ABI::CoverCaptureApply::Applied);
    assert(!requested && ready && calls==expected);
    calls.clear();requested=true;ready=false;
    assert(IPL_ABI::applyCoverCaptureResult(23,nullptr,8,2,1,4,requested,ready)==IPL_ABI::CoverCaptureApply::InvalidImage);
    assert(calls.empty() && !requested && ready);
    NS_MultiScreenBase::CoverUtil cover;
    using Access=NS_MultiScreenBase::CoverUtilTestAccess;
    assert(Access::defaults(cover));
    uint8_t pixels[]={0,127,255,0,0,127,255,255};
    Access::prepare(cover,pixels);
    hsl::MapSurfaceViewObserver* map=&cover;
    bl::IBLMapBusinessDataObserver* business=&cover;
    assert(reinterpret_cast<char*>(business)-reinterpret_cast<char*>(&cover)==sizeof(void*));
    map->MapSurfaceViewStatusChange(8,static_cast<MAPSURFACE>(1),nullptr,0,0,0);
    assert(Access::count(cover)==9);
    map->MapSurfaceViewStatusChange(7,static_cast<MAPSURFACE>(1),nullptr,0,0,0);
    assert(Access::count(cover)==0);
    calls.clear();
    assert(business->onBusinessDataObserver(23,nullptr,nullptr));
    assert(!Access::requested(cover)&&Access::ready(cover)&&calls==expected);
    assert(!cover.isUIFirstRenderFinish());
}
