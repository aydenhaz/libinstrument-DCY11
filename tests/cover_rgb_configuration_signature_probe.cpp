#include "../internal/abi/CoverCaptureResult.h"
void configureCover(const IPL_ABI::CoverRgbAverage& value){IPL_ABI::writeCoverRgbConfiguration(value);}

IPL_ABI::CoverCaptureApply applyCapture(int event,const uint8_t* data,std::size_t length,int w,int h,int stride,bool& requested,bool& ready){
 return IPL_ABI::applyCoverCaptureResult(event,data,length,w,h,stride,requested,ready);
}
