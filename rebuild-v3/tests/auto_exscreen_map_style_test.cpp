#include "../internal/NS_MultiScreenBase/AutoExscreenMapStyle.h"
#include <cassert>
#include <cstring>
#include <string>

namespace {
void* ownSlots[0x434 / 4] = {};
void* referenceSlots[0x434 / 4] = {};
struct FakeView { void** methods; } own = {ownSlots}, reference = {referenceSlots};
bool attached = false;
int nightMode = 0;
int theme = 0;
int styleTime = -1;
bool specified = false;
std::string appliedName;
std::string events;

int getDevice(void* view) {
    assert(view == &reference);
    events += 'd';
    return 7;
}
bool isAttached(void* view, int id) {
    assert(view == &reference && id == 7);
    events += 'a';
    return attached;
}
void getTheme(void* view, void* output) {
    assert(view == &reference);
    auto* bytes = static_cast<unsigned char*>(output);
    assert(bytes[0x10] == 1 && bytes[0x11] == 0x81 && bytes[0x50] == 0);
    std::memcpy(bytes + 4, &theme, sizeof(theme));
    events += 'h';
}
int getNight(void* view) {
    assert(view == &reference);
    events += 'n';
    return nightMode;
}
void setSpecified(void* view, bool enable) {
    assert(view == &own);
    specified = enable;
    events += 's';
}
void setName(void* view, const char* name) {
    assert(view == &own && specified);
    appliedName = name;
    events += 'N';
}
void setTime(void* view, int value, bool a, bool b, bool c) {
    assert(view == &own && !a && !b && !c);
    styleTime = value;
    events += 't';
}
void reset() { styleTime = -1; specified = false; appliedName.clear(); events.clear(); }
}

int main() {
    ownSlots[0x150 / 4] = reinterpret_cast<void*>(&setSpecified);
    ownSlots[0x14c / 4] = reinterpret_cast<void*>(&setName);
    ownSlots[0x158 / 4] = reinterpret_cast<void*>(&setTime);
    referenceSlots[0xa8 / 4] = reinterpret_cast<void*>(&getDevice);
    referenceSlots[0x430 / 4] = reinterpret_cast<void*>(&isAttached);
    referenceSlots[0x168 / 4] = reinterpret_cast<void*>(&getTheme);
    referenceSlots[0x16c / 4] = reinterpret_cast<void*>(&getNight);
    using NS_MultiScreenBase::AutoExscreenUtil_ApplyMapStyleWithViews;

    AutoExscreenUtil_ApplyMapStyleWithViews(&own, &reference, 2, "day", "night");
    assert(events == "sNt" && specified && appliedName == "day" && styleTime == 0);
    reset();
    AutoExscreenUtil_ApplyMapStyleWithViews(&own, &reference, 1, "day", "night");
    assert(events == "sNt" && specified && appliedName == "night" && styleTime == 1);
    reset();
    AutoExscreenUtil_ApplyMapStyleWithViews(&own, &reference, 1, "day", "");
    assert(events == "st" && !specified && styleTime == 1);
    reset();
    nightMode = 1;
    AutoExscreenUtil_ApplyMapStyleWithViews(&own, &reference, 0, "day", "night");
    assert(events == "dansNt" && appliedName == "night" && styleTime == 1);
    reset();
    attached = true;
    theme = 0;
    AutoExscreenUtil_ApplyMapStyleWithViews(&own, &reference, 0, "day", "night");
    assert(events == "dahsNt" && appliedName == "day" && styleTime == 0);
    reset();
    theme = 1;
    AutoExscreenUtil_ApplyMapStyleWithViews(&own, &reference, 0, "day", "");
    assert(events == "dahsNt" && appliedName == "day" && styleTime == 1);
    reset();
    AutoExscreenUtil_ApplyMapStyleWithViews(&own, &reference, 0, "", "");
    assert(events == "dahst" && !specified && styleTime == 1);
    reset();
    AutoExscreenUtil_ApplyMapStyleWithViews(nullptr, &reference, 0, "day", "night");
    AutoExscreenUtil_ApplyMapStyleWithViews(&own, &reference, 42, "day", "night");
    assert(events.empty());
}
