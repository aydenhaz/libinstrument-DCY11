#pragma once
#include <cstdint>
#include <cstddef>
namespace GPI { class Closure; }
namespace GNS_FRAME { class CGCanvas; class CGIntent; class CGFragment; class CGPackage; class CGContext; }
namespace hmi { class UITaskThreadProxy; }
namespace hsl { class IActivateVerification; }
namespace NS_InstrumentPannelLarge {

class PacketFactory;

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
    void onDestroy();                    // sub_2f2f0   372B  batch8

    // Original factory publishes only after host/CoverUtil construction and
    // the four derived vptr stores. This partial constructor does not publish.
    static CGIPLPacket* activePacket() {return active_;}
    static constexpr std::size_t taskProxyOffset();
    static void TopRenderCallback(GNS_FRAME::CGCanvas*);

private:
    friend class PacketFactory;
    // PacketFactory is not implemented. It must call this only after the
    // complete host object exists. The shutdown contract test simulates that.
    static void publishConstructed(CGIPLPacket*);
    // The V21 host package and CoverUtil bases occupy the entire prefix.
    // Their constructors, virtual layout and owning members are not modeled
    // by this storage-only class; the factory must remain disconnected.
    alignas(void*) unsigned char unconstructed_host_and_cover_[0x168];
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
