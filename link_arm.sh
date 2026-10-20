#!/bin/sh
set -eu
: "${IPL_NDK_ROOT:?set IPL_NDK_ROOT to the Android NDK r27c directory}"
IPL_SOURCE_DIR=$(CDPATH= cd -- "$(dirname -- "$0")" && pwd)
exec python3 "$IPL_SOURCE_DIR/build_android.py" --ndk "$IPL_NDK_ROOT" "$@"
