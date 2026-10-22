#pragma once
#include <AbiTypes.h>

namespace NS_MultiScreenBase {

// V21 sub_168e8: field indices 0..5 are level, view mode, cross control,
// eagle eye, preview and day/night respectively.
void AutoExscreenUtil_SetMapParamsToUiConfig(ScreenType screen, int mode,
                                              int scene, int field, int value);
// V21 sub_17ad0: the requested level is dynamic under these two conditions.
bool AutoExscreenUtil_IsDynamicLevelRequest(int configuredLevel, int requestedLevel);

// V21 sub_1984c returns the address of one of three process-local scene IDs.
// An invalid screen selector falls back to the first slot in the original.
int* AutoExscreenUtil_GetCurrentSceneSlot(ScreenType screen);

// V21 sub_173fc / sub_171b8. Both take the current screen mode from
// AutoConfigScreenMode and write through sub_168e8. View mode written in the
// preview scene (2) belongs to the guide scene (1) in the UI configuration.
void AutoExscreenUtil_SetMapViewLevelToUiConfig(ScreenType screen, int level);
void AutoExscreenUtil_SetMapViewModeToUiConfig(ScreenType screen, int mode);

}
