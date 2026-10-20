#pragma once
#include <cstddef>
#include <cstdint>
#include <limits>

namespace IPL_ABI {
struct CoverRgbAverage { int32_t red, green, blue; };
// Valid RGB/RGBA image domain of V21 0x17ed6..0x17f68. Not a complete
// business-observer callback. Original callback logging/services remain external.
inline bool coverRgbAverage(const uint8_t* bytes, std::size_t length,
        int32_t width, int32_t height, int32_t stride, CoverRgbAverage& out) {
    if (!bytes || width<=0 || height<=0 || (stride!=3 && stride!=4)) return false;
    const uint64_t pixels=uint64_t(width)*uint64_t(height);
    if (pixels>uint64_t(std::numeric_limits<int32_t>::max()) ||
        pixels*uint64_t(stride)!=length) return false;
    float red=0,green=0,blue=0;
    for(std::size_t offset=0;offset<length;offset+=stride) {
        red+=bytes[offset];green+=bytes[offset+1];blue+=bytes[offset+2];
    }
    const float divisor=static_cast<float>(static_cast<int32_t>(pixels));
    out={static_cast<int32_t>(red/divisor),static_cast<int32_t>(green/divisor),
         static_cast<int32_t>(blue/divisor)};
    return true;
}
// V21 0x17ec2..0x17ece: only event 0x17 clears request and sets state flag.
inline void coverCaptureResultState(int32_t event,bool& requested,bool& stateFlag) {
    if(event==0x17) {requested=false;stateFlag=true;}
}
}
