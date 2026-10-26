#include "ExscreenUiObservers.h"
#if defined(__arm__) && !defined(__aarch64__)
#include <cstdint>
#include <cstring>
#include <new>
namespace IPL_ABI {
namespace {
unsigned char* at(void* object, unsigned offset) {
    return static_cast<unsigned char*>(object) + offset;
}
template<class T> T read(void* object, unsigned offset) {
    T value; std::memcpy(&value, at(object, offset), sizeof value); return value;
}
template<class T> void write(void* object, unsigned offset, T value) {
    std::memcpy(at(object, offset), &value, sizeof value);
}
template<class F> F slot(void* object, unsigned offset) {
    return reinterpret_cast<F>(read<void**>(object, 0)[offset / 4]);
}
class Lock {
    void* object_;
public:
    explicit Lock(void* object): object_(object) {
        slot<void(*)(void*)>(object_, 8)(object_);
    }
    ~Lock() { slot<void(*)(void*)>(object_, 12)(object_); }
    Lock(const Lock&) = delete;
    Lock& operator=(const Lock&) = delete;
};
void add(void* owner, unsigned lockOffset, void* observer) {
    Lock lock(at(owner, lockOffset));
    if (!observer) return;
    void* array = at(owner, lockOffset + 0x10);
    void** data = read<void**>(array, 0);
    uint32_t capacity = read<uint32_t>(array, 4), count = read<uint32_t>(array, 8);
    for (uint32_t i = 0; i < count; ++i) if (data[i] == observer) return;
    const uint32_t maxCount = UINT32_MAX / sizeof(void*);
    if (count >= maxCount) throw std::bad_alloc();
    if (count + 1 > capacity) {
        uint32_t next = count + 1;
        if ((read<uint8_t>(array, 0x10) & 15) == 1) {
            uint32_t extra = capacity < 5 ? 5 : capacity >= 500 ? count / 4 : count;
            if (extra > maxCount - next) throw std::bad_alloc();
            next += extra;
        }
        void* allocator = at(array, 12);
        void** replacement = static_cast<void**>(
            slot<void*(*)(void*, uint32_t)>(allocator, 8)(allocator, next * sizeof(void*)));
        write(array, 0, replacement); write(array, 4, next);
        for (uint32_t i = 0; i < count; ++i)
            if (data && replacement) replacement[i] = data[i];
        slot<void(*)(void*, void*)>(allocator, 12)(allocator, data);
        data = replacement;
    }
    data[count] = observer;
    write(array, 8, count + 1);
}
void remove(void* owner, unsigned lockOffset, void* observer) {
    Lock lock(at(owner, lockOffset));
    void* array = at(owner, lockOffset + 0x10);
    void** data = read<void**>(array, 0);
    uint32_t count = read<uint32_t>(array, 8);
    for (uint32_t i = 0; i < count; ++i) {
        if (data[i] != observer) continue;
        for (uint32_t j = i + 1; j < count; ++j) data[j - 1] = data[j];
        write(array, 8, count - 1);
        return; // preserve original trailing word; remove only first match
    }
}
void set(void* owner, unsigned offset, void* observer) {
    Lock lock(at(owner, 0xb4));
    write(owner, offset, observer);
}
}
void addMapRenderObserver(void* owner, void* observer) { add(owner, 0x24, observer); }
void removeMapRenderObserver(void* owner, void* observer) { remove(owner, 0x24, observer); }
void addUiUpdateObserver(void* owner, void* observer) { add(owner, 0x48, observer); }
void removeUiUpdateObserver(void* owner, void* observer) { remove(owner, 0x48, observer); }
void setProjectionSizeListener(void* owner, void* observer) { set(owner, 0xc4, observer); }
void setUiStyleListener(void* owner, void* observer) { set(owner, 0xc8, observer); }
}
#endif
