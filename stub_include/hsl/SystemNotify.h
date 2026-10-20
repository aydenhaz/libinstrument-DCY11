#pragma once
namespace hsl {
class SystemNotify {
public:
    static SystemNotify *GetSystemNotifyHandle();
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
