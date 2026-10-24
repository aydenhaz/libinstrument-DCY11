#include "OpeningActivityGate.h"

#include <cstring>
#include <stdexcept>

namespace IPL_ABI {

OpeningActivityGateResult inspectOpeningActivity(void* activity) {
    if (!activity) throw std::invalid_argument("constructed activity required");
    void** table;
    std::memcpy(&table, activity, sizeof(table));
    if (!table || !table[12])
        throw std::invalid_argument("activity is missing V21 virtual slots");
    // V21 offsets are ARM32 virtual-table *bytes*. Native 64-bit mock tables
    // retain their numeric slot indices (12 and 25).
    using First = void (*)(void*);
    using Second = std::uintptr_t (*)(void*);
    reinterpret_cast<First>(table[12])(activity);
    // V21 reloads the object's vptr at 0x2ece6 after the first call.
    // The first virtual operation may replace its dispatch table/state.
    std::memcpy(&table, activity, sizeof(table));
    if (!table || !table[25])
        throw std::invalid_argument("activity is missing the second V21 virtual slot");
    const std::uintptr_t second =
        reinterpret_cast<Second>(table[25])(activity);
    void* attached;
    std::memcpy(&attached, static_cast<unsigned char*>(activity) + 8,
                sizeof(attached));
    return {second, attached != nullptr};
}

} // namespace IPL_ABI
