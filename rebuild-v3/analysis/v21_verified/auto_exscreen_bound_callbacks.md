# V21 AutoExscreen scheduling ABI

Original ARM32 DSO SHA-256:
`6f5bd54c8e694ac40f02cd4280f6c6a049ea34896703e1eb5547fbc9e41d65e3`.

At 0x2a5a8 the bound object allocation is 0x18 bytes and captures the
map-engine ID and Boolean. At 0x2a7e0 it is 0x20 bytes and captures engine,
level, mode and position. At 0x2aa28 it is 0x34 bytes and copies the complete
0x24-byte tagSTExtScreenControl payload. Each schedules work through
`hsl::UIObject::RunAsyncDelayByEngineId` on the subobject at `this+0xe8`.
The call uses a hidden result slot for a shared ownership pair and a nested
`GPI::Closure::Callable<void()>` argument. Its imported mangled name is
`_ZN3hsl8UIObject23RunAsyncDelayByEngineIdERKN3GPI7Closure8CallableIFvvEEEN2bl11MapEngineIDEjy`.

The Callable stores a vptr, cloned wrapper pointer, zero inline-dispatch word,
and two-word shared method ownership (20 bytes total). The wrapper vtable
has two destructor entries, invoke and clone. The method object has
equality, two destructor entries, target/member address accessors and invoke.
The builder's ARM32 size assertions confirm the 20/24/32/52 byte extents.
Host ASan/UBSan tests check copying, resetting and executing all three
argument shapes.

The 64-bit source token passed to scheduling is currently zero; exact logging
metadata and object construction at +0xe8 remain unverified. The host test
checks our callable's behavior, not a V21 Android host scheduler.
