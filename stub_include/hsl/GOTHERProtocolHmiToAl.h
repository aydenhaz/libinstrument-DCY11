#pragma once
#include <ExternalScreenTypes.h>
namespace hsl {
class GOTHERProtocolHmiToAl {
public:
    static void ExScreenRenderActionHmiToAl(tagSTExtScreen*);
    static void MultiScreenStatusHmiToAl(tagSTExtScreenControl&);
};
}
