#pragma once
namespace GNS_FRAME { class CGFragment; }
namespace NS_InstrumentPannelLarge {

// concrete fragment classes derive from CGFragment (stub: use void* factory here)
#define IPL_FRAGMENT_HEAD(cls) \
class cls { \
public: \
    static GNS_FRAME::CGFragment *create(); \
    void destroy(); \
};

IPL_FRAGMENT_HEAD(CGInstrumentPannelLargeOpeningFragment)
IPL_FRAGMENT_HEAD(CGInstrumentPannelLargeGuideFragment)
IPL_FRAGMENT_HEAD(CGInstrumentPannelLargeCruiseFragment)
IPL_FRAGMENT_HEAD(CGInstrumentPannelLargeARGuideFragment)
IPL_FRAGMENT_HEAD(CGInstrumentPannelLargePreviewFragment)
IPL_FRAGMENT_HEAD(CGInstrumentPannelLargeGuideS311Fragment)
IPL_FRAGMENT_HEAD(CGInstrumentPannelLargeCruiseS311Fragment)
IPL_FRAGMENT_HEAD(CGInstrumentPannelLargeGuideGuangQiA106Fragment)
IPL_FRAGMENT_HEAD(CGInstrumentPannelLargeCruiseGuangQiA106Fragment)
IPL_FRAGMENT_HEAD(CGInstrumentPannelLargeGuideS311UnnormalFragment)

// shared base-ctor helpers (A106 delegation)
void BaseCtor(void*);
void BaseCtor2(void*);
void CruiseInit(void*);
void UnnormalWiring(void*);

}
