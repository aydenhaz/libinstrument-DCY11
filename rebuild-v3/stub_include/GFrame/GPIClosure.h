#pragma once
#include <cstddef>
#include <memory>
#include <tuple>
#include <type_traits>
#include <utility>

namespace GPI {
namespace detail {
template<std::size_t... N> struct Indices {};
template<std::size_t Count, std::size_t... N>
struct MakeIndices : MakeIndices<Count-1, Count-1, N...> {};
template<std::size_t... N> struct MakeIndices<0, N...> {
    typedef Indices<N...> type;
};
struct MethodInvoker {
    virtual bool equals(const MethodInvoker*) const = 0;
    virtual ~MethodInvoker() {}
    virtual const void* targetAddress() const = 0;
    virtual const void* methodAddress() const = 0;
    virtual void invoke() = 0;
};
template<class T, class... P> class BoundMember final : public MethodInvoker {
    T* target_;
    void (T::*method_)(P...);
    std::tuple<typename std::decay<P>::type...> values_;
    template<std::size_t... N> void call(Indices<N...>) {
        (target_->*method_)(std::get<N>(values_)...);
    }
public:
    BoundMember(T* target, void (T::*method)(P...),
                typename std::decay<P>::type... values)
        : target_(target), method_(method), values_(values...) {}
    bool equals(const MethodInvoker* other) const override {
        const BoundMember* p = dynamic_cast<const BoundMember*>(other);
        return p && p->target_ == target_ && p->method_ == method_ &&
               p->values_ == values_;
    }
    const void* targetAddress() const override { return &target_; }
    const void* methodAddress() const override { return &method_; }
    void invoke() override { call(typename MakeIndices<sizeof...(P)>::type{}); }
};
// Four ARM vtable slots: two destructors, invoke and clone.
struct Wrapper {
    virtual ~Wrapper() {}
    virtual void invoke() = 0;
    virtual Wrapper* clone() const = 0;
};
struct MemberWrapper final : Wrapper {
    MethodInvoker* method_;
    explicit MemberWrapper(MethodInvoker* method) : method_(method) {}
    void invoke() override { method_->invoke(); }
    Wrapper* clone() const override { return new MemberWrapper(*this); }
};
}
class Closure {
public:
    template<class Signature> class Callable;
    template<class T, class... P>
    static Callable<void()> bind(T* target, void (T::*method)(P...),
                                 typename std::decay<P>::type... values);
};
template<> class Closure::Callable<void()> {
    detail::Wrapper* wrapper_;
    void* inlineDispatch_;
    std::shared_ptr<detail::MethodInvoker> method_;
public:
    Callable() : wrapper_(nullptr), inlineDispatch_(nullptr) {}
    explicit Callable(std::shared_ptr<detail::MethodInvoker> method)
        : wrapper_(new detail::MemberWrapper(method.get())),
          inlineDispatch_(nullptr), method_(std::move(method)) {}
    Callable(const Callable& other)
        : wrapper_(other.wrapper_ ? other.wrapper_->clone() : nullptr),
          inlineDispatch_(other.inlineDispatch_), method_(other.method_) {}
    Callable(Callable&& other) noexcept
        : wrapper_(other.wrapper_), inlineDispatch_(other.inlineDispatch_),
          method_(std::move(other.method_)) {
        other.wrapper_ = nullptr; other.inlineDispatch_ = nullptr;
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
    virtual detail::MethodInvoker* invoker() const { return method_.get(); }
    void invoke() { if (wrapper_) wrapper_->invoke(); }
    void swap(Callable& other) noexcept {
        std::swap(wrapper_, other.wrapper_);
        std::swap(inlineDispatch_, other.inlineDispatch_);
        method_.swap(other.method_);
    }
};
template<class T, class... P>
Closure::Callable<void()> Closure::bind(
    T* target, void (T::*method)(P...),
    typename std::decay<P>::type... values) {
    typedef detail::BoundMember<T, P...> Bound;
    return Callable<void()>(std::shared_ptr<detail::MethodInvoker>(
        new Bound(target, method, values...)));
}
#if defined(__arm__) && !defined(__aarch64__)
static_assert(sizeof(Closure::Callable<void()>) == 20, "V21 GPI Callable extent");
#endif
}
