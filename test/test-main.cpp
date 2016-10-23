#include "../lich/lua.hpp"
#include "../lich/program.h"
#include "../lich/ref.h"
#include "must.h"

using namespace std;

int main()
{
	lua_State* L = luaL_newstate();
	luaL_openlibs(L);
	lich::enable_ref(L);
	MUST_EQUAL(lua_gettop(L), 0);

	// run_program/�� ���α׷�
	{
		tuple<> rv;
		MUST_EQUAL(
			lich::run_program(L, "", "", rv),
			make_pair(true, string()));
	}
	MUST_EQUAL(lua_gettop(L), 0);

	// run_program/���� �ϳ�
	{
		tuple<int> rv;
		MUST_EQUAL(
			lich::run_program(L, "return 42", "", rv),
			make_pair(true, string()));
		MUST_EQUAL(get<0>(rv), 42);
	}
	MUST_EQUAL(lua_gettop(L), 0);

	// run_program/���� Ÿ�� ����
	{
		tuple<string, float, double> rv;
		auto result = lich::run_program(L, "return '5', 7, 3", "", rv);
		MUST_EQUAL(result, make_pair(true, string()));
		MUST_EQUAL(get<0>(rv), "5");
		MUST_EQUAL(get<1>(rv), 7.0f);
		MUST_EQUAL(get<2>(rv), 3.0);
	}
	MUST_EQUAL(lua_gettop(L), 0);

	// push_program + pcall + ref
	{
		tuple<lich::ref> rv0;
		auto result = lich::run_program(L,
			"return function(a, b, c) return c, b, a end", "", rv0);
		MUST_EQUAL(result, make_pair(true, string()));

		tuple<string, string, string> rv1;
		result = lich::pcall(L, get<0>(rv0), make_tuple("����ģ��", "���ǰ�", "�ϴÿ���"), rv1);
		MUST_EQUAL(result, make_pair(true, string()));
		MUST_EQUAL(get<0>(rv1), "�ϴÿ���");
		MUST_EQUAL(get<1>(rv1), "���ǰ�");
		MUST_EQUAL(get<2>(rv1), "����ģ��");
	}
	MUST_EQUAL(lua_gettop(L), 0);

	// run program/������ ����
	{
		tuple<> rv0;
		auto result = lich::run_program(L, "local local", "", rv0);
		MUST_EQUAL(
			result,
			make_pair(false, string("[string \"\"]:1: '<name>' expected near 'local'")));
	}
	MUST_EQUAL(lua_gettop(L), 0);

	// run program/��Ÿ�� ����
	{
		tuple<> rv0;
		auto result = lich::run_program(L, "print('b' + 'c')", "", rv0);
		MUST_EQUAL(result.first, false);
		MUST_TRUE(result.second.find("perform arithmetic") != string::npos);
	}

	MUST_EQUAL(lua_gettop(L), 0);
	lua_close(L);

	cout << "OK" << endl;
    return 0;
}

