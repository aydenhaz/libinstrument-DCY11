# Complete source ARM32 compile-only check

All 31 translation units pass after the current source changes.

Compiler: Zig 0.17.0 C++ driver.
Target: arm-linux-musleabi, -std=c++11 -fPIC -mthumb.
Not NDK r27c, not Android system headers, and not a shared-library link.

The explicit compiler launcher contains:

    exec /path/to/zig c++ -target arm-linux-musleabi "$@"

Run:

    python3 build_android.py --cxx /path/to/launcher --compile-only --out /tmp/arm32-check

Result:

    Compiled 31/31 source files
    Compile-only: no shared library linked or published

Per-unit results are in ARM32_COMPILE_ONLY.log. The first pass identified a
local abstract GPI::Closure redefinition in Batch14 conflicting with the real
header declaration now included by CGAsyncObject. The invalid local definition
is removed; no dummy invoke implementation replaces it. Existing placeholder
task bodies remain incomplete.

--compile-only removes stale final/pending SO files at entry and returns after
source compilation, before checking host libraries or linking. Publication
contract tests verify that it cannot leave a stale SO appearing successful.

## Navigation mode ABI correction

The V21 host method is:

    hsl::MultiScreenService::SetNaviType(ENaviMode_, ECruiseMode_)

Both enums are global types. The header declares those ABI types without
inventing their values and adapts existing integer callers with explicit
casts. The observed host implementation returns zero; the declaration returns
int, and current callers ignore it. A native argument-forwarding test passes.
An ARM compile-only probe emits exactly the actual host symbol:

    _ZN3hsl18MultiScreenService11SetNaviTypeE10ENaviMode_12ECruiseMode_

This fixes a specific wrong declaration; it does not prove that the existing
callers' zero-valued mode choices reproduce the original business behavior.

## Remaining limits

No strict Android shared-library link passed in this checkpoint. Existing
unrecovered Packet/CoverUtil/Opening Fragment construction, callbacks,
registrations and placeholder business functions are still blockers. Compilation
alone does not establish ODR consistency, host ABI or runtime equivalence.
The screenshot business-data callback's full signature is still unresolved;
a similarly named dice interface was not substituted for the distinct bl type.
