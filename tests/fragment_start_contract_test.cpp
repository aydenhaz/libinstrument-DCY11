#include <GFrame/CGPackage.h>
#include <GFrame/CGIntent.h>
#include <cassert>
static GNS_FRAME::CGFragment *parent,*child;
static const GNS_FRAME::CGIntent* expectedIntent;
static void* identity;
static int calls;
GNS_FRAME::CGIntent::CGIntent(){}
GNS_FRAME::CGIntent::~CGIntent(){}
GNS_FRAME::CGFragment* GNS_FRAME::CGFragment::tryStartExistFragment(const CGIntent& intent,int mode,void* token){
 assert(this==parent&&&intent==expectedIntent&&mode==2&&token==identity);++calls;return child;
}
void GNS_FRAME::CGFragment::startNewFragment(CGFragment* next,void* token,int mode,const CGIntent& intent,int request){
 assert(this==parent&&next==child&&this!=next&&token==identity);
 assert(mode==2&&&intent==expectedIntent&&request==-1);++calls;
}
int main(){
 GNS_FRAME::CGFragment p,c;GNS_FRAME::CGIntent intent;int token;
 parent=&p;child=&c;identity=&token;expectedIntent=&intent;
 assert(p.tryStartExistFragment(intent,2,identity)==child);
 p.startNewFragment(&c,identity,2,intent,-1);assert(calls==2);
}
