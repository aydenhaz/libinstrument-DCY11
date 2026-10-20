#include "CoverUtil.h"
#include <GFrame/CGImage.h>
namespace NS_MultiScreenBase {
bool CoverUtil::recreateImageStorage() {
    if(!enabled_) return false; // Stage precondition, not a complete setConfig.
    delete static_cast<GNS_FRAME::CGImage*>(image_);
    image_=nullptr;
    auto* image=new GNS_FRAME::CGImage;
    image_=image;
    const GPI::CGSize size(static_cast<int32_t>(surface_.scalar[0]),static_cast<int32_t>(surface_.scalar[1]));
    if(!image->create(size,EGColorType_RGBA,nullptr)) {
        delete image;image_=nullptr;return false;
    }
    return true;
}
bool CoverUtil::bindScreenshotSource(const GPI::CGSize& finalSize) {
    if(!image_) return false;
    auto* image=static_cast<GNS_FRAME::CGImage*>(image_);
    screenshot_.unknown4=5;
    screenshot_.stride=4;
    screenshot_.width=finalSize.w;screenshot_.height=finalSize.h;
    screenshot_.length=image->getDataLen();
    screenshot_.bytes=image->getData();
    return true;
}
}
