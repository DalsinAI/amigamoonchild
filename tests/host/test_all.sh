#!/bin/sh
# Every host test of the port:
#  - amiga/patches apply to upstream at the locked commit, and the patched
#    sources read every multi-byte file value through mc_le.h;
#  - mc_le.h's big-endian forms, on the real mc.art and a level map
#    (test_le.c + check_data.py), and every fastfile entry inflates to a
#    PCX within the game's buffer.
#   sh tests/host/test_all.sh        (MC=/path/to/moonchild to skip the fetch)
# Copyright (c) 2026 AmiCradle Team. MIT, like Moon Child's code.
set -eu
HERE=$(CDPATH= cd -- "$(dirname -- "$0")" && pwd)
ROOT=$(CDPATH= cd -- "$HERE/../.." && pwd)
. "$ROOT/package/moonchild.lock"
T=$(mktemp -d); trap 'rm -rf "$T"' EXIT
if [ -z "${MC:-}" ]; then
    git clone -q --depth 1 "$MOONCHILD_URL" "$T/mc"
    git -C "$T/mc" checkout -q "$MOONCHILD_COMMIT"
    MC=$T/mc
fi
mkdir -p "$T/p" && cp -R "$MC/src" "$T/p/"
for p in "$ROOT"/amiga/patches/*.patch; do (cd "$T/p" && git apply "$p"); done
echo "patches: apply to $MOONCHILD_COMMIT"
# every fastfile, PCX-size and map read goes through mc_le.h
for want in "fastfile.cpp:MC_LE32(dwFECnt)" "fastfile.cpp:MC_LE32(pFE\[e\].offset)" \
            "pcxff.cpp:MC_LE16(\*width)" "pcxff.cpp:MC_LE16(xsize)" "pcxff.cpp:MC_LE16(width)" \
            "mc.cpp:MC_LE16(player1.loadedmap->map\[b\])" "main.cpp:SDL_PIXELFORMAT_ARGB8888"; do
    f=${want%%:*}; pat=${want#*:}
    grep -q "$pat" $(find "$T/p/src" -name "$f") || { echo "FAIL: $f lacks $pat"; exit 1; }
done
echo "patches: fastfile table, PCX sizes, level maps through mc_le.h; frame as ARGB8888 words"
${CC:-cc} -std=c99 -Wall -Wextra -DMC_LE_SWAP -I"$ROOT/amiga" -o "$T/test_le" "$HERE/test_le.c"
"$T/test_le" "$MC/assets/moonchild/mc.art" "$MC/assets/moonchild/lvl11.map" > "$T/le.txt"
python3 -I "$HERE/check_data.py" "$MC/assets/moonchild" "$T/le.txt" lvl11.map
python3 -I "$HERE/test_lha.py"
echo PASS
