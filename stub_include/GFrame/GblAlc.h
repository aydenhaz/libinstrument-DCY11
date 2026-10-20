#pragma once
#ifndef __cplusplus
#include <stdbool.h>
#endif
#ifdef __cplusplus
extern "C" {
#endif
void *I_GetAlcPtr(void);
void *I_GetAlcDevPtr(void);
// V21 callers normalize a condition to 0/1 in r0; no string argument.
void  I_Assert(bool condition);
void  I_Sleep(unsigned int ms);
unsigned long I_ThreadGetCurID(void);
#ifdef __cplusplus
}
#endif
