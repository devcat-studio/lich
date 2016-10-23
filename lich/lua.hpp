#pragma once

extern "C"
{
	#include "../lua-5.1.5/lua.h"
	#include "../lua-5.1.5/lualib.h"
	#include "../lua-5.1.5/lauxlib.h"
}

namespace lich
{
	inline int unpseudo(lua_State* L, int idx)
	{
		if (idx >= 0)
		{
			return idx;
		}
		else
		{
			return lua_gettop(L) + 1 + idx;
		}
	}
}
