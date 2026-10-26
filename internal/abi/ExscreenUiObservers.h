#pragma once
#if defined(__arm__) && !defined(__aarch64__)
namespace IPL_ABI {
// V21 AutoExscreenNotifyObserver storage, with constructed locks/allocators.
// Listeners are non-owning; caller must remove them before target destruction.
void addMapRenderObserver(void* owner, void* observer);
void removeMapRenderObserver(void* owner, void* observer);
void addUiUpdateObserver(void* owner, void* observer);
void removeUiUpdateObserver(void* owner, void* observer);
void setProjectionSizeListener(void* owner, void* observer);
void setUiStyleListener(void* owner, void* observer);
}
#endif
