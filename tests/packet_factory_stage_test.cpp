#include "../internal/abi/PacketFactoryStages.h"
#include "../internal/NS_InstrumentPannelLarge/CGIPLPacket.h"

#include <cassert>
#include <cstring>
#include <new>
#include <stdexcept>
#include <vector>

using IPL_ABI::PendingPacket;
using IPL_ABI::PacketConstructionOps;
using IPL_ABI::PacketDerivedTables;
using NS_InstrumentPannelLarge::CGIPLPacket;

namespace {
enum Step { Alloc, HostCtor, CoverCtor, CoverDtor, HostDtor, Free };
std::vector<Step> steps;
unsigned char* allocated;
bool failHost;
bool failCover;
void* contextToken;

void* allocate(std::size_t count) {
    assert(count == IPL_ABI::kPacketAllocationSize);
    steps.push_back(Alloc);
    allocated = static_cast<unsigned char*>(::operator new(count));
    std::memset(allocated, 0xa5, count);
    return allocated;
}
void deallocate(void* p) {
    assert(p == allocated);
    steps.push_back(Free);
    ::operator delete(p);
    allocated = nullptr;
}
void constructHost(void* p, GNS_FRAME::CGContext* ctx) {
    assert(p == allocated && ctx == contextToken);
    steps.push_back(HostCtor);
    if (failHost) throw std::runtime_error("host construction failed");
    allocated[0] = 0x11;
    allocated[0x40] = 0x22;
}
void destroyHost(void* p) {
    assert(p == allocated);
    steps.push_back(HostDtor);
}
void constructCover(void* p) {
    assert(p == allocated + IPL_ABI::kPacketCoverOffset);
    steps.push_back(CoverCtor);
    if (failCover) throw std::runtime_error("cover construction failed");
    allocated[IPL_ABI::kPacketCoverOffset] = 0x33;
}
void destroyCover(void* p) {
    assert(p == allocated + IPL_ABI::kPacketCoverOffset);
    steps.push_back(CoverDtor);
}
const PacketConstructionOps ops = {
    allocate, deallocate, constructHost, destroyHost, constructCover, destroyCover
};
void reset() {
    assert(!allocated);
    steps.clear();
    failHost = failCover = false;
}
}

int main() {
    contextToken = reinterpret_cast<void*>(0x1234);
    const auto* ctx = reinterpret_cast<GNS_FRAME::CGContext*>(contextToken);
    assert(CGIPLPacket::activePacket() == nullptr);

    {
        PendingPacket pending(const_cast<GNS_FRAME::CGContext*>(ctx), ops);
        assert(pending.bytes() == allocated);
        assert(allocated[0] == 0x11 && allocated[0x40] == 0x22);
        assert(allocated[IPL_ABI::kPacketCoverOffset] == 0x33);
        for (std::size_t i = IPL_ABI::kPacketProxyOffset;
             i < IPL_ABI::kPacketProxyOffset + 4; ++i) assert(allocated[i] == 0);
        assert(allocated[IPL_ABI::kPacketProxyOffset - 1] == 0xa5);
        assert(CGIPLPacket::activePacket() == nullptr);
        assert((steps == std::vector<Step>{Alloc, HostCtor, CoverCtor}));
        assert(!pending.hasDerivedTables());
        const PacketDerivedTables partial = {0x11111111, 0, 0x33333333, 0x44444444};
        try {
            pending.installDerivedTables(partial);
            assert(false);
        } catch (const std::invalid_argument&) {}
        assert(!pending.hasDerivedTables());
        assert(allocated[0] == 0x11 && allocated[0x40] == 0x22 &&
               allocated[IPL_ABI::kPacketCoverOffset] == 0x33);
        const PacketDerivedTables tables = PacketDerivedTables::fromVTable(0x10000);
        assert(tables.hostPackage == 0x10008 && tables.hostObjMem == 0x10050);
        assert(tables.coverMap == 0x10060 && tables.coverBusiness == 0x10078);
        auto swapped = tables;
        swapped.coverMap = tables.coverBusiness;
        swapped.coverBusiness = tables.coverMap;
        try { pending.installDerivedTables(swapped); assert(false); }
        catch (const std::invalid_argument&) {}
        assert(!pending.hasDerivedTables() && allocated[0] == 0x11);
        pending.installDerivedTables(tables);
        assert(pending.hasDerivedTables());
        auto word = [](std::size_t offset) {
            std::uint32_t result;
            std::memcpy(&result, allocated + offset, sizeof(result));
            return result;
        };
        assert(word(0xc0) == tables.coverMap);
        assert(word(0xc4) == tables.coverBusiness);
        assert(word(0x40) == tables.hostObjMem);
        assert(word(0x00) == tables.hostPackage);
        assert(word(0x168) == 0);
        try {
            pending.installDerivedTables(tables);
            assert(false);
        } catch (const std::logic_error&) {}
        assert(CGIPLPacket::activePacket() == nullptr);
        assert((steps == std::vector<Step>{Alloc, HostCtor, CoverCtor}));
    }
    assert((steps == std::vector<Step>{Alloc, HostCtor, CoverCtor,
                                       CoverDtor, HostDtor, Free}));
    reset();

    for (std::uint32_t base : {0u, 1u, 0xfffffffcu, 0xffffff80u}) {
        try { PacketDerivedTables::fromVTable(base); assert(false); }
        catch (const std::invalid_argument&) {}
    }
    assert(PacketDerivedTables::fromVTable(0xffffff7cu).coverBusiness == 0xfffffff4u);

    failCover = true;
    try {
        PendingPacket pending(const_cast<GNS_FRAME::CGContext*>(ctx), ops);
        assert(false);
    } catch (const std::runtime_error&) {}
    assert((steps == std::vector<Step>{Alloc, HostCtor, CoverCtor, HostDtor, Free}));
    reset();

    failHost = true;
    try {
        PendingPacket pending(const_cast<GNS_FRAME::CGContext*>(ctx), ops);
        assert(false);
    } catch (const std::runtime_error&) {}
    assert((steps == std::vector<Step>{Alloc, HostCtor, Free}));
    reset();

    PacketConstructionOps incomplete = ops;
    incomplete.destroyCover = nullptr;
    try {
        PendingPacket pending(const_cast<GNS_FRAME::CGContext*>(ctx), incomplete);
        assert(false);
    } catch (const std::invalid_argument&) {}
    assert(steps.empty() && !allocated);
}
