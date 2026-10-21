#include <cassert>
#include <cstdint>
#include <cstring>
#include <string>
#include "InstrumentPannelLarge.h"
#include <GFrame/CGString.h>
#include <GFrame/CGIntent.h>
#include <GFrame/CGPackageMgr.h>
#include <GFrame/CGFrameSimulation.h>
#include <hsl/MultiScreenService.h>

namespace {
bool created = false;
bool service_available=true, load_result=true;
int loads = 0, unloads = 0, sets = 0, extras = 0, alc_calls = 0;
GNS_FRAME::CGPackageMgr package_mgr;
GNS_FRAME::CGFrameSimulation simulation, second_simulation;
GNS_FRAME::CGFrameSimulation* expected_simulation=&simulation;
int manager_queries=0;
hsl::MultiScreenService service;
void alc_emit(void*, int count, int reserved, uintptr_t fourth,
              uint32_t flags, uint32_t zero, const char* module,
              const char* aux, const char* sig, int tag, const char* name) {
    ++alc_calls;
    assert(count == 8 && reserved == 0 && flags == 0x08000000 && zero == 0);
    assert(std::strcmp(module, "HMI_PANNELLARGE") == 0);
    assert(aux && *aux == 0 && sig && name);
    if (tag == 25) {
        assert(fourth == reinterpret_cast<uintptr_t>(&alc_emit));
        assert(std::strcmp(name, "StartupInstrumentPannelLarge") == 0);
    } else {
        assert(tag == 56 && fourth == reinterpret_cast<uintptr_t>(module));
        assert(std::strcmp(name, "ExitInstrumentPannelLarge") == 0);
    }
}
void* alc_vtable[] = {reinterpret_cast<void*>(&alc_emit)};
void** alc_object = alc_vtable;
}
extern "C" void* I_GetAlcPtr() { return &alc_object; }
CGString::CGString(const unsigned short* text) {
    const char16_t* expected = u"InstrumentPannelLargePackage";
    for (; *expected; ++expected, ++text) assert(*expected == *text);
    assert(*text == 0);
}
CGString::~CGString() {}
namespace GNS_FRAME {
CGIntent::CGIntent() {}
CGIntent::~CGIntent() {}
void CGIntent::putStringExtra(const unsigned short* key, const char* value) {
    const std::u16string k(reinterpret_cast<const char16_t*>(key));
    if(extras%2==0){assert(k==u"fromPackage");assert(std::strcmp(value,"instrumentPannelLargePackage")==0);}
    else {assert(k==u"packageType");assert(std::strcmp(value,"com.autonavi.package.GIPLPacket")==0);}
    ++extras;
}
CGPackageMgr* CGFrameSimulation::getPackageMgr() { assert(this==expected_simulation);++manager_queries;return &package_mgr; }
bool CGPackageMgr::loadPackage(CGString&, CGIntent&, bool a, bool b) {
    assert(!a && !b); ++loads; return load_result;
}
bool CGPackageMgr::unloadPackage(CGString&, bool a) { assert(!a); ++unloads; return true; }
}
namespace hsl {
MultiScreenService* MultiScreenService::GetInstance() { return service_available?&service:nullptr; }
bool MultiScreenService::IsCreatedLargeScreen() { return created; }
void MultiScreenService::SetCreatedLargeScreen(bool value) { created = value; ++sets; }
}
int main() {
    assert(StartupInstrumentPannelLarge(&simulation) == 1);
    assert(created && sets == 1 && loads == 1 && extras == 2);
    assert(StartupInstrumentPannelLarge(&simulation) == 0);
    assert(sets == 1 && loads == 1 && extras == 2);
    assert(manager_queries==1);
    expected_simulation=&second_simulation;
    ExitInstrumentPannelLarge(&second_simulation);
    assert(manager_queries==2);
    assert(unloads == 1 && alc_calls == 3);
    expected_simulation=&simulation;created=false;load_result=false;
    assert(StartupInstrumentPannelLarge(&simulation)==1);
    assert(created&&sets==2&&loads==2&&extras==4&&manager_queries==3);
    service_available=false;created=false;
    assert(StartupInstrumentPannelLarge(&simulation)==1);
    assert(!created&&sets==2&&loads==3&&extras==6&&manager_queries==4);
    LogInit();
}
