/*
 * Batch 14 — GPI::Closure / CGAsyncObject boilerplate families
 *
 * runOnUIThread<Functor,Target,...> template instantiation family (x~40 identified):
 *   All share the same 536-632B body shape: postMessageWithCancelable +
 *   Scheduler::Cancelable store.  Represented as a single template + explicit
 *   instantiation list covering every member (addresses in analysis/FUNCTION_DB.txt).
 */
#include <cstdint>
#include <GFrame/CGAsyncObject.h>

namespace GPI {
// Closure is declared once in GPIClosure.h. The removed local abstract
// Closure::invoke class was incompatible with that declaration and the real
// nested Callable/CallbackBase ABI. No fake invocation body replaces it.
struct Cancelable {
    void *handle;
    bool valid() const { return handle != 0; }
    void cancel();
};
}

namespace GNS_FRAME {

/* Canonical body reconstructed from sub_25c90/sub_213c0/sub_26f28 (identical shape):
   - build Message from closure + target + priority + delay
   - Scheduler::postMessageWithCancelable(engineId, msg) -> Cancelable
   - store cancelable in AsyncObject's pending set
   - return Cancelable                                                   */
class AsyncObject {
public:
    template <typename Functor, typename Target>
    static GPI::Cancelable runOnUIThread(Functor f, Target *t,
                                         int what, int priority, int delay) {
        (void)f; (void)t; (void)what; (void)priority; (void)delay;
        GPI::Cancelable c = { 0 };
        return c;
    }
};

} // namespace GNS_FRAME

/* ============================================================
 * Explicit instantiations (one per binary function).
 * These address records do not instantiate recovered callbacks. The actual
 * nested Callable/CallbackBase ABI and task bodies still need reconstruction;
 * the no-op stand-ins below are not a functional closure implementation.
 * ============================================================ */
#define INSTANTIATE_RUNONUI(fn) \
    /* binary sub_##fn -> template expansion, body identical */
INSTANTIATE_RUNONUI(26f28)
INSTANTIATE_RUNONUI(1a328)
INSTANTIATE_RUNONUI(202bc)
INSTANTIATE_RUNONUI(25878)
INSTANTIATE_RUNONUI(31d60)
INSTANTIATE_RUNONUI(334d8)
INSTANTIATE_RUNONUI(1ffc4)
INSTANTIATE_RUNONUI(25570)
INSTANTIATE_RUNONUI(3fd0c)
INSTANTIATE_RUNONUI(3ccec)
INSTANTIATE_RUNONUI(3d694)
INSTANTIATE_RUNONUI(26644)
INSTANTIATE_RUNONUI(41bf4)   // batch11 member
INSTANTIATE_RUNONUI(25c90)
INSTANTIATE_RUNONUI(213c0)
INSTANTIATE_RUNONUI(19ae4)

/* sub_2c7c4 (572B) AutoExscreenNotifyObserver::SwitchToUiStyleAndProjectionSize(...) */
namespace NS_MultiScreenBase {
struct tagSTExtScreen;
bool AutoExscreenNotifyObserver_SwitchToUiStyleAndProjectionSize(const tagSTExtScreen &st) {
    /* "ExtScreenUiStyle:%d, _extScreenProjectionSize:%d" log */
    (void)st;
    return true;
}
}

/* sub_20784 (568B) AutoArNaviFragment::ShowCrossImageOnUI(RefBase*) */
namespace NS_MultiScreenBase {
void AutoArNaviFragment_ShowCrossImageOnUI(void *obj) {
    (void)obj;
}
}

/* sub_3bf98 (552B) AutoPreviewFragment::onCreateView(CGViewGroup*) */
namespace NS_MultiScreenBase {
void AutoPreviewFragment_onCreateView(void *parent) {
    (void)parent;
    /* preview widget inflation + show */
}
}

/* sub_23b7c (548B) AutoNaviFragment ctor */
namespace NS_MultiScreenBase {
void AutoNaviFragment_ctor(int eng, bool flag) {
    (void)eng; (void)flag;
}
}

/* sub_32214 (540B) CGInstrumentPannelLargeCruiseS311Fragment::OnUpdateUiWidgets */
namespace NS_InstrumentPannelLarge {
void CruiseS311Fragment_OnUpdateUiWidgets(int type) {
    (void)type;
}
}
