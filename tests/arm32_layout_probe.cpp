// Compile-only layout check. Android runtime compatibility requires NDK checks.
#include "NS_InstrumentPannelLarge/CGIPLPacket.h"
#include "abi/VoidCallable.h"
#include <hsl/UITaskThreadProxy.h>
#if !defined(__arm__) || defined(__aarch64__)
#error This probe must be compiled for ARM32
#endif
static_assert(sizeof(void*)==4,"ARM32 pointer");
static_assert(sizeof(NS_InstrumentPannelLarge::CGIPLPacket)==0x16c,"Packet extent");
static_assert(NS_InstrumentPannelLarge::CGIPLPacket::taskProxyOffset()==0x168,"proxy field");
static_assert(sizeof(hmi::UITaskThreadProxy)==0xf0,"proxy extent including vptr");
static_assert(sizeof(IPL_ABI::VoidCallable)==20,"callback extent");
static_assert(sizeof(IPL_ABI::MemberInvoker<IPL_ABI::MemberCallableSizeProbe>)==24,"method extent");
