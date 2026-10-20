#include <GFrame/CGAsyncObject.h>
void probe(GNS_FRAME::CGAsyncObject* object,GPI::Closure::Callable<void()>& callback){
    object->scheduleTaskHelper(callback,1000,0,-1,nullptr,0);
}
