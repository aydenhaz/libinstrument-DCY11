#include "HSL.h"
#pragma once
namespace hsl {
class SystemNotify {
public:
    static SystemNotify *GetSystemNotifyHandle() { return HSL::GetSystemNotifyHandle(); }
    void AddInstrumentPannelObserver(void*);
    void RemoveInstrumentPannelObserver(void*);
    void AddMapSurfaceViewObserver(void*);
    void AddMapSurfaceObserver(void*);
    void AddLanguageSwitchObserver(void*, bool);
    void AddNetObserver(void*);
    void AddTestNumObserver(void*);
    void RemoveTestNumObserver(void*);
};
}
