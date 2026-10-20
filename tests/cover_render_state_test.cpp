#include "../internal/abi/CoverRenderState.h"
#include <cassert>
#include <cstdint>
#include <climits>
int main() {
    using IPL_ABI::coverUiFirstRenderFinished;
    const int32_t values[] = {INT_MIN, -1, 0, 1, 2, 3, INT_MAX};
    for (int image=0;image<2;++image)
    for (int enabled=0;enabled<2;++enabled)
    for (int state=0;state<2;++state)
    for (int32_t count:values)
    for (int32_t threshold:values) {
        bool expected=true;
        // Follow original branch order: missing image / disabled / flag clear skip BLE.
        if (image && enabled && state && count<=threshold) expected=false;
        assert(coverUiFirstRenderFinished(image,enabled,state,count,threshold)==expected);
    }
    for(int same=0;same<2;++same)
    for(int event=-1;event<=2;++event)
    for(int image=0;image<2;++image)
    for(int enabled=0;enabled<2;++enabled) {
        int32_t count=9;
        IPL_ABI::coverSurfaceStatusChanged(7,same?7:8,event,image,enabled,count);
        assert(count==((same && event==1 && image && enabled)?0:9));
    }
    assert(!coverUiFirstRenderFinished(true,true,true,2,2));
    assert(coverUiFirstRenderFinished(true,true,true,3,2));
    assert(!coverUiFirstRenderFinished(true,true,true,-1,2));
}
