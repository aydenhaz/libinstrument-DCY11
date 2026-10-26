#pragma once
#if defined(__arm__) && !defined(__aarch64__)
namespace IPL_ABI {
// Requires a fully constructed V21 Cruise Fragment, including observer vptrs.
void beginCruiseView(void* fragment, void* parent);
void attachCruiseGuide(void* fragment);
void attachCruiseViewTail(void* fragment);
// Host-observer subset: caller removes broadcasts first; UI cleanup pending.
void detachCruiseView(void* fragment);
}
#endif
