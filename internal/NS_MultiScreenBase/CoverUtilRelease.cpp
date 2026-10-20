#include "CoverUtil.h"
#include <CAutoUtil.h>
#include <GFrame/CGImage.h>
#include <hsl/SystemNotify.h>
namespace NS_MultiScreenBase {
void CoverUtil::releaseConfiguredCover() {
    if(!image_ || !enabled_) return;
    screenshot_.bytes=nullptr;
    screenshot_.length=0;
    text_=dice::String16(static_cast<const unsigned short*>(nullptr));
    state_=0;
    unknown0c_=nullptr;
    unknown10_=nullptr;
    enabled_=0;
    for(const char16_t* name : {u"widgetCoverR",u"widgetCoverG",u"widgetCoverB"}) {
        const dice::String16 key(reinterpret_cast<const unsigned short*>(name));
        const dice::String16 value(reinterpret_cast<const unsigned short*>(u"127"));
        CAutoUtil::setSMConfigValue(key,value);
    }
    // External configuration callbacks may have changed the image or flags.
    if(!image_) return;
    hsl::HSL::GetSystemNotifyHandle()->RemoveMapSurfaceViewObserver(this);
    if(unknown14_) uninstallTopRenderCallback();
    delete static_cast<GNS_FRAME::CGImage*>(image_);
    image_=nullptr;
}
}
