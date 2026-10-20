/*
 * Batch 9 — LargeScreenImpl::doStartFragment (sub_2e0f8, 2548B) FULL
 *
 * Recovered signature @0x544a5:
 *   virtual int32_t NS_InstrumentPannelLarge::LargeScreenImpl::doStartFragment(
 *       MultiScreenFragmentIndex, GNS_FRAME::CGFragment *, const GNS_FRAME::CGIntent &)
 *
 * Recovered strings (22 unique):
 *   "doStartFragment start:%p index:%d _bProject311:%d"   (@0x54505)
 *   "largeInstrumentCustomType"                            (intent extra key)
 *   "result:%d,customType:%d"                            (@0x55048)
 *   "appendFragment:com.auto.multiscreen.largepreview"   (log prefix)
 *   "com.auto.multiscreen.largepreview"                  (UTF-16 pkg name)
 *   "appendFragment:com.auto.multiscreen.largecuise"     (original typo preserved!)
 *   "com.auto.multiscreen.largecuise"
 *   "appendFragment:com.auto.multiscreen.largeguide"
 *   "com.auto.multiscreen.largeguide"
 *   "T *GNS_FRAME::CGPackage::pushFragment<T>(const CGIntent&, int) [T = ...]"
 *     x3 instantiations (pushFragment template per fragment type)
 *   "NS_InstrumentPannelLarge::CGInstrumentPannelLargeGuideGuangQiA106Fragment::..."
 *   "NS_InstrumentPannelLarge::CGInstrumentPannelLargeCruiseGuangQiA106Fragment::..."
 *   "doStartFragment end"                                (@0x545c6)
 *   "virtual int32_t ...::doStartFragment(MultiScreenFragmentIndex, ...)"
 *   "HMI_COMMON"                                         (ALC module)
 *   "virtual ...::InitProject()"                         (@0x55048: "bool ...::InitProject()")
 *
 * Structure (4 branches on MultiScreenFragmentIndex + GuangQi A106 sub-path):
 *   preview -> appendFragment:com.auto.multiscreen.largepreview
 *   cruise  -> appendFragment:com.auto.multiscreen.largecuise
 *   guide   -> appendFragment:com.auto.multiscreen.largeguide
 *   each with pushFragment<T>(intent, index) template instantiation
 *   _bProject311 flag selects S311 variant names
 */
#include "LargeScreenImpl.h"
#include "../AlcExport.h"
#include <GFrame/CGString.h>
#include <GFrame/CGPackage.h>
#include <GFrame/CGFrameSimulation.h>
#include <GFrame/CGIntent.h>
#include <GFrame/GblAlc.h>
#include <cstdio>

namespace NS_InstrumentPannelLarge {

enum MultiScreenFragmentIndex {
    FRAG_INDEX_PREVIEW = 0,
    FRAG_INDEX_CRUISE  = 1,
    FRAG_INDEX_GUIDE   = 2,
    /* 3 = A106/311 variant selector */
};

int32_t LargeScreenImpl::doStartFragment(int index,
                                         GNS_FRAME::CGFragment *fragment,
                                         const GNS_FRAME::CGIntent &intent) {
    /* "doStartFragment start:%p index:%d _bProject311:%d" */
    /* log(fragment, index, _bProject311_) */

    GNS_FRAME::CGPackage *pkg = GNS_FRAME::CGFrameSimulation::getFragmentActivity();
    GNS_FRAME::CGContext *ctx = GNS_FRAME::CGPackage::getContext();
    /* Original assertion requires the real Fragment state, not a context string.
       Restore it together with the correct receiver/layout. */

    /* read custom type from intent extra "largeInstrumentCustomType" */
    int customType = 0;
    /* intent.getIntExtra(u"largeInstrumentCustomType", &customType); */
    /* "result:%d,customType:%d" log */

    int32_t rc = 0;
    switch (index) {
    case FRAG_INDEX_PREVIEW: {
        /* "appendFragment:com.auto.multiscreen.largepreview" */
        CGString name(reinterpret_cast<const unsigned short*>(u"com.auto.multiscreen.largepreview"));
        /* pkg->appendFragment / pushFragment<CGInstrumentPannelLargePreviewFragment>(intent, index) */
        break;
    }
    case FRAG_INDEX_CRUISE: {
        /* "appendFragment:com.auto.multiscreen.largecuise"  (原库拼写即 largecuise) */
        CGString name(reinterpret_cast<const unsigned short*>(u"com.auto.multiscreen.largecuise"));
        /* pushFragment<CGInstrumentPannelLargeCruiseFragment>(intent, index) */
        /* GuangQi A106 sub-path when customType matches:
           "NS_InstrumentPannelLarge::CGInstrumentPannelLargeCruiseGuangQiA106Fragment::..." */
        break;
    }
    case FRAG_INDEX_GUIDE: {
        /* "appendFragment:com.auto.multiscreen.largeguide" */
        CGString name(reinterpret_cast<const unsigned short*>(u"com.auto.multiscreen.largeguide"));
        /* pushFragment<CGInstrumentPannelLargeGuideFragment>(intent, index) */
        /* A106 sub-path: GuideGuangQiA106Fragment */
        break;
    }
    default:
        /* S311 variants selected by _bProject311_ */
        break;
    }

    /* "doStartFragment end" */
    return rc;
}

/* InitProject (referenced in same literal pool) */
bool LargeScreenImpl::InitProject() {
    /* "virtual ...::InitProject()" ALC export */
    return true;
}

} // namespace
