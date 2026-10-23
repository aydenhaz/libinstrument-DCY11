#include "../internal/abi/OpeningActivityGate.h"

#include <cassert>
#include <cstdint>
#include <cstring>
#include <stdexcept>
#include <vector>

namespace {
struct Activity {
    void** table;
    void* attached;
};
std::vector<int> order;
void first(void* receiver) {
    auto* activity = static_cast<Activity*>(receiver);
    assert(activity && activity->table);
    order.push_back(1);
}
std::uintptr_t second(void* receiver) {
    auto* activity = static_cast<Activity*>(receiver);
    assert(activity && activity->table);
    order.push_back(2);
    return 0x12345678;
}
}

int main() {
    void* table[26] = {};
    table[12] = reinterpret_cast<void*>(&first);
    table[25] = reinterpret_cast<void*>(&second);
    Activity activity = {table, nullptr};
    auto result = IPL_ABI::inspectOpeningActivity(&activity);
    assert((order == std::vector<int>{1, 2}));
    assert(result.secondVirtualResult == 0x12345678 &&
           !result.hasAttachedActivityState);
    order.clear();
    activity.attached = &activity;
    result = IPL_ABI::inspectOpeningActivity(&activity);
    assert((order == std::vector<int>{1, 2}));
    assert(result.hasAttachedActivityState);
    try { IPL_ABI::inspectOpeningActivity(nullptr); assert(false); }
    catch (const std::invalid_argument&) {}
}
