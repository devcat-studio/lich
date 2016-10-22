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

	// run_program/�� ���α׷�
	{
		tuple<> rv;
		MUST_EQUAL(
			lich::run_program(L, "", "", rv),
			make_pair(true, string()));
	}

	// run_program/���� �ϳ�
	{
		tuple<int> rv;
		MUST_EQUAL(
			lich::run_program(L, "return 42", "", rv),
			make_pair(true, string()));
		MUST_EQUAL(get<0>(rv), 42);
	}

	// run_program/���� Ÿ�� ����
	{
		tuple<string, float, double> rv;
		auto result = lich::run_program(L, "return '5', 7, 3", "", rv);
		MUST_EQUAL(result, make_pair(true, string()));
		MUST_EQUAL(get<0>(rv), "5");
		MUST_EQUAL(get<1>(rv), 7.0f);
		MUST_EQUAL(get<2>(rv), 3.0);
	}

	// push_program + pcall
	{
		tuple<> rv0;
		auto result = lich::run_program(L,
			"A = function(a, b, c) return c, b, a end", "", rv0);
		MUST_EQUAL(result, make_pair(true, string()));

		lua_getglobal(L, "A");
		tuple<string, string, string> rv;
		result = lich::pcall(L, -1, make_tuple("����ģ��", "���ǰ�", "�ϴÿ���"), rv);
		MUST_EQUAL(result, make_pair(true, string()));
		MUST_EQUAL(get<0>(rv), "�ϴÿ���");
		MUST_EQUAL(get<1>(rv), "���ǰ�");
		MUST_EQUAL(get<2>(rv), "����ģ��");
	}

	// run program/������ ����

	// run program/��Ÿ�� ����

	lua_close(L);

    return 0;
}

