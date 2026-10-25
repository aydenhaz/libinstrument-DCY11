"""Pin post-load facts to the original V21 ELF, not reconstructed comments."""
from pathlib import Path
import sys

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / "tools"))
from inspect_packet_factory import ELF32, plt_symbol

elf = ELF32(ROOT / "reference/v21/libInstrumentPannelLarge.so")

def literal(pc, pool, encoding):
    return elf.at(pc + elf.u32(pool), 200).decode(encoding).split("\0")[0]

assert elf.at(0x2f626, 4).hex() == "4ff48d7c"  # mov.w r12,#0x11a
assert literal(0x2f63e, 0x2f65c, "ascii") == "onPostLoadRes"
assert literal(0x2f642, 0x2f660, "ascii") == (
    "virtual void NS_InstrumentPannelLarge::CGIPLPacket::onPostLoadRes()")
for address in (0x2f67c, 0x2f872, 0x2fa18):
    assert elf.at(address, 2).hex() == "3820"  # movs r0,#0x38
for pc, pool, name in (
    (0x2f674, 0x2f70c, "largecuise"),
    (0x2f86e, 0x2fbd8, "largeguide"),
    (0x2fa14, 0x2fc04, "largepreview"),
):
    assert literal(pc, pool, "utf-16le") == "com.auto.multiscreen." + name
assert plt_symbol(elf, 0x14700)[1] == (
    "_ZN9GNS_FRAME9CGPackage17_registerFragmentERK8CGStringPNS_17CGFragmentBuilderE")
assert "CGFragmentBuilderC2" in plt_symbol(elf, 0x146f4)[1]
assert elf.at(0x2fbb2, 10).hex() == "d8f80000816940468847"  # final slot 6
print("V21 post-load trace, builder sizes/names, registration and final slot: PASS")
