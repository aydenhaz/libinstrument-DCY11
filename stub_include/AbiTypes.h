#pragma once
namespace bl { enum MapEngineID : int {}; }
namespace NS_MultiScreenBase { enum ScreenType : int { SCREEN_TYPE_MAIN = 0 }; }
enum ModuleType_ : int { MODULE_TYPE_DEFAULT = 1, MODULE_TYPE_MAP = 0x180 };
using ModuleType = ModuleType_;

// V21 host ABI types. Values remain unspecified pending caller recovery.
enum ENaviMode_ : int;
enum ECruiseMode_ : int;
