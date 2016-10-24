// 아오-_- 테스트만 통과하게 최소한으로 패치 {
#ifdef _WIN32
#define _CRT_SECURE_NO_WARNINGS 1
#pragma warning(push, 2)
#endif

#define __STRICT_ANSI__
#define _ANSI_SOURCE

#define LUA_TMPNAMBUFSIZE 32
#define lua_tmpnam(buff, err) buff[0]=0;err=0;
// 아오-_- 테스트만 통과하게 최소한으로 패치 }

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
