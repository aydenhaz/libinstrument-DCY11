#pragma once
#include <GFrame/CGFragment.h>
namespace GNS_FRAME {
class CGContext;
class CGIntent;
class CGFragment;
class CGPackage {
public:
    static CGPackage *getHostFragmentActivity();
    static CGContext *getContext();
    void closeAllFragments();
};
class CGExhibition;
class CGActivity {
public:
    void setFragment(int, CGFragment*, const CGIntent&);
};
}
