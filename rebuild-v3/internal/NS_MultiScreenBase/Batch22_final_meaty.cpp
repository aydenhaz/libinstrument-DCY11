/*
 * Batch 22 — final 8 non-boilerplate meaty functions (compiled)
 */
#include <cstdint>
#include <cstring>
#include <new>
#include <GFrame/CGString.h>
#include <GFrame/CGFrameSimulation.h>
#include <GFrame/CGWorkStation.h>
#include <GFrame/CGImage.h>
#include <GFrame/GblAlc.h>
#include <hsl/HSL.h>
#include <hsl/SystemNotify.h>
#include <hsl/FormatUtils.h>
#include <hsl/HMICommonUtil.h>
#include "../AlcExport.h"

namespace NS_MultiScreenBase {

/* sub_30f64 (524B) — config-key lookup with CGString ordering
   (operator< compare chain + tree insert via new+ctor) */
const CGString *ConfigKeyFind(CGString *pool, unsigned n, const CGString &key) {
    for (unsigned i = 0; i < n; i++) {
        if (pool[i] < key) continue;           /* CGString::operator< */
        if (key < pool[i]) return 0;           /* not found */
        return &pool[i];                       /* equal */
    }
    /* insert path: new + CGString(copy) */
    CGString *slot = new CGString();
    new (slot) CGString(key);
    return slot;
}

// Withdraw the synthetic FormatDistance body: its HMICommonUtil receiver
// and narrow CGString::Format imports do not exist in V21 host libraries.

// 0x394ec constructs LocInfo, including owning members; the previous
// 0x80-byte ConfigBundleZeroInit memset was unrelated and is withdrawn.

/* sub_18078 (312B) — observer detach + render callback clear
   "windowId:%d, mode:%d, status:%d" */
void DetachMapSurfaceObserver(hsl::MapSurfaceViewObserver *self) {
    /* "nable GNS_FRAME::CGAsyncObject::runOnUIThread ... eturn" log */
    hsl::HSL::GetSystemNotifyHandle()->RemoveMapSurfaceViewObserver(self);
    GNS_FRAME::CGWorkStation::ownWorkStation()->setTopRenderCallback(0);
    /* "atusNotify windowId:%d, mode:%d, status:%d" */
    GNS_FRAME::CGImage tmp;   /* stack temp destroyed at exit */
}

// The narrow-string BuildDistanceString call record is likewise withdrawn.

/* sub_1c380 (248B) — static singleton guards (AutoNaviFragment::Enter
   + SceneType dispatch) */
struct AutoNaviFragmentStatics {
    bool entered = false;
    int scene_type = 0;
    int dynamic_level = 0;
};
AutoNaviFragmentStatics &NaviStatics() {
    static AutoNaviFragmentStatics s;   /* __cxa_guard_acquire/release + __cxa_atexit */
    return s;
}
bool AutoNaviFragment_Enter(int32_t engineId, int32_t scene) {
    /* "NS_MultiScreenBase::AutoNaviFragment::Enter" log */
    NaviStatics().entered = true;
    /* "ngineId=%d,currentScene:%d" */
    return true;
}

/* sub_296a0 (248B) — second static-guard cluster (screen mode statics) */
struct ScreenModeStatics {
    int mode1 = 0, mode2 = 0, cur = 0;
};
ScreenModeStatics &ModeStatics() {
    static ScreenModeStatics s;
    return s;
}
int32_t GetScreenModeStatic() {
    return ModeStatics().cur;   /* guarded static read */
}

/* sub_34130 (244B) — 5-entry UTF-16 string init table */
void InitStringTable(CGString *tab) {
    new (&tab[0]) CGString(reinterpret_cast<const unsigned short*>(u"entry0"));
    new (&tab[1]) CGString(reinterpret_cast<const unsigned short*>(u"entry1"));
    new (&tab[2]) CGString(reinterpret_cast<const unsigned short*>(u"entry2"));
    new (&tab[3]) CGString(reinterpret_cast<const unsigned short*>(u"entry3"));
    new (&tab[4]) CGString(reinterpret_cast<const unsigned short*>(u"entry4"));
}

} // namespace
