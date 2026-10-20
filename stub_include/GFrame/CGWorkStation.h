#pragma once
class CGString;
namespace GNS_FRAME {
class CGWorkStation {
public:
    static CGWorkStation *ownWorkStation();
    void setMapRenderCallback(void (*cb)(void*));
    void setTopRenderCallback(void (*cb)(void*));
    void showFpsInfo(bool, CGString&, int);
};
}
