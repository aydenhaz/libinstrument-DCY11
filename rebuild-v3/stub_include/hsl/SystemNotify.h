#include "HSL.h"
#include "MapSurfaceViewObserver.h"
#pragma once
namespace hsl {
class TestNumObserver;
class SystemNotify {
public:
    static SystemNotify *GetSystemNotifyHandle() { return HSL::GetSystemNotifyHandle(); }
    void AddInstrumentPannelObserver(void*);
    void RemoveInstrumentPannelObserver(void*);
    void AddMapSurfaceViewObserver(MapSurfaceViewObserver*);
    // Legacy unresolved overload: incomplete Opening Fragment setup still uses
    // this wrong declaration. It must not silently cast a Packet to observer.
    void AddMapSurfaceViewObserver(void*);
    void RemoveMapSurfaceViewObserver(MapSurfaceViewObserver*);
    void AddMapSurfaceObserver(void*);
    void AddLanguageSwitchObserver(void*, bool);
    void AddNetObserver(void*);
    void AddTestNumObserver(TestNumObserver*);
    void RemoveTestNumObserver(TestNumObserver*);
};
}
