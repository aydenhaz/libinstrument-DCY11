#pragma once
#ifdef __cplusplus
extern "C" {
#endif
void *I_GetAlcPtr(void);
void *I_GetAlcDevPtr(void);
void  I_Assert(const char *expr);
void  I_Sleep(unsigned int ms);
unsigned long I_ThreadGetCurID(void);
#ifdef __cplusplus
}
#endif
