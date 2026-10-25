#include "CruiseLocation.h"
#if defined(__arm__) && !defined(__aarch64__)
#include <hsl/HSL.h>
#include <hsl/Pos.h>
namespace IPL_ABI {
namespace {
template<class Function> Function slot(void* object, unsigned offset) {
    return reinterpret_cast<Function>((*static_cast<void***>(object))[offset / 4]);
}
void* guide() { return hsl::HSL::GetGuideHandle(); }
void* service(void* g) { return slot<void*(*)(void*)>(g, 0x20)(g); }
int mode0(void* s) { return slot<int(*)(void*)>(s, 0xac)(s); }
int mode1(void* s) { return slot<int(*)(void*)>(s, 0xc4)(s); }
void carPosition(bl::CarLoc& car, int engine) {
    hsl::HSL::GetCarPosition(car, static_cast<bl::MapEngineID>(engine));
}
void* position() { return hsl::HSL::GetPosHandle(); }
void locInfo(void* pos, bl::LocInfo& info) { static_cast<hsl::Pos*>(pos)->GetLocInfo(info); }
void goTo(void* map, double x, double y) {
    // V21 0x3edf4..0x3ee18: softfp double args and four stack words.
    slot<void(*)(void*, double, double, int, int, int, int)>(map, 0x1a8)(map, x, y, 0, 0, 0, 1);
}
void setCar(void* map, const bl::CarLoc& car) {
    void* layer = slot<void*(*)(void*)>(map, 0x2cc)(map);
    slot<void(*)(void*, const bl::CarLoc&)>(layer, 8)(layer, car);
}
}
const CruiseLocationOps& cruiseLocationHostOps() {
    static const CruiseLocationOps ops = {guide, service, mode0, mode1, carPosition,
        position, locInfo, goTo, setCar};
    return ops;
}
}
#endif
