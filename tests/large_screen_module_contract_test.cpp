// Execute the restored module ABI with mock registration and ALC services.
#include <cassert>
#include <cstdarg>
#include <cstdint>
#include <cstring>
#include <vector>
#include <GFrame/GblAlc.h>
#include <GFrame/CGIntent.h>
#include <ExModuleManager.h>
#include "AlcExport.h"
#include "NS_InstrumentPannelLarge/LargeScreenImpl.h"

using NS_InstrumentPannelLarge::LargeScreenImpl;
static LargeScreenImpl* registered;
static LargeScreenImpl* lastModule;
static int registrations, unregisters, dispatches;
static std::vector<int> traceLines;
static void emit(void*, int level, int reserved, uintptr_t r3,
                 uint32_t flag, uint32_t zero, const char* module,
                 const char* aux, const char* sig, int line,
                 const char* name, ...) {
    assert(level == 8 && reserved == 0 && flag == 0x08000000 && zero == 0);
    assert(!aux[0] && sig[0]);
    va_list values;
    va_start(values, name);
    if (line == 0x12e) {
        assert(!std::strcmp(module, "HMI_PANNELLARGE"));
        assert(!std::strcmp(name, "onInitEnd"));
        assert(r3 == reinterpret_cast<uintptr_t>(&emit));
    } else {
        assert(!std::strcmp(module, "HMI_COMMON"));
        assert(va_arg(values, LargeScreenImpl*) == lastModule);
        if (line == 0x22) {
            assert(!std::strcmp(name, "ModuleReg:%p"));
            assert(r3 == reinterpret_cast<uintptr_t>(aux));
        } else if (line == 0x4b) {
            assert(!std::strcmp(name, "ModuleUnReg:%p"));
            assert(r3 == 0x08000000);
        } else if (line == 0x9f) {
            assert(!std::strcmp(name, "QueryInterface:%p"));
            assert(r3 == reinterpret_cast<uintptr_t>(&emit));
        } else {
            assert(line == 0xa6 && !std::strcmp(name, "QueryInterface:%p %p"));
            assert(va_arg(values, void*) == static_cast<IMultiScreenInterfaces*>(lastModule));
            assert(r3 == reinterpret_cast<uintptr_t>(&emit));
        }
    }
    va_end(values);
    traceLines.push_back(line);
}
extern "C" void* I_GetAlcPtr() {
    static void* table[] = {reinterpret_cast<void*>(&emit)};
    static void** object = table;
    return &object;
}
GNS_FRAME::CGIntent::CGIntent() {}
GNS_FRAME::CGIntent::~CGIntent() {}
ExModuleManager* ExModuleManager::GetInstance() {
    static ExModuleManager manager;
    return &manager;
}
int ExModuleManager::ModuleReg(ModuleInterfaceId id, void* object) {
    assert(static_cast<int>(id) == 0x1f40);
    assert(object && !registered);
    registered = static_cast<LargeScreenImpl*>(object);
    lastModule = registered;
    ++registrations;
    return 0;
}
int ExModuleManager::ModuleUnReg(ModuleInterfaceId id) {
    assert(static_cast<int>(id) == 0x1f40 && registered);
    registered = nullptr;
    ++unregisters;
    return 0;
}
int32_t LargeScreenImpl::doStartFragment(int index, GNS_FRAME::CGFragment*,
                                          const GNS_FRAME::CGIntent&) {
    assert(this == registered && index == 7);
    ++dispatches;
    return 31;
}

int main() {
#if UINTPTR_MAX == 0xffffffffu
    static_assert(sizeof(LargeScreenImpl) == 0x10, "V21 module allocation");
#endif
    AlcExport("onInitEnd", "virtual void NS_InstrumentPannelLarge::CGIPLPacket::onInitEnd()", 0x12e);
    auto* module = new LargeScreenImpl;
    assert(registered == module && registrations == 1);
    IBaseModuleEx* primary = module;
    IMultiScreenInterfaces* secondary = module;
    assert(reinterpret_cast<uintptr_t>(secondary) -
           reinterpret_cast<uintptr_t>(primary) == sizeof(void*));
    void* out = nullptr;
    assert(primary->QueryInterface(123, &out) == 0 && out == secondary);
    assert(primary->QueryInterface(123, nullptr) == -1);
    GNS_FRAME::CGIntent intent;
    assert(primary->doStartFragment(7, nullptr, intent) == 31);
    assert(secondary->doStartFragment(7, nullptr, intent) == 31);
    assert(dispatches == 2);
    delete secondary;
    assert(!registered && unregisters == 1);
    assert((traceLines == std::vector<int>{0x12e, 0x22, 0x9f, 0xa6, 0x9f, 0x4b}));
}
