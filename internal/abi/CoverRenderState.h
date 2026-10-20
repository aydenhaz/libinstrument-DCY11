#pragma once
#include <cstdint>

namespace IPL_ABI {
// Extracted decision logic from V21 CoverUtil::isUIFirstRenderFinish, 0x18d9c.
// This helper is not a CoverUtil object or a replacement observer/vtable.
inline bool coverUiFirstRenderFinished(bool hasImage, bool enabled, bool stateFlag,
                                       int32_t counter, int32_t threshold) {
    if (!hasImage || !enabled || !stateFlag) return true;
    // Original CMP followed by BLE: equality and signed negative counts matter.
    return counter > threshold;
}

// State mutation at 0x17d8a..0x17da4. Caller handles original logging.
inline void coverSurfaceStatusChanged(int32_t configuredSurface, int32_t surface,
                                      int32_t event, bool hasImage, bool enabled,
                                      int32_t& counter) {
    if (configuredSurface == surface && event == 1 && hasImage && enabled)
        counter = 0;
}
}
