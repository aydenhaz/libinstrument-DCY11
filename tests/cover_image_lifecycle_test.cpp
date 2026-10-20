#include "../internal/NS_MultiScreenBase/CoverUtil.h"
#include <GFrame/CGImage.h>
#include <hsl/StringUtils.h>
#include <CAutoUtil.h>
#include <cassert>
#include <cstring>
#include <vector>
dice::String16 hsl::StringUtils::Format(const unsigned short*,...){assert(false);return dice::String16();}
void CAutoUtil::setSMConfigValue(const dice::String16&,const dice::String16&){assert(false);}
static std::vector<int> calls;
static bool success=true;
static unsigned char pixels[16];
CGString::CGString(){}
CGString::~CGString(){}
GNS_FRAME::CGImage::CGImage(){std::memset(static_cast<void*>(this),0,0x1e0);calls.push_back(1);}
GNS_FRAME::CGImage::~CGImage(){calls.push_back(3);}
bool GNS_FRAME::CGImage::create(const GPI::CGSize& size,EGColorType color,unsigned char* data){
 assert(size.w==640&&size.h==480&&static_cast<int>(color)==0x101&&!data);calls.push_back(2);return success;
}
uint32_t GNS_FRAME::CGImage::getDataLen()const{calls.push_back(4);return sizeof(pixels);}
unsigned char* GNS_FRAME::CGImage::getData()const{calls.push_back(5);return pixels;}
namespace NS_MultiScreenBase {
struct CoverUtilTestAccess {
 static void prepare(CoverUtil& c){c.enabled_=1;c.surface_.scalar[0]=640;c.surface_.scalar[1]=480;}
 static void* image(CoverUtil& c){return c.image_;}
 static bl::ScreenShotInfo& info(CoverUtil& c){return c.screenshot_;}
 static void cleanup(CoverUtil& c){delete static_cast<GNS_FRAME::CGImage*>(c.image_);c.image_=nullptr;}
};
}
// Guard mocks above fail if image stages unexpectedly invoke RGB config services.
int main(){
 using Access=NS_MultiScreenBase::CoverUtilTestAccess;
 // Raw construction of CoverUtil still uses its real constructor/destructor.
 NS_MultiScreenBase::CoverUtil c;
 assert(!c.recreateImageStorage()&&calls.empty());
 assert(!c.bindScreenshotSource(GPI::CGSize(2,2)));
 Access::prepare(c);
 assert(c.recreateImageStorage());assert((calls==std::vector<int>{1,2}));
 calls.clear();assert(c.bindScreenshotSource(GPI::CGSize(2,2)));
 assert((calls==std::vector<int>{4,5}));
 auto& info=Access::info(c);assert(info.unknown4==5&&info.stride==4&&info.width==2&&info.height==2&&info.length==16&&info.bytes==pixels);
 calls.clear();success=false;assert(!c.recreateImageStorage());
 assert((calls==std::vector<int>{3,1,2,3})&&!Access::image(c));
 assert(!c.bindScreenshotSource(GPI::CGSize(2,2)));
 calls.clear();success=true;assert(c.recreateImageStorage());Access::cleanup(c);
 assert((calls==std::vector<int>{1,2,3}));
}
