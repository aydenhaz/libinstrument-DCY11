#include <AbiTypes.h>
#pragma once
#include <vector>
namespace NS_MultiScreenBase {
struct SurfaceConfig {};
struct tagMultiViewModeInfo {};
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
