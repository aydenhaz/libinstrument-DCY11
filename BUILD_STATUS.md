# ARM32 rebuild checkpoint — 2026-10-20

Status: NOT a usable replacement. No successful shared-library link.

## Validation

Android NDK r27c, armv7a-linux-androideabi21-clang++, all 26 translation units compile. Strict --no-undefined link fails with 23 remaining unresolved symbols. Four duplicate definitions have been removed without allowing multiple definitions. No empty implementations were added to satisfy the linker.

## Fixes checked against supplied host exports

- Shared named ScreenType, MapEngineID and ModuleType_ enums; Config::VALUE_STRATEGY enum.
- GetSurfaceConfig is const and takes ScreenType; scene-mode APIs take ScreenType.
- Correct CGPackageMgr forward declaration namespace and __dso_handle C linkage.
- Callback parameter types (CGCanvas* / no arguments), CGActivity intent constness, CGFragment void* parameter.
- CGExhibition namespace, const string and CGExclusionInfo* parameters.
- CGImage EGColorType_ spelling; CGFrameSimulation sendMsg 64-bit argument and message-control pointer.
- Correct crash logger argument types, UTF-16 format literals, HSL system-notify accessor.
- Build scripts now compile all internal and expanded translation units and retain strict linking.

## Replacement blockers

The uploaded code is a partial reconstruction. Numerous functions contain only comments or index entries. SurfaceConfig, tagMultiViewModeInfo, CGString, CGIntent and other objects have incomplete storage declarations; calls into real host constructors/getters can overwrite memory. Observer inheritance/subobject offsets, closure ownership and factory registrations are not reconstructed. Enum name corrections alone do not establish full ABI equivalence. Callback signatures are corrected but their bodies remain missing. The original init/fini code is still a skeleton.

The 7.5 V21 APK and analysis archive were located in Library, but the current bundled download helper failed with hosted-apps HTTP 403. Their binary contents have NOT been read in this checkpoint. No claim of V21 behavior equivalence is made.

## Build

```sh
sh host_libs/restore_libGbl.sh
python3 build_android.py --ndk /path/to/android-ndk-r27c
```

The supplied hosts are ARM32. This build is not an arm64 replacement for 9.5.

## Remaining strict-link errors

```text
ld.lld: error: undefined symbol: NS_InstrumentPannelLarge::CGIPLPacket::TopRenderCallback(GNS_FRAME::CGCanvas*)
ld.lld: error: undefined symbol: hsl::UITaskThreadProxy::UITaskThreadProxy(int)
ld.lld: error: undefined symbol: hsl::CommonUtils::GetEngineIdByDeviceId(int)
ld.lld: error: undefined symbol: hsl::ThirdManager::SetUITaskThread(int, int, int, bool, void*)
ld.lld: error: undefined symbol: hsl::SystemNotify::AddMapSurfaceViewObserver(void*)
ld.lld: error: undefined symbol: hsl::SystemNotify::AddLanguageSwitchObserver(void*, bool)
ld.lld: error: undefined symbol: hsl::SystemNotify::AddNetObserver(void*)
ld.lld: error: undefined symbol: hsl::SystemNotify::AddInstrumentPannelObserver(void*)
ld.lld: error: undefined symbol: hsl::MultiScreenService::SetStartOpenScreenMode(int, NS_MultiScreenBase::tagSTExtScreen const&)
ld.lld: error: undefined symbol: hsl::SystemNotify::RemoveInstrumentPannelObserver(void*)
ld.lld: error: undefined symbol: hsl::SystemNotify::AddMapSurfaceObserver(void*)
ld.lld: error: undefined symbol: GPI::Closure GPI::Closure::bind<NS_InstrumentPannelLarge::CGIPLPacket, void (NS_InstrumentPannelLarge::CGIPLPacket::*)()>(NS_InstrumentPannelLarge::CGIPLPacket*, void (NS_InstrumentPannelLarge::CGIPLPacket::*)())
ld.lld: error: undefined symbol: GNS_FRAME::CGFuncInfoTool::addFuncInfo(GPI::Closure const&, void*, char const*, char const*, int)
ld.lld: error: undefined symbol: hsl::GOTHERProtocolHmiToAl::ExScreenRenderActionHmiToAl(NS_MultiScreenBase::tagSTExtScreen*)
ld.lld: error: undefined symbol: GPI::Closure GPI::Closure::bind<NS_MultiScreenBase::AutoExscreenNotifyObserver, void (NS_MultiScreenBase::AutoExscreenNotifyObserver::*)()>(NS_MultiScreenBase::AutoExscreenNotifyObserver*, void (NS_MultiScreenBase::AutoExscreenNotifyObserver::*)())
ld.lld: error: undefined symbol: hsl::UIObject::RunAsyncDelayByEngineId(GPI::Closure const&, bl::MapEngineID, int)
ld.lld: error: undefined symbol: NS_MultiScreenBase::AutoExscreenNotifyObserver::ActionA()
ld.lld: error: undefined symbol: NS_MultiScreenBase::AutoExscreenNotifyObserver::ActionB()
ld.lld: error: undefined symbol: NS_MultiScreenBase::AutoExscreenNotifyObserver::ActionC()
ld.lld: error: undefined symbol: hsl::MultiScreenService::SetNaviType(int, int)
ld.lld: error: undefined symbol: NS_MultiScreenBase::AutoOpeningFragment::EnableMapRender(bool, int)
ld.lld: error: undefined symbol: hsl::SystemNotify::AddTestNumObserver(void*)
ld.lld: error: undefined symbol: hsl::GOTHERProtocolHmiToAl::MultiScreenStatusHmiToAl(NS_MultiScreenBase::tagSTExtScreen const&)
```
