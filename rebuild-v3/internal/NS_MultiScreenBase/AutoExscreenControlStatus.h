#pragma once
#include <AbiTypes.h>
#include <ExternalScreenTypes.h>

namespace NS_MultiScreenBase {

// V21 0x2be04. On a failed map-view lookup, leave the copied control record
// intact. A constructed per-screen AutoMultiScreenConfig is required.
void AutoExscreenUtil_FillControlStatus(ScreenType screen, int mapEngineId,
                                        tagSTExtScreenControl& status);
// V21 0x2b8d4..0x2bb28: provisional values from the reference map view.
// The final status filler overwrites words 0..6 when its own lookup succeeds.
void AutoExscreenUtil_ProvisionalControlStatus(tagSTExtScreenControl& status);

}
