#pragma once
#include <cstdint>
namespace bl { struct ObjectRuntimeInfo; }
namespace GPI { class Closure; }
namespace GNS_FRAME { class CGCanvas; class CGIntent; class CGFragment; class CGPackage; class CGContext; }
namespace hmi { class UITaskThreadProxy; }
namespace hsl { class IActivateVerification; }
namespace NS_InstrumentPannelLarge {

class LargeScreenImpl;

class CGIPLPacket {
public:
    CGIPLPacket();
    ~CGIPLPacket();

    // script-binding glue (sub_30dc8)
    static CGIPLPacket *ScriptThis(void *obj);

    // lifecycle family (string xref verified)
    void onInitEnd();                    // sub_2eb88  1892B  batch5
    void onPreLoad();                    // sub_2f464   400B  batch8
    void onPostLoadRes();                // sub_2f5fc  1588B  batch8
    void onPostAlreadyLoadRes();         // sub_2fc30   112B  batch8
    void refreshGFramePaint();           // sub_30d34   128B  batch8
    void onResume();                     // sub_3f080   692B  batch8
    void onDestroy();                    // sub_2f2f0   372B  batch8

    static void TopRenderCallback(GNS_FRAME::CGCanvas*);

private:
    // batch5 members
    LargeScreenImpl *impl_;
    void *third_;
    hmi::UITaskThreadProxy *uiThreadProxy_ = nullptr;
    GNS_FRAME::CGContext *ctx_;
    bl::ObjectRuntimeInfo *blObject_;
    GNS_FRAME::CGFragment *hostFragment_;
    void *activity_;
    int mode_;
    // batch7 members
    hsl::IActivateVerification *verify_;
    uint16_t state_;        // +0x39a bit0 = verify ok
    uint32_t state394_;     // 12-way switch state
    uint64_t timerId_;
    uint64_t lastElapsed_;
    int screenType_;
    char opaque_[32];
};

}
