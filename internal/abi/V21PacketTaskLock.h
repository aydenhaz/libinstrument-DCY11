#pragma once
#include <cstring>
namespace IPL_ABI {
// Verified only for V21 Packet's constructed host base. Not a general lock ABI.
class PacketTaskLock {
    void* lock_;
    void (*unlock_)(void*);
public:
    explicit PacketTaskLock(void* packet) {
        lock_=static_cast<unsigned char*>(packet)+8;
        void** table;
        std::memcpy(&table,lock_,sizeof(table));
        unlock_=reinterpret_cast<void (*)(void*)>(table[3]);
        reinterpret_cast<void (*)(void*)>(table[2])(lock_);
    }
    ~PacketTaskLock(){unlock_(lock_);}
    PacketTaskLock(const PacketTaskLock&)=delete;
    PacketTaskLock& operator=(const PacketTaskLock&)=delete;
};
}
