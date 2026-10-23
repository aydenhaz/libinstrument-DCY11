#include "PacketFactoryStages.h"

#include <cstring>
#include <new>
#include <stdexcept>

namespace IPL_ABI {

PendingPacket::PendingPacket(GNS_FRAME::CGContext* context,
                             const PacketConstructionOps& ops) : ops_(ops) {
    if (!ops_.allocate || !ops_.deallocate ||
        !ops_.constructHost || !ops_.destroyHost ||
        !ops_.constructCover || !ops_.destroyCover) {
        throw std::invalid_argument("Packet construction requires every host operation");
    }
    storage_ = ops_.allocate(kPacketAllocationSize);
    if (!storage_) throw std::bad_alloc();
    try {
        ops_.constructHost(storage_, context);
        hostConstructed_ = true;
        ops_.constructCover(static_cast<unsigned char*>(storage_) + kPacketCoverOffset);
        coverConstructed_ = true;
        // The original factory writes one ARM32 pointer, not host sizeof(void*).
        const std::uint32_t emptyProxy = 0;
        std::memcpy(static_cast<unsigned char*>(storage_) + kPacketProxyOffset,
                    &emptyProxy, sizeof(emptyProxy));
    } catch (...) {
        cleanup();
        throw;
    }
}

PendingPacket::~PendingPacket() noexcept { cleanup(); }

void PendingPacket::cleanup() noexcept {
    if (!storage_) return;
    if (coverConstructed_) ops_.destroyCover(
        static_cast<unsigned char*>(storage_) + kPacketCoverOffset);
    if (hostConstructed_) ops_.destroyHost(storage_);
    ops_.deallocate(storage_);
    storage_ = nullptr;
}

} // namespace IPL_ABI
