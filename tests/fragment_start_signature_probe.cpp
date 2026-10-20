#include <GFrame/CGPackage.h>
#include <GFrame/CGIntent.h>
GNS_FRAME::CGFragment* findExisting(GNS_FRAME::CGFragment* parent,const GNS_FRAME::CGIntent& intent,void* identity){
 return parent->tryStartExistFragment(intent,2,identity);
}
void startChild(GNS_FRAME::CGFragment* parent,GNS_FRAME::CGFragment* child,void* identity,const GNS_FRAME::CGIntent& intent){
 parent->startNewFragment(child,identity,2,intent,-1);
}
