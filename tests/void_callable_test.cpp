#include "abi/VoidCallable.h"
#include <cassert>
#include <utility>
struct Target {int count=0;void run(){++count;}void alternate(){count+=10;}};
struct Left {virtual ~Left(){};int padding=1;};
struct Right {virtual ~Right(){};virtual void run()=0;};
struct Multiple:Left,Right {int count=0;void run() override {++count;}};
int main(){
 using IPL_ABI::VoidCallable;
 Target target,other;
 VoidCallable empty;assert(empty.empty());empty.invoke();assert(!empty.equals(empty));
 auto a=VoidCallable::bind(&target,&Target::run);
 auto same=VoidCallable::bind(&target,&Target::run);
 auto different=VoidCallable::bind(&other,&Target::run);
 auto alternate=VoidCallable::bind(&target,&Target::alternate);
 assert(!a.empty()&&a.equals(same)&&!a.equals(different)&&!a.equals(alternate));
 VoidCallable copy(a);assert(copy.equals(a));a.reset();assert(a.empty());copy.invoke();
 VoidCallable moved(std::move(copy));assert(copy.empty());moved.invoke();assert(target.count==2);
 moved=moved;moved.invoke();assert(target.count==3);
 moved=std::move(same);assert(same.empty());moved.invoke();assert(target.count==4);
 Multiple multi;auto virt=VoidCallable::bind(static_cast<Right*>(&multi),&Right::run);
 VoidCallable clone(virt);virt.reset();clone.invoke();assert(multi.count==1);
 clone.reset();clone.reset();assert(clone.empty());
}
