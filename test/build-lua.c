#include <stdio.h>

#ifdef _WIN32
#define _CRT_SECURE_NO_WARNINGS 1
#pragma warning(push, 2)
#else
#define __STRICT__ANSI__
#endif

#define _ANSI_SOURCE

#define luaall_c
#define LUA_CORE
#define LUA_USE_APICHECK

#include "../lua-5.1.5/luaconf.h"
#undef LUA_DL_DLL

#include "../lua-5.1.5/lapi.c"
#include "../lua-5.1.5/lauxlib.c"
#include "../lua-5.1.5/lbaselib.c"
#include "../lua-5.1.5/lcode.c"
#include "../lua-5.1.5/ldblib.c"
#include "../lua-5.1.5/ldebug.c"
#include "../lua-5.1.5/ldo.c"
#include "../lua-5.1.5/ldump.c"
#include "../lua-5.1.5/lfunc.c"
#include "../lua-5.1.5/lgc.c"
#include "../lua-5.1.5/linit.c"
#include "../lua-5.1.5/liolib.c"
#include "../lua-5.1.5/llex.c"
#include "../lua-5.1.5/lmathlib.c"
#include "../lua-5.1.5/lmem.c"
#include "../lua-5.1.5/loadlib.c"
#include "../lua-5.1.5/lobject.c"
#include "../lua-5.1.5/lopcodes.c"
#include "../lua-5.1.5/loslib.c"
#include "../lua-5.1.5/lparser.c"
#include "../lua-5.1.5/lstate.c"
#include "../lua-5.1.5/lstring.c"
#include "../lua-5.1.5/lstrlib.c"
#include "../lua-5.1.5/ltable.c"
#include "../lua-5.1.5/ltablib.c"
#include "../lua-5.1.5/ltm.c"
#include "../lua-5.1.5/lundump.c"
#include "../lua-5.1.5/lvm.c"
#include "../lua-5.1.5/lzio.c"
#include "../lua-5.1.5/print.c"
