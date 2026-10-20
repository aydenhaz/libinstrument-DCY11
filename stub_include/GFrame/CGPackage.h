#pragma once
#include <GFrame/CGFragment.h>
namespace GNS_FRAME {
class CGContext;
class CGIntent;
class CGFragment;
class CGPackage {
public:
    CGPackage *getHostFragmentActivity(); // pointed-to activity type still unverified
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
