#pragma once
#include <GFrame/GblAlc.h>
typedef void (*AlcExportFnT)(void *alc, int count, int reserved, const void *block);
inline void AlcExport(const char *name, const char *sig, int tag, void *self) {
    void *alc = I_GetAlcPtr();
    if (!alc) return;
    alc = I_GetAlcPtr();
    if (!alc) return;
    void **table = *reinterpret_cast<void ***>(alc);
    const void *block[8] = { 0, 0, "HMI_PANNELLARGE", 0, (const void *)(long)tag, sig, self, name };
    reinterpret_cast<AlcExportFnT>(*table)(alc, 8, 0, block);
}
