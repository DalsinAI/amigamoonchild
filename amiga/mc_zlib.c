/* uncompress() through z.library (GFOpenZlib, LIBS:z.library 53.13 or
 * later; Uncompress is in every z.library's table). Opened on the first
 * call, closed at exit.
 * Copyright (c) 2026 AmiCradle Team. MIT, like Moon Child's code. */
#include <exec/types.h>
#include <proto/exec.h>
#include <proto/z.h>
#include <stdlib.h>
#include "zlib.h"

struct Library *ZBase;

static void close_z(void)
{
	if (ZBase) {
		CloseLibrary(ZBase);
		ZBase = NULL;
	}
}

int uncompress(Bytef *dest, uLongf *destLen, const Bytef *source, uLong sourceLen)
{
	ULONG len = *destLen;
	LONG rc;

	if (!ZBase) {
		ZBase = OpenLibrary((CONST_STRPTR)"z.library", 53);
		if (!ZBase)
			return Z_VERSION_ERROR;
		atexit(close_z);
	}
	rc = Uncompress((UBYTE *)dest, &len, (CONST UBYTE *)source, (ULONG)sourceLen);
	*destLen = len;
	return (int)rc;
}
