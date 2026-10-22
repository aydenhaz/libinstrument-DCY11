#!/usr/bin/env python3
"""Assemble the V21 source tree plus the rebuild-v3 overrides into a build root."""
import argparse
from pathlib import Path
import shutil

parser = argparse.ArgumentParser()
parser.add_argument("--out", type=Path, required=True)
args = parser.parse_args()
overlay = Path(__file__).resolve().parent
repository = overlay.parent
out = args.out.resolve()
if out == repository or out == overlay or repository in out.parents:
    parser.error("choose an output outside the repository")
out.mkdir(parents=True, exist_ok=True)
for name in ("include", "stub_include", "internal", "expanded", "src",
             "host_libs", "tests", "analysis"):
    shutil.copytree(repository / name, out / name, dirs_exist_ok=True)
for source in overlay.rglob("*"):
    if not source.is_file() or "__pycache__" in source.parts:
        continue
    relative = source.relative_to(overlay)
    destination = out / relative
    destination.parent.mkdir(parents=True, exist_ok=True)
    shutil.copy2(source, destination)
print(out)
