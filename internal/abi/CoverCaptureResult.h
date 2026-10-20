#pragma once
#include "CoverRgbConfiguration.h"
namespace IPL_ABI {
// Internal result status, NOT the return type/signature of the host observer.
enum class CoverCaptureApply { IgnoredEvent, Applied, InvalidImage };
inline CoverCaptureApply applyCoverCaptureResult(int32_t event,
        const uint8_t* bytes, std::size_t length, int32_t width, int32_t height,
        int32_t stride, bool& requested, bool& stateFlag) {
    if(event!=0x17) return CoverCaptureApply::IgnoredEvent;
    // Original updates these flags before image arithmetic/configuration calls.
    coverCaptureResultState(event,requested,stateFlag);
    CoverRgbAverage average;
    if(!coverRgbAverage(bytes,length,width,height,stride,average))
        return CoverCaptureApply::InvalidImage;
    writeCoverRgbConfiguration(average);
    return CoverCaptureApply::Applied;
}
}
