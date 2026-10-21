#pragma once
namespace NS_MultiScreenBase {
class CoverUtil;
// Stage adapter for a genuinely constructed V21 ARM32 Opening Fragment.
// Never pass the empty legacy AutoOpeningFragment declaration to this function.
// Does not construct a Fragment, enable a factory, or own the CoverUtil pointer.
void bindOpeningCover(void* originalFragment,CoverUtil* cover);
}
