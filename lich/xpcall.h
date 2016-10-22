#pragma once

#include "to.h"
#include "top_guard.h"
#include <functional>

namespace lich
{
	int error_handler(lua_State* L);

	template<typename ARG_TUPLE, typename RET_TUPLE>
	void xpcall(
		lua_State* L,
		int funcIdx,
		std::function<void (const std::string&)> errorHandler,
		const ARG_TUPLE& args,
		RET_TUPLE& rets)
	{
		if (funcIdx < 0)
		{
			funcIdx = lua_gettop(L) + 1 - funcIdx;
		}

		top_guard _(L);
		lua_pushlightuserdata(L, &errorHandler);
		lua_pushcclosure(L, &error_handler, 1);
		int errorHandlerIdx = lua_gettop(L);
		lua_pushvalue(L, funcIdx);
		pushv(L, args);
		if (lua_pcall(L, tuple_size<args>(), tuple_size<rets>(), errorHandlerIdx) == 0)
		{
			tov(L, lua_top() - tuple_size<rets>()  + 1, rets);
		}
		return ret;
	}
}