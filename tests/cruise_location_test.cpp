#include "abi/CruiseLocation.h"
#include <cassert>
#include <cstring>
#include <limits>
#include <vector>
using namespace IPL_ABI;
static int g, s, a, b, p, count;
static float speed;
static std::vector<int> calls;
static void* token = reinterpret_cast<void*>(0x1234);
static void* guide() { calls.push_back(1); return g ? token : nullptr; }
static void* service(void* v) { assert(v == token); calls.push_back(2); return s ? token : nullptr; }
static int mode0(void*) { calls.push_back(3); return a; }
static int mode1(void*) { calls.push_back(4); return b; }
static void car(bl::CarLoc& car, int engine) {
    assert(engine == 1); calls.push_back(5);
    bl::CarLocationEntry e = {};
    double x = 12.5, y = -23.25;
    std::memcpy(e.bytes + 0x18, &x, 8); std::memcpy(e.bytes + 0x20, &y, 8);
    if (count) car.locations.append(e);
}
static void* position() { calls.push_back(6); return p ? token : nullptr; }
static void info(void*, bl::LocInfo& l) {
    calls.push_back(7);
    std::memcpy(l.prefix + 0x10, &speed, 4);
    std::memcpy(l.suffix + 0x4b0 - 0x12c, &count, 4);
    for (int i = 0; i < 3; ++i) {
        double x = 12.5 + i, y = -23.25 - i;
        std::memcpy(l.suffix + 0x180 - 0x12c + i * 0x110, &x, 8);
        std::memcpy(l.suffix + 0x188 - 0x12c + i * 0x110, &y, 8);
    }
}
static int32_t expectedSpeed;
static void go(void* map, double x, double y) {
    assert(map == token && x == 12.5 && y == -23.25); calls.push_back(8);
}
static void set(void* map, const bl::CarLoc& car) {
    assert(map == token && calls.back() == 8); calls.push_back(9);
    assert(car.locations.count == unsigned(count));
    assert(int32_t(car.prefix[0]) == expectedSpeed);
}
int main() {
    CruiseLocationOps ops = {guide, service, mode0, mode1, car, position, info, go, set};
    assert(!initializeCruiseLocation(nullptr, ops) && calls.empty());
    for (g = 0; g < 2; ++g) for (s = 0; s < 2; ++s)
    for (a = 0; a < 2; ++a) for (b = 0; b < 2; ++b) {
        calls.clear();
        int expected = !g || !s ? 2 : a ? 0 : b ? 1 : 2;
        assert(cruiseNaviType(ops) == expected);
        std::vector<int> trace{1}; if(g) { trace.push_back(2); if(s) {trace.push_back(3); if(!a)trace.push_back(4);}}
        assert(calls == trace);
        for (p = 0; p < 2; ++p) for(count = 0; count <= 3; ++count) {
            if (expected == 1 && count > 1) continue;
            calls.clear(); speed = 17.9f; expectedSpeed = expected == 1 ? 0 : 17;
            bool ok = initializeCruiseLocation(token, ops);
            assert(ok == bool(count && (expected == 1 || p)));
            if(ok) assert(calls.back() == 9);
        }
    }
    g = s = 1; a = b = 0; p = 1;
    for(count = 4; count < 6; ++count) {
        calls.clear(); assert(!initializeCruiseLocation(token, ops)); assert(calls.back() == 7);
    }
    count = 1;
    const float values[] = {-17.9f, std::numeric_limits<float>::infinity(),
        -std::numeric_limits<float>::infinity(), std::numeric_limits<float>::quiet_NaN()};
    const int32_t expected[] = {-17, INT32_MAX, INT32_MIN, 0};
    for(int i=0;i<4;++i) { speed=values[i]; expectedSpeed=expected[i]; assert(initializeCruiseLocation(token,ops)); }
}
