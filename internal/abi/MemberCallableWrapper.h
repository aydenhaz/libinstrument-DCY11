#pragma once
namespace IPL_ABI {
// V21 wrapper vtable: complete destructor, deleting destructor, invoke, clone.
// This component is not yet wired to GPI Callable's shared invoker ownership.
struct CallableWrapperBase {
    virtual ~CallableWrapperBase() {}
    virtual void invoke() = 0;
    virtual CallableWrapperBase* clone() const = 0;
};
template<class T> class MemberCallableWrapper final : public CallableWrapperBase {
    T* target_;
    void (T::*method_)();
public:
    MemberCallableWrapper(T* target,void (T::*method)()):target_(target),method_(method){}
    void invoke() override {(target_->*method_)();}
    CallableWrapperBase* clone() const override {return new MemberCallableWrapper(*this);}
};
#if defined(__arm__) && !defined(__aarch64__)
struct MemberCallableSizeProbe {void run();};
static_assert(sizeof(void (MemberCallableSizeProbe::*)())==8,"V21 ARM member pointer");
static_assert(sizeof(MemberCallableWrapper<MemberCallableSizeProbe>)==16,"V21 wrapper size");
#endif
}
