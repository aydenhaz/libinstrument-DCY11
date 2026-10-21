#pragma once
#include <GFrame/CGString.h>
#include <cstdint>
namespace GNS_FRAME {
class CGPackage;
class CGContext;
using PackageFactory=CGPackage* (*)(CGContext*);
// Host constructor/destructor own registration; no local no-op definitions.
class ClassNode_CGPackage {
    CGString name_;
    PackageFactory factory_;
    ClassNode_CGPackage* next_;
public:
    ClassNode_CGPackage(const CGString&,PackageFactory,const CGString&);
    ~ClassNode_CGPackage();
    ClassNode_CGPackage(const ClassNode_CGPackage&)=delete;
    ClassNode_CGPackage& operator=(const ClassNode_CGPackage&)=delete;
};
#if UINTPTR_MAX == 0xffffffffu
static_assert(sizeof(ClassNode_CGPackage)==0x20,"V21 registration node extent");
#endif
}
