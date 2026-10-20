#pragma once
class CGString;
class CGPackageMgr;
namespace GPI { class Closure; }
namespace GNS_FRAME {
class CGContext; class CGPackage; class CGExhibition;
class CGFrameSimulation {
public:
    static CGPackageMgr *getPackageMgr();
    static CGFrameSimulation *ownWorkStation();
    static int getId();
    static int getRenderThreadExecutorId();
    static int getRenderThreadId();
    static CGPackage *getFragmentActivity();
    static CGExhibition *getExhibition();
    bool isRender(bool);
    void sendMsg(int, int, int, int, void*);
    void runOnUIThreadHelper(const GPI::Closure&, void*, int, int, int, int);
};
}
