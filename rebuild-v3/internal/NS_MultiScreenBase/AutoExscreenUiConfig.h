#pragma once
#include <AbiTypes.h>

namespace NS_MultiScreenBase {

// V21 sub_168e8: field indices 0..5 are level, view mode, cross control,
// eagle eye, preview and day/night respectively.
void AutoExscreenUtil_SetMapParamsToUiConfig(ScreenType screen, int mode,
                                              int scene, int field, int value);
// V21 sub_17ad0: the requested level is dynamic under these two conditions.
bool AutoExscreenUtil_IsDynamicLevelRequest(int configuredLevel, int requestedLevel);

}
