#pragma once

namespace NS_MultiScreenBase {
// V21 sub_1e790. The containing AutoMultiScreenConfig object must have been
// constructed to its original 0x340-byte layout before this is called.
void AutoMultiScreenConfig_ResetMapBindingList(void* instance, int scene);
}
