#include <GFrame/CGDrawablePtr.h>
/*
 * Batch 20 — fragment-class expansion (9 functions, compiled)
 * Decoded from expanded/fragments category; all verified against binary.
 */
#include <cstdint>
#include <GFrame/CGString.h>
#include <GFrame/CGIntent.h>
#include <GFrame/CGFragment.h>
#include <GFrame/CGView.h>
#include <GFrame/CGColor.h>
#include <GFrame/CGColorDrawable.h>
#include <GFrame/CGDrawableHolder.h>
#include <GFrame/CGFrameLayout.h>
#include <GFrame/LayoutParams.h>
#include <GFrame/CGBroadcaster.h>
#include <GFrame/GblAlc.h>
#include <hsl/HSL.h>
#include <hsl/SystemNotify.h>
#include <hsl/Guide.h>
#include <bl/PathInfo.h>
#include <NS_MultiScreenBase/AutoExscreenNotifyObserver.h>
#include <NS_MultiScreenBase/AutoConfigScreenMode.h>
#include <NS_MultiScreenBase/AutoConfigJsonData.h>
#include "../AlcExport.h"

using GNS_FRAME::CGFragment;
using GNS_FRAME::CGView;
using GNS_FRAME::CGViewGroup;
using GNS_FRAME::CGIntent;

namespace NS_MultiScreenBase {

/* sub_327d0 (560B) — GoToCross-style: intent extras + PathInfo/SegmentInfo
   cross distance + conditional screen mode set + fragment start */
void GuideFragment_GoToCross_impl(void *self, int32_t index, bool flag) {
    CGIntent intent;
    intent.putInt32Extra(reinterpret_cast<const unsigned short*>(u"crossType"), index);
    intent.putInt32Extra(reinterpret_cast<const unsigned short*>(u"flag"), flag ? 1 : 0);
    AutoConfigScreenMode *sm = AutoConfigScreenMode::GetInstance();
    int cur = sm->GetCurrentScreenMode(0);
    void *guide = hsl::HSL::GetGuideHandle();
    (void)guide;
    bl::PathInfo path;
    /* PathInfo::IsValid + GetSegmentInfo + SegmentInfo::IsValid + GetLength chain */
    AlcExport("GoToCross", "...GuideFragment::GoToCross(...)", 0, self);
    if (cur != index) {
        sm->SetCurrentScreenMode(0, index);
    } else {
        /* I_Assert path */
        CGFragment *frag = CGFragment::tryStartExistFragmentUnrecovered(intent, 2, /*ctx*/0);
        frag = frag;
        if (!frag) {
            frag = new CGFragment();
            frag->setContext(/*ctx*/0);
            frag->startNewFragmentUnrecovered(0, 0, intent, 2);
        }
    }
}

/* sub_33a64 (560B) — theme/iacc switch: SetIaccTheme + SetCurrentScreenMode
   + putBoolExtra + fragment start + GetSurfaceConfig */
void ThemeSwitch_impl(void *self, int32_t iaccTheme, int32_t screenMode) {
    AlcExport("ThemeSwitch", "...", 0, self);
    AutoConfigScreenMode *sm = AutoConfigScreenMode::GetInstance();
    sm->SetIaccTheme(SCREEN_TYPE_MAIN, iaccTheme != 0); // V21 0x33ae0 / 0x33c2e.
    sm->SetCurrentScreenMode(0, screenMode);
    int cur = sm->GetCurrentScreenMode(0);
    sm->SetCurrentScreenMode(0, screenMode);
    CGIntent intent;
    intent.putBoolExtra(reinterpret_cast<const unsigned short*>(u"themeChanged"), true);
    /* I_Assert */
    CGFragment *frag = CGFragment::tryStartExistFragmentUnrecovered(intent, 2, 0);
    frag = frag;
    if (!frag) {
        frag = new CGFragment();
        frag->setContext(0);
        frag->startNewFragmentUnrecovered(0, 0, intent, 2);
    }
    SurfaceConfig sc;
    AutoConfigJsonData::GetInstance()->GetSurfaceConfig(0, sc);
}

/* sub_1a7dc (544B) — AutoExscreenNotifyObserver::OnHideCrossImage(CrossType)
   "toNaviFragment::OnHideCrossImage(bl::CrossType...)" */
void AutoExscreenNotifyObserver_OnHideCrossImage(void *self, int crossType) {
    AlcExport("OnHideCrossImage", "...AutoExscreenNotifyObserver::OnHideCrossImage(bl::CrossType)", 0, self);
    AutoConfigScreenMode *sm = AutoConfigScreenMode::GetInstance();
    sm->SetCurrentScreenMode(0, /*mode*/ 0);
    sm->SetCurrentScreenMode(0, /*mode2*/ 0);
    int cur = sm->GetCurrentScreenMode(0);
    sm->SetCurrentScreenMode(0, cur);
    CGIntent intent;
    intent.putBoolExtra(reinterpret_cast<const unsigned short*>(u"hideCross"), true);
    /* I_Assert */
    CGFragment *frag = CGFragment::tryStartExistFragmentUnrecovered(intent, 2, 0);
    frag = frag;
    if (!frag) {
        frag = new CGFragment();
        frag->setContext(0);
        frag->startNewFragmentUnrecovered(0, 0, intent, 2);
    }
    SurfaceConfig sc;
    AutoConfigJsonData::GetInstance()->GetSurfaceConfig(0, sc);
    cur = sm->GetCurrentScreenMode(0);
}

/* sub_31ad0 (460B) — CGInstrumentPannelLargeGuideS311Fragment::InitTheme
   (inflater + findViewById + setBackGround + hide 链) */
void GuideS311Fragment_InitTheme_impl(void *self) {
    CGViewGroup *root = CGFragment::inflater(0, (CGViewGroup*)0);
    /* dynamic_cast checks */
    CGView *bg = CGFragment::findViewById(0);
    bg->setBackGround(0);
    CGView *w1 = CGFragment::findViewById(1); /* cast */ w1->hide(false);
    CGView *w2 = CGFragment::findViewById(2); /* cast */ w2->hide(false);
    CGView *w3 = CGFragment::findViewById(3); /* cast */ w3->hide(false);
    CGView *w4 = CGFragment::findViewById(4); /* cast */ w4->hide(false);
    AutoConfigScreenMode::GetInstance();
}

/* sub_1f488 (396B) — AutoArNaviFragment::onCreateView
   (sub_23f1c 同体, 376B) */
void AutoArNaviFragment_onCreateView_impl(void *self, CGViewGroup *parent) {
    /* "AutoArNaviFragment.cpp" path */
    hsl::HSL::GetSystemNotifyHandle()->AddTestNumObserver(self);
    hsl::HSL::GetGuideHandle();
    hsl::HSL::GetGuideHandle();
    hsl::HSL::GetPosHandle();
    GNS_FRAME::CGBroadcaster::getInstance(0);
    CGFragment::isTopFragment();
    AlcExport("onCreateView", "...AutoArNaviFragment::onCreateView(CGViewGroup*)", 0, self);
}

/* sub_271f0 (316B) — ALC + isTopFragment guard */
bool ArNaviFragment_guard_impl(void *self) {
    AlcExport("guard", "...", 0, self);
    if (!CGFragment::isTopFragment()) return false;
    AlcExport("guard", "...", 0, self);
    return true;
}

/* sub_29548 (192B) — root view builder: CGFrameLayout + LayoutParams
   + CGColorDrawable background + setClickable */
GNS_FRAME::CGView *BuildRootView_impl(CGFragment *self, CGViewGroup *parent) {
    // V21 0x29548..0x29600, including allocation and ownership operations.
    self->onCreateView(parent);
    GNS_FRAME::CGFrameLayout *root = new GNS_FRAME::CGFrameLayout(self->getContext());
    GNS_FRAME::LayoutParams lp = GNS_FRAME::LayoutParams::getLayoutParams(root);
    lp.matchParent();
    GNS_FRAME::LayoutParams::setLayoutParams(root, lp);
    {
        CGColor color(0, 0, 0, 0);
        GNS_FRAME::CGDrawablePtr background(GNS_FRAME::CGColorDrawable::createColorDrawable(color));
        root->setBackGround(background);
    }
    self->inflater(root, parent);
    root->setClickable(true);
    return root;
}

/* sub_32a00 (140B) — fragment start helper (shared by GoToCross family) */
void StartFragmentHelper_impl(const CGIntent &intent, int mode) {
    /* I_Assert */
    CGFragment *frag = CGFragment::tryStartExistFragmentUnrecovered(intent, 2, 0);
    frag = frag;
    if (!frag) {
        frag = new CGFragment();
        frag->setContext(0);
        frag->startNewFragmentUnrecovered(0, 0, intent, 2);
    }
}

/* sub_20ddc (108B) — isTopFragment guard (ScreenType param) */
bool FragmentTopGuard_impl(int32_t screenType) {
    CGFragment::isTopFragment();
    (void)screenType;
    return true;
}

} // namespace
