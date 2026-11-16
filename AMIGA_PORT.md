Moon Child for GFOS
===================

A port of Moon Child, Reinier van Vliet's 1997 platform game, from his own
SDL version released in 2026 (https://bitbucket.org/rhinoid/moonchild at
commit f8e0ff358432f4b6324ab74547d6fa8135d49d71), for GFOS: AmigaOS 3.2
with GFOpenUp, on a 68040 or 68060 with an FPU, on GFOpenUp's SDL 2 and
z.library.

Guide Moon Child through four worlds of platforms, switches, monsters and
puzzles, 640 x 480 in 256 colours, with the original music.

Status: built in Cradle's os32-gcc16-open6 stove; not yet run in a lab.


What you need
-------------

- GFOS: AmigaOS 3.2 with GFOpenUp 0.8.26 or later, which installs
  LIBS:GFOpenGPU/SDL2.module and SDL2_mixer.module, and z.library 53 or
  later (GFOpenZlib, in GFOS).
- A 68040 or 68060 with an FPU (not an LC040 or EC040), an RTG screen
  with room for a 640 x 480 window, about 24 MB of free memory and 48 MB
  of disk.
- Nothing else: the pictures, levels, sounds and music are in the drawer.


How to play
-----------

Install it one of three ways:

- GFOpenUp: `GFOpenUpTool INSTALL Parts/MoonChild` puts it in
  SYS:Games/MoonChild.
- Run Install-MoonChild from the archive (it offers Work:Games).
- Or copy the MoonChild drawer where you want it.

Then double-click MoonChild. The cursor keys move and jump, Space fires or
works switches, Esc leaves a level; the drawer's Controls file has them
all. Options and high scores are kept in the drawer's User drawer, which
Install-MoonChild and GFOpenUp keep when they update the game.

Not in this port yet: the cut scenes (the intro, the bumpers between the
worlds, the end and game over). The desktop version plays them as MP4
files through FFmpeg; GFOS has no H.264 decoder for the game to call, so
each one is skipped, as when a player presses a key, and they are not
shipped.


Building
--------

    sh package/build-amiga.sh        (STOVE=/path/to/os32-gcc16-open6)

or, in Cradle, stove_build with the os32-gcc16-open6 stove and target
package. It fetches upstream at the commit package/moonchild.lock names,
applies amiga/patches, removes upstream's own zlib.h, zconf.h and unzip.c,
fetches z.library's SDK headers (GFOpenZlib, amicradle-guest, at the commit
the lock names), builds with amiga/Makefile.amigaos and packs
MoonChild-VERSION-68040.lha and the GFOpenUp part Parts/MoonChild.

Host tests: `sh tests/host/test_all.sh` checks the patch applies and reads
every little-endian file value through mc_le.h, then runs mc_le.h's
big-endian forms over the real mc.art and a level map as a 68k loads them
and compares each value with a little-endian reading; every one of the
1,076 pictures in mc.art inflates to a PCX within the game's buffer.


What was changed
----------------

Moon Child was written for a little-endian PC; the 68k is big-endian.

- amiga/patches/0001-amigaos-byte-order-paths.patch:
  - framewrk/fastfile.cpp: the art file's entry count and offsets,
    framewrk/pcxff.cpp: the PCX picture sizes, moonchild/mc.cpp: the level
    maps' 16-bit block numbers, all read as little-endian (amiga/mc_le.h);
  - framewrk/sprite.cpp: four pixels copied as bytes, not built into a
    little-endian word;
  - main.cpp: the frame is handed to SDL as ARGB8888 words, which is what
    the game writes on either byte order (BGRA32 bytes on a PC);
  - Util.cpp: the data in PROGDIR:Data, the player's files in
    PROGDIR:User; mc.cpp: the options go there too.
- amiga/zlib.h, amiga/mc_zlib.c: the art file's pictures are zlib streams;
  they are unpacked by z.library (GFOpenZlib) instead of a compiled-in
  zlib.
- amiga/MoviePlayer_amiga.cpp: the movie player, without FFmpeg (above).
- amiga/mc_le.h: the little-endian reads.
- amiga/amiga_start.c: 512 KB of stack, the $VER string, PROGDIR:User
  made at start.
- amiga/: the Makefile, the GlowIcons (our own art), the Installer script
  and its spec. package/: the build, the release archive and the GFOpenUp
  part.

Thanks to TheFlowerVixen's Wii and N64 ports of Moon Child
(https://github.com/TheFlowerVixen/MoonChild-Port), whose big-endian
fixes showed where to look; this port's changes are its own.

The AmiCradle Team.
