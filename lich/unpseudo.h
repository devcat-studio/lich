#pragma once

#include "lua.hpp"

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
