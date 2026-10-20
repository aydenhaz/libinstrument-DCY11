#pragma once
namespace GNS_FRAME {
class CGView {
public:
    void show();
    void hide(bool);
    void setBackGround(int);
    void setBackGroundDrawable(class CGDrawableHolder*);
};
class CGViewGroup {};
class CGTextView : public CGView { public: void setEllipsizeType(int); };
}
