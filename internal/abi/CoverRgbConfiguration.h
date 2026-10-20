#pragma once
#include "CoverRgbAverage.h"
#include <hsl/StringUtils.h>
#include <CAutoUtil.h>
namespace IPL_ABI {
// V21 0x17f2e..0x17fbc: format all values first, then write R/G/B.
// Only configuration side effects; observer construction/dispatch remains separate.
inline void writeCoverRgbConfiguration(const CoverRgbAverage& average) {
    const auto* format=reinterpret_cast<const unsigned short*>(u"%d");
    const dice::String16 red=hsl::StringUtils::Format(format,average.red);
    const dice::String16 green=hsl::StringUtils::Format(format,average.green);
    const dice::String16 blue=hsl::StringUtils::Format(format,average.blue);
    {const dice::String16 key(reinterpret_cast<const unsigned short*>(u"widgetCoverR"));
     CAutoUtil::setSMConfigValue(key,red);}
    {const dice::String16 key(reinterpret_cast<const unsigned short*>(u"widgetCoverG"));
     CAutoUtil::setSMConfigValue(key,green);}
    {const dice::String16 key(reinterpret_cast<const unsigned short*>(u"widgetCoverB"));
     CAutoUtil::setSMConfigValue(key,blue);}
}
}
