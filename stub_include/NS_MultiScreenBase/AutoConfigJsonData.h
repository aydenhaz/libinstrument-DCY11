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
};
static_assert(sizeof(SurfaceConfig) == 0x50, "V21 ARM32 SurfaceConfig");
static_assert(offsetof(SurfaceConfig, name) == 0x24, "SurfaceConfig string offset");
static_assert(offsetof(SurfaceConfig, tail) == 0x3c, "SurfaceConfig tail offset");
// GetMultiViewModeInfo 0x34bbfc..0x34bc08 appends one byte per mode.
struct tagMultiViewModeInfo { uint8_t value; };
static_assert(sizeof(tagMultiViewModeInfo) == 1, "V21 mode vector element stride");
class AutoConfigJsonData {
public:
    static AutoConfigJsonData *GetInstance();
    void Init();
    void GetSurfaceConfig(ScreenType, SurfaceConfig&) const;
    void GetSurfaceConfig(int screen, SurfaceConfig& config) const { GetSurfaceConfig(static_cast<ScreenType>(screen), config); }
    void GetMultiViewModeInfo(ScreenType, std::vector<tagMultiViewModeInfo>&);
    void GetMultiViewModeInfo(int screen, std::vector<tagMultiViewModeInfo>& modes) { GetMultiViewModeInfo(static_cast<ScreenType>(screen), modes); }
};
}
