#pragma once
#include <vector>
namespace NS_MultiScreenBase {
struct SurfaceConfig {};
struct tagMultiViewModeInfo {};
class AutoConfigJsonData {
public:
    static AutoConfigJsonData *GetInstance();
    void Init();
    void GetSurfaceConfig(int, SurfaceConfig&);
    void GetMultiViewModeInfo(int, std::vector<tagMultiViewModeInfo>&);
};
}
