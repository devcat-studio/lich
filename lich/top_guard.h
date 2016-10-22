#pragma once

#include "lua.hpp"

namespace lich
{
	///////////////////////////////////////////////////////////////////////////
	// C++ 스코프를 빠져나갈 때 원래 스택 top으로 되돌린다
	//
	class top_guard
	{
	public:
		top_guard(lua_State* L) : L(L)
		{
			old_top = lua_gettop(L);
		}
		~top_guard()
		{
			reset();
		}

		void reset()
		{
			lua_settop(L, old_top);
		}

		top_guard(const top_guard&) = delete;
		void operator=(const top_guard&) = delete;

	private:
		lua_State* L;
		int old_top;
	};
}