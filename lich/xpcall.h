#pragma once

#include "push.h"
#include "to.h"
#include "top_guard.h"
#include <functional>
#include <tuple>
#include <cassert>

namespace lich
{
	int error_handler(lua_State* L);

	//-------------------------------------------------------------------------
	template<typename TUPLE, int LEFT>
	struct for_tuple
	{
		template<int I> static void push(lua_State* L, const TUPLE& t)
		{
			lich::push(L, std::get<I>(t));
			for_tuple<TUPLE, LEFT - 1>::push<I + 1>(L, t);
		};
		template<int I> static void to(lua_State* L, int stack_base, TUPLE& t)
		{
			lich::to(L, stack_base + I, std::get<I>(t));
			for_tuple<TUPLE, LEFT - 1>::to<I + 1>(L, stack_base, t);
		};
	};

	template<typename TUPLE>
	struct for_tuple<TUPLE, 0>
	{
		template<int I> static void push(lua_State*, const TUPLE&) {}
		template<int I> static void to(lua_State*, int, TUPLE&) {}
	};

	//-------------------------------------------------------------------------
	// 에러가 발생하면 function<> 으로 콜백받는다.
	// 에러가 발생하지 않으면 true 리턴.
	template<typename ARG_TUPLE, typename RET_TUPLE>
	bool xpcall(
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
		assert(lua_type(L, funcIdx) == LUA_TFUNCTION);

		top_guard _(L);
		lua_pushlightuserdata(L, &errorHandler);
		lua_pushcclosure(L, &error_handler, 1);
		int errorHandlerIdx = lua_gettop(L);
		lua_pushvalue(L, funcIdx);
		constexpr int NARGS = (int)std::tuple_size<typename ARG_TUPLE>();
		constexpr int NRETS = (int)std::tuple_size<typename RET_TUPLE>();
		for_tuple<typename ARG_TUPLE, NARGS>::push<0>(L, args);
		auto pcall_result = lua_pcall(L, NARGS, NRETS, errorHandlerIdx);
		if (pcall_result == 0)
		{
			for_tuple<typename RET_TUPLE, NRETS>::to<0>(L, lua_gettop(L) - NRETS + 1, rets);
			return true;
		}
		else
		{
			return false;
		}
	}

	//-------------------------------------------------------------------------
	// 에러가 발생한 시점의 내용과 스택 트레이스를 리턴.second로 받는다.
	// 에러가 발생하지 않으면 리턴.first = true
	template<typename ARG_TUPLE, typename RET_TUPLE>
	std::pair<bool, std::string> pcall(
		lua_State* L,
		int funcIdx,
		const ARG_TUPLE& args,
		RET_TUPLE& rets)
	{
		if (funcIdx < 0)
		{
			funcIdx = lua_gettop(L) + 1 + funcIdx;
		}
		auto rv = make_pair<bool, std::string>(true, std::string());
		auto ehandler = [&rv](const std::string& e) {
			rv.first = false;
			rv.second = e;
		};
		xpcall(L, funcIdx, ehandler, args, rets);
		return rv;
	}
}