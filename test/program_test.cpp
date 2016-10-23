#include "../lich/program.h"
#include "../lich/ref.h"
#include "must.h"

using namespace std;

void program_test_do(lua_State* L)
{
	MUST_EQUAL(lua_gettop(L), 0);

	// run_program/빈 프로그램
	{
		tuple<> rv;
		MUST_EQUAL(
			lich::run_program(L, "", "", rv),
			make_pair(true, string()));
	}
	MUST_EQUAL(lua_gettop(L), 0);

	// run_program/리턴 하나
	{
		tuple<int> rv;
		MUST_EQUAL(
			lich::run_program(L, "return 42", "", rv),
			make_pair(true, string()));
		MUST_EQUAL(get<0>(rv), 42);
	}
	MUST_EQUAL(lua_gettop(L), 0);

	// run_program/여러 타입 리턴
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
		result = lich::pcall(get<0>(rv0), make_tuple("빗발친다", "정의가", "하늘에서"), rv1);
		MUST_EQUAL(result, make_pair(true, string()));
		MUST_EQUAL(get<0>(rv1), "하늘에서");
		MUST_EQUAL(get<1>(rv1), "정의가");
		MUST_EQUAL(get<2>(rv1), "빗발친다");
	}
	MUST_EQUAL(lua_gettop(L), 0);

	// run program/컴파일 에러
	{
		tuple<> rv0;
		auto result = lich::run_program(L, "local local", "", rv0);
		MUST_EQUAL(
			result,
			make_pair(false, string("[string \"\"]:1: '<name>' expected near 'local'")));
	}
	MUST_EQUAL(lua_gettop(L), 0);

	// run program/런타임 에러
	{
		tuple<> rv0;
		auto result = lich::run_program(L, "print('b' + 'c')", "", rv0);
		MUST_EQUAL(result.first, false);
		MUST_TRUE(result.second.find("perform arithmetic") != string::npos);
	}

	MUST_EQUAL(lua_gettop(L), 0);
}

void program_test()
{
	// openlibs 하고 테스트
	{
		lua_State* L = luaL_newstate();
		luaL_openlibs(L);
		lich::enable_ref(L);
		program_test_do(L);
		lua_close(L);
	}

	// openlibs 없이 테스트 (debug 라이브러리 없음)
	{
		lua_State* L = luaL_newstate();
		lich::enable_ref(L);
		program_test_do(L);
		lua_close(L);
	}

}