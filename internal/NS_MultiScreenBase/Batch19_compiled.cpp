/*
 * Batch 19 — behavior completion: ExscreenNotify ctor + S311 OnUpdateUiWidgets
 *            + ArNaviMapView init + ARGuide callback (compiled)
 */
#include <cstdint>
#include <GFrame/CGString.h>
#include <GFrame/CGUITimerInstance.h>
#include <GFrame/GblAlc.h>
#include <GFrame/CGStringUtils.h>
#include <asl/String8Utils.h>
#include <hsl/ObserverBase.h>
#include <hsl/AsyncObject.h>
#include <hsl/InstrumentManager.h>
#include <hsl/ArNaviMapView.h>
#include <hsl/ArNaviMapView.h>
#include <NS_MultiScreenBase/AutoExscreenNotifyObserver.h>
#include <NS_MultiScreenBase/AutoConfigScreenMode.h>
#include <NS_MultiScreenBase/AutoConfigJsonData.h>
#include <GAI2.h>
#include "../AlcExport.h"

namespace NS_MultiScreenBase {
class AutoExscreenNotifyObserver {
public:
    AutoExscreenNotifyObserver();
};
class CruiseS311Fragment {
public:
    void OnUpdateUiWidgets(int32_t);
};
void InitArNaviMapView(void *ar, int eng);
void ARGuideFragment_ArStatusDispatch(int32_t status);

/* sub_29798 (804B) AutoExscreenNotifyObserver ctor */
void AutoExscreenNotifyObserver_ctor_impl() {
    char nameBuf_[64]; int style_ = 0; unsigned w_ = 0, h_ = 0; void *timer_ = 0;
    /* base ctors */
    /* hsl::ObserverBase base ctor: name="AutoExscreenNotifyObserver" */
    /* hsl::AsyncObject base ctor */
    /* snprintf into embedded name buffer */
    asl::String8Utils::snprintf(nameBuf_, sizeof(nameBuf_), "AutoExscreenNotifyObserver");
    /* GAI queries for ext screen style/size */
    style_ = GAI_GetExtScreenUIStyle();
    GAI_GetExtScreenUISize(&w_, &h_);
    /* ALC export pair (8-word block, tag from r12) */
    AlcExport("AutoExscreenNotifyObserver", "AutoExscreenNotifyObserver::AutoExscreenNotifyObserver()", 0, (void*)0);
    /* CGString label("ExscreenNotify"); */
    /* CGUITimerInstance(ownWorkStation(), interval) */
    /* CGString label("ExscreenNotify"); */
    /* name store */
}

/* sub_32214 (540B) CGInstrumentPannelLargeCruiseS311Fragment::OnUpdateUiWidgets */
void CruiseS311Fragment_OnUpdateUiWidgets_impl(int32_t type) {
    int mode1_ = 0, mode2_ = 0, mode3_ = 0, modeFinal_ = 0;
    (void)type;
    AlcExport("OnUpdateUiWidgets", "virtual void ...CruiseS311Fragment::OnUpdateUiWidgets(int32_t)", 0, (void*)0);
    AutoConfigScreenMode *sm = AutoConfigScreenMode::GetInstance();
    int cur = sm->GetCurrentScreenMode(0);
    if (cur != mode1_) sm->SetCurrentScreenMode(0, mode1_);       /* goto 0x64580 */
    else {
        cur = sm->GetCurrentScreenMode(0);
        if (cur != mode2_) sm->SetCurrentScreenMode(0, mode2_);   /* goto 0x645bc */
        else {
            sm->GetInstance();  /* refresh */
            sm->SetCurrentScreenMode(0, mode3_);                  /* goto 0x645f4 */
        }
    }
    AlcExport(0, 0, 0, (void*)0);
    sm->GetCurrentScreenMode(0);
    AlcExport(0, 0, 0, (void*)0);
    sm->SetCurrentScreenMode(0, modeFinal_);
    AlcExport(0, 0, 0, (void*)0);
    GAI_ExtScreenChangeScreenModeNotify(modeFinal_);
    SurfaceConfig sc;
    AutoConfigJsonData::GetInstance()->GetSurfaceConfig(0, sc);
}

/* sub_1efbc (244B) ArNaviMapView init sequence ("status]" log tail) */
void InitArNaviMapView(void *arp, int eng) {
    hsl::ArNaviMapView *ar = static_cast<hsl::ArNaviMapView*>(arp);
    unsigned w = 0, h = 0;
    hsl::InstrumentManager::GetInstrumentScreenSize(w, h);
    ar->SetScreenSize(w, h);
    unsigned mw = 0, mh = 0, mx = 0, my = 0;
    hsl::InstrumentManager::GetInstrumentMapSize(mw, mh, mx, my);
    ar->SetMapSize(mw, mh);
    ar->SetMapStartPos(mx, my);
    ar->ResetMapView(static_cast<bl::MapEngineID>(eng));
    ar->ResetArMapView();
    if (ar->IsArReplaying()) ar->StopArReplaying();
    ar->SetArScreenViewType(static_cast<bl::MapEngineID>(eng));
    ar->SetTrafficLightListener(0);
    ar->SetArMapViewObserver(0);
    ar->SetArStatusObserver(0);
    /* teardown or reuse: DestroyArMapView(ar) on re-init path */
    GAI_NaviInfoBroadcasting();
}

/* sub_195bc (212B) CGInstrumentPannelLargeARGuideFragment — AR status dispatch */
void ARGuideFragment_ArStatusDispatch(int32_t status) {
    AlcExport("ArStatus", "...ARGuideFragment, Functor ...", 0, /*self*/ 0);
    GAI_DispatchArStatusNotify(status);
}

} // namespace
