#pragma once
#include <GFrame/CGFragment.h>
namespace GNS_FRAME {
class CGContext;
class CGFragmentActivity;
class CGIntent;
class CGFragment;
class CGPackage {
public:
    CGFragmentActivity *getHostFragmentActivity();
    static CGPackage *getHostFragmentActivityUnrecovered();
    CGContext *getContext();
    // Legacy paths do not yet provide the constructed package receiver.
    static CGContext *getContextUnrecovered();
    void closeAllFragments();
};
class CGExhibition;
class CGActivity {
public:
    void setFragment(int, CGFragment*, const CGIntent&);
};
}
