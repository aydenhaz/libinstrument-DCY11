#pragma once
#include <GFrame/GblAlc.h>
#include <cstdint>

// The V21 logger receives three ordinary register arguments, an ambient r3
// value, and its fields as arguments on the ARM32 stack. Passing a pointer to
// an eight-word array as r3 loses all seven required stack fields.
enum class AlcRegister3 { Callback, Flag, Auxiliary };

template<class... Values>
inline void AlcExportFields(const char *module, const char *name,
                            const char *sig, int line, AlcRegister3 r3Source,
                            Values... values) {
    void *alc = I_GetAlcPtr();
    if (!alc) return;
    alc = I_GetAlcPtr();
    if (!alc) return;
    typedef void (*Emit)(void*, int, int, std::uintptr_t, std::uint32_t,
                         std::uint32_t, const char*, const char*, const char*,
                         int, const char*, Values...);
    void **table = *reinterpret_cast<void***>(alc);
    Emit emit = reinterpret_cast<Emit>(*table);
    const char *aux = "";
    const std::uintptr_t r3 = r3Source == AlcRegister3::Callback
        ? reinterpret_cast<std::uintptr_t>(emit)
        : r3Source == AlcRegister3::Flag
          ? 0x08000000u : reinterpret_cast<std::uintptr_t>(aux);
    emit(alc, 8, 0, r3, 0x08000000u, 0, module, aux,
         sig, line, name, values...);
}

template<class... Values>
inline void AlcExport(const char *name, const char *sig, int line,
                      Values... values) {
    AlcExportFields("HMI_PANNELLARGE", name, sig, line,
                    AlcRegister3::Callback, values...);
}

template<class... Values>
inline void AlcExportCommon(const char *name, const char *sig, int line,
                            AlcRegister3 r3Source, Values... values) {
    AlcExportFields("HMI_COMMON", name, sig, line, r3Source, values...);
}
