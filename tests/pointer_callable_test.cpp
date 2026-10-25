#include <GFrame/CGFragmentBuilder.h>
#include <cassert>
#include <cstring>
#include <memory>
#include <stdexcept>
struct Value { int n; };
struct Padding { virtual ~Padding() {} int pad = 7; };
struct Target { virtual ~Target() {} virtual Value* get() = 0; };
struct Object : Padding, Target {
    Value value{42};
    bool fail = false;
    Value* get() override { if (fail) throw std::runtime_error("factory"); return &value; }
    void* type() { return &value; }
};
struct Counted : GPI::detail::PointerMethod<Value> {
    int* destroyed;
    Value value{9};
    explicit Counted(int* p) : destroyed(p) {}
    ~Counted() override { ++*destroyed; }
    bool equals(const PointerMethod* rhs) const override { return this == rhs; }
    const void* targetAddress() const override { return nullptr; }
    const void* methodAddress() const override { return nullptr; }
    Value* invoke() override { return &value; }
};
int main() {
    using Callback = GPI::Closure::Callable<Value*()>;
    Callback empty; assert(empty.empty() && !empty.invoke());
    Object object;
    auto callback = GPI::bindPointerCallable(static_cast<Target*>(&object), &Target::get);
    auto type = GPI::bindPointerCallable(&object, &Object::type);
    assert(callback.invoke() == &object.value && type.invoke() == &object.value);
    auto copy = callback;
    assert(copy.equals(callback));
    // Emulate the host's wrapper clone/invoke entrypoints on native ABI.
    GPI::detail::PointerWrapper<Value>* wrapper;
    std::memcpy(&wrapper, reinterpret_cast<const char*>(&callback) + sizeof(void*), sizeof(wrapper));
    std::unique_ptr<GPI::detail::PointerWrapper<Value>> clone(wrapper->clone());
    callback.reset(); // copy still owns the shared method used by both clones
    assert(clone->invoke() == &object.value && copy.invoke() == &object.value);
    object.fail = true;
    try { copy.invoke(); assert(false); } catch (const std::runtime_error&) {}
    object.fail = false;
    auto moved = std::move(copy); assert(copy.empty() && moved.invoke() == &object.value);
    moved = moved; assert(moved.invoke() == &object.value);
    clone.reset(); moved.reset(); moved.reset();
    int destroyed = 0;
    {
        auto owner = std::make_shared<Counted>(&destroyed);
        Callback a(owner), b(a); owner.reset(); a.reset();
        assert(!destroyed && b.invoke()->n == 9);
        b.reset(); assert(destroyed == 1);
    }
    assert(destroyed == 1);
}
