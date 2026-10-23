#pragma once

namespace NS_MultiScreenBase {
// V21 0x2c1a4..2c1c8: operate on the timer state pointer at observer+0xcc.
// A constructed CGUITimer subobject must already exist at state+0x10.
// Returns true only when start succeeds and the started byte is set.
bool AutoExscreenUtil_TryStartMapStatusTimer(void* state);
}
