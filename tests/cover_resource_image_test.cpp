#include "../internal/NS_MultiScreenBase/CoverUtil.h"
#include <GFrame/CGImage.h>
#include <hsl/UtilPath.h>
#include <hsl/FileUtils.h>
#include <hsl/StringUtils.h>
#include <CAutoUtil.h>
#include <cassert>
#include <string>
#include <vector>
static std::vector<int> calls;
static bool exists=false,loaded=false;
static std::string path;
static std::string text(const unsigned short* p){std::string s;while(*p)s+=char(*p++);return s;}
CGString::CGString(){}
CGString::CGString(const unsigned short* p){path=text(p);calls.push_back(3);}
CGString::~CGString(){if(!path.empty())calls.push_back(5);}
GNS_FRAME::CGImage::CGImage(){}
GNS_FRAME::CGImage::~CGImage(){}
dice::String16 hsl::StringUtils::Format(const unsigned short*,...){assert(false);return dice::String16();}
void CAutoUtil::setSMConfigValue(const dice::String16&,const dice::String16&){assert(false);}
void hsl::UtilPath::GetPathRelativeToRes(const dice::String16& key,dice::String16& out,PathType_ type){
 assert(text(key.c_str())=="HmiResPath"&&static_cast<int>(type)==3);calls.push_back(1);out=dice::String16("/res/");
}
bool hsl::FileUtils::IsExistFile(const dice::String16& file){assert(text(file.c_str())=="/res/sm_cover.png");calls.push_back(2);return exists;}
bool GNS_FRAME::CGImage::loadImageFromFile(const CGString&,int mode,bool first,bool second,CGString* out){
 assert(path=="/res/sm_cover.png"&&mode==-1&&!first&&!second&&!out);calls.push_back(4);return loaded;
}
namespace NS_MultiScreenBase {struct CoverUtilTestAccess {
 static void image(CoverUtil& c,void* p){c.image_=p;}
 static bool inhibit(CoverUtil& c){return c.inhibitCapture_;}
 static bool state(CoverUtil& c){return c.state_;}
 static unsigned marker(CoverUtil& c){return reinterpret_cast<unsigned char*>(&c.screenshot_.tail)[1];}
};}
int main(){
 using Cover=NS_MultiScreenBase::CoverUtil;using Access=NS_MultiScreenBase::CoverUtilTestAccess;using Result=NS_MultiScreenBase::CoverResourceResult;
 Cover cover;GNS_FRAME::CGImage image;Access::image(cover,&image);
 assert(cover.loadConfiguredResourceImage()==Result::MissingFile);
 assert((calls==std::vector<int>{1,2})&&Access::marker(cover)==1&&!Access::inhibit(cover)&&!Access::state(cover));
 calls.clear();exists=true;loaded=false;
 assert(cover.loadConfiguredResourceImage()==Result::LoadFailed);
 assert((calls==std::vector<int>{1,2,3,4,5})&&Access::inhibit(cover)&&!Access::state(cover));
 calls.clear();loaded=true;
 assert(cover.loadConfiguredResourceImage()==Result::Loaded);
 assert((calls==std::vector<int>{1,2,3,4,5})&&Access::state(cover));
 calls.clear();exists=false;
 assert(cover.loadConfiguredResourceImage()==Result::MissingFile);
 assert(Access::inhibit(cover)&&Access::state(cover));
 calls.clear();exists=true;Access::image(cover,nullptr);
 assert(cover.loadConfiguredResourceImage()==Result::NoImage);
 assert((calls==std::vector<int>{1,2})&&Access::inhibit(cover));
 path.clear(); // Other CGString instances belong to the fixture, not file paths.
}
