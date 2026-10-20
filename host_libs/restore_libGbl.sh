#!/bin/sh
# Restore the original host library from its lossless gzip copy.
set -eu
cd "$(dirname "$0")"
gzip -dc libGbl.so.gz > libGbl.so.tmp
printf "%s  %s\n" "14214e2e918922a8b7d8bca1c095716d515ffe6f" "$(git hash-object libGbl.so.tmp)" >/dev/null
actual=$(git hash-object libGbl.so.tmp)
[ "$actual" = "14214e2e918922a8b7d8bca1c095716d515ffe6f" ] || { rm -f libGbl.so.tmp; echo "Hash mismatch" >&2; exit 1; }
mv libGbl.so.tmp libGbl.so
echo "Restored libGbl.so; original Git blob hash verified."
