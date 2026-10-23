#include "PacketFactoryStages.h"

#if defined(__arm__) && !defined(__aarch64__) && UINTPTR_MAX == 0xffffffffu
#include "../NS_MultiScreenBase/CoverUtil.h"
#include <new>

// ARM C++ ABI entrypoints exported by the matching V21 libGFrame.so.
// Use the C2/D2 subobject forms: the original Packet constructor invokes C2
// at 0x2eb1a, and its destructor ends in the host base destructor.
extern "C" void v21FragmentPackageC2(void*, GNS_FRAME::CGContext*)
    __asm__("_ZN9GNS_FRAME17CGFragmentPackageC2EPNS_9CGContextE");
extern "C" void v21FragmentPackageD2(void*)
    __asm__("_ZN9GNS_FRAME17CGFragmentPackageD2Ev");

namespace IPL_ABI {
namespace {
void* allocate(std::size_t bytes) { return ::operator new(bytes); }
void deallocate(void* p) { ::operator delete(p); }
void constructHost(void* p, GNS_FRAME::CGContext* context) {
    v21FragmentPackageC2(p, context);
}
void destroyHost(void* p) { v21FragmentPackageD2(p); }
void constructCover(void* p) { ::new (p) NS_MultiScreenBase::CoverUtil(); }
void destroyCover(void* p) {
    static_cast<NS_MultiScreenBase::CoverUtil*>(p)->~CoverUtil();
}
}

const PacketConstructionOps& hostPacketConstructionOps() {
    static const PacketConstructionOps ops = {
        allocate, deallocate, constructHost, destroyHost,
        constructCover, destroyCover
    };
    return ops;
}
} // namespace IPL_ABI
#endif
