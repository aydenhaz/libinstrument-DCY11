#pragma once
namespace hsl { class IUITaskThread; }
namespace hmi {
// V21 allocation at IPL 0x2ec66 is 0xf0; real ctor is Common 0x3877a8.
class UITaskThreadProxy {
    alignas(4) unsigned char abi_storage_[0xf0];
public:
    explicit UITaskThreadProxy(int executor_id);
    ~UITaskThreadProxy();
    // Original call 0x2ec96..0x2ecac passes the allocation address unchanged.
    hsl::IUITaskThread* taskInterface() { return reinterpret_cast<hsl::IUITaskThread*>(this); }
};
static_assert(sizeof(UITaskThreadProxy) == 0xf0, "V21 task proxy allocation size");
}
