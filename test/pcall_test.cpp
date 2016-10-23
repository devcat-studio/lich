#include "../lich/push_to_primitive.h"
#include "../lich/push_to_string.h"
#include "../lich/program.h"
#include "../lich/ref.h"
#include "must.h"

using namespace std;

void pcall_test()
{
	lua_State* L = luaL_newstate();
	luaL_openlibs(L);
	lich::enable_ref(L);

	// 여러 인자, 여러 리턴
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

	// 글로벌 함수 실행
	{
		tuple<string> rv;
		MUST_TRUE(lich::pcall(L, "type", make_tuple(1), rv).first);
		MUST_EQUAL(get<0>(rv), "number");

		// 원소 하나짜리 투플로 실행하면 그냥 글로벌 함수로 취급
		MUST_TRUE(lich::pcall(L, make_tuple("type"), make_tuple(1), rv).first);
		MUST_EQUAL(get<0>(rv), "number");
	}

	// 글로벌 함수 실행: 에러 처리
	{
		tuple<string> rv;
		auto result = lich::pcall(L, "XXX", tuple<>(), rv);
		MUST_TRUE(result.second.find("attempt to call a nil value") != string::npos);
	}

	// 모듈 함수 실행
	{
		// string.rep("아", 5)
		tuple<string> rv;
		MUST_TRUE(lich::pcall(L, make_tuple("string", "rep"), make_tuple("아", 5), rv).first);
		MUST_EQUAL(get<0>(rv), "아아아아아");

		// A.B.C("아", 5)
		lich::run_program(L, "A = { B = { C = string.rep } }", "", rv);
		MUST_TRUE(lich::pcall(L, make_tuple("A", "B", "C"), make_tuple("아", 6), rv).first);
		MUST_EQUAL(get<0>(rv), "아아아아아아");
	}

	// 모듈 함수 실행: 에러 처리
	{
		// a.b.c.d()
		tuple<> rv;
		auto result = lich::pcall(L, make_tuple("a", "b", "c", "d"), tuple<>(), rv);
		MUST_EQUAL(result.first, false);
		MUST_TRUE(result.second.find("attempt to call a nil value") != string::npos);

		// string.XXX.YYY()
		lich::pcall(L, make_tuple("string", "XXX", "YYY"), tuple<>(), rv);
		MUST_EQUAL(result.first, false);
		MUST_TRUE(result.second.find("attempt to call a nil value") != string::npos);
	}

	// 메서드 실행
	{
		// s = "justive rains from above"
		lich::push(L, "justive rains from above");
		lich::ref s;
		lich::to(L, -1, s);
		lua_pop(L, 1);

		// s:upper() == s.upper(s)
		tuple<string> rv;
		MUST_TRUE(lich::pcall(L, tie(s, "upper"), tie(s), rv).first);
		MUST_EQUAL(get<0>(rv), "JUSTIVE RAINS FROM ABOVE");
	}

	lua_close(L);
}