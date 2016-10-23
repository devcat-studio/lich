#pragma once

#include "lua.hpp"
#include <string>

namespace lich
{
	//-------------------------------------------------------------------------
	inline void push(lua_State* L, const std::string& v)
	{
		lua_pushlstring(L, v.c_str(), v.size());
	}

	//-------------------------------------------------------------------------
	inline void to(lua_State* L, int idx, std::string& v)
	{
		size_t len = 0;
		const char* s = lua_tolstring(L, idx, &len);
		if (s == nullptr) { s = ""; }
		v.assign(s, len);
	}
}
