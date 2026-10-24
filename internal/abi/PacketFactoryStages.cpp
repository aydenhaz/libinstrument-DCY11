#include "PacketFactoryStages.h"

#include <cstring>
#include <new>
#include <stdexcept>

namespace IPL_ABI {

PacketDerivedTables PacketDerivedTables::fromVTable(std::uint32_t base) {
    // Last group contains three callable slots, through base+0x80.
    if (!base || (base & 3u) || base > UINT32_MAX - 0x83u)
        throw std::invalid_argument("Packet vtable group must be aligned and fit ARM32");
    return {base + 0x60u, base + 0x78u, base + 0x50u, base + 8u};
}

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

void PendingPacket::installDerivedTables(const PacketDerivedTables& tables) {
    if (derivedTablesInstalled_ || !hostConstructed_ || !coverConstructed_)
        throw std::logic_error("Packet derived tables require completed bases exactly once");
    if (!tables.coverMap || !tables.coverBusiness ||
        !tables.hostObjMem || !tables.hostPackage)
        throw std::invalid_argument("Packet requires four non-null derived table points");
    if (tables.hostPackage < 8u)
        throw std::invalid_argument("Packet primary address point lacks its header");
    const auto expected = PacketDerivedTables::fromVTable(tables.hostPackage - 8u);
    if (tables.coverMap != expected.coverMap ||
        tables.coverBusiness != expected.coverBusiness ||
        tables.hostObjMem != expected.hostObjMem)
        throw std::invalid_argument("Packet address points do not match the V21 vtable group");

    // V21 0x2eb36..0x2eb42. Deliberately use four-byte ARM words, including
    // in native tests where host pointers are eight bytes. This is an ABI
    // staging step, not native virtual dispatch through these projections.
    unsigned char* const bytes = static_cast<unsigned char*>(storage_);
    std::memcpy(bytes + 0xc0, &tables.coverMap, sizeof(tables.coverMap));
    std::memcpy(bytes + 0xc4, &tables.coverBusiness, sizeof(tables.coverBusiness));
    std::memcpy(bytes + 0x40, &tables.hostObjMem, sizeof(tables.hostObjMem));
    std::memcpy(bytes, &tables.hostPackage, sizeof(tables.hostPackage));
    derivedTablesInstalled_ = true;
}

void PendingPacket::cleanup() noexcept {
    if (!storage_) return;
    if (coverConstructed_) ops_.destroyCover(
        static_cast<unsigned char*>(storage_) + kPacketCoverOffset);
    if (hostConstructed_) ops_.destroyHost(storage_);
    ops_.deallocate(storage_);
    storage_ = nullptr;
}

} // namespace IPL_ABI
