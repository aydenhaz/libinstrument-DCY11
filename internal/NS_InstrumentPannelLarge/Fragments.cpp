// Legacy factory bodies quarantined: they allocated empty placeholder types,
// cast them to host CGFragment objects, and in A106 wrote beyond the allocation.
// Do not provide no-op/null factories: missing implementations must fail linking.
// Evidence and original text: analysis/FRAGMENT_FACTORY_QUARANTINE.md and rejected/.
#include "Fragments.h"
