#include "../internal/registration/PackageRegistration.h"
#include <cassert>
#include <map>
#include <string>
#include <vector>
#include <stdexcept>
static std::map<const CGString*,std::u16string> strings;
static std::vector<std::string> effects;
static GNS_FRAME::PackageFactory expected;
CGString::CGString(){strings[this]=u"";}
CGString::CGString(const unsigned short* s){strings[this]=std::u16string(reinterpret_cast<const char16_t*>(s));effects.push_back("string");}
CGString::~CGString(){assert(strings.erase(this)==1);effects.push_back("destroy-string");}
namespace GNS_FRAME {
ClassNode_CGPackage::ClassNode_CGPackage(const CGString& name,PackageFactory factory,const CGString& alias):factory_(factory),next_(nullptr){
 assert(strings.at(&name)==u"CGIPLPacket");assert(strings.at(&alias)==u"com.autonavi.package.GIPLPacket");assert(factory==expected);effects.push_back("register");
}
ClassNode_CGPackage::~ClassNode_CGPackage(){effects.push_back("unregister");}
}
static GNS_FRAME::CGPackage* factory(GNS_FRAME::CGContext*){assert(false);return nullptr;}
int main(){
 bool rejected=false;try{IPL_ABI::createPackageRegistration(nullptr);}catch(const std::invalid_argument&){rejected=true;}
 assert(rejected&&effects.empty());expected=&factory;
 auto node=IPL_ABI::createPackageRegistration(expected);
 assert((effects==std::vector<std::string>{"string","string","register","destroy-string","destroy-string"}));
 assert(strings.size()==1);node.reset();assert(strings.empty());
 assert(effects[5]=="unregister"&&effects[6]=="destroy-string");
}
