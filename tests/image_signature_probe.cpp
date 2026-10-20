#include <GFrame/CGImage.h>
bool createCoverImage(){
    GNS_FRAME::CGImage* image=new GNS_FRAME::CGImage;
    const GPI::CGSize size(640,480);
    const bool ok=image->create(size,EGColorType_RGBA,nullptr);
    delete image;
    return ok;
}
