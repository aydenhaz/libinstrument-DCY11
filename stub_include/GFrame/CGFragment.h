#pragma once
namespace GNS_FRAME {
class CGViewGroup; class CGView; class CGIntent; class CGContext;
class CGFragment {
public:
    static CGFragment *tryStartExistFragment(const CGIntent&, int, void*);
    static CGViewGroup *inflater(int, CGViewGroup*);
    static CGView *findViewById(int);
    void onDestroyView();
    CGViewGroup* onCreateView(CGViewGroup*);
    CGContext* getContext();
    void inflater(CGView*, CGViewGroup*);
    static bool isTopFragment();
    void setContext(CGContext*);
    void setType(void*);
    void startNewFragment(void*, int, const CGIntent&, int);
};
}
