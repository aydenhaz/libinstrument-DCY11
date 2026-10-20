#pragma once
class CGString;
namespace GNS_FRAME {
class CGCanvas;
class CGWorkStation {
public:
    static CGWorkStation *ownWorkStation();
    void setMapRenderCallback(void (*cb)());
    void setTopRenderCallback(void (*cb)(CGCanvas*));
    void showFpsInfo(bool, CGString&, int);
};
}
