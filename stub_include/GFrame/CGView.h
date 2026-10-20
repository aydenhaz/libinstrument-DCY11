#pragma once
namespace GNS_FRAME {
class CGView {
public:
    void show();
    void hide(bool);
    void setBackGround(int);
    void setBackGround(const class CGDrawablePtr&);
    void setClickable(bool);
};
class CGViewGroup {};
class CGTextView : public CGView { public: void setEllipsizeType(int); };
}
