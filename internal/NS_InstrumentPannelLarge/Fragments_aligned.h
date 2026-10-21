#pragma once
// Retired: the old four-slot hierarchy was not a valid Fragment ABI.
// Original 0x31474 is the A106 complete destructor, 0x31528 its deleting
// destructor. Slots 2/3 read bytes +4/+5; they are not null/adjustment thunks.
// Do not reintroduce placeholder inheritance; recover host bases first.
// See analysis/FRAGMENT_VTABLE_CORRECTION.md and rejected/Fragments_aligned.h.txt.
