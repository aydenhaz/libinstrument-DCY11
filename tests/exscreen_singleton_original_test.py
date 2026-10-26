"""Execute V21's singleton getter to pin selector and publication semantics.

The constructor and C++ runtime are intercepted. This verifies the getter,
not construction, callbacks, thread safety, or a rebuilt implementation.
"""
from pathlib import Path
import struct
import sys
from unicorn import Uc, UC_ARCH_ARM, UC_MODE_THUMB, UC_HOOK_CODE
from unicorn.arm_const import UC_ARM_REG_R0, UC_ARM_REG_R1, UC_ARM_REG_R2
from unicorn.arm_const import UC_ARM_REG_PC, UC_ARM_REG_LR, UC_ARM_REG_SP

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / 'tools'))
from inspect_packet_factory import ELF32, plt_symbol


def run(selectors):
    elf = ELF32(ROOT / 'reference/v21/libInstrumentPannelLarge.so')
    machine = Uc(UC_ARCH_ARM, UC_MODE_THUMB)
    machine.mem_map(0, 0x400000)
    for kind, offset, address, _, size, *_ in elf.segments:
        if kind == 1:
            machine.mem_write(address, elf.data[offset:offset + size])
    events, pending, complete, constructed, registrations = [], set(), set(), {}, {}
    handlers = {
        0x13f68: '__cxa_guard_acquire',
        0x13f74: '__cxa_guard_release',
        0x139ec: '__cxa_atexit',
        0x29798: 'construct',
    }
    for address, name in handlers.items():
        if name != 'construct':
            assert plt_symbol(elf, address)[1] == name

    def hook(uc, address, size, unused):
        name = handlers.get(address)
        if name is None:
            return
        a, b, c = (uc.reg_read(reg) for reg in
                   (UC_ARM_REG_R0, UC_ARM_REG_R1, UC_ARM_REG_R2))
        events.append((name, a, b, c))
        result = 0
        if name == '__cxa_guard_acquire':
            assert a not in pending and a not in complete
            pending.add(a)
            result = 1
        elif name == 'construct':
            assert len(pending) == 1 and a not in constructed
            constructed[a] = b
            result = a
        elif name == '__cxa_atexit':
            assert b in constructed and b not in registrations and len(pending) == 1
            assert a & 1  # Thumb destructor, registered before guard release.
            registrations[b] = (a, c)
        else:
            assert a in pending and len(registrations) == len(constructed)
            pending.remove(a)
            complete.add(a)
            machine.mem_write(a, b'\x01')
        uc.reg_write(UC_ARM_REG_R0, result)
        uc.reg_write(UC_ARM_REG_PC, uc.reg_read(UC_ARM_REG_LR))

    machine.hook_add(UC_HOOK_CODE, hook)
    slots, first = {}, {}
    for selector in selectors:
        key = selector if selector in (1, 2) else 'fallback'
        before = len(events)
        machine.reg_write(UC_ARM_REG_R0, selector & 0xffffffff)
        machine.reg_write(UC_ARM_REG_SP, 0x180000)
        machine.reg_write(UC_ARM_REG_LR, 0x1f0001)
        machine.emu_start(0x296a1, 0x1f0000, count=2000)
        assert machine.reg_read(UC_ARM_REG_PC) == 0x1f0000
        result = machine.reg_read(UC_ARM_REG_R0)
        if key in slots:
            assert result == slots[key] and len(events) == before
        else:
            assert result not in slots.values()
            slots[key] = result
            first[key] = selector & 0xffffffff
            assert [event[0] for event in events[before:]] == [
                '__cxa_guard_acquire', 'construct', '__cxa_atexit', '__cxa_guard_release']
        assert constructed[result] == first[key]
    assert not pending and len(complete) == len(slots)
    # All slots use the same destructor and DSO identity.
    assert len(set(registrations.values())) == 1
    assert len(constructed) == len(slots)


if __name__ == '__main__':
    cases = [(1, 2, 0, 1, 2, 3, -1, 0),
             (7, 0, 2, -1, 1, 7, 2, 1),
             (-1, 1, 2, 0, 0x7fffffff, -1)]
    for case in cases:
        run(case)
    print('V21 singleton: 3 selector orders, fallback first-call argument, '
          'stable identities and construct/atexit/release order: PASS')


def check_notification_registration(available):
    elf = ELF32(ROOT / 'reference/v21/libInstrumentPannelLarge.so')
    machine = Uc(UC_ARCH_ARM, UC_MODE_THUMB)
    machine.mem_map(0, 0x400000)
    for kind, offset, address, _, size, *_ in elf.segments:
        if kind == 1:
            machine.mem_write(address, elf.data[offset:offset + size])
    target = (0x456b4 + elf.u32(0x456b0)) & 0xffffffff
    assert plt_symbol(elf, target)[1] == (
        '_ZN3hsl12SystemNotify25AddExscreenNotifyObserverEPNS_22ExscreenNotifyObserverE')
    assert plt_symbol(elf, 0x13bfc)[1] == '_ZN3hsl3HSL21GetSystemNotifyHandleEv'
    observer, service = 0x90000, 0x91000
    calls = []

    def hook(uc, address, size, unused):
        if address == 0x13bfc:
            calls.append('lookup')
            result = service if available else 0
        elif address == 0x456a4:
            assert uc.reg_read(UC_ARM_REG_R0) == service
            assert uc.reg_read(UC_ARM_REG_R1) == observer
            calls.append('register')
            result = 0
        elif address == 0x13c14:
            calls.append('logger')
            result = 0  # Exercise the absent logger branch only.
        else:
            return
        uc.reg_write(UC_ARM_REG_R0, result)
        uc.reg_write(UC_ARM_REG_PC, uc.reg_read(UC_ARM_REG_LR))

    machine.hook_add(UC_HOOK_CODE, hook)
    machine.reg_write(UC_ARM_REG_R0, observer)
    machine.reg_write(UC_ARM_REG_SP, 0x180000)
    machine.reg_write(UC_ARM_REG_LR, 0x1f0001)
    machine.emu_start(0x2d265, 0x1f0000, count=2000)
    assert machine.reg_read(UC_ARM_REG_PC) == 0x1f0000
    assert calls == ['lookup', 'register' if available else 'logger']


if __name__ == '__main__':
    check_notification_registration(True)
    check_notification_registration(False)
    print('V21 constructor notification tail: service present/absent: PASS')
