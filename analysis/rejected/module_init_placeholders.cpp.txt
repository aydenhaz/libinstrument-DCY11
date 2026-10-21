/* CORRECTION: V21 has 5 init entries, including 0x14e41. The following 4-entry skeleton does not register the actual factories and is not replacement-ready. See BUILD_STATUS.md. */
/* The four constructors below are legacy placeholders and do not register
 * factories. Verified original init entry roles:
 * 0x14cf8, 0x14d30: guarded lock-wrapper construction through 0x14f10,
 *                    which calls asl::LockLinux::LockLinux().
 * 0x14d68: initialize a global polymorphic object and register its destructor.
 * 0x14d9c: construct four static objects (concrete type not yet recovered).
 * 0x14e40: ClassNode_CGPackage(name, factory, alias), factory=0x2eb09.
 * The old claim that the first four entries were Fragment factories was false.
 * Evidence: analysis/v21_verified/module_initializers.txt, module_lock_ctor.txt.
 */

#include "InstrumentPannelLarge.h"

extern "C" int __cxa_atexit(void (*fn)(void *), void *arg, void *dso);

extern "C" void *__dso_handle;

namespace {

struct StaticFrame { void *node; };

StaticFrame g_frames[4];

void dtor0(void *p) { (void)p; }
void dtor1(void *p) { (void)p; }
void dtor2(void *p) { (void)p; }
void dtor3(void *p) { (void)p; }

/* init_array[0] — original @0x14cf9 */
__attribute__((constructor(101))) static void IPL_Ctor0(void) {
    __cxa_atexit(dtor0, &g_frames[0], &__dso_handle);
}
/* init_array[1] — original @0x14d31 */
__attribute__((constructor(102))) static void IPL_Ctor1(void) {
    __cxa_atexit(dtor1, &g_frames[1], &__dso_handle);
}
/* init_array[2] — original @0x14d69 */
__attribute__((constructor(103))) static void IPL_Ctor2(void) {
    __cxa_atexit(dtor2, &g_frames[2], &__dso_handle);
}
/* init_array[3] — original @0x14d9d */
__attribute__((constructor(104))) static void IPL_Ctor3(void) {
    __cxa_atexit(dtor3, &g_frames[3], &__dso_handle);
}

} /* anonymous namespace */
