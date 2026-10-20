#pragma once
namespace bl { typedef int MapEngineID; }
namespace hsl {
class ArNaviMapView {
public:
    void SetScreenSize(unsigned, unsigned);
    void SetMapSize(unsigned, unsigned);
    void SetMapStartPos(unsigned, unsigned);
    void ResetMapView(bl::MapEngineID);
    void ResetArMapView();
    bool IsArReplaying();
    void StopArReplaying();
    void SetArScreenViewType(bl::MapEngineID);
    void SetTrafficLightListener(void*);
    void SetArMapViewObserver(void*);
    void SetArStatusObserver(void*);
};
class ArMapViewInstance {
public:
    static void DestroyArMapView(ArNaviMapView*);
};
}
