#include "../internal/abi/PacketFactoryStages.h"
#include "../internal/NS_InstrumentPannelLarge/CGIPLPacket.h"

#include <cassert>
#include <cstring>
#include <new>
#include <stdexcept>
#include <vector>

using IPL_ABI::PendingPacket;
using IPL_ABI::PacketConstructionOps;
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
    assert(p == allocated && allocated[0] == 0x11);
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
    assert(allocated[IPL_ABI::kPacketCoverOffset] == 0x33);
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
    }
    assert((steps == std::vector<Step>{Alloc, HostCtor, CoverCtor,
                                       CoverDtor, HostDtor, Free}));
    reset();

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
