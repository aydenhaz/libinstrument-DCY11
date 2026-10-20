#pragma once
#include "MemberCallableWrapper.h"
#include <memory>
#include <utility>

namespace IPL_ABI {
// Recovered GMethodInvoker slot order. Equality precedes the virtual destructor.
struct MethodInvokerBase {
    virtual bool equals(const MethodInvokerBase*) const = 0;
    virtual ~MethodInvokerBase() {}
    virtual const void* targetAddress() const = 0;
    virtual const void* methodAddress() const = 0;
    virtual void invoke() = 0;
};
template<class T> class MemberInvoker final : public MethodInvokerBase {
    T* target_;
    void (T::*method_)();
    std::shared_ptr<void> retainedInvoker_; // original onPreLoad initializes empty
public:
    MemberInvoker(T* target,void (T::*method)()):target_(target),method_(method){}
    bool equals(const MethodInvokerBase* other) const override {
        auto* rhs=dynamic_cast<const MemberInvoker*>(other);
        return rhs && target_==rhs->target_ && method_==rhs->method_;
    }
    const void* targetAddress() const override {return &target_;}
    const void* methodAddress() const override {return &method_;}
    void invoke() override {(target_->*method_)();}
};

// An isolated model; NOT a GPI::Closure::Callable ABI replacement yet.
// It uses the observed six virtual slots and 20-byte ARM32 field arrangement.
class VoidCallable {
    CallableWrapperBase* wrapper_ = nullptr;
    void* inlineDispatch_ = nullptr;
    std::shared_ptr<MethodInvokerBase> invoker_;
public:
    VoidCallable() = default;
    template<class T> static VoidCallable bind(T* target,void (T::*method)()) {
        VoidCallable result;
        // Acquire the shared object first so failure cannot leak the wrapper.
        result.invoker_=std::make_shared<MemberInvoker<T>>(target,method);
        result.wrapper_=new MemberCallableWrapper<T>(target,method);
        return result;
    }
    VoidCallable(const VoidCallable& other):
        wrapper_(other.wrapper_?other.wrapper_->clone():nullptr),invoker_(other.invoker_){}
    VoidCallable(VoidCallable&& other) noexcept:
        wrapper_(other.wrapper_),inlineDispatch_(other.inlineDispatch_),invoker_(std::move(other.invoker_)) {
        other.wrapper_=nullptr;other.inlineDispatch_=nullptr;
    }
    VoidCallable& operator=(VoidCallable other) noexcept {swap(other);return *this;}
    virtual ~VoidCallable() {invoker_.reset();delete wrapper_;}
    virtual bool empty() const {return !wrapper_&&!inlineDispatch_&&!invoker_;}
    virtual void reset() {delete wrapper_;wrapper_=nullptr;inlineDispatch_=nullptr;invoker_.reset();}
    virtual bool equals(const VoidCallable& other) const {
        return invoker_?invoker_->equals(other.invoker()):false;
    }
    virtual MethodInvokerBase* invoker() const {return invoker_.get();}
    void invoke() {if(wrapper_)wrapper_->invoke();}
    void swap(VoidCallable& other) noexcept {
        std::swap(wrapper_,other.wrapper_);std::swap(inlineDispatch_,other.inlineDispatch_);invoker_.swap(other.invoker_);
    }
};
#if defined(__arm__) && !defined(__aarch64__)
static_assert(sizeof(VoidCallable)==20,"V21 void Callable object size");
static_assert(sizeof(MemberInvoker<MemberCallableSizeProbe>)==24,"V21 member invoker size");
#endif
}
