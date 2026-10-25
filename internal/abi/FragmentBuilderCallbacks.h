#pragma once

namespace IPL_ABI {
// Rebuilt identity keys for the three V21 post-load fragment types. Each
// getter is stable across calls and distinct from the other getters. Future
// Fragment getClassType implementations must use these same getters.
void* cruiseFragmentType();
void* guideFragmentType();
void* previewFragmentType();

// V21 0x2fde4: null is allowed; non-null must point to a genuinely
// constructed polymorphic Fragment with D1/D0 in virtual slots 0/1.
// This is not appropriate for PendingOpeningHost or raw factory storage.
void destroyBuiltFragment(void* fragment);
}
