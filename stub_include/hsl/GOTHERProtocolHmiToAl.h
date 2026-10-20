#pragma once
namespace NS_MultiScreenBase { struct tagSTExtScreen; }
namespace hsl {
class GOTHERProtocolHmiToAl {
public:
    static void ExScreenRenderActionHmiToAl(NS_MultiScreenBase::tagSTExtScreen*);
    static void MultiScreenStatusHmiToAl(const NS_MultiScreenBase::tagSTExtScreen&);
};
}
