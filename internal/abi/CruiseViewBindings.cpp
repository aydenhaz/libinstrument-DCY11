#include "CruiseViewBindings.h"
#if defined(__arm__) && !defined(__aarch64__)
#include <hsl/HSL.h>
#include <cstdint>
#include <cstring>

extern "C" void cruiseBaseCreate(void*, void*)
    __asm__("_ZN9GNS_FRAME10CGFragment12onCreateViewEPNS_11CGViewGroupE");
extern "C" void cruiseBaseDestroy(void*)
    __asm__("_ZN9GNS_FRAME10CGFragment13onDestroyViewEv");
extern "C" void cruiseAddTest(void*, void*)
    __asm__("_ZN3hsl12SystemNotify18AddTestNumObserverEPNS_15TestNumObserverE");
extern "C" void cruiseRemoveTest(void*, void*)
    __asm__("_ZN3hsl12SystemNotify21RemoveTestNumObserverEPNS_15TestNumObserverE");
extern "C" void cruiseAddConfig(void*, void*)
    __asm__("_ZN3hsl6Config17AddConfigObserverEPNS_14ConfigObserverE");
extern "C" void cruiseRemoveConfig(void*, void*)
    __asm__("_ZN3hsl6Config20RemoveConfigObserverEPNS_14ConfigObserverE");

namespace IPL_ABI {
namespace {
unsigned char* at(void* fragment, unsigned offset) {
    return static_cast<unsigned char*>(fragment) + offset;
}
void* load(void* fragment, unsigned offset) {
    void* value;
    std::memcpy(&value, at(fragment, offset), sizeof value);
    return value;
}
void save(void* fragment, unsigned offset, void* value) {
    std::memcpy(at(fragment, offset), &value, sizeof value);
}
template<class Function> Function slot(void* object, unsigned offset) {
    return reinterpret_cast<Function>((*static_cast<void***>(object))[offset / 4]);
}
void* guideService() {
    void* guide = hsl::HSL::GetGuideHandle();
    return guide ? slot<void*(*)(void*)>(guide, 0x20)(guide) : nullptr;
}
}
void beginCruiseView(void* fragment, void* parent) {
    // AutoBaseFragment 0x41a50 only logs before tail-calling CGFragment.
    cruiseBaseCreate(fragment, parent);
    cruiseAddTest(hsl::HSL::GetSystemNotifyHandle(), at(fragment, 0x2e4));
}
void attachCruiseGuide(void* fragment) {
    if (void* service = guideService())
        slot<void(*)(void*, void*)>(service, 0x18)(service, at(fragment, 0x364));
}
void attachCruiseViewTail(void* fragment) {
    if (void* map = load(fragment, 0x3c0))
        save(fragment, 0x3c4, slot<void*(*)(void*)>(map, 0x2b4)(map));
    void* pos = hsl::HSL::GetPosHandle();
    save(fragment, 0x3c8, pos);
    if (pos) slot<void(*)(void*, void*)>(pos, 0x1c)(pos, at(fragment, 0x368));
    void* config = hsl::HSL::GetConfigHandle();
    save(fragment, 0x3d0, config);
    if (config) cruiseAddConfig(config, at(fragment, 0x38c));
}
void detachCruiseView(void* fragment) {
    cruiseRemoveTest(hsl::HSL::GetSystemNotifyHandle(), at(fragment, 0x2e4));
    // Original cleanup uses cached Pos/Config, but re-queries Guide service.
    if (void* pos = load(fragment, 0x3c8))
        slot<void(*)(void*, void*)>(pos, 0x20)(pos, at(fragment, 0x368));
    if (void* config = load(fragment, 0x3d0))
        cruiseRemoveConfig(config, at(fragment, 0x38c));
    if (void* service = guideService())
        slot<void(*)(void*, void*)>(service, 0x1c)(service, at(fragment, 0x364));
    // Broadcasts are removed by the caller; pending UI removals belong before this.
    cruiseBaseDestroy(fragment);
}
}
#endif
