/* The Amiga port's start: the stack, the version string and PROGDIR:User
 * (the high scores and options), made before main().
 * Copyright (c) 2026 AmiCradle Team. MIT, like Moon Child's code. */
#include <sys/stat.h>

/* A Shell gives 4 KB of stack; SDL 2 and the game want more. */
unsigned long __stack = 512 * 1024;
static const char version[] __attribute__((used)) =
    "$VER: MoonChild 1.0 (16.11.2026)";

static void __attribute__((constructor)) amiga_user_drawer(void)
{
	mkdir("PROGDIR:User", 0755);
}
