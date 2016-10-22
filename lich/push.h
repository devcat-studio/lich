#pragma once

#include "lua.hpp"
#include <string>
#include <vector>

namespace lich
{
	//-------------------------------------------------------------------------
	inline void push(lua_State* L, int v)
	{
		lua_pushinteger(L, v);
	}
	
	//-------------------------------------------------------------------------
	inline void push(lua_State* L, const std::string& v)
	{
		lua_pushlstring(L, v.c_str(), v.size());
	}

	//-------------------------------------------------------------------------
	inline void push(lua_State* L, const char* v)
	{
		lua_pushstring(L, v);
	}

	//-------------------------------------------------------------------------
	template<typename T>
	inline void push(lua_State* L, const std::vector<T>& vec)
	{
		lua_newtable(L);
		for (size_t i = 0; i < vec.size(); ++i)
		{
			push(L, vec[i]);
			lua_rawseti(L, -2, i + 1);
		}
	}

	//-------------------------------------------------------------------------
	template<typename CAR, typename... CDR>
	inline void pushv(lua_State* L, CAR&& car, CDR&&... cdr)
	{
		push(L, car);
		pushv(L, cdr);
	}

	inline void pushv(lua_State*)
	{
	}
}