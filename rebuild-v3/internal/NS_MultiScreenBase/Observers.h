#pragma once
#include <ExternalScreenTypes.h>
#include <AbiTypes.h>
#include <cstdint>
namespace bl {  }
namespace GPI { class Closure; }
namespace NS_MultiScreenBase {


struct SurfaceConfig;
struct tagMultiViewModeInfo;

// Four collections are embedded at +0x24, +0x48, +0x6c, +0x90. In ARM32
// the CrossControl array, capacity and count lie at +0x7c/+0x80/+0x84.
struct ObserverCollectionAbi {
    uint32_t mutexWords[4];
    void** observers;
    uint32_t capacity;
    uint32_t count;
    void* allocator;
    uint32_t flags;
};
#if defined(__arm__) && !defined(__aarch64__)
static_assert(sizeof(ObserverCollectionAbi) == 0x24, "V21 observer collection");
#endif

struct AutoMapCrossControlObserver {
    virtual void OnSetExternalScreenCrossControlUi(ScreenType, bool) = 0;
};

class AutoExscreenNotifyObserver {
public:
    virtual ~AutoExscreenNotifyObserver() {}
    void OnExScreenNotify(int32_t event, tagSTExtScreen *st);   // sub_29bf0
    void DeferActionA(int screen, bool enabled);                // sub_2a5a8
    void DeferActionB(int screen, int level, int mode, int pos); // sub_2a7e0
    void DeferActionC(const tagSTExtScreenControl& control);    // sub_2aa28
    void OnSetExternalScreenCrossControlUi(int mapEngineId, bool enabled);
    void OnSetExternalScreenMapStatusUi(int mapEngineId, int level,
                                        int mode, int position);
    void OnSetExScreenMapStatusUi(const tagSTExtScreenControl& control);
private:
    unsigned char inherited_[0x20];
    ObserverCollectionAbi renderObservers_;
    ObserverCollectionAbi updateObservers_;
    ObserverCollectionAbi crossControlObservers_;
    ObserverCollectionAbi modeObservers_;
    unsigned char memberState_[0x28];
    ScreenType screenType_;
    unsigned char beforeAsync_[8];
    unsigned char asyncState_[0x40];
};
#if defined(__arm__) && !defined(__aarch64__)
static_assert(sizeof(AutoExscreenNotifyObserver) == 0x128,
              "V21 observer object extent");
#endif

}
