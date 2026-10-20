#pragma once
#include <cstdint>
#include <type_traits>
// V21 global ABI types. Word semantics remain unnamed until dataflow is proven.
struct tagSTExtScreen {std::int32_t words[4];};
// Original OnMapLevelChanged initializes one zero followed by eight 99 sentinels.
struct tagSTExtScreenControl {std::int32_t words[9]={0,99,99,99,99,99,99,99,99};};
static_assert(sizeof(tagSTExtScreen)==16,"V21 external-screen value extent");
static_assert(alignof(tagSTExtScreen)==4,"V21 external-screen value alignment");
static_assert(std::is_trivially_copyable<tagSTExtScreen>::value,"V21 by-value screen payload");
static_assert(sizeof(tagSTExtScreenControl)==36,"V21 external-screen control extent");
namespace NS_MultiScreenBase {
using ::tagSTExtScreen;
using ::tagSTExtScreenControl;
}
