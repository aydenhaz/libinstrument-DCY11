#include "../internal/abi/CoverRgbAverage.h"
#include <cassert>
int main(){
    const uint8_t rgba[]={10,20,30,255,30,40,50,0};
    IPL_ABI::CoverRgbAverage out{};
    assert(IPL_ABI::coverRgbAverage(rgba,8,2,1,4,out));
    assert(out.red==20&&out.green==30&&out.blue==40);
    assert(!IPL_ABI::coverRgbAverage(rgba,7,2,1,4,out));
    assert(!IPL_ABI::coverRgbAverage(nullptr,8,2,1,4,out));
    assert(!IPL_ABI::coverRgbAverage(rgba,8,0,1,4,out));
    assert(!IPL_ABI::coverRgbAverage(rgba,8,2,1,0,out));
    assert(!IPL_ABI::coverRgbAverage(rgba,8,2147483647,2147483647,4,out));
    bool requested=true,ready=false;
    IPL_ABI::coverCaptureResultState(22,requested,ready);assert(requested&&!ready);
    IPL_ABI::coverCaptureResultState(23,requested,ready);assert(!requested&&ready);
}
