#pragma once
class CGColor;
namespace GNS_FRAME {
class CGColorDrawable {
public:
    static CGColorDrawable *createColorDrawable(const CGColor&);
};
}
