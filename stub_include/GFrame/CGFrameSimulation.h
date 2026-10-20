#pragma once
class CGString;
namespace GPI { class Closure; class MessageCtrl; template<class T> class ScopedRefptr; }
namespace GNS_FRAME {
class CGPackageMgr;
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
    void setRenderEndCallback(void (*)());
    void sendMsg(int, int, int, unsigned long long, GPI::ScopedRefptr<GPI::MessageCtrl>*);
    void showFpsInfo(bool, CGString&, int);
    void runOnUIThreadHelper(const GPI::Closure&, void*, int, int, int, int);
};
}
