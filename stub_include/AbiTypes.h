#pragma once
namespace bl { enum MapEngineID : int {}; }
namespace NS_MultiScreenBase { enum ScreenType : int { SCREEN_TYPE_MAIN = 0 }; }
enum ModuleType_ : int { MODULE_TYPE_DEFAULT = 1, MODULE_TYPE_MAP = 0x180 };
using ModuleType = ModuleType_;
