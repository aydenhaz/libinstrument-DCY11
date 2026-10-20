#pragma once
#include <cstdint>
#include <GFrame/GblAlc.h>
namespace IPL_ABI {
inline void packetTrace(const char* name,const char* sig,int line,bool targetR3) {
    void* alc=I_GetAlcPtr(); if(!alc)return;
    alc=I_GetAlcPtr(); if(!alc)return;
    typedef void (*Emit)(void*,int,int,uintptr_t,uint32_t,uint32_t,const char*,const char*,const char*,int,const char*);
    Emit emit=reinterpret_cast<Emit>(*static_cast<void***>(alc)[0]);
    emit(alc,8,0,targetR3?reinterpret_cast<uintptr_t>(emit):0x08000000u,
         0x08000000u,0,"HMI_PANNELLARGE","",sig,line,name);
}
}
