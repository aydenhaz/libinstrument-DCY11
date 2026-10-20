#pragma once
// The host enum's values are not redefined here. V21 uses a 32-bit enum.
enum MAPSURFACE_ : int;
typedef MAPSURFACE_ MAPSURFACE;
namespace hsl {
// V21 CoverUtil vtable confirms callback slot 0, destructor slots 1 and 2.
class MapSurfaceViewObserver {
public:
    virtual void MapSurfaceViewStatusChange(int surfaceId,MAPSURFACE type,
        void* handle,int format,int width,int height)=0;
    virtual ~MapSurfaceViewObserver() = default;
};
#if defined(__arm__) && !defined(__aarch64__)
static_assert(sizeof(MapSurfaceViewObserver)==4,"V21 observer interface extent");
#endif
}
