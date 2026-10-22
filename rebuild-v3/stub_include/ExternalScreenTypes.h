#pragma once
#include <cstdint>
#include <type_traits>
// V21 global ABI types. Word semantics remain unnamed until dataflow is proven.
struct tagSTExtScreen {std::int32_t words[4];};
// Original OnMapLevelChanged initializes one zero followed by eight 99 sentinels.
struct tagSTExtScreenControl {std::int32_t words[9]={0,99,99,99,99,99,99,99,99};};
// Indices read directly by V21 sub_2ace0. The final two words do not yet
// have proven semantics; keep the 36-byte record and the original sentinels.
enum ExtScreenControlWord {
    kExtScreenSelector = 0,        // MultiScreenService::GetMapEngineId(int)
    kExtRequestedViewMode = 1,     // SetMapViewMode and UI-config field 1
    kExtRequestedLevel = 2,        // SetMapLevel and UI-config field 0
    kExtCrossControl = 3,          // OnSetExternalScreenCrossControlUi
    kExtEagleEye = 4,              // UI-config field 3
    kExtPreview = 5,               // UI-config field 4
    kExtScreenModeCondition = 6    // compared with current mode + 1 or 99
};
inline bool operator==(const tagSTExtScreenControl& a, const tagSTExtScreenControl& b) {
    for (unsigned i = 0; i < 9; ++i) if (a.words[i] != b.words[i]) return false;
    return true;
}
static_assert(sizeof(tagSTExtScreen)==16,"V21 external-screen value extent");
static_assert(alignof(tagSTExtScreen)==4,"V21 external-screen value alignment");
static_assert(std::is_trivially_copyable<tagSTExtScreen>::value,"V21 by-value screen payload");
static_assert(sizeof(tagSTExtScreenControl)==36,"V21 external-screen control extent");
namespace NS_MultiScreenBase {
using ::tagSTExtScreen;
using ::tagSTExtScreenControl;
}
