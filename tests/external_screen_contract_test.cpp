#include <hsl/MultiScreenService.h>
#include <hsl/GOTHERProtocolHmiToAl.h>
#include <cassert>
static hsl::MultiScreenService* expected;
static tagSTExtScreen* expectedScreen;
static tagSTExtScreenControl* expectedControl;
tagSTExtScreen hsl::MultiScreenService::GetStartOpenScreenMode(int key){
 assert(this==expected&&key==5);return tagSTExtScreen{{11,22,33,44}};
}
void hsl::MultiScreenService::SetStartOpenScreenMode(int key,tagSTExtScreen value){
 assert(this==expected&&key==5);
 for(int i=0;i<4;++i)assert(value.words[i]==11*(i+1));
 value.words[0]=-1; // a by-value mutation must not change the caller
}
void hsl::GOTHERProtocolHmiToAl::ExScreenRenderActionHmiToAl(tagSTExtScreen* value){assert(value==expectedScreen);}
void hsl::GOTHERProtocolHmiToAl::MultiScreenStatusHmiToAl(tagSTExtScreenControl& value){
 assert(&value==expectedControl);assert(value.words[0]==0);
 for(int i=1;i<9;++i)assert(value.words[i]==99);
 value.words[8]=17;
}
int main(){
 hsl::MultiScreenService service;expected=&service;
 auto screen=service.GetStartOpenScreenMode(5);service.SetStartOpenScreenMode(5,screen);assert(screen.words[0]==11);
 expectedScreen=&screen;hsl::GOTHERProtocolHmiToAl::ExScreenRenderActionHmiToAl(&screen);
 tagSTExtScreenControl control;expectedControl=&control;
 hsl::GOTHERProtocolHmiToAl::MultiScreenStatusHmiToAl(control);assert(control.words[8]==17);
}
