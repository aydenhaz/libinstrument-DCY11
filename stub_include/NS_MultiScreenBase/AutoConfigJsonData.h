#include <AbiTypes.h>
#pragma once
#include <vector>
#include <cstdint>
#include <cstddef>
#include <GFrame/CGString.h>
namespace NS_MultiScreenBase {
// V21 libCommon GetData(SurfaceConfig&) at 0x34aebc:
// copy 0x24 scalar bytes, assign CGString at +0x24, copy 0x11 bytes at +0x3c.
struct SurfaceConfig {
    uint32_t scalar[9]{};
    CGString name;
    uint32_t tail[4]{};
    uint8_t final_flag{};
    // V21 constructor 0x16302: +0x1c=-1, +0x20=8, +0x48=1.
    SurfaceConfig() {scalar[7]=0xffffffffu;scalar[8]=8;tail[3]=1;}
};
static_assert(sizeof(SurfaceConfig) == 0x50, "V21 ARM32 SurfaceConfig");
static_assert(offsetof(SurfaceConfig, name) == 0x24, "SurfaceConfig string offset");
static_assert(offsetof(SurfaceConfig, tail) == 0x3c, "SurfaceConfig tail offset");
// GetMultiViewModeInfo 0x34bb10..0x34be70 returns success in r0 and
// appends one byte per mode. Return type is omitted from C++ symbol mangling.
struct tagMultiViewModeInfo { uint8_t value; };
static_assert(sizeof(tagMultiViewModeInfo) == 1, "V21 mode vector element stride");
class AutoConfigJsonData {
public:
    static AutoConfigJsonData *GetInstance();
    void Init();
    void GetSurfaceConfig(ScreenType, SurfaceConfig&) const;
    void GetSurfaceConfig(int screen, SurfaceConfig& config) const { GetSurfaceConfig(static_cast<ScreenType>(screen), config); }
    bool GetMultiViewModeInfo(ScreenType, std::vector<tagMultiViewModeInfo>&);
    bool GetMultiViewModeInfo(int screen, std::vector<tagMultiViewModeInfo>& modes) { return GetMultiViewModeInfo(static_cast<ScreenType>(screen), modes); }
};
}
