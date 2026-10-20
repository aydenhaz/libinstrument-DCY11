# Current ARM object symbol inventory

Generated from the 31 ARM Linux musl compile-only objects against the nine supplied host libraries. This is NOT an Android linker result. Historical REBUILD_LINK_ERRORS.txt is not the current inventory.

Counts: {"rebuilt": 2, "host": 177, "weak-unprovided": 0, "unprovided": 43}

“host” means a matching exported name only. It does not prove receiver, return-value ABI, object layout or behavior. Runtime/C++ namespace differences below require an Android NDK rebuild; do not patch aliases to make this inventory pass.

## Unprovided symbol names

- [source/interface] `NS_MultiScreenBase::AutoConfigJsonData::GetMultiViewModeInfo(NS_MultiScreenBase::ScreenType, std::__1::vector<NS_MultiScreenBase::tagMultiViewModeInfo, std::__1::allocator<NS_MultiScreenBase::tagMultiViewModeInfo> >&)`
- [source/interface] `NS_MultiScreenBase::AutoOpeningFragment::EnableMapRender(bool, int)`
- [source/interface] `NS_MultiScreenBase::AutoExscreenNotifyObserver::ActionA()`
- [source/interface] `NS_MultiScreenBase::AutoExscreenNotifyObserver::ActionB()`
- [source/interface] `NS_MultiScreenBase::AutoExscreenNotifyObserver::ActionC()`
- [source/interface] `NS_InstrumentPannelLarge::CGIPLPacket::TopRenderCallback(GNS_FRAME::CGCanvas*)`
- [source/interface] `GPI::Closure GPI::Closure::bind<NS_MultiScreenBase::AutoExscreenNotifyObserver, void (NS_MultiScreenBase::AutoExscreenNotifyObserver::*)()>(NS_MultiScreenBase::AutoExscreenNotifyObserver*, void (NS_MultiScreenBase::AutoExscreenNotifyObserver::*)())`
- [source/interface] `hsl::SystemNotify::AddNetObserver(void*)`
- [source/interface] `hsl::SystemNotify::AddTestNumObserver(void*)`
- [source/interface] `hsl::SystemNotify::RemoveTestNumObserver(void*)`
- [source/interface] `hsl::SystemNotify::AddLanguageSwitchObserver(void*, bool)`
- [source/interface] `hsl::SystemNotify::AddMapSurfaceViewObserver(void*)`
- [source/interface] `hsl::SystemNotify::AddInstrumentPannelObserver(void*)`
- [source/interface] `hsl::SystemNotify::RemoveInstrumentPannelObserver(void*)`
- [source/interface] `hsl::HMICommonUtil::FormatDistance(void*, double)`
- [source/interface] `hsl::UIObject::RunAsyncDelayByEngineId(GPI::Closure const&, bl::MapEngineID, int)`
- [source/interface] `CGString::Format(char const*, ...)`
- [source/interface] `GNS_FRAME::CGFragment::inflaterUnrecovered(int, GNS_FRAME::CGViewGroup*)`
- [source/interface] `GNS_FRAME::CGFragment::findViewByIdUnrecovered(int)`
- [source/interface] `GNS_FRAME::CGFragment::isTopFragmentUnrecovered()`
- [source/interface] `GNS_FRAME::CGFragment::startNewFragmentUnrecovered(void*, int, GNS_FRAME::CGIntent const&, int)`
- [source/interface] `GNS_FRAME::CGFragment::tryStartExistFragmentUnrecovered(GNS_FRAME::CGIntent const&, int, void*)`
- [toolchain/runtime] `std::__1::mutex::lock()`
- [toolchain/runtime] `std::__1::mutex::unlock()`
- [toolchain/runtime] `std::__1::mutex::~mutex()`
- [toolchain/runtime] `__aeabi_uidiv`
- [toolchain/runtime] `__aeabi_unwind_cpp_pr0`
- [toolchain/runtime] `__aeabi_unwind_cpp_pr1`
- [toolchain/runtime] `__cxa_atexit`
- [toolchain/runtime] `__dso_handle`
- [toolchain/runtime] `__stack_chk_fail`
- [toolchain/runtime] `__stack_chk_guard`
- [toolchain/runtime] `__ubsan_handle_builtin_unreachable`
- [toolchain/runtime] `__ubsan_handle_divrem_overflow`
- [toolchain/runtime] `__ubsan_handle_load_invalid_value`
- [toolchain/runtime] `__ubsan_handle_out_of_bounds`
- [toolchain/runtime] `__ubsan_handle_pointer_overflow`
- [toolchain/runtime] `__ubsan_handle_shift_out_of_bounds`
- [toolchain/runtime] `__ubsan_handle_type_mismatch_v1`
- [toolchain/runtime] `memcpy`
- [toolchain/runtime] `memmove`
- [toolchain/runtime] `memset`
- [toolchain/runtime] `strncpy`

GetMultiViewModeInfo contains std::__1 in these objects; original Android hosts use std::__ndk1. This symbol is listed above as a source/interface boundary but its observed mismatch is toolchain-related, not evidence that its business signature should be changed.

Reproduce (requires pyelftools):
```sh
PYTHONPATH=/tmp/ipl-analysis-deps python3 tools/audit_elf_symbols.py --objects /tmp/ipl-fragment-view-check --hosts host_libs --output /tmp/ipl-symbol-audit.json
```

The object directory must come from a successful, fresh all-source compilation; the inventory does not detect stale objects. System runtime dependencies and linker-provided symbols are not included in the supplied host set.
