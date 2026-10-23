#pragma once

#include <cstddef>
#include <cstdint>

namespace GNS_FRAME { class CGContext; }

namespace IPL_ABI {

// V21 ARM32 layout. The pending object is never registered or returned to
// GFrame: Packet's derived virtual tables and Fragment lifecycle are missing.
constexpr std::size_t kPacketAllocationSize = 0x16c;
constexpr std::size_t kPacketCoverOffset = 0xc0;
constexpr std::size_t kPacketProxyOffset = 0x168;
static_assert(kPacketCoverOffset + 0xa8 == kPacketProxyOffset,
              "V21 CoverUtil ends at the proxy field");
static_assert(kPacketProxyOffset + sizeof(std::uint32_t) == kPacketAllocationSize,
              "V21 Packet allocation ends after its ARM32 proxy");

struct PacketConstructionOps {
    void* (*allocate)(std::size_t);
    void (*deallocate)(void*);
    void (*constructHost)(void*, GNS_FRAME::CGContext*);
    void (*destroyHost)(void*);
    void (*constructCover)(void*);
    void (*destroyCover)(void*);
};

class PendingPacket {
public:
    PendingPacket(GNS_FRAME::CGContext*, const PacketConstructionOps&);
    ~PendingPacket() noexcept;
    PendingPacket(const PendingPacket&) = delete;
    PendingPacket& operator=(const PendingPacket&) = delete;
    // For inspection while this guard is alive. No ownership transfer exists.
    const unsigned char* bytes() const { return static_cast<const unsigned char*>(storage_); }
private:
    void cleanup() noexcept;
    PacketConstructionOps ops_;
    void* storage_ = nullptr;
    bool hostConstructed_ = false;
    bool coverConstructed_ = false;
};

#if defined(__arm__) && !defined(__aarch64__) && UINTPTR_MAX == 0xffffffffu
// Calls the imported V21 CGFragmentPackage C2/D2 symbols and the local
// CoverUtil constructor/destructor. The returned operations are not wired to
// StartupInstrumentPannelLarge or ClassNode_CGPackage registration.
const PacketConstructionOps& hostPacketConstructionOps();
#endif

} // namespace IPL_ABI
