#include "../internal/abi/CoverCaptureResult.h"
#include "../internal/NS_MultiScreenBase/CoverUtil.h"
#include <cassert>
#include <cstdarg>
#include <string>
#include <vector>
#include <hsl/HSL.h>
#include <hsl/Config.h>
#include <asl/String16Utils.h>
#include <map>
#include <cstdlib>
#include <GFrame/CGImage.h>
#include <hsl/UtilPath.h>
#include <hsl/FileUtils.h>
#include <hsl/SystemNotify.h>
#include <GFrame/CGWorkStation.h>
static int stringCopies;
CGString::CGString() {}
CGString::CGString(const unsigned short*) {}
CGString::~CGString() {}
CGString::CGString(const CGString&) {++stringCopies;}
CGString& CGString::operator=(const CGString&){return *this;}
namespace NS_MultiScreenBase {
struct CoverUtilTestAccess {
 static void prepare(CoverUtil& c,uint8_t* data){c.screenshot_.bytes=data;c.screenshot_.width=2;c.screenshot_.height=1;c.screenshot_.length=8;c.screenshot_.stride=4;c.image_=&c;c.enabled_=1;c.requested_=1;c.state_=0;c.counter_=9;c.surface_.scalar[7]=7;}
 static void pending(CoverUtil& c){c.requested_=1;c.counter_=23;c.inhibitCapture_=1;}
 static bool retained(const CoverUtil& c){return c.requested_==1&&c.counter_==23&&c.inhibitCapture_==1&&c.screenshot_.width==2&&c.screenshot_.height==2;}
 static void disable(CoverUtil& c){c.enabled_=0;}
 static void enable(CoverUtil& c){c.enabled_=1;}
 static bool defaults(const CoverUtil& c){return c.threshold_==2&&!c.image_&&!c.enabled_&&!c.requested_&&!c.state_&&!c.counter_&&c.unknown94_==1&&c.surface_.scalar[7]==0xffffffffu;}
 static bool requested(const CoverUtil& c){return c.requested_;}
 static bool ready(const CoverUtil& c){return c.state_;}
 static int count(const CoverUtil& c){return c.counter_;}
 static int threshold(const CoverUtil& c){return c.threshold_;}
 static bool enabled(const CoverUtil& c){return c.enabled_;}
 static GNS_FRAME::CGContext* context(const CoverUtil& c){return c.context_;}
 static bl::ScreenShotInfo& info(CoverUtil& c){return c.screenshot_;}
 static void optionalRender(CoverUtil& c){c.unknown14_=1;}
 static void cleanup(CoverUtil& c){delete static_cast<GNS_FRAME::CGImage*>(c.image_);c.image_=nullptr;}
 static int surface(const CoverUtil& c){return static_cast<int>(c.surface_.scalar[7]);}
};
}
static std::vector<std::string> calls;
static std::string text(const dice::String16& value){
    std::string out;
    for(unsigned i=0;i<value.size();++i)out+=char(value.c_str()[i]);
    return out;
}
dice::String16 hsl::StringUtils::Format(const unsigned short* format,...){
    assert(format[0]=='%'&&format[1]=='d'&&format[2]==0);
    va_list args;va_start(args,format);int value=va_arg(args,int);va_end(args);
    calls.push_back("format:"+std::to_string(value));
    return dice::String16(std::to_string(value).c_str());
}
void CAutoUtil::setSMConfigValue(const dice::String16& key,const dice::String16& value){
    calls.push_back(text(key)+"="+text(value));
}
static std::map<std::string,std::string> configuration;
static int configLookups;
hsl::Config* hsl::HSL::GetConfigHandle(){static hsl::Config config;++configLookups;return &config;}
bool hsl::Config::GetString(const dice::String16& key,dice::String16& value,VALUE_STRATEGY strategy){
 assert(strategy==VALUE_STRATEGY_DEFAULT);
 auto found=configuration.find(text(key));
 if(found==configuration.end())return false;
 value=dice::String16(found->second.c_str());return true;
}
int asl::String16Utils::atoi(const unsigned short* value){std::string s;while(*value)s+=char(*value++);return std::atoi(s.c_str());}
int hsl::StringUtils::FindStr(const dice::String16& value,const dice::String16& needle,unsigned start){
 auto pos=text(value).find(text(needle),start);return pos==std::string::npos?-1:static_cast<int>(pos);
}
static bool imageCreated=true,filePresent=false,fileLoaded=false;
static int registrations;
static int removals;
static bool requestTopDuringRegistration=false;
static void (*registeredTop)(GNS_FRAME::CGCanvas*);
static int topRegistrations;
GNS_FRAME::CGWorkStation* GNS_FRAME::CGWorkStation::ownWorkStation(){static CGWorkStation station;return &station;}
void GNS_FRAME::CGWorkStation::setTopRenderCallback(void(*callback)(CGCanvas*)){registeredTop=callback;++topRegistrations;}
static unsigned char boundPixels[16];
static NS_MultiScreenBase::CoverUtil* initializing;
GNS_FRAME::CGImage::CGImage(){}
GNS_FRAME::CGImage::~CGImage(){}
bool GNS_FRAME::CGImage::create(const GPI::CGSize&,EGColorType color,unsigned char* data){assert(static_cast<int>(color)==0x101&&!data);return imageCreated;}
bool GNS_FRAME::CGImage::loadImageFromFile(const CGString&,int mode,bool a,bool b,CGString* out){assert(mode==-1&&!a&&!b&&!out);return fileLoaded;}
uint32_t GNS_FRAME::CGImage::getDataLen()const{return sizeof(boundPixels);}
unsigned char* GNS_FRAME::CGImage::getData()const{return boundPixels;}
void hsl::UtilPath::GetPathRelativeToRes(const dice::String16& key,dice::String16& out,PathType_ type){assert(text(key)=="HmiResPath"&&int(type)==3);out=dice::String16("/res/");}
bool hsl::FileUtils::IsExistFile(const dice::String16& path){assert(text(path)=="/res/sm_cover.png");return filePresent;}
hsl::SystemNotify* hsl::HSL::GetSystemNotifyHandle(){static hsl::SystemNotify notify;return &notify;}
void hsl::SystemNotify::AddMapSurfaceViewObserver(MapSurfaceViewObserver* observer){
 assert(observer==static_cast<MapSurfaceViewObserver*>(initializing));
 auto& info=NS_MultiScreenBase::CoverUtilTestAccess::info(*initializing);
 assert(info.bytes==boundPixels&&info.length==sizeof(boundPixels)&&info.stride==4);
 assert(info.width==2&&info.height==2);++registrations;
 if(requestTopDuringRegistration)NS_MultiScreenBase::CoverUtilTestAccess::optionalRender(*initializing);
 // Registration is permitted to synchronously notify the actual observer.
 observer->MapSurfaceViewStatusChange(0,static_cast<MAPSURFACE>(1),nullptr,0,0,0);
}
void hsl::SystemNotify::RemoveMapSurfaceViewObserver(MapSurfaceViewObserver* observer){
 assert(observer==static_cast<MapSurfaceViewObserver*>(initializing));
 assert(!NS_MultiScreenBase::CoverUtilTestAccess::enabled(*initializing));
 calls.push_back("remove");++removals;
}
int main(){
    IPL_ABI::writeCoverRgbConfiguration({0,127,255});
    const std::vector<std::string> expected={"format:0","format:127","format:255","widgetCoverR=0","widgetCoverG=127","widgetCoverB=255"};
    assert(calls==expected);
    calls.clear();
    const uint8_t image[]={0,127,255,33,0,127,255,44};
    bool requested=true,ready=false;
    assert(IPL_ABI::applyCoverCaptureResult(22,image,8,2,1,4,requested,ready)==IPL_ABI::CoverCaptureApply::IgnoredEvent);
    assert(calls.empty() && requested && !ready);
    assert(IPL_ABI::applyCoverCaptureResult(23,image,8,2,1,4,requested,ready)==IPL_ABI::CoverCaptureApply::Applied);
    assert(!requested && ready && calls==expected);
    calls.clear();requested=true;ready=false;
    assert(IPL_ABI::applyCoverCaptureResult(23,nullptr,8,2,1,4,requested,ready)==IPL_ABI::CoverCaptureApply::InvalidImage);
    assert(calls.empty() && !requested && ready);
    NS_MultiScreenBase::CoverUtil cover;
    using Access=NS_MultiScreenBase::CoverUtilTestAccess;
    assert(Access::defaults(cover));
    uint8_t pixels[]={0,127,255,0,0,127,255,255};
    Access::prepare(cover,pixels);
    hsl::MapSurfaceViewObserver* map=&cover;
    bl::IBLMapBusinessDataObserver* business=&cover;
    assert(reinterpret_cast<char*>(business)-reinterpret_cast<char*>(&cover)==sizeof(void*));
    map->MapSurfaceViewStatusChange(8,static_cast<MAPSURFACE>(1),nullptr,0,0,0);
    assert(Access::count(cover)==9);
    map->MapSurfaceViewStatusChange(7,static_cast<MAPSURFACE>(1),nullptr,0,0,0);
    assert(Access::count(cover)==0);
    calls.clear();
    assert(business->onBusinessDataObserver(23,nullptr,nullptr));
    assert(!Access::requested(cover)&&Access::ready(cover)&&calls==expected);
    assert(!cover.isUIFirstRenderFinish());
    using Stage=NS_MultiScreenBase::CoverConfigurationStage;
    NS_MultiScreenBase::SurfaceConfig config;
    config.scalar[7]=0;
    auto* context=reinterpret_cast<GNS_FRAME::CGContext*>(&config);
    auto* device=reinterpret_cast<bl::IMapDevice*>(&config);
    auto* view=reinterpret_cast<bl::IMapView*>(&config);
    const int before=configLookups;
    assert(cover.prepareConfiguration(nullptr,device,view,config)==Stage::InvalidInputs);
    assert(configLookups==before&&Access::context(cover)==nullptr);
    configuration.clear();
    assert(cover.prepareConfiguration(context,device,view,config)==Stage::NeedsImageSetup);
    assert(Access::enabled(cover)&&Access::threshold(cover)==2&&Access::context(cover)==context);
    config.scalar[7]=1;
    assert(cover.prepareConfiguration(context,device,view,config)==Stage::Disabled);
    configuration["sm_enable_widgetCoverForWndId"]="10";
    configuration["sm_support_widgetCoverMaxFrame"]="-1";
    assert(cover.prepareConfiguration(context,device,view,config)==Stage::NeedsImageSetup);
    assert(Access::enabled(cover)&&Access::threshold(cover)==-1&&Access::surface(cover)==1);
    using Init=NS_MultiScreenBase::CoverInitializationResult;
    NS_MultiScreenBase::CoverUtil initialized;initializing=&initialized;
    config.scalar[0]=2;config.scalar[1]=2;config.scalar[7]=0;configuration.clear();
    assert(initialized.initializeConfiguredCover(nullptr,device,view,config)==Init::InvalidInputs&&registrations==0);
    config.scalar[7]=1;
    assert(initialized.initializeConfiguredCover(context,device,view,config)==Init::Disabled&&registrations==0);
    config.scalar[7]=0;imageCreated=false;
    assert(initialized.initializeConfiguredCover(context,device,view,config)==Init::ImageCreateFailed&&registrations==0);
    imageCreated=true;filePresent=false;
    assert(initialized.initializeConfiguredCover(context,device,view,config)==Init::MapObserverRegistered&&registrations==1);
    filePresent=true;fileLoaded=false;
    assert(initialized.initializeConfiguredCover(context,device,view,config)==Init::MapObserverRegistered&&registrations==2);
    requestTopDuringRegistration=true;
    assert(initialized.initializeConfiguredCover(context,device,view,config)==Init::TopRenderRegistered&&registrations==3&&topRegistrations==1&&registeredTop);
    const int copiesBefore=stringCopies;
    initialized.setConfig(context,device,view,config);
    assert(stringCopies==copiesBefore+1);
    assert(registrations==4&&topRegistrations==2);
    calls.clear();
    auto oldDispatch=registeredTop;
    Access::pending(initialized);
    initialized.releaseConfiguredCover();
    assert((calls==std::vector<std::string>{"widgetCoverR=127","widgetCoverG=127","widgetCoverB=127","remove"}));
    assert(removals==1&&topRegistrations==3&&!registeredTop);
    assert(!Access::info(initialized).bytes&&!Access::info(initialized).length);
    assert(!Access::enabled(initialized)&&!Access::ready(initialized));
    assert(Access::context(initialized)==context&&Access::retained(initialized));
    oldDispatch(nullptr); // Private target cleared as well as host callback.
    calls.clear();initialized.releaseConfiguredCover();assert(calls.empty()&&removals==1);
    NS_MultiScreenBase::CoverUtil empty;
    empty.releaseConfiguredCover();assert(calls.empty());
    requestTopDuringRegistration=false;initializing=&empty;
    empty.setConfig(context,device,view,config);
    calls.clear();Access::disable(empty);empty.releaseConfiguredCover();
    assert(calls.empty()&&Access::info(empty).bytes==boundPixels&&removals==1);
    Access::enable(empty);empty.releaseConfiguredCover();
    assert(removals==2&&topRegistrations==3); // Optional top callback untouched.

}
