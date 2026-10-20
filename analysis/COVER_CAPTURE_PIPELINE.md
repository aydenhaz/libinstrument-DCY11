# Cover capture result pipeline and secondary receiver

applyCoverCaptureResult now composes the verified operations: ignore non-0x17 events; clear requested/set state; calculate RGB averages for supported well-formed buffers; format and write widgetCoverR/G/B. Its internal enum reports IgnoredEvent/Applied/InvalidImage, and is explicitly not the host observer return type. Invalid-buffer rejection is a helper boundary; original callback does not implement these checks and always returns 1 in the observed code.

Native sanitizer tests exercise valid input through configuration calls, ignored events preserving state, and invalid input performing no config writes. ARM Thumb compilation of the complete helper succeeds. The header is not yet consumed by production Packet/CoverUtil: full observer construction/ABI is still missing.

Original CoverUtil primary vtable begins at 0x69120 with map callback, destructor pair and business callback. Its secondary table begins 0x69138, with destructors at slots 0/1 and business callback at slot 2 (0x18069). All secondary entries subtract four from this before dispatching to the complete CoverUtil object. Business thunk additionally returns literal 1 after dispatch. The +4 subobject is established by RTTI, vtable offset-to-top -4 and actual adjustment instructions.

Extended Unicorn tests execute the original primary callback and secondary thunk with +4 receiver; both produce the same complete RGB config trace, flag transitions, return value and temporary destruction. Ignored events (-1/22/24) preserve flags and perform no writes on both entry paths. External string/config/logging services are intercepted, not validated.

The exact formal parameter list and callback name of bl::IBLMapBusinessDataObserver remain unresolved. The body uses the event in r1 and ignores other incoming argument registers; this does not establish that the interface has only one explicit parameter. The similarly named OperatorDMap callback belongs to dice::IMapBusinessDataObserver, so it must not be copied as the bl interface declaration. No guessed virtual interface is introduced.

Evidence: v21_verified/cover_observer_vtables.txt and cover_secondary_thunks.txt. No Android strict-link success or replacement SO.
