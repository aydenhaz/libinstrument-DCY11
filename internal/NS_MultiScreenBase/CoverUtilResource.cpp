#include "CoverUtil.h"
#include <GFrame/CGImage.h>
#include <hsl/UtilPath.h>
#include <hsl/FileUtils.h>
#include <string>
namespace NS_MultiScreenBase {
CoverResourceResult CoverUtil::loadConfiguredResourceImage() {
    dice::String16 root;
    hsl::UtilPath::GetPathRelativeToRes(
        dice::String16(reinterpret_cast<const unsigned short*>(u"HmiResPath")),root,
        static_cast<hsl::PathType_>(3));
    std::u16string joined(reinterpret_cast<const char16_t*>(root.c_str()),root.size());
    joined+=u"sm_cover.png"; // Original appends directly; no extra slash.
    const dice::String16 file(reinterpret_cast<const unsigned short*>(joined.c_str()));
    if(!hsl::FileUtils::IsExistFile(file)) {
        reinterpret_cast<unsigned char*>(&screenshot_.tail)[1]=1; // CoverUtil+0x91
        return CoverResourceResult::MissingFile;
    }
    inhibitCapture_=1;
    if(!image_) return CoverResourceResult::NoImage;
    const CGString path(file.c_str());
    state_=static_cast<GNS_FRAME::CGImage*>(image_)->loadImageFromFile(path,-1,false,false,nullptr);
    return state_?CoverResourceResult::Loaded:CoverResourceResult::LoadFailed;
}
}
