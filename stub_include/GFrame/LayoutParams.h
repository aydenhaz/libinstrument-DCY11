#pragma once
namespace GNS_FRAME {
class CGView;
class LayoutParams {
public:
    static LayoutParams getLayoutParams(CGView*);
    static void setLayoutParams(CGView*, const LayoutParams&);
};
}
