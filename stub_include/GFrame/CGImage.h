#pragma once
namespace GPI { struct CGSize { int w,h; }; }
enum EGColorType_ { EGColorType_RGBA=0 };
using EGColorType = EGColorType_;
namespace GNS_FRAME { class CGImage { public: CGImage(); ~CGImage(); void create(const GPI::CGSize&, EGColorType, unsigned char*); }; }
