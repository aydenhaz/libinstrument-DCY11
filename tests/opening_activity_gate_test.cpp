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
void** replacementTable;
bool changeAttached;
void first(void* receiver) {
    auto* activity = static_cast<Activity*>(receiver);
    assert(activity && activity->table);
    order.push_back(1);
    if (replacementTable) activity->table = replacementTable;
}
std::uintptr_t second(void* receiver) {
    auto* activity = static_cast<Activity*>(receiver);
    assert(activity && activity->table);
    order.push_back(2);
    if (changeAttached) activity->attached = activity;
    return 0x12345678;
}
std::uintptr_t staleSecond(void*) { assert(false); return 0; }
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
    // First call installs a new table. The old second slot must neither be
    // called nor required to exist before that first call.
    void* updated[26] = {};
    updated[25] = reinterpret_cast<void*>(&second);
    replacementTable = updated;
    changeAttached = true;
    for (void* old : {static_cast<void*>(nullptr), reinterpret_cast<void*>(&staleSecond)}) {
        table[25] = old;
        activity = {table, nullptr};
        order.clear();
        result = IPL_ABI::inspectOpeningActivity(&activity);
        assert((order == std::vector<int>{1, 2}));
        assert(result.secondVirtualResult == 0x12345678);
        assert(result.hasAttachedActivityState);
    }
    updated[25] = nullptr;
    activity = {table, nullptr};
    order.clear();
    try { IPL_ABI::inspectOpeningActivity(&activity); assert(false); }
    catch (const std::invalid_argument&) {}
    assert((order == std::vector<int>{1}));
    try { IPL_ABI::inspectOpeningActivity(nullptr); assert(false); }
    catch (const std::invalid_argument&) {}
}
