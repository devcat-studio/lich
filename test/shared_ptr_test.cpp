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

	// push, to 두번씩
	{
		auto a = make_shared<string>("프로의 손에");
		auto b = make_shared<string>("맡기십시오");
		lich::push(L, a);
		lich::push(L, b);

		shared_ptr<string> a_;
		shared_ptr<string> b_;
		lich::to(L, 1, a_);
		lich::to(L, 2, b_);
		MUST_EQUAL(a.get(), a_.get());
		MUST_EQUAL(b.get(), b_.get());

		lua_pop(L, 2);
	}

	lua_close(L);
}