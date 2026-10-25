#include "CruiseLocation.h"
#include <cmath>
#include <cstring>
#include <limits>
namespace IPL_ABI {
int cruiseNaviType(const CruiseLocationOps& ops) {
    void* guide = ops.guide();
    if (!guide) return 2;
    void* service = ops.guideService(guide);
    if (!service) return 2;
    if (ops.serviceMode0(service)) return 0;
    return ops.serviceMode1(service) ? 1 : 2;
}
// ARM VCVT.S32.F32 truncates, saturates and maps NaN to zero.
static int32_t speedWord(float speed) {
    if (std::isnan(speed)) return 0;
    if (double(speed) >= double(INT32_MAX)) return INT32_MAX;
    if (double(speed) <= double(INT32_MIN)) return INT32_MIN;
    return static_cast<int32_t>(speed);
}
bool initializeCruiseLocation(void* map, const CruiseLocationOps& ops) {
    if (!map) return false;
    bl::CarLoc car;
    if (cruiseNaviType(ops) == 1) {
        ops.carPosition(car, 1);
    } else {
        void* pos = ops.position();
        if (!pos) return false;
        bl::LocInfo info;
        ops.locInfo(pos, info);
        uint32_t count;
        std::memcpy(&count, info.suffix + 0x4b0 - 0x12c, 4);
        if (count > 3) return false; // only three inline 0x110-byte records
        float speed;
        std::memcpy(&speed, info.prefix + 0x10, 4);
        car.prefix[0] = static_cast<uint32_t>(speedWord(speed));
        for (uint32_t i = 0; i < count; ++i) {
            bl::CarLocationEntry entry = {};
            entry.assignLocInfoRecord(info.suffix + 0x180 - 0x12c + i * 0x110);
            car.locations.append(entry);
        }
    }
    if (!car.locations.count || !car.locations.data) return false;
    const auto* first = static_cast<const unsigned char*>(car.locations.data);
    double x, y;
    std::memcpy(&x, first + 0x18, 8);
    std::memcpy(&y, first + 0x20, 8);
    ops.goTo(map, x, y);
    ops.setCar(map, car);
    return true;
}
}
