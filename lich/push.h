#pragma once

#include "lua.hpp"
#include <string>
#include <vector>
#include <map>

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
			lua_rawseti(L, -2, (int)i + 1);
		}
	}

	//-------------------------------------------------------------------------
	template<typename K, typename V>
	inline void push(lua_State* L, const std::map<K, V>& m)
	{
		lua_newtable(L);
		for (const auto& kv : m)
		{
			push(L, kv.first);
			push(L, kv.second);
			lua_rawset(L, -3);
		}
	}
}