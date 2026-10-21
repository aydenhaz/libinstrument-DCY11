#pragma once
namespace GNS_FRAME {
class CGViewGroup; class CGView; class CGIntent; class CGContext;
class CGFragment {
public:
    // Interface projection only: original constructor takes ELaunchMode and
    // initializes multiple bases. Empty host objects must never be allocated.
    CGFragment() = delete;
    static CGFragment* createFragmentUnrecovered();
    CGFragment *tryStartExistFragment(const CGIntent&, int, void*);
    // Intentionally undefined: legacy callers lack a valid parent/type identity.
    // Never alias these to real host methods just to make the linker pass.
    static CGFragment *tryStartExistFragmentUnrecovered(const CGIntent&, int, void*);
    // Instance methods: r0 must be a constructed CGFragment.
    // Unrecovered declarations intentionally block receiver-less legacy code.
    CGViewGroup *inflater(int, CGViewGroup*);
    static CGViewGroup *inflaterUnrecovered(int, CGViewGroup*);
    CGView *findViewById(int);
    static CGView *findViewByIdUnrecovered(int);
    void onDestroyView();
    CGViewGroup* onCreateView(CGViewGroup*);
    CGContext* getContext();
    void inflater(CGView*, CGViewGroup*);
    bool isTopFragment();
    static bool isTopFragmentUnrecovered();
    void setContext(CGContext*);
    void setType(void*);
    void startNewFragment(CGFragment* child, void* typeIdentity, int mode, const CGIntent&, int requestCode);
    void startNewFragmentUnrecovered(void*, int, const CGIntent&, int);
};
}
