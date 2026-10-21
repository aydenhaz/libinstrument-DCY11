#include <type_traits>
#include <GFrame/CGFragment.h>
#include "../internal/NS_InstrumentPannelLarge/Fragments.h"
static_assert(!std::is_default_constructible<GNS_FRAME::CGFragment>::value,"No empty host Fragment construction");
#define CHECK(T) static_assert(!std::is_default_constructible<NS_InstrumentPannelLarge::T>::value,"No placeholder concrete Fragment construction")
CHECK(CGInstrumentPannelLargeOpeningFragment);
CHECK(CGInstrumentPannelLargeGuideFragment);
CHECK(CGInstrumentPannelLargeCruiseFragment);
CHECK(CGInstrumentPannelLargeARGuideFragment);
CHECK(CGInstrumentPannelLargePreviewFragment);
CHECK(CGInstrumentPannelLargeGuideS311Fragment);
CHECK(CGInstrumentPannelLargeCruiseS311Fragment);
CHECK(CGInstrumentPannelLargeGuideGuangQiA106Fragment);
CHECK(CGInstrumentPannelLargeCruiseGuangQiA106Fragment);
CHECK(CGInstrumentPannelLargeGuideS311UnnormalFragment);
int main(){}
