/* The one zlib call Moon Child makes (fastfile.cpp's uncompress), answered
 * by z.library (GFOpenZlib) instead of a compiled-in zlib. The upstream
 * tree's own zlib.h, zconf.h and unzip.c are removed before the build
 * (package/build-amiga.sh), so fastfile.cpp's #include "zlib.h" lands here.
 * Copyright (c) 2026 AmiCradle Team. MIT, like Moon Child's code. */
#ifndef MC_AMIGA_ZLIB_H
#define MC_AMIGA_ZLIB_H

#ifdef __cplusplus
extern "C" {
#endif

typedef unsigned char Bytef;
typedef unsigned long uLong;
typedef unsigned long uLongf;

#define Z_OK		0
#define Z_MEM_ERROR	(-4)
#define Z_BUF_ERROR	(-5)
#define Z_DATA_ERROR	(-3)
#define Z_VERSION_ERROR	(-6)

/* zlib's uncompress(): *destLen in is the room at dest, out the bytes
 * written. Z_VERSION_ERROR when z.library can't be opened. */
int uncompress(Bytef *dest, uLongf *destLen, const Bytef *source, uLong sourceLen);

#ifdef __cplusplus
}
#endif

#endif
