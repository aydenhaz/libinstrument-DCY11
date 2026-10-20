#pragma once
namespace GPI { class Closure; }
namespace GNS_FRAME {
class CGFuncInfoTool {
public:
    static void addFuncInfo(const GPI::Closure&, void*, const char*, const char*, int);
};
}
