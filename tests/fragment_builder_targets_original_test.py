"""Pin post-load factory/type/deletion targets to the original V21 ELF."""
from pathlib import Path
import sys
root = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(root / 'tools'))
from inspect_packet_factory import ELF32
elf = ELF32(root / 'reference/v21/libInstrumentPannelLarge.so')
# Literal-based member addresses bound at the original call sites.
for pc, pool, target in (
    (0x2f6aa, 0x2f718, 0x305ad), (0x2f776, 0x2f7d8, 0x305c1),
    (0x2f89c, 0x2fbe0, 0x302f1), (0x2f93c, 0x2fbf0, 0x30309),
    (0x2fa42, 0x2fc0c, 0x2fcc5), (0x2fae2, 0x2fc1c, 0x2fdd9),
    (0x2f83c, 0x2fbd4, 0x2fde5), (0x2f9e6, 0x2fc00, 0x2fde5),
    (0x2fb8c, 0x2fc2c, 0x2fde5),
):
    assert pc + elf.u32(pool) == target, (hex(pc), hex(pool))
# Each get-type body returns a unique, fixed Thumb marker; no allocation.
markers = []
for entry, marker in ((0x305c0, 0x30711), (0x30308, 0x30459), (0x2fdd8, 0x30111)):
    assert elf.at(entry, 8).hex() == '01487844704700bf'
    assert entry + 6 + elf.u32(entry + 8) == marker
    assert elf.at(marker & ~1, 2).hex() == '7047'
    markers.append(marker)
assert len(set(markers)) == 3
# create callbacks: exact allocation + delegation (guide adds mode 1).
assert elf.at(0x305ac, 20).hex() == '80b56f464ff47770e3f7e6eabde880400ef024b8'
assert elf.at(0x302f0, 22).hex() == '80b56f464ff40a60e3f744ec0121bde8804012f08bba'
assert elf.at(0x2fccc, 4).hex() == '4ff46070' # preview allocation 0x380
for address, expected in ((0x2f826, '0220'), (0x2f9d0, '0220'), (0x2fb76, '0020')):
    assert elf.at(address, 2).hex() == expected
# cbz r0; ldr r1,[r0]; ldr r1,[r1,#4]; bx r1; bx lr
assert elf.at(0x2fde4, 10).hex() == '10b10168496808477047'
print('V21 three factory/type targets, launch modes, identity markers and deleting cleanup: PASS')
