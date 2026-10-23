#pragma once
#include <string>

namespace NS_MultiScreenBase {

// V21 0x2b42c..0x2b86c. Both supplied views must be valid. Their virtual
// tables use the ARM32 host offsets documented in map_view_virtuals.md.
void AutoExscreenUtil_ApplyMapStyleWithViews(
    void* own, void* reference, int selector,
    const std::string& dayName, const std::string& nightName);

}
