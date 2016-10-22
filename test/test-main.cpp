#include <iostream>
#include "../lich/lua.hpp"

#ifdef _WIN32
	#define WIN32_LEAN_AND_MEAN
	#include <windows.h>
#else
	#define _CrtDbgBreak()
#endif

using namespace std;

#define CHECK(cond)	\
	if (!(cond)) { \
		cerr << "FAILED: " << #cond << endl; \
		_CrtDbgBreak(); \
	}

int main()
{
	lua_State* L = luaL_newstate();


	lua_close(L);

    return 0;
}

