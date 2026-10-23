#pragma once

namespace hsl { class MapView; }
namespace NS_MultiScreenBase {
// V21 0x1c210 and 0x3bec0: initialize one 0xe4-byte scene record and one
// 0x20-byte binding record. Neither helper constructs the containing object.
void AutoMultiScreenConfig_InitSceneRecord(void* record);
void AutoMultiScreenConfig_InitBindingRecord(void* record, int screen);
// V21 sub_1ddd8: obtain the map view for the screen at instance+0x30c.
// A fully constructed 0x340-byte AutoMultiScreenConfig is required.
hsl::MapView* AutoMultiScreenConfig_GetMapViewHandle(const void* instance);
// V21 sub_1e790. The containing AutoMultiScreenConfig object must have been
// constructed to its original 0x340-byte layout before this is called.
void AutoMultiScreenConfig_ResetMapBindingList(void* instance, int scene);
// V21 sub_1def4. This is also exported as the real instance method through
// AutoMultiScreenConfig::SetMapViewMode(int,int); a constructed object is
// required, and both its own map view and engine-1 map view must exist.
void AutoMultiScreenConfig_SetMapViewMode(void* instance, int fallbackMode,
                                         int requestedMode);
// V21 sub_1781c: scene 0 closes dynamic type 1 when open; scene 1 closes
// type 0 when open. Other scenes do not change either type.
void AutoMultiScreenConfig_CloseSceneDynamicLevel(int screen, int scene);
// V21 sub_1765c: create the 0x88-byte DynamicLevelParam from the selected
// scene, initialize the screen's map view and open the scene's dynamic type.
// This requires the same constructed 0x340-byte singleton as the other helpers.
void AutoMultiScreenConfig_InitSceneDynamicLevel(void* instance, int scene);
}
