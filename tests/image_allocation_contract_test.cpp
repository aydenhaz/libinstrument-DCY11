#include <GFrame/CGImage.h>
#include <cassert>
#include <cstring>
static int constructions,destructions;
GNS_FRAME::CGImage::CGImage(){
    // A host ctor can write across its entire verified allocation. A one-byte
    // placeholder allocation would fail this contract under ASan.
    std::memset(static_cast<void*>(this),0xa5,0x1e0);++constructions;
}
GNS_FRAME::CGImage::~CGImage(){++destructions;}
bool GNS_FRAME::CGImage::create(const GPI::CGSize& size,EGColorType type,unsigned char* bytes){
    assert(size.w==640&&size.h==480&&static_cast<int>(type)==0x101);
    return bytes!=nullptr;
}
int main(){
    auto* image=new GNS_FRAME::CGImage;
    const GPI::CGSize size(640,480);
    assert(!image->create(size,EGColorType_RGBA,nullptr));
    unsigned char pixel=0;
    assert(image->create(size,EGColorType_RGBA,&pixel));
    delete image;assert(constructions==1&&destructions==1);
}
