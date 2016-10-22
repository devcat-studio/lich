#pragma once

#include "to.h"
#include "top_guard.h"
#include <functional>

namespace lich
{
	int error_handler(lua_State* L);

	template<typename RET, typename... ARGS>
	RET pcall(
		lua_State* L,
		int funcIdx,
		std::function<void (const std::string&)> errorHandler,
		ARGS&&... args)
	{
		top_guard _(L);
		lua_pushlightuserdata(L, &errorHandler);
		lua_pushcclosure(L, &error_handler, 1);
		int errorHandlerIdx = lua_top(L);
		lua_pushvalue(L, funcIdx);
		pushv(L, args...);		
		lua_pcall(L, lua_top(L) - errorHandlerIdx - 1, 1, errorHandlerIdx);
		RET ret;
		to(L, -1, ret);
		return ret;
	}
}