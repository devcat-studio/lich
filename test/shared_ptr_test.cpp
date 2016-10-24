#include "../lich/all.h"
#include "must.h"

using namespace std;

void shared_ptr_test()
{
	lua_State* L = luaL_newstate();
	luaL_openlibs(L);
	lich::enable_ref(L);

	// 기초적인 push, to
	{
		auto s = make_shared<string>("똥쥬뿌싄쪄");
		MUST_EQUAL(s.use_count(), 1);

		lich::push(L, s);
		MUST_EQUAL(s.use_count(), 2);

		shared_ptr<string> p;
		lich::to(L, -1, p);
		MUST_EQUAL(p.get(), s.get());
		MUST_EQUAL(s.use_count(), 3);

		lua_pop(L, 1);
		lua_gc(L, LUA_GCCOLLECT, 0);
		MUST_EQUAL(s.use_count(), 2);
	}

	lua_close(L);
}