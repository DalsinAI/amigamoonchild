#!/bin/sh
# The release archive and the GFOpenUp part, for Cradle's stove_build
# (target package):
#  1. upstream Moon Child, fetched at the commit package/moonchild.lock
#     names (it is never kept in this repository), with amiga/patches
#     applied and its own zlib.h, zconf.h and unzip.c removed (z.library
#     does the unpacking); z.library's SDK headers, from GFOpenZlib at the
#     commit the lock names;
#  2. the game built by amiga/Makefile.amigaos;
#  3. the drawer, its icon and the Installer script beside it packed as
#     NAME-VERSION-68040.lha (tools/pack_dist.py), with its .sha256 and a
#     short .readme;
#  4. OUT/Parts/MoonChild, a GFOpenUp part: its Part file and the drawer's
#     files for SYS:Games/MoonChild, installed with
#       GFOpenUpTool INSTALL Parts/MoonChild
#     It needs the GFOpenGPU part, whose SDL2.module draws the game.
# Nothing is published from here.
# MIT, Copyright (c) 2026 AmiCradle Team.
set -eu

HERE=$(CDPATH= cd -- "$(dirname -- "$0")" && pwd)
ROOT=$(CDPATH= cd -- "$HERE/.." && pwd)
. "$HERE/package.env"
. "$HERE/moonchild.lock"
OUT=${OUT:-"$ROOT/build-amiga-package"}
mkdir -p "$OUT"
OUT=$(CDPATH= cd -- "$OUT" && pwd)
STOVE=${STOVE:-"$HOME/AmiCradle/stoves/os32-gcc16-open6"}
# outside the checkout: Cradle cleans it (git clean -ffdx) before every job
WORK=${WORK:-"$HOME/.cache/amigamoonchild"}
cd "$ROOT"

# 1. upstream at the locked commit, patched afresh every time
MC="$WORK/moonchild"
if [ "$(git -C "$MC" rev-parse HEAD 2>/dev/null)" != "$MOONCHILD_COMMIT" ]; then
    rm -rf "$MC"; mkdir -p "$MC"
    git -C "$MC" init -q
    git -C "$MC" fetch -q --depth 1 "$MOONCHILD_URL" "$MOONCHILD_COMMIT" ||
        git -C "$MC" fetch -q "$MOONCHILD_URL" "$MOONCHILD_COMMIT"
    git -C "$MC" checkout -q FETCH_HEAD
fi
git -C "$MC" checkout -q -- . && git -C "$MC" clean -qfdx
for p in "$ROOT"/amiga/patches/*.patch; do
    git -C "$MC" apply "$p"
done
rm -f "$MC/src/framewrk/zlib.h" "$MC/src/framewrk/zconf.h" "$MC/src/framewrk/unzip.c"

ZS="$WORK/zsdk"
if [ "$(git -C "$ZS" rev-parse HEAD 2>/dev/null)" != "$ZSDK_COMMIT" ]; then
    rm -rf "$ZS"; mkdir -p "$ZS"
    git -C "$ZS" init -q
    git -C "$ZS" fetch -q --depth 1 "$ZSDK_URL" "$ZSDK_COMMIT"
    git -C "$ZS" checkout -q FETCH_HEAD -- "$ZSDK_PATH"
    git -C "$ZS" update-ref HEAD FETCH_HEAD
fi

# 2. the game (a stove's compiler is in its prefix/, or STOVE is the prefix)
if [ -x "$STOVE/prefix/bin/m68k-amigaos-gcc" ]; then PREFIX="$STOVE/prefix"; else PREFIX="$STOVE"; fi
make -f amiga/Makefile.amigaos MC="$MC" ZSDK="$ZS/$ZSDK_PATH" STOVE="$STOVE" PREFIX="$PREFIX" -j4 $TARGET \
    > "$OUT/build-output.txt" 2>&1 || { tail -n 60 "$OUT/build-output.txt"; exit 1; }

# 3. the archive
python3 "$ROOT/tools/pack_dist.py" --dist "$DIST" --name "$NAME" --out "$OUT"

# 4. the GFOpenUp part
sh "$ROOT/package/write-part.sh" "$DIST/$NAME" "$OUT/Parts/$NAME" "$NAME" "$PORT_VERSION" \
    "Moon Child: Reinier van Vliet's 1997 platform game, four worlds"
echo "part: $(grep -c '^copy' "$OUT/Parts/$NAME/Part") files, $(du -sk "$OUT/Parts/$NAME" | cut -f1) KB"
ls -l "$DIST/$NAME/$NAME"
