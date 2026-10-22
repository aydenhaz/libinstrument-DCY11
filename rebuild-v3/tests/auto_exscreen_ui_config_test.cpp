#include "../internal/NS_MultiScreenBase/AutoExscreenUiConfig.h"
#include "../internal/NS_MultiScreenBase/AutoExscreenMapStatusPlan.h"
#include <DiceString16.h>
#include <NS_MultiScreenBase/AutoConfigScreenMode.h>
#include <hsl/Bundle.h>
#include <hsl/Config.h>
#include <hsl/HSL.h>
#include <cassert>
#include <cstdint>
#include <map>
#include <string>

namespace {
using Key = std::u16string;
Key key(const dice::String16& s) {
    return Key(reinterpret_cast<const char16_t*>(s.c_str()), s.size());
}
struct Node {
    std::map<Key, Node> bundles;
    std::map<Key, int32_t> ints;
};
std::map<const hsl::Bundle*, Node> bundleData;
std::map<Key, Node> rootData;
hsl::Config config;
}
namespace hsl {
Bundle::Bundle() { bundleData[this] = Node{}; }
Bundle::~Bundle() { bundleData.erase(this); }
void Bundle::PutBundle(const dice::String16& k, const Bundle& value) {
    bundleData[this].bundles[key(k)] = bundleData.at(&value);
}
bool Bundle::GetBundle(const dice::String16& k, Bundle& value) {
    const auto found = bundleData.at(this).bundles.find(key(k));
    if (found == bundleData.at(this).bundles.end()) return false;
    bundleData[&value] = found->second;
    return true;
}
void Bundle::PutInt32(const dice::String16& k, int32_t value) {
    bundleData[this].ints[key(k)] = value;
}
bool Bundle::GetInt32(const dice::String16& k, int32_t& value) {
    const auto found = bundleData.at(this).ints.find(key(k));
    if (found == bundleData.at(this).ints.end()) return false;
    value = found->second;
    return true;
}
bool Config::GetBundle(const dice::String16& k, Bundle& result) {
    const auto found = rootData.find(key(k));
    if (found == rootData.end()) return false;
    bundleData[&result] = found->second;
    return true;
}
bool Config::SetBundle(const dice::String16& k, const Bundle& value, SetterType_ setter) {
    assert(static_cast<int>(setter) == 1);
    rootData[key(k)] = bundleData.at(&value);
    return true;
}
Config* HSL::GetConfigHandle() { return &config; }
}
namespace NS_MultiScreenBase {
AutoConfigScreenMode* AutoConfigScreenMode::GetInstance() {
    static AutoConfigScreenMode instance;
    return &instance;
}
int AutoConfigScreenMode::GetCurrentScreenMode(ScreenType) { return 7; }
}
int main() {
    const Key screen = u"ExScreenLarge";
    Node& before = rootData[screen];
    before.bundles[u"kExScreenModeKey7"].bundles[u"kExScreenGuideKey"]
          .ints[u"int32MapLevel"] = 12;
    before.bundles[u"kExScreenModeKey7"].bundles[u"kExScreenCuriseKey"]
          .ints[u"int32MapViewMode"] = 3;
    NS_MultiScreenBase::AutoExscreenUtil_SetMapParamsToUiConfig(
        NS_MultiScreenBase::SCREEN_TYPE_MAIN, 7, 1, 2, 1);
    const auto& after = rootData.at(screen);
    const auto& mode7 = after.bundles.at(u"kExScreenModeKey7");
    const auto& guide = mode7.bundles.at(u"kExScreenGuideKey");
    assert(guide.ints.at(u"int32CrossControl") == 1);
    assert(guide.ints.at(u"int32MapLevel") == 12);
    assert(mode7.bundles.at(u"kExScreenCuriseKey").ints.at(u"int32MapViewMode") == 3);
    assert(after.bundles.at(u"kExScreenModeKey19")
           .bundles.at(u"kExScreenPreviewKey").ints.at(u"int32MapPreviwe") == 99);
    const Node unchanged = after;
    NS_MultiScreenBase::AutoExscreenUtil_SetMapParamsToUiConfig(
        NS_MultiScreenBase::SCREEN_TYPE_MAIN, 20, 1, 2, 1);
    assert(rootData.at(screen).bundles.size() == unchanged.bundles.size());
    assert(NS_MultiScreenBase::AutoExscreenUtil_IsDynamicLevelRequest(0, -1));
    assert(NS_MultiScreenBase::AutoExscreenUtil_IsDynamicLevelRequest(-1, 99));
    assert(!NS_MultiScreenBase::AutoExscreenUtil_IsDynamicLevelRequest(0, 99));
    assert(!NS_MultiScreenBase::AutoExscreenUtil_IsDynamicLevelRequest(-1, 1));

    using namespace NS_MultiScreenBase;
    int* currentScene = AutoExscreenUtil_GetCurrentSceneSlot(SCREEN_TYPE_MAIN);
    assert(*currentScene == 0);
    *currentScene = 2;
    AutoExscreenUtil_SetMapViewLevelToUiConfig(SCREEN_TYPE_MAIN, 15);
    AutoExscreenUtil_SetMapViewModeToUiConfig(SCREEN_TYPE_MAIN, 2);
    const auto& written = rootData.at(screen).bundles.at(u"kExScreenModeKey7");
    assert(written.bundles.at(u"kExScreenPreviewKey").ints.at(u"int32MapLevel") == 15);
    assert(written.bundles.at(u"kExScreenPreviewKey").ints.at(u"int32MapViewMode") == 99);
    assert(written.bundles.at(u"kExScreenGuideKey").ints.at(u"int32MapViewMode") == 2);
    assert(written.bundles.at(u"kExScreenGuideKey").ints.at(u"int32CrossControl") == 1);
    *currentScene = 0;
    AutoExscreenUtil_SetMapViewModeToUiConfig(SCREEN_TYPE_MAIN, 3);
    assert(rootData.at(screen).bundles.at(u"kExScreenModeKey7")
           .bundles.at(u"kExScreenCuriseKey").ints.at(u"int32MapViewMode") == 3);

    const MapStatusPlan noChanges = AutoExscreenUtil_PlanMapStatus(4, 99, 99);
    assert(!noChanges.writeLevel && !noChanges.writeMode);
    assert(noChanges.resetMapBindingList);
    const MapStatusPlan dynamic = AutoExscreenUtil_PlanMapStatus(-1, 99, 1);
    assert(!dynamic.writeLevel && dynamic.writeMode && dynamic.clearDynamicLevel);
    assert(dynamic.attemptTimerStart && dynamic.setMapViewMode);
    const MapStatusPlan requestDynamic = AutoExscreenUtil_PlanMapStatus(10, -1, 1);
    assert(requestDynamic.writeLevel && requestDynamic.setMapLevel);
    assert(requestDynamic.writeMode && !requestDynamic.clearDynamicLevel);
    assert(!requestDynamic.attemptTimerStart && !requestDynamic.setMapViewMode);
    const MapStatusPlan fixed = AutoExscreenUtil_PlanMapStatus(10, 12, 1);
    assert(fixed.writeLevel && fixed.writeMode && fixed.setMapViewMode);
    assert(!fixed.clearDynamicLevel && !fixed.attemptTimerStart);
}
