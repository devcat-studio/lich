#include "../lich/lua.hpp"
#include "../lich/program.h"
#include "../lich/ref.h"
#include "../lich/pcall.h"
#include "../lich/push_to_string.h"
#include "../lich/push_to_primitive.h"
#include "must.h"

using namespace std;

void ref_test()
{
	lua_State* L = luaL_newstate();
	luaL_openlibs(L);
	lich::enable_ref(L);

	MUST_EQUAL(lua_gettop(L), 0);
	MUST_EQUAL(lich::count_all_ref_for_debug(L), 0);

	//---------------------------------------------
	// 문자열을 ref로 바꾸었다가 다시 꺼내기
	{
		lich::push(L, "하이용");
		lich::ref ref;
		MUST_EQUAL(ref.is_valid(), false);
		lich::to(L, -1, ref); // 이제 ref가 '하이용' 을 가리키고 있다.
		lua_pop(L, 1);
		MUST_EQUAL(ref.is_valid(), true);

		MUST_EQUAL(lua_gettop(L), 0);
		MUST_EQUAL(lich::count_all_ref_for_debug(L), 1);

		string s;
		ref.to(s);
		MUST_EQUAL(s, "하이용");

		ref.release();
		MUST_EQUAL(lich::count_all_ref_for_debug(L), 0);
	}

	//---------------------------------------------
	// invalid ref를 push하기
	{
		lich::ref ref;
		MUST_EQUAL(ref.is_valid(), false);
		lich::push(L, ref); // 여기까지는 허용된다. nil이 푸시됨.
		MUST_EQUAL(lua_isnil(L, -1), true);
		lua_pop(L, 1);

		// string s; ref.to(s); // 이건 허용 안된다. L이 없기 때문에...

		MUST_EQUAL(lich::count_all_ref_for_debug(L), 0);
	}

	//---------------------------------------------
	// ref를 다른 ref로 to로 복사하기
	{
		lich::ref ref1;
		lich::ref ref2;

		// ref1을 펑션으로 설정하고 ref2로 복사
		lich::load_program(L, "return 42", "", ref1);
		ref1.to(ref2);

		// 두 개가 되었고 같은 것을 가리키고 있다.
		MUST_EQUAL(lich::count_all_ref_for_debug(L), 2);

		// ref1로 실행해본다
		tuple<int> rv1;
		MUST_EQUAL(lich::pcall(ref1, tuple<>(), rv1).first, true);
		MUST_EQUAL(get<0>(rv1), 42);

		// ref2로 실행해본다
		tuple<int> rv2;
		MUST_EQUAL(lich::pcall(ref2, tuple<>(), rv2).first, true);
		MUST_EQUAL(get<0>(rv2), 42);
	}

	//---------------------------------------------
	// ref를 자기자신에게 to해도 안전한가?
	{
		lich::ref ref;
		lich::load_program(L, "return 42", "", ref);
		ref.to(ref);
		MUST_EQUAL(lich::count_all_ref_for_debug(L), 1);

		tuple<int> rv;
		MUST_EQUAL(lich::pcall(ref, tuple<>(), rv).first, true);
		MUST_EQUAL(get<0>(rv), 42);
	}

	//---------------------------------------------
	// lua_close 전에 이렇게 모든 ref가 소멸되었는지 확인해주는 것이 좋음.
	MUST_EQUAL(lich::count_all_ref_for_debug(L), 0);
	lua_close(L);
}