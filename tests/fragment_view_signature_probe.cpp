#include <GFrame/CGFragment.h>
GNS_FRAME::CGViewGroup* inflateView(GNS_FRAME::CGFragment* f,int id,GNS_FRAME::CGViewGroup* parent){ return f->inflater(id,parent); }
GNS_FRAME::CGView* lookupView(GNS_FRAME::CGFragment* f,int id){ return f->findViewById(id); }
bool isTop(GNS_FRAME::CGFragment* f){ return f->isTopFragment(); }
