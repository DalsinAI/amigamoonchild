/* Host test of amiga/mc_le.h on Moon Child's own files, as a 68k reads
 * them. Built with MC_LE_SWAP on a little-endian PC: each value is first
 * loaded the way a big-endian CPU would load those bytes (reversed), then
 * MC_LE16/MC_LE32 must give back what the PC reads natively. Prints
 * "name value" lines test_data.sh compares with Python's reading.
 *   test_le <mc.art> <lvlNN.map>
 * Copyright (c) 2026 AmiCradle Team. MIT, like Moon Child's code. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mc_le.h"

static uint32_t as_be32(const unsigned char *p)	/* what a 68k loads */
{
	return ((uint32_t)p[0] << 24) | ((uint32_t)p[1] << 16) | ((uint32_t)p[2] << 8) | p[3];
}

static uint16_t as_be16(const unsigned char *p)
{
	return (uint16_t)((p[0] << 8) | p[1]);
}

static unsigned char *slurp(const char *path, long *len)
{
	FILE *fp = fopen(path, "rb");
	unsigned char *b;

	if (!fp || fseek(fp, 0, SEEK_END) || (*len = ftell(fp)) < 0 || fseek(fp, 0, SEEK_SET))
		exit(2);
	b = malloc((size_t)*len);
	if (!b || fread(b, 1, (size_t)*len, fp) != (size_t)*len)
		exit(2);
	fclose(fp);
	return b;
}

int main(int argc, char **argv)
{
	long alen, mlen, i;
	unsigned char *art, *map;
	uint32_t n;

	if (argc != 3)
		return 2;
	if (MC_LE16(0x3412) != 0x1234 || MC_LE32(0x78563412u) != 0x12345678u) {
		printf("swap forms wrong\n");
		return 1;
	}
	art = slurp(argv[1], &alen);
	/* fastfile.cpp: the count, then {int offset; char name[16];} each */
	n = MC_LE32(as_be32(art));
	printf("count %lu\n", (unsigned long)n);
	for (i = 0; i < (long)n; i++)
		printf("offset %lu\n", (unsigned long)MC_LE32(as_be32(art + 4 + i * 20)));
	/* mc.cpp: a level map is 16-bit block numbers */
	map = slurp(argv[2], &mlen);
	for (i = 0; i + 1 < mlen; i += 2)
		printf("block %u\n", (unsigned)MC_LE16(as_be16(map + i)));
	free(art);
	free(map);
	return 0;
}
