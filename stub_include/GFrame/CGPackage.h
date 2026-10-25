#pragma once
#include <GFrame/CGFragment.h>
class CGString;
namespace GNS_FRAME {
class CGContext;
class CGFragmentActivity;
class CGIntent;
class CGFragmentBuilder;
class CGFragment;
class CGPackage {
public:
    CGFragmentActivity *getHostFragmentActivity();
    static CGPackage *getHostFragmentActivityUnrecovered();
    CGContext *getContext();
    // Legacy paths do not yet provide the constructed package receiver.
    static CGContext *getContextUnrecovered();
    void closeAllFragments();
    void _registerFragment(const CGString&, CGFragmentBuilder*);
};
class CGExhibition;
class CGActivity {
public:
    void setFragment(int, CGFragment*, const CGIntent&);
};
}
