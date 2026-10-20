#pragma once
#include <hsl/HSL.h>
#include <hsl/Config.h>
#include <hsl/StringUtils.h>
#include <asl/String16Utils.h>
namespace IPL_ABI {
inline void readCoverConfigPolicy(int32_t surfaceId,dice::String16& enabledWindows,
        int32_t& threshold,uint8_t& enabled) {
    enabledWindows=dice::String16(reinterpret_cast<const unsigned short*>(u"0"));
    hsl::HSL::GetConfigHandle()->GetString(
        dice::String16(reinterpret_cast<const unsigned short*>(u"sm_enable_widgetCoverForWndId")),
        enabledWindows,hsl::Config::VALUE_STRATEGY_DEFAULT);
    dice::String16 maximum(reinterpret_cast<const unsigned short*>(u"2"));
    hsl::HSL::GetConfigHandle()->GetString(
        dice::String16(reinterpret_cast<const unsigned short*>(u"sm_support_widgetCoverMaxFrame")),
        maximum,hsl::Config::VALUE_STRATEGY_DEFAULT);
    threshold=asl::String16Utils::atoi(maximum.c_str());
    const auto id=hsl::StringUtils::Format(reinterpret_cast<const unsigned short*>(u"%d"),surfaceId);
    enabled=hsl::StringUtils::FindStr(enabledWindows,id,0)>=0;
}
}
