#include <GFrame/CGPackage.h>
GNS_FRAME::CGContext* packageContext(GNS_FRAME::CGPackage* package){ return package->getContext(); }
GNS_FRAME::CGFragmentActivity* packageActivity(GNS_FRAME::CGPackage* package){ return package->getHostFragmentActivity(); }

#include <GFrame/CGFrameSimulation.h>
#include <type_traits>
static_assert(std::is_member_function_pointer<decltype(&GNS_FRAME::CGFrameSimulation::getFragmentActivity)>::value,"activity lookup requires simulation receiver");
GNS_FRAME::CGFragmentActivity* simulationActivity(GNS_FRAME::CGFrameSimulation* simulation){return simulation->getFragmentActivity();}
