#pragma once

#include "lua.hpp"
#include <vector>

namespace lich
{
	//-------------------------------------------------------------------------
	template<typename T>
	inline void push(lua_State* L, const std::vector<T>& vec)
	{
		lua_newtable(L);
		for (size_t i = 0; i < vec.size(); ++i)
		{
			push(L, vec[i]);
			lua_rawseti(L, -2, (int)i + 1);
		}
	}

	//-------------------------------------------------------------------------
	template<typename T>
	inline void to(lua_State* L, int idx, std::vector<T>& vec)
	{
		vec.clear();
		for (int i = 1; ; ++i)
		{
			lua_rawgeti(L, idx, i);
			if (lua_isnil(L, -1))
			{
				lua_pop(L, 1);
				break;
			}
			else
			{
				T val;
				to(L, -1, val);
				vec.push_back(val);
				lua_pop(L, 1);
			}
		}
	}
}
