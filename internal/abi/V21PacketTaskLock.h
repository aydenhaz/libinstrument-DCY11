#pragma once
#include <cstring>
namespace IPL_ABI {
// Verified only for V21 Packet's constructed host base. Not a general lock ABI.
class PacketTaskLock {
    void* lock_;
public:
    explicit PacketTaskLock(void* packet) {
        lock_=static_cast<unsigned char*>(packet)+8;
        void** table;
        std::memcpy(&table,lock_,sizeof(table));
        reinterpret_cast<void (*)(void*)>(table[2])(lock_);
    }
    ~PacketTaskLock(){
        // Both preload (0x2f5a4) and destroy (0x2f390) reload the lock
        // vptr after the host task operation, immediately before unlock.
        void** table;
        std::memcpy(&table,lock_,sizeof(table));
        reinterpret_cast<void (*)(void*)>(table[3])(lock_);
    }
    PacketTaskLock(const PacketTaskLock&)=delete;
    PacketTaskLock& operator=(const PacketTaskLock&)=delete;
};
}
