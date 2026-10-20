#include <hsl/SystemNotify.h>
void probe(hsl::SystemNotify* notifier,hsl::MapSurfaceViewObserver* observer){
 notifier->AddMapSurfaceViewObserver(observer);
 notifier->RemoveMapSurfaceViewObserver(observer);
}
