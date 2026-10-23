/* INCOMPLETE Packet construction. Original factory allocates 0x16c,
 * constructs CGFragmentPackage, then the 0xa8-byte CoverUtil subobject at +0xc0.
 * Original destructor destroys CoverUtil state first, then the host package base.
 * The constructor does not publish the singleton; the original factory stores
 * it only after host/CoverUtil construction and four derived vptr stores.
 * Host construction and destruction are NOT implemented.
 * 0x30dc8 is member-method equality, not ScriptThis/dynamic script resolution.
 */
#include "CGIPLPacket.h"

namespace NS_InstrumentPannelLarge {

CGIPLPacket* CGIPLPacket::active_ = nullptr;
CGIPLPacket::CGIPLPacket() {}
CGIPLPacket::~CGIPLPacket() {}
void CGIPLPacket::publishConstructed(CGIPLPacket* packet) {active_=packet;}

} // namespace
