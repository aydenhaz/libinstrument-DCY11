#include <hsl/MultiScreenService.h>
#include <cassert>
static hsl::MultiScreenService* expected;
int hsl::MultiScreenService::SetNaviType(ENaviMode_ navigation,ECruiseMode_ cruise){
 assert(this==expected&&static_cast<int>(navigation)==7&&static_cast<int>(cruise)==11);
 return 0;
}
int main(){hsl::MultiScreenService service;expected=&service;assert(service.SetNaviType(7,11)==0);}
