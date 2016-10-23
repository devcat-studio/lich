#pragma once

#include "ref.h"
#include "top_guard.h"
#include <functional>
#include <tuple>
#include <cassert>

namespace lich
{
	int error_handler_proxy(lua_State* L);

	//-------------------------------------------------------------------------
	template<typename TUPLE, int I, int LEFT>
	struct for_tuple
	{
		static void push(lua_State* L, const TUPLE& t)
		{
			lich::push(L, std::get<I>(t));
			for_tuple<TUPLE, I + 1, LEFT - 1>::push(L, t);
		};
		static void to(lua_State* L, int stack_base, TUPLE& t)
		{
			lich::to(L, stack_base + I, std::get<I>(t));
			for_tuple<TUPLE, I + 1, LEFT - 1>::to(L, stack_base, t);
		};
	};

	template<typename TUPLE, int I>
	struct for_tuple<TUPLE, I, 0>
	{
		static void push(lua_State*, const TUPLE&) {}
		static void to(lua_State*, int, TUPLE&) {}
	};

	//-------------------------------------------------------------------------
	// 에러가 발생하면 function<> 으로 콜백받는다.
	// 에러가 발생하지 않으면 true 리턴.
	template<typename ARG_TUPLE, typename RET_TUPLE>
	bool xpcall(
		lua_State* L,
		int funcIdx,
		const ARG_TUPLE& args,
		RET_TUPLE& rets,
		std::function<void(const std::string&)> errorHandler)
	{
		funcIdx = unpseudo(L, funcIdx);
		assert(lua_type(L, funcIdx) == LUA_TFUNCTION);

		top_guard _(L);
		lua_pushlightuserdata(L, (void*)&errorHandler);
		lua_pushcclosure(L, &error_handler_proxy, 1);
		int errorHandlerIdx = lua_gettop(L);
		lua_pushvalue(L, funcIdx);
		constexpr size_t NARGS = std::tuple_size<ARG_TUPLE>::value;
		constexpr size_t NRETS = std::tuple_size<RET_TUPLE>::value;
		for_tuple<ARG_TUPLE, 0, NARGS>::push(L, args);
		auto pcall_result = lua_pcall(L, (int)NARGS, (int)NRETS, errorHandlerIdx);
		if (pcall_result == 0)
		{
			for_tuple<RET_TUPLE, 0, NRETS>::to(L, lua_gettop(L) - (int)NRETS + 1, rets);
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
		std::pair<bool, std::string> rv(true, std::string());
		auto ehandler = [&rv](const std::string& e) { rv.second = e; };
		rv.first = xpcall(L, funcIdx, args, rets, ehandler);
		return rv;
	}

	//-------------------------------------------------------------------------
	// 에러가 발생한 시점의 내용과 스택 트레이스를 리턴.second로 받는다.
	// 에러가 발생하지 않으면 리턴.first = true
	template<typename ARG_TUPLE, typename RET_TUPLE>
	std::pair<bool, std::string> xpcall(
		const ref& funcRef,
		const ARG_TUPLE& args,
		RET_TUPLE& rets,
		std::function<void(const std::string&)> errorHandler)
	{
		lua_State* L = funcRef.lua_state();
		top_guard _(L);
		push(L, funcRef);
		return xpcall(L, -1, args, rets, errorHandler);
	}

	//-------------------------------------------------------------------------
	// 에러가 발생한 시점의 내용과 스택 트레이스를 리턴.second로 받는다.
	// 에러가 발생하지 않으면 리턴.first = true
	template<typename ARG_TUPLE, typename RET_TUPLE>
	std::pair<bool, std::string> pcall(
		const ref& funcRef,
		const ARG_TUPLE& args,
		RET_TUPLE& rets)
	{
		lua_State* L = funcRef.lua_state();
		top_guard _(L);
		push(L, funcRef);
		return pcall(L, -1, args, rets);
	}
}
