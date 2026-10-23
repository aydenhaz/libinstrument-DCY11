#pragma once

namespace hsl { class MapView; }
namespace NS_MultiScreenBase {
// V21 sub_1ddd8: obtain the map view for the screen at instance+0x30c.
// A fully constructed 0x340-byte AutoMultiScreenConfig is required.
hsl::MapView* AutoMultiScreenConfig_GetMapViewHandle(const void* instance);
// V21 sub_1e790. The containing AutoMultiScreenConfig object must have been
// constructed to its original 0x340-byte layout before this is called.
void AutoMultiScreenConfig_ResetMapBindingList(void* instance, int scene);
}
