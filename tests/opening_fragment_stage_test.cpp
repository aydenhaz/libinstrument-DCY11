#include "../internal/abi/OpeningFragmentStages.h"

#include <cassert>
#include <cstring>
#include <new>
#include <stdexcept>
#include <vector>

using IPL_ABI::OpeningHostOps;
using IPL_ABI::PendingOpeningHost;

namespace {
enum Step { Allocate, FragmentCtor, ObserverCtor, FragmentDtor, Deallocate };
std::vector<Step> steps;
unsigned char* storage;
bool failFragment, failObserver;
void* allocate(std::size_t size) {
    assert(size == 0x440 && !storage);
    steps.push_back(Allocate);
    storage = static_cast<unsigned char*>(::operator new(size));
    std::memset(storage, 0xa5, size);
    return storage;
}
void deallocate(void* p) {
    assert(p == storage);
    steps.push_back(Deallocate);
    ::operator delete(p);
    storage = nullptr;
}
void constructFragment(void* p, int mode) {
    assert(p == storage && mode == 0);
    steps.push_back(FragmentCtor);
    if (failFragment) throw std::runtime_error("host fragment failed");
    storage[0] = 0x11;
}
void destroyFragment(void* p) {
    assert(p == storage && storage[0] == 0x11);
    steps.push_back(FragmentDtor);
}
void constructObserver(void* p, const char* name) {
    assert(p == storage + 0x2e8 && !std::strcmp(name, "AutoBaseFragment"));
    steps.push_back(ObserverCtor);
    if (failObserver) throw std::runtime_error("observer failed");
    std::strncpy(static_cast<char*>(p), name, 0x1f);
    storage[0x2e8 + 0x1f] = 0;
}
const OpeningHostOps ops = {
    allocate, deallocate, constructFragment, destroyFragment, constructObserver
};
void reset() { assert(!storage); steps.clear(); failFragment = failObserver = false; }
}

int main() {
    {
        PendingOpeningHost opening(ops);
        assert(opening.bytes() == storage);
        assert(std::strcmp(reinterpret_cast<const char*>(storage + 0x2e8),
                           "AutoBaseFragment") == 0);
        std::uint32_t engine;
        std::memcpy(&engine, storage + 0x308, sizeof(engine));
        assert(engine == 5 && storage[0x30c] == 0xa5);
        assert((steps == std::vector<Step>{Allocate, FragmentCtor, ObserverCtor}));
    }
    assert((steps == std::vector<Step>{Allocate, FragmentCtor, ObserverCtor,
                                       FragmentDtor, Deallocate}));
    reset();
    failObserver = true;
    try { PendingOpeningHost opening(ops); assert(false); }
    catch (const std::runtime_error&) {}
    assert((steps == std::vector<Step>{Allocate, FragmentCtor, ObserverCtor,
                                       FragmentDtor, Deallocate}));
    reset();
    failFragment = true;
    try { PendingOpeningHost opening(ops); assert(false); }
    catch (const std::runtime_error&) {}
    assert((steps == std::vector<Step>{Allocate, FragmentCtor, Deallocate}));
    reset();
    OpeningHostOps incomplete = ops;
    incomplete.constructObserver = nullptr;
    try { PendingOpeningHost opening(incomplete); assert(false); }
    catch (const std::invalid_argument&) {}
    assert(steps.empty() && !storage);
}
