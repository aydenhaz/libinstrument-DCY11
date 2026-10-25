#pragma once
#include <GFrame/GPIClosure.h>
#include <memory>
#include <utility>

namespace GPI {
namespace detail {
// Pointer-return counterpart of the V21 six-slot method object. Its C++
// identity is private to this adapter; no original RTTI identity is claimed.
template<class R> struct PointerMethod {
    virtual bool equals(const PointerMethod*) const = 0;
    virtual ~PointerMethod() {}
    virtual const void* targetAddress() const = 0;
    virtual const void* methodAddress() const = 0;
    virtual R* invoke() = 0;
};
template<class R, class T> class PointerMember final : public PointerMethod<R> {
    T* target_;
    R* (T::*method_)();
public:
    PointerMember(T* target, R* (T::*method)()) : target_(target), method_(method) {}
    bool equals(const PointerMethod<R>* other) const override {
        const auto* rhs = dynamic_cast<const PointerMember*>(other);
        return rhs && target_ == rhs->target_ && method_ == rhs->method_;
    }
    const void* targetAddress() const override { return &target_; }
    const void* methodAddress() const override { return &method_; }
    R* invoke() override { return (target_->*method_)(); }
};
template<class R> struct PointerWrapper {
    virtual ~PointerWrapper() {}
    virtual R* invoke() = 0;
    virtual PointerWrapper* clone() const = 0;
};
template<class R> struct PointerMemberWrapper final : PointerWrapper<R> {
    PointerMethod<R>* method_;
    explicit PointerMemberWrapper(PointerMethod<R>* method) : method_(method) {}
    R* invoke() override { return method_->invoke(); }
    PointerWrapper<R>* clone() const override { return new PointerMemberWrapper(*this); }
};
}

// ARM32: vptr, owned wrapper, zero inline-dispatch word, shared method pair.
// Only member binding with inlineDispatch == 0 is supported. Host builder C2
// clones wrapper slot 3 and retains the pair; calls use wrapper slot 2.
template<class R> class Closure::Callable<R*()> {
    detail::PointerWrapper<R>* wrapper_ = nullptr;
    void* inlineDispatch_ = nullptr;
    std::shared_ptr<detail::PointerMethod<R>> method_;
public:
    Callable() = default;
    explicit Callable(std::shared_ptr<detail::PointerMethod<R>> method)
        : wrapper_(method ? new detail::PointerMemberWrapper<R>(method.get()) : nullptr),
          method_(std::move(method)) {}
    Callable(const Callable& other)
        : wrapper_(other.wrapper_ ? other.wrapper_->clone() : nullptr),
          method_(other.method_) {}
    Callable(Callable&& other) noexcept
        : wrapper_(other.wrapper_), method_(std::move(other.method_)) {
        other.wrapper_ = nullptr;
    }
    Callable& operator=(Callable other) noexcept { swap(other); return *this; }
    virtual ~Callable() { method_.reset(); delete wrapper_; }
    virtual bool empty() const { return !wrapper_ && !inlineDispatch_ && !method_; }
    virtual void reset() {
        delete wrapper_; wrapper_ = nullptr; inlineDispatch_ = nullptr;
        method_.reset();
    }
    virtual bool equals(const Callable& other) const {
        return method_ && method_->equals(other.method_.get());
    }
    virtual detail::PointerMethod<R>* invoker() const { return method_.get(); }
    R* invoke() { return wrapper_ ? wrapper_->invoke() : nullptr; }
    void swap(Callable& other) noexcept {
        std::swap(wrapper_, other.wrapper_);
        std::swap(inlineDispatch_, other.inlineDispatch_);
        method_.swap(other.method_);
    }
};
template<class R, class T>
Closure::Callable<R*()> bindPointerCallable(T* target, R* (T::*method)()) {
    return Closure::Callable<R*()>(std::shared_ptr<detail::PointerMethod<R>>(
        new detail::PointerMember<R, T>(target, method)));
}
#if defined(__arm__) && !defined(__aarch64__)
static_assert(sizeof(Closure::Callable<void*()>) == 20, "V21 pointer Callable extent");
#endif
}
