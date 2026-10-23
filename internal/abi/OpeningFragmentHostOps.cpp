#include "OpeningFragmentStages.h"

#if defined(__arm__) && !defined(__aarch64__) && UINTPTR_MAX == 0xffffffffu
#include <new>

extern "C" void v21FragmentC2(void*, int)
    __asm__("_ZN9GNS_FRAME10CGFragmentC2ENS0_11ELaunchModeE");
extern "C" void v21FragmentD2(void*)
    __asm__("_ZN9GNS_FRAME10CGFragmentD2Ev");
extern "C" void v21ObserverBaseC2(void*, const char*)
    __asm__("_ZN3hsl12ObserverBaseC2EPKc");

namespace IPL_ABI {
namespace {
void* allocate(std::size_t size) { return ::operator new(size); }
void deallocate(void* storage) { ::operator delete(storage); }
void constructFragment(void* storage, int mode) { v21FragmentC2(storage, mode); }
void destroyFragment(void* storage) { v21FragmentD2(storage); }
void constructObserver(void* storage, const char* name) {
    v21ObserverBaseC2(storage, name);
}
}
const OpeningHostOps& openingHostOps() {
    static const OpeningHostOps ops = {
        allocate, deallocate, constructFragment, destroyFragment, constructObserver
    };
    return ops;
}
} // namespace IPL_ABI
#endif
