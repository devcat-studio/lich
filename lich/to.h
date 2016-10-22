#pragma once

#include "lua.hpp"
#include <string>
#include <vector>

namespace lich
{
	//-------------------------------------------------------------------------
	inline void to(lua_State* L, int idx, int& v)
	{
		v = static_cast<int>(lua_tointeger(L, idx));
	}

	//-------------------------------------------------------------------------
	inline void to(lua_State* L, int idx, float& v)
	{
		v = static_cast<float>(lua_tonumber(L, idx));
	}

	//-------------------------------------------------------------------------
	inline void to(lua_State* L, int idx, double& v)
	{
		v = lua_tonumber(L, idx);
	}

	//-------------------------------------------------------------------------
	inline void to(lua_State* L, int idx, std::string& v)
	{
		size_t len = 0;
		const char* s = lua_tolstring(L, idx, &len);
		if (s == nullptr) { s = ""; }
		v.assign(s, len);
	}

	//-------------------------------------------------------------------------
	template<typename T>
	inline void to(lua_State* L, int idx, std::vector<T>& vec)
	{
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