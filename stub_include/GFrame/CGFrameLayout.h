#include "CGView.h"
#pragma once
namespace GNS_FRAME {
class CGContext;
class CGDrawablePtr_;
class CGView;
class CGFrameLayout : public CGView {
    alignas(4) unsigned char abi_storage_[0x464]; // original allocation 0x29572.
public:
    explicit CGFrameLayout(CGContext*);
};
}

static_assert(sizeof(GNS_FRAME::CGFrameLayout) == 0x464, "V21 root allocation size");
