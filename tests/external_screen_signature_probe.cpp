#include <hsl/MultiScreenService.h>
#include <hsl/GOTHERProtocolHmiToAl.h>
void setScreen(hsl::MultiScreenService* service){
 tagSTExtScreen screen{{11,22,33,44}};service->SetStartOpenScreenMode(5,screen);
}
tagSTExtScreen getScreen(hsl::MultiScreenService* service){return service->GetStartOpenScreenMode(5);}
void sendScreen(tagSTExtScreen* screen,tagSTExtScreenControl& control){
 hsl::GOTHERProtocolHmiToAl::ExScreenRenderActionHmiToAl(screen);
 hsl::GOTHERProtocolHmiToAl::MultiScreenStatusHmiToAl(control);
}
