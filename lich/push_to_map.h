#pragma once

#include "unpseudo.h"
#include "lua.hpp"
#include <map>

namespace lich
{
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
