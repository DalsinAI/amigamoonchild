/* Moon Child's data files were written by a 1997 x86 PC: the fastfile's
 * table, the PCX sizes and the level maps are little-endian. These read a
 * little-endian value already in memory on any byte order (no-ops on x86).
 * Copyright (c) 2026 AmiCradle Team. MIT, like Moon Child's code. */
#ifndef MC_LE_H
#define MC_LE_H

#include <stdint.h>

/* MC_LE_SWAP: the host test builds the swapping forms on a little-endian
 * PC, to check them on the real files as a 68k sees them */
#if (defined(__BYTE_ORDER__) && __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__) || defined(MC_LE_SWAP)
#define MC_LE16(x) ((uint16_t)((((uint16_t)(x)) >> 8) | (((uint16_t)(x)) << 8)))
#define MC_LE32(x) ((uint32_t)( \
	((((uint32_t)(x)) >> 24) & 0x000000ffu) | ((((uint32_t)(x)) >> 8) & 0x0000ff00u) | \
	((((uint32_t)(x)) << 8) & 0x00ff0000u) | ((((uint32_t)(x)) << 24) & 0xff000000u)))
#else
#define MC_LE16(x) (x)
#define MC_LE32(x) (x)
#endif

#endif
