#pragma once
enum ModuleInterfaceId { };
class ExModuleManager {
public:
    static ExModuleManager *GetInstance();
    int ModuleReg(ModuleInterfaceId, void*);
    int ModuleUnReg(ModuleInterfaceId);
};
