#include "abi/MemberCallableWrapper.h"
#include <cassert>
#include <memory>
struct Target {int count=0;void run(){++count;}};
struct Left {virtual ~Left(){};int padding=7;};
struct Right {virtual ~Right(){};virtual void run()=0;};
struct Derived:Left,Right {int count=0;void run() override {++count;}};
int main(){
 Target target;
 IPL_ABI::MemberCallableWrapper<Target> wrapper(&target,&Target::run);
 wrapper.invoke();std::unique_ptr<IPL_ABI::CallableWrapperBase> copy(wrapper.clone());copy->invoke();assert(target.count==2);
 Derived derived;Right* secondary=&derived;
 assert(static_cast<void*>(secondary)!=static_cast<void*>(&derived));
 IPL_ABI::MemberCallableWrapper<Right> virt(secondary,&Right::run);virt.invoke();
 copy.reset(virt.clone());copy->invoke();assert(derived.count==2);
}
