#pragma once
extern "C" {
int GAI_GetExtScreenUIStyle();
void GAI_GetExtScreenUISize(unsigned*, unsigned*);
void GAI_ExtScreenChangeScreenModeNotify(int);
void GAI_NaviInfoBroadcasting();
void GAI_DispatchArStatusNotify(int);
}
