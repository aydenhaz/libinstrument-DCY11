#pragma once
#include <cstdint>
#include <cstring>

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
// TopRenderCoverCallback (0x18898) decisions. External draw/capture calls
// must execute in original order; draw/capture can change state synchronously.
// Re-evaluate requestCapture with refreshed state AFTER drawImage is dispatched.
struct CoverPaintDecision { bool active; bool drawImage; bool requestCapture; };
inline CoverPaintDecision coverPaintDecision(bool hasImage, bool enabled,
        bool stateFlag, int32_t counter, int32_t threshold,
        bool inhibitCapture, bool alreadyRequested) {
    const bool active = hasImage && enabled;
    return {active, active && stateFlag && counter <= threshold,
            active && counter == 10 && !inhibitCapture && !alreadyRequested};
}
// Call AFTER external effects and reload the actual counter. ARM ADDS wraps;
// avoid signed-overflow UB for INT_MAX in the reconstructed C++.
inline int32_t coverPaintNextCounter(int32_t counter) {
    const uint32_t bits = static_cast<uint32_t>(counter) + uint32_t(1);
    int32_t next;
    std::memcpy(&next, &bits, sizeof(next));
    return next;
}
}
