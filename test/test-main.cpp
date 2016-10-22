#include <iostream>
#include "../lich/lua.hpp"
#include "../lich/program.h"

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

	string program = "print('1') return 42";
	tuple<int> rv;
	lich::run_program(L, program.c_str(), program.size(), "(string)", rv);
	CHECK(get<0>(rv) == 42);
		
	lua_close(L);

    return 0;
}

