// Execute the restored module ABI with mock registration and ALC services.
#include <cassert>
#include <cstdint>
#include <GFrame/GblAlc.h>
#include <GFrame/CGIntent.h>
#include <ExModuleManager.h>
#include "NS_InstrumentPannelLarge/LargeScreenImpl.h"

using NS_InstrumentPannelLarge::LargeScreenImpl;
static LargeScreenImpl* registered;
static int registrations, unregisters, dispatches;
extern "C" void* I_GetAlcPtr() { return nullptr; }
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
}
