# Map surface observer interface checkpoint

The original CoverUtil log signature at 0x4c9f1 is:

    virtual void NS_MultiScreenBase::CoverUtil::MapSurfaceViewStatusChange(int32_t, MAPSURFACE, void *, int32_t, int32_t, int32_t)

The following format at 0x4ca70 identifies the arguments as surface ID, event
type, handle, format, width and height. The main observer vtable's callback is
slot 0; complete/deleting destructor slots follow it. CoverUtil appends its
business-data callback after those inherited slots.

hsl/MapSurfaceViewObserver.h declares this interface with a forward enum,
without inventing enum values. The slot-0 dispatch test passes under host
ASan/UBSan, including virtual destruction. ARM compile checks observer size 4
and a three-slot derived vtable (0x14 bytes including ABI header).

Typed SystemNotify registration/removal declarations produce these exact
original host symbols in an ARM compile-only probe:

    _ZN3hsl12SystemNotify25AddMapSurfaceViewObserverEPNS_22MapSurfaceViewObserverE
    _ZN3hsl12SystemNotify28RemoveMapSurfaceViewObserverEPNS_22MapSurfaceViewObserverE

The standalone incomplete DetachMapSurfaceObserver helper now takes the actual
observer pointer type; this fixes its host removal symbol, not the helper's
remaining incomplete cleanup behavior.

The legacy AddMapSurfaceViewObserver(void*) declaration is still explicitly
retained as unresolved because incomplete Packet::onInitEnd passes Packet
where it should pass an observer subobject of the newly created Opening
Fragment. No reinterpret-cast adapter was added to disguise that ownership
error. CoverUtil's own observer interfaces are a separate part of Packet.

The second business-data observer callback is not yet declared. A similarly
named exported onBusinessDataObserver(int,const void*,void*) in libGbl belongs
to dice::IMapBusinessDataObserver, NOT bl::IBLMapBusinessDataObserver, as RTTI
confirms. Its signature must not be copied solely because names are similar.

## RTTI tool correction

A regression probe on libGbl revealed that some RTTI names/bases use defined
symbol R_ARM_ABS32 relocations, with zero raw words. The reader now resolves
S+A (and GLOB_DAT S), while retaining undefined imported references. It rejects
null names instead of interpreting the ELF header as a name. Original Packet
and CoverUtil output remains byte-for-byte unchanged. The defined-symbol
regression output is v21_verified/rtti_defined_symbols.json.

All ARM checks here are compile-only using arm-linux-musleabi; they do not
establish Android runtime compatibility or a working replacement library.
