#pragma once
#include <GFrame/GPIClosure.h>
#include <cstdint>
#include <cstring>
#include <memory>
#include <utility>
namespace GNS_FRAME { struct STGBroadcastMsg_; }
namespace GPI {
// ARM member pointer: function/virtual byte offset and (this adjustment << 1)
// with the low bit identifying virtual dispatch. Original Cruise uses e4/1
// and ec/1. Targets must have real constructed vtables before invocation.
struct BroadcastMemberAddress {
    uintptr_t function;
    intptr_t adjustment;
    bool operator==(const BroadcastMemberAddress& rhs) const {
        return function == rhs.function &&
            ((function == 0 && ((adjustment | rhs.adjustment) & 1) == 0) || adjustment == rhs.adjustment);
    }
};
namespace detail {
inline void dispatchBroadcast(void* target, BroadcastMemberAddress member,
                              const GNS_FRAME::STGBroadcastMsg_& message) {
    auto* receiver = static_cast<unsigned char*>(target) + (member.adjustment >> 1);
    uintptr_t function = member.function;
    if (member.adjustment & 1) {
        unsigned char* table;
        std::memcpy(&table, receiver, sizeof table);
        std::memcpy(&function, table + member.function, sizeof function);
    }
    reinterpret_cast<void(*)(void*, const GNS_FRAME::STGBroadcastMsg_&)>(function)(receiver, message);
}
struct BroadcastMethod {
    virtual bool equals(const BroadcastMethod*) const = 0;
    virtual ~BroadcastMethod() {}
    virtual const void* targetAddress() const = 0;
    virtual const void* methodAddress() const = 0;
    virtual void invoke(const GNS_FRAME::STGBroadcastMsg_&) = 0;
};
class BroadcastMember final : public BroadcastMethod {
    void* target_;
    BroadcastMemberAddress member_;
    std::shared_ptr<void> retained_; // empty in the observed Cruise binding
public:
    BroadcastMember(void* target, BroadcastMemberAddress member):target_(target),member_(member){}
    bool equals(const BroadcastMethod* other) const override {
        const auto* rhs = dynamic_cast<const BroadcastMember*>(other);
        return rhs && target_ == rhs->target_ && member_ == rhs->member_;
    }
    const void* targetAddress() const override {return &target_;}
    const void* methodAddress() const override {return &member_;}
    void invoke(const GNS_FRAME::STGBroadcastMsg_& message) override {dispatchBroadcast(target_,member_,message);}
};
struct BroadcastWrapper {
    virtual ~BroadcastWrapper() {}
    virtual void invoke(const GNS_FRAME::STGBroadcastMsg_&) = 0;
    virtual BroadcastWrapper* clone() const = 0;
};
class BroadcastMemberWrapper final : public BroadcastWrapper {
    void* target_;
    BroadcastMemberAddress member_;
public:
    BroadcastMemberWrapper(void* target,BroadcastMemberAddress member):target_(target),member_(member){}
    void invoke(const GNS_FRAME::STGBroadcastMsg_& message) override {dispatchBroadcast(target_,member_,message);}
    BroadcastWrapper* clone() const override {return new BroadcastMemberWrapper(*this);}
};
}
template<> class Closure::Callable<void(const GNS_FRAME::STGBroadcastMsg_&)> {
    detail::BroadcastWrapper* wrapper_ = nullptr;
    void* inlineDispatch_ = nullptr;
    std::shared_ptr<detail::BroadcastMethod> method_;
public:
    Callable() = default;
    Callable(void* target,BroadcastMemberAddress member)
        : method_(std::make_shared<detail::BroadcastMember>(target,member)) {
        wrapper_ = new detail::BroadcastMemberWrapper(target,member);
    }
    Callable(const Callable& rhs):wrapper_(rhs.wrapper_ ? rhs.wrapper_->clone() : nullptr),method_(rhs.method_){}
    Callable(Callable&& rhs) noexcept:wrapper_(rhs.wrapper_),method_(std::move(rhs.method_)) {rhs.wrapper_=nullptr;}
    Callable& operator=(Callable rhs) noexcept {swap(rhs);return *this;}
    virtual ~Callable() {method_.reset();delete wrapper_;}
    virtual bool empty() const {return !wrapper_ && !inlineDispatch_ && !method_;}
    virtual void reset() {delete wrapper_;wrapper_=nullptr;inlineDispatch_=nullptr;method_.reset();}
    virtual bool equals(const Callable& rhs) const {return method_ && method_->equals(rhs.method_.get());}
    virtual detail::BroadcastMethod* invoker() const {return method_.get();}
    void invoke(const GNS_FRAME::STGBroadcastMsg_& message) {if(wrapper_)wrapper_->invoke(message);}
    void swap(Callable& rhs) noexcept {std::swap(wrapper_,rhs.wrapper_);std::swap(inlineDispatch_,rhs.inlineDispatch_);method_.swap(rhs.method_);}
};
using BroadcastCallable = Closure::Callable<void(const GNS_FRAME::STGBroadcastMsg_&)>;
#if defined(__arm__) && !defined(__aarch64__)
static_assert(sizeof(BroadcastMemberAddress)==8,"V21 member pointer");
static_assert(sizeof(detail::BroadcastMemberWrapper)==16,"V21 wrapper extent");
static_assert(sizeof(detail::BroadcastMember)==24,"V21 method extent");
static_assert(sizeof(BroadcastCallable)==20,"V21 broadcast Callable extent");
#endif
}
