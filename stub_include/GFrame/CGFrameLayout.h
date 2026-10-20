#pragma once
namespace GNS_FRAME {
class CGContext;
class CGDrawablePtr_;
class CGView;
class CGFrameLayout : public CGView {
public:
    explicit CGFrameLayout(CGContext*);
    void setClickable(bool);
};
}
