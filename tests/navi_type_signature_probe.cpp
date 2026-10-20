#include <hsl/MultiScreenService.h>
int probe(hsl::MultiScreenService* service,int navigation,int cruise){
 return service->SetNaviType(navigation,cruise);
}
