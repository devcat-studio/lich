#pragma once

#include "lua.hpp"

namespace lich
{
	//-------------------------------------------------------------------------
	inline void push(lua_State* L, lua_Number v)
	{
		lua_pushnumber(L, v);
	}

	//-------------------------------------------------------------------------
	inline void push(lua_State* L, const char* v)
	{
		lua_pushstring(L, v);
	}

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
}
