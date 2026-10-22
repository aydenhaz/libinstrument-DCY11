#include <ExternalScreenTypes.h>
#include <GFrame/GPIClosure.h>
#include <cassert>

struct Receiver {
    int total = 0;
    void cross(int engine, bool enabled) { total += engine + (enabled ? 1 : 0); }
    void map(int engine, int level, int mode, int position) {
        total += engine + level + mode + position;
    }
    void control(const tagSTExtScreenControl& value) { total += value.words[1]; }
};
#if defined(__arm__) && !defined(__aarch64__)
static_assert(sizeof(GPI::detail::BoundMember<Receiver, int, bool>) == 24,
              "V21 CrossControl bound arguments");
static_assert(sizeof(GPI::detail::BoundMember<Receiver, int, int, int, int>) == 32,
              "V21 MapStatus bound arguments");
static_assert(sizeof(GPI::detail::BoundMember<Receiver, const tagSTExtScreenControl&>) == 52,
              "V21 control-record bound arguments");
#endif
int main() {
    Receiver receiver;
    auto cross = GPI::Closure::bind(&receiver, &Receiver::cross, 3, true);
    assert(!cross.empty());
    auto copied = cross;
    assert(copied.equals(cross));
    cross.reset();
    assert(cross.empty());
    copied.invoke();
    assert(receiver.total == 4);

    auto map = GPI::Closure::bind(&receiver, &Receiver::map, 2, 5, 7, 9);
    map.invoke();
    assert(receiver.total == 27);

    tagSTExtScreenControl value;
    value.words[1] = 12;
    auto control = GPI::Closure::bind(&receiver, &Receiver::control, value);
    value.words[1] = 44;
    control.invoke();
    assert(receiver.total == 39);
}
