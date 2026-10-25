#include "../internal/abi/FragmentBuilderCallbacks.h"
#include <cassert>
#include <cstddef>
#include <cstdlib>

static int destroyed, deallocated;
struct Fragment {
    virtual ~Fragment() { ++destroyed; }
    static void operator delete(void* p) { ++deallocated; std::free(p); }
    static void* operator new(std::size_t n) { return std::malloc(n); }
};
struct Left { virtual ~Left() {} long padding = 0; };
struct Secondary : Left, Fragment {
    static void operator delete(void* p) { ++deallocated; std::free(p); }
    static void* operator new(std::size_t n) { return std::malloc(n); }
};
int main() {
    using namespace IPL_ABI;
    void* keys[] = {cruiseFragmentType(), guideFragmentType(), previewFragmentType()};
    assert(keys[0] && keys[1] && keys[2]);
    assert(keys[0] != keys[1] && keys[1] != keys[2] && keys[0] != keys[2]);
    for (int i = 0; i < 100; ++i) {
        assert(keys[0] == cruiseFragmentType());
        assert(keys[1] == guideFragmentType());
        assert(keys[2] == previewFragmentType());
    }
    destroyBuiltFragment(nullptr);
    assert(!destroyed && !deallocated);
    destroyBuiltFragment(new Fragment);
    assert(destroyed == 1 && deallocated == 1);
    auto* object = new Secondary;
    auto* base = static_cast<Fragment*>(object);
    assert(static_cast<void*>(object) != static_cast<void*>(base));
    destroyBuiltFragment(base); // deleting-destructor thunk adjusts receiver
    assert(destroyed == 2 && deallocated == 2);
}
