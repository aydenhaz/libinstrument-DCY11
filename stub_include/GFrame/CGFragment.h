#pragma once
namespace GNS_FRAME {
class CGViewGroup; class CGView; class CGIntent; class CGContext;
class CGFragment {
public:
    CGFragment *tryStartExistFragment(const CGIntent&, int, void*);
    // Intentionally undefined: legacy callers lack a valid parent/type identity.
    // Never alias these to real host methods just to make the linker pass.
    static CGFragment *tryStartExistFragmentUnrecovered(const CGIntent&, int, void*);
    static CGViewGroup *inflater(int, CGViewGroup*);
    static CGView *findViewById(int);
    void onDestroyView();
    CGViewGroup* onCreateView(CGViewGroup*);
    CGContext* getContext();
    void inflater(CGView*, CGViewGroup*);
    static bool isTopFragment();
    void setContext(CGContext*);
    void setType(void*);
    void startNewFragment(CGFragment* child, void* typeIdentity, int mode, const CGIntent&, int requestCode);
    void startNewFragmentUnrecovered(void*, int, const CGIntent&, int);
};
}
