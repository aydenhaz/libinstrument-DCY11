/*
 * Batch 3 — CGIPLPacket (main module class) + GPI binding glue
 *
 * CGIPLPacket is a NON-virtual class (no own vtable). It is exposed to the
 * script/engine layer through two binding artifacts:
 *
 * 1) GPI::Closure::bind<CGIPLPacket> CallableWrapper, vt@0x6bc94:
 *      [0] sub_1ef7c  (28B) — BX lr, empty/null slot
 *      [1] sub_30ebc         — (wrapper body)
 *      [2] sub_30ec0
 *      [3] sub_30ed4
 * 2) GPI::internal::GClassMethod<CGIPLPacket,...> vt@0x6bc5c (6 slots):
 *      [0] sub_30dc8  (100B) — __dynamic_cast(this, CGIPLPacket) wrapper
 *                             (class-func invoker: resolves script "this")
 *      [1] sub_30e2c  (32B)  — accessor (method-dispatch entry)
 *      [2..5] sub_30e4c/30e5c/30e60/30e64 — empty slots
 *
 * Conclusion: CGIPLPacket's interesting logic lives in the fragment
 * implementations (batch 2) and LargeScreenImpl (batch 1); the packet class
 * itself is a thin registration bundle.
 */
#include "CGIPLPacket.h"

namespace NS_InstrumentPannelLarge {

CGIPLPacket::CGIPLPacket() {}
CGIPLPacket::~CGIPLPacket() {}

/* Script-binding entry: dynamic_cast resolution of the script "this" pointer
   to CGIPLPacket* (sub_30dc8). */
CGIPLPacket *CGIPLPacket::ScriptThis(void *obj) {
    return dynamic_cast<CGIPLPacket*>(static_cast<CGIPLPacket*>(obj));  // 100B pattern @0x30dc8
}

} // namespace
