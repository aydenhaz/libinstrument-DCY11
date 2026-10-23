#pragma once

#include <cstddef>
#include <cstdint>

namespace IPL_ABI {

constexpr std::size_t kOpeningAllocationSize = 0x440;
constexpr std::size_t kOpeningObserverOffset = 0x2e8;
constexpr std::size_t kOpeningEngineOffset = 0x308;
static_assert(kOpeningObserverOffset + 0x20 == kOpeningEngineOffset,
              "V21 ObserverBase's inline name ends before the engine word");

struct OpeningHostOps {
    void* (*allocate)(std::size_t);
    void (*deallocate)(void*);
    void (*constructFragment)(void*, int launchMode);
    void (*destroyFragment)(void*);
    void (*constructObserver)(void*, const char* name);
};

// First two V21 Opening constructors only. This guard always rolls back;
// derived vptrs, ObjectRuntimeInfo, surface state and activity ownership are
// still required before an Opening Fragment can escape to GFrame.
class PendingOpeningHost {
public:
    explicit PendingOpeningHost(const OpeningHostOps&);
    ~PendingOpeningHost() noexcept;
    PendingOpeningHost(const PendingOpeningHost&) = delete;
    PendingOpeningHost& operator=(const PendingOpeningHost&) = delete;
    const unsigned char* bytes() const {
        return static_cast<const unsigned char*>(storage_);
    }
private:
    OpeningHostOps ops_;
    void* storage_ = nullptr;
    bool fragmentConstructed_ = false;
};

#if defined(__arm__) && !defined(__aarch64__) && UINTPTR_MAX == 0xffffffffu
const OpeningHostOps& openingHostOps();
#endif

} // namespace IPL_ABI
