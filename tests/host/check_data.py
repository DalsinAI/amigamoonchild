"""Moon Child's data as the Amiga port reads it, checked on a PC:
  - mc.art, the fastfile: little-endian count and offsets (as test_le
    printed them through mc_le.h's swapping forms), each entry a zlib
    stream that inflates (through Python's zlib, the format z.library's
    Uncompress reads) to at most fastfile.cpp's 829990-byte buffer, and is
    a PCX whose little-endian size fields give a sane picture;
  - a level map: its 16-bit block numbers as test_le printed them.
    python3 check_data.py <assets/moonchild> <test_le output> <map name>
Copyright (c) 2026 AmiCradle Team. MIT, like Moon Child's code."""
import struct, sys, zlib
from pathlib import Path

art_dir, le_out, map_name = Path(sys.argv[1]), sys.argv[2], sys.argv[3]
art = (art_dir / "mc.art").read_bytes()
lines = [l.split() for l in open(le_out)]
count = struct.unpack("<I", art[:4])[0]
assert lines[0] == ["count", str(count)], lines[0]
offsets = [struct.unpack("<i", art[4 + i * 20:8 + i * 20])[0] for i in range(count)]
got = [int(v) for k, v in lines[1:1 + count]]
assert got == offsets, "fastfile offsets differ through MC_LE32"
names = [art[8 + i * 20:24 + i * 20].split(b"\0")[0].decode("latin-1") for i in range(count)]
pcx = 0
for i in range(count - 1):
    raw = art[offsets[i]:offsets[i + 1]]
    if not names[i].upper().endswith(".RZ"):
        continue  # the packer's own tool, left in the file; the game asks for none
    u = zlib.decompress(raw)
    assert len(u) <= 829990, (names[i], len(u))
    assert u[0] == 10, names[i]  # PCX
    xmin, ymin, xmax, ymax = struct.unpack("<HHHH", u[4:12])
    assert xmin == 0 and ymin == 0 and 0 < xmax < 2048 and 0 < ymax < 2048, (names[i], xmax, ymax)
    pcx += 1
print("mc.art: %d entries, %d PCX pictures inflate within the buffer, sizes read little-endian" % (count, pcx))
m = (art_dir / map_name).read_bytes()
blocks = [struct.unpack("<H", m[i:i + 2])[0] for i in range(0, len(m) - 1, 2)]
got = [int(v) for k, v in lines[1 + count:]]
assert got == blocks, "map blocks differ through MC_LE16"
assert max(blocks) < 4096, max(blocks)
print("%s: %d blocks read little-endian, highest %d" % (map_name, len(blocks), max(blocks)))
