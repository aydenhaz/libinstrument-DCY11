#pragma once
namespace GPI { struct CGSize { int w,h; }; }
enum EGColorType { EGColorType_RGBA=0 };
namespace GNS_FRAME { class CGImage { public: CGImage(); ~CGImage(); void create(const GPI::CGSize&, EGColorType, unsigned char*); }; }
