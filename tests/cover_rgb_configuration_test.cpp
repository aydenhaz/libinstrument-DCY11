#include "../internal/abi/CoverCaptureResult.h"
#include <cassert>
#include <cstdarg>
#include <string>
#include <vector>
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
}
