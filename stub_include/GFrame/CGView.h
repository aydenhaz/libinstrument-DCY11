#pragma once
namespace GNS_FRAME {
class CGView { public: void show(); void setBackGround(int); };
class CGViewGroup {};
class CGTextView : public CGView { public: void setEllipsizeType(int); };
}
