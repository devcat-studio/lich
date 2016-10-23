#pragma once

#include "lua.hpp"
#include <string>
#include <vector>
#include <map>

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

	//-------------------------------------------------------------------------
	template<typename K, typename V>
	inline void to(lua_State* L, int idx, std::map<K, V>& m)
	{
		idx = unpseudo(L, idx);
		m.clear();
		lua_pushnil(L);
		while (lua_next(L, idx) != 0)
		{
			K k;
			V v;
			to(L, -2, k);
			to(L, -1, v);
			m[k] = v;
			lua_pop(L, 1);
		}
	}
}
