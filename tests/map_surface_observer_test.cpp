#include <hsl/MapSurfaceViewObserver.h>
#include <cassert>
#include <cstring>
struct Observed: hsl::MapSurfaceViewObserver {
    static int calls,deaths;
    void MapSurfaceViewStatusChange(int id,MAPSURFACE type,void* handle,int format,int width,int height) override {
        assert(id==31&&static_cast<int>(type)==1&&handle==this);
        assert(format==2&&width==800&&height==480);++calls;
    }
    ~Observed() override {++deaths;}
};
int Observed::calls=0;int Observed::deaths=0;
int main(){
 hsl::MapSurfaceViewObserver* observer=new Observed;
 void** table;std::memcpy(&table,static_cast<void*>(observer),sizeof(table));
 using Notify=void (*)(hsl::MapSurfaceViewObserver*,int,MAPSURFACE,void*,int,int,int);
 reinterpret_cast<Notify>(table[0])(observer,31,static_cast<MAPSURFACE>(1),observer,2,800,480);
 assert(Observed::calls==1);delete observer;assert(Observed::deaths==1);
}
