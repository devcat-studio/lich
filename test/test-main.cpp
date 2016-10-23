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

#define MUST_TRUE(cond)	\
	if (!(cond)) { \
		cerr << "FAILED: MUST_TRUE(" << #cond << ")" << endl; \
		_CrtDbgBreak(); \
	}

#define MUST_EQUAL(a, b) \
	if ((a) != (b)) { \
		cerr << "FAILED: MUST_EQUAL(" << #a << ", " << #b << ")" << endl; \
		_CrtDbgBreak(); \
	}


int main()
{
	lua_State* L = luaL_newstate();
	luaL_openlibs(L);
	assert(lua_gettop(L) == 0);

	// run_program/빈 프로그램
	{
		tuple<> rv;
		MUST_EQUAL(
			lich::run_program(L, "", "", rv),
			make_pair(true, string()));
	}
	assert(lua_gettop(L) == 0);

	// run_program/리턴 하나
	{
		tuple<int> rv;
		MUST_EQUAL(
			lich::run_program(L, "return 42", "", rv),
			make_pair(true, string()));
		MUST_EQUAL(get<0>(rv), 42);
	}
	assert(lua_gettop(L) == 0);

	// run_program/여러 타입 리턴
	{
		tuple<string, float, double> rv;
		auto result = lich::run_program(L, "return '5', 7, 3", "", rv);
		MUST_EQUAL(result, make_pair(true, string()));
		MUST_EQUAL(get<0>(rv), "5");
		MUST_EQUAL(get<1>(rv), 7.0f);
		MUST_EQUAL(get<2>(rv), 3.0);
	}
	assert(lua_gettop(L) == 0);

	// push_program + pcall
	{
		tuple<> rv0;
		auto result = lich::run_program(L,
			"A = function(a, b, c) return c, b, a end", "", rv0);
		MUST_EQUAL(result, make_pair(true, string()));

		lua_getglobal(L, "A");
		tuple<string, string, string> rv;
		result = lich::pcall(L, -1, make_tuple("빗발친다", "정의가", "하늘에서"), rv);
		MUST_EQUAL(result, make_pair(true, string()));
		MUST_EQUAL(get<0>(rv), "하늘에서");
		MUST_EQUAL(get<1>(rv), "정의가");
		MUST_EQUAL(get<2>(rv), "빗발친다");
		lua_pop(L, 1);
	}
	assert(lua_gettop(L) == 0);

	// run program/컴파일 에러
	{
		tuple<> rv0;
		auto result = lich::run_program(L, "local local", "", rv0);
		MUST_EQUAL(
			result,
			make_pair(false, string("[string \"\"]:1: '<name>' expected near 'local'")));
	}
	assert(lua_gettop(L) == 0);

	// run program/런타임 에러
	{
		tuple<> rv0;
		auto result = lich::run_program(L, "print('b' + 'c')", "", rv0);
		MUST_EQUAL(result.first, false);
		MUST_TRUE(result.second.find("perform arithmetic") != string::npos);
	}

	assert(lua_gettop(L) == 0);
	lua_close(L);

    return 0;
}

