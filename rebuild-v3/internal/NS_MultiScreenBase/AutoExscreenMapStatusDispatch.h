#pragma once
#include <AbiTypes.h>
namespace hsl { class MapView; }

namespace NS_MultiScreenBase {
// V21 0x2c070's operational path after its observer resolves a constructed
// per-screen AutoMultiScreenConfig. The caller supplies observer+0xcc's
// timer-state pointer; the fourth original integer is used only for logging.
// A false return means the input map engine has no map view.
bool AutoExscreenUtil_ApplyMapStatus(void* instance, ScreenType screen,
                                     void* timerState, int mapEngineId,
                                     int requestedLevel, int requestedMode);
// The observer has already passed V21's entry GetMapViewHandle gate. Reuse
// that same handle, preserving the original single lookup before GetInstance.
void AutoExscreenUtil_ApplyMapStatusWithView(void* instance, ScreenType screen,
                                              void* timerState, hsl::MapView* view,
                                              int requestedLevel, int requestedMode);
}
