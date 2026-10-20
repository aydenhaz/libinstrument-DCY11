#pragma once
#include <cstdint>
#include <cstddef>
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

    // lifecycle family (string xref verified)
    void onInitEnd();                    // sub_2eb88  1892B  batch5
    void onPreLoad();                    // sub_2f464   400B  batch8
    void onPostLoadRes();                // sub_2f5fc  1588B  batch8
    void onPostAlreadyLoadRes();         // sub_2fc30   112B  batch8
    void refreshGFramePaint();           // sub_30d34   128B  batch8
    void onResume();                     // sub_3f080   692B  batch8
    void onDestroy();                    // sub_2f2f0   372B  batch8

    // Internal rebuilt singleton; original factory writes it after construction.
    static CGIPLPacket* activePacket() {return active_;}
    static constexpr std::size_t taskProxyOffset();
    static void TopRenderCallback(GNS_FRAME::CGCanvas*);

private:
    // UNVERIFIED legacy fields used by the incomplete onInitEnd. These are
    // NOT a reconstructed host base or script object. Do not register factories
    // or treat prefix size alone as object ABI compatibility.
    struct UnverifiedLegacyFields {
        LargeScreenImpl* impl_;
        void* third_;
        GNS_FRAME::CGContext* ctx_;
        bl::ObjectRuntimeInfo* blObject_;
        GNS_FRAME::CGFragment* hostFragment_;
        void* activity_;
        int mode_;
    } legacy_;
    unsigned char unconstructed_host_prefix_[0x168-sizeof(UnverifiedLegacyFields)];
    hmi::UITaskThreadProxy* uiThreadProxy_ = nullptr;
    static CGIPLPacket* active_;
};
inline constexpr std::size_t CGIPLPacket::taskProxyOffset() {
    return offsetof(CGIPLPacket,uiThreadProxy_);
}
static_assert(CGIPLPacket::taskProxyOffset()==0x168,"V21 task proxy field offset");
#if defined(__arm__) && !defined(__aarch64__)
static_assert(sizeof(CGIPLPacket)==0x16c,"V21 Packet allocation extent only; host prefix unconstructed");
#endif

}
