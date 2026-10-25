#include "FragmentBuilderCallbacks.h"
#include <cstring>

namespace IPL_ABI {
namespace {
// The originals return three distinct Thumb function addresses whose bodies
// are BX LR. They are identity tokens, not invoked here. Distinct writable
// bytes avoid identical-code/constant merging and original-image addresses.
unsigned char typeKeys[3];
}
void* cruiseFragmentType() { return &typeKeys[0]; }
void* guideFragmentType() { return &typeKeys[1]; }
void* previewFragmentType() { return &typeKeys[2]; }

void destroyBuiltFragment(void* fragment) {
    if (!fragment) return;
    void** table;
    std::memcpy(&table, fragment, sizeof(table));
    // The deleting destructor performs both destruction and deallocation.
    // Never follow this with operator delete or use complete destructor slot 0.
    reinterpret_cast<void (*)(void*)>(table[1])(fragment);
}
}
