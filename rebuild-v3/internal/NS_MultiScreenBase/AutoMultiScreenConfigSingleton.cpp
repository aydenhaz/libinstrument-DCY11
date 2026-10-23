#include "AutoMultiScreenConfigBinding.h"
#include <NS_MultiScreenBase/AutoConfigJsonData.h>
#include <NS_MultiScreenBase/AutoMultiScreenConfig.h>
#include <cstdio>
#include <cstdint>
#include <cstring>
#include <new>
#include <string>

// The V21 singleton and asl::Mutex/LockLinux objects have four-byte pointers.
// A native x86_64 object cannot reproduce their layout.
#if defined(__arm__) && !defined(__aarch64__)
static_assert(sizeof(void*) == 4, "V21 ARM32 pointers");
static_assert(sizeof(std::string) == 12, "V21 scene string extent");
static_assert(alignof(std::string) == 4, "V21 scene string alignment");

extern "C" unsigned char _ZTVN3asl5MutexE[];
extern "C" void v21LockCtor(void*) __asm__("_ZN3asl9LockLinuxC1Ev");
extern "C" void v21LockDtor(void*) __asm__("_ZN3asl9LockLinuxD1Ev");

namespace NS_MultiScreenBase {
namespace {
void writeWord(unsigned char* at, int32_t value) {
    std::memcpy(at, &value, sizeof(value));
}
void prepareMutex(unsigned char* at) {
    void* addressPoint = _ZTVN3asl5MutexE + 8;
    std::memcpy(at, &addressPoint, sizeof(addressPoint));
}
void mutexOperation(unsigned char* wrapper, unsigned slot) {
    typedef void (*Operation)(void*);
    void** vtable = *reinterpret_cast<void***>(wrapper);
    reinterpret_cast<Operation>(vtable[slot])(wrapper);
}
void destroySceneStrings(unsigned char* scene) {
    typedef std::string SceneString;
    for (unsigned offset : {0x6cu, 0x60u, 0x54u})
        reinterpret_cast<SceneString*>(scene + offset)->~SceneString();
}
}

AutoMultiScreenConfig::AutoMultiScreenConfig(ScreenType screen) : raw_{} {
    const int screenIndex = static_cast<int>(screen);
    for (int i = 0; i < 3; ++i) {
        unsigned char* record = raw_ + 0xe4 * i;
        new (record) SceneConfig();
        AutoMultiScreenConfig_InitSceneRecord(record);
        for (unsigned offset : {0x54u, 0x60u, 0x6cu})
            new (record + offset) std::string();
        AutoMultiScreenConfig_InitBindingRecord(raw_ + 0x2ac + 0x20 * i,
                                                 screenIndex);
    }
    writeWord(raw_ + 0x30c, screenIndex);
    writeWord(raw_ + 0x310, 0);
    writeWord(raw_ + 0x314, 3);
    prepareMutex(raw_ + 0x318);
    v21LockCtor(raw_ + 0x31c);
    writeWord(raw_ + 0x338, -1);
    raw_[0x33c] = 0;
    std::memset(raw_ + 0x328, 0, 16);
    const int engine = screenIndex == 0 ? 5 :
                       screenIndex == 1 ? 3 : screenIndex == 2 ? 7 : -1;
    std::snprintf(reinterpret_cast<char*>(raw_ + 0x328), 15,
                  "HMI_CONFIG-%d", engine);

    mutexOperation(raw_ + 0x318, 2);
    AutoConfigJsonData* json = AutoConfigJsonData::GetInstance();
    for (int i = 0; i < 3; ++i) {
        json->GetSceneConfig(screen, static_cast<SceneType>(i),
                             *reinterpret_cast<SceneConfig*>(raw_ + 0xe4 * i));
    }
    mutexOperation(raw_ + 0x318, 3);
}

AutoMultiScreenConfig::~AutoMultiScreenConfig() {
    prepareMutex(raw_ + 0x318);
    v21LockDtor(raw_ + 0x31c);
    for (int i = 2; i >= 0; --i)
        destroySceneStrings(raw_ + 0xe4 * i);
}

AutoMultiScreenConfig* AutoMultiScreenConfig::GetInstance(ScreenType screen) {
    if (static_cast<int>(screen) == 1) {
        static AutoMultiScreenConfig small(screen);
        return &small;
    }
    if (static_cast<int>(screen) == 2) {
        static AutoMultiScreenConfig widget(screen);
        return &widget;
    }
    // V21 sends other selectors to the first singleton. The first call's
    // selector, even when invalid, is passed to its constructor.
    static AutoMultiScreenConfig large(screen);
    return &large;
}
}
#endif
