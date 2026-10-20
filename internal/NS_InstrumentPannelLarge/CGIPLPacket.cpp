/* INCOMPLETE Packet construction. Original factory allocates 0x16c,
 * constructs CGFragmentPackage, then the 0xa8-byte script subobject at +0xc0.
 * Original destructor destroys script state first, then the host package base.
 * These empty C++ constructors do NOT implement that chain.
 * 0x30dc8 is member-method equality, not ScriptThis/dynamic script resolution.
 */
#include "CGIPLPacket.h"

namespace NS_InstrumentPannelLarge {

CGIPLPacket::CGIPLPacket() {}
CGIPLPacket::~CGIPLPacket() {}

} // namespace
