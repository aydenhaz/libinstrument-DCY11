#pragma once
#include <bl/LocationTypes.h>
namespace IPL_ABI {
struct CruiseLocationOps {
    void* (*guide)();
    void* (*guideService)(void*);
    int (*serviceMode0)(void*);
    int (*serviceMode1)(void*);
    void (*carPosition)(bl::CarLoc&, int);
    void* (*position)();
    void (*locInfo)(void*, bl::LocInfo&);
    void (*goTo)(void*, double, double);
    void (*setCar)(void*, const bl::CarLoc&);
};
int cruiseNaviType(const CruiseLocationOps&);
// Returns false for missing/empty location or a corrupt host record count.
// Owns temporary location storage through the synchronous map calls.
bool initializeCruiseLocation(void* map, const CruiseLocationOps&);
#if defined(__arm__) && !defined(__aarch64__)
const CruiseLocationOps& cruiseLocationHostOps();
#endif
}
