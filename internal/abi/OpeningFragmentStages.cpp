#include "OpeningFragmentStages.h"

#include <cstring>
#include <new>
#include <stdexcept>

namespace IPL_ABI {

PendingOpeningHost::PendingOpeningHost(const OpeningHostOps& ops) : ops_(ops) {
    if (!ops_.allocate || !ops_.deallocate || !ops_.constructFragment ||
        !ops_.destroyFragment || !ops_.constructObserver)
        throw std::invalid_argument("Opening host construction needs every operation");
    storage_ = ops_.allocate(kOpeningAllocationSize);
    if (!storage_) throw std::bad_alloc();
    try {
        // V21 0x41960..0x4197a: CGFragment launch mode 0, followed by the
        // inline hsl::ObserverBase name at +0x2e8, then engine ID 5.
        ops_.constructFragment(storage_, 0);
        fragmentConstructed_ = true;
        ops_.constructObserver(static_cast<unsigned char*>(storage_) +
                               kOpeningObserverOffset, "AutoBaseFragment");
        const std::uint32_t engine = 5;
        std::memcpy(static_cast<unsigned char*>(storage_) + kOpeningEngineOffset,
                    &engine, sizeof(engine));
    } catch (...) {
        if (fragmentConstructed_) ops_.destroyFragment(storage_);
        ops_.deallocate(storage_);
        storage_ = nullptr;
        throw;
    }
}

PendingOpeningHost::~PendingOpeningHost() noexcept {
    if (!storage_) return;
    // ObserverBase stores only a bounded inline name in this host version;
    // there is no exported ObserverBase destructor. The host Fragment owns
    // its separate prefix and is destroyed with its imported D2 entrypoint.
    if (fragmentConstructed_) ops_.destroyFragment(storage_);
    ops_.deallocate(storage_);
}

} // namespace IPL_ABI
