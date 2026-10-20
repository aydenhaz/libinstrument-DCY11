#include <AbiTypes.h>
#pragma once
namespace bl {  }
namespace hsl {
class ArTrafficLightObserver; class ArNaviMapViewObserver; class ArStatusObserver;
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
    void SetTrafficLightListener(ArTrafficLightObserver*);
    void SetArMapViewObserver(ArNaviMapViewObserver*);
    void SetArStatusObserver(ArStatusObserver*);
};
class ArMapViewInstance {
public:
    static void DestroyArMapView(ArNaviMapView*);
};
}
