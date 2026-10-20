#pragma once
namespace GNS_FRAME {
class CGContext;
class CGIntent;
class CGFragment;
class CGPackage {
public:
    static CGPackage *getHostFragmentActivity();
    static CGContext *getContext();
    static void closeAllFragments();
};
class CGFragment {
public:
    static CGFragment *tryStartExistFragment(const CGIntent&, int, CGContext*);
    void setContext(CGContext*);
    void setType(void*);
    void startNewFragment(void*, int, const CGIntent&, int);
};
class CGExhibition;
class CGActivity {
public:
    void setFragment(int, CGFragment*, CGIntent&);
};
}
