#pragma once

#include "unpseudo.h"
#include "ref.h"
#include "top_guard.h"
#include <functional>
#include <tuple>
#include <cassert>
#include <string>

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
		static void repeat_lookup(lua_State* L, const TUPLE& t)
		{
			if (lua_type(L, -1) == LUA_TTABLE ||
				lua_type(L, -1) == LUA_TUSERDATA ||
				lua_type(L, -1) == LUA_TSTRING)
			{
				lich::push(L, std::get<I>(t));
				lua_gettable(L, -2);
				for_tuple<TUPLE, I + 1, LEFT - 1>::repeat_lookup(L, t);
			}
			else
			{
				lua_pop(L, 1);
				lua_pushnil(L);
			}
		}
	};

	template<typename TUPLE, int I>
	struct for_tuple<TUPLE, I, 0>
	{
		static void push(lua_State*, const TUPLE&) {}
		static void to(lua_State*, int, TUPLE&) {}
		static void repeat_lookup(lua_State*, const TUPLE&) {}
	};

	//-------------------------------------------------------------------------
	// xpcall(스택 인덱스)
	template<typename ARG_TUPLE, typename RET_TUPLE>
	bool xpcall(
		lua_State* L,
		int funcIdx,
		const ARG_TUPLE& args,
		RET_TUPLE& rets,
		std::function<void(const std::string&)> errorHandler)
	{
		funcIdx = unpseudo(L, funcIdx);

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
	// pcall(스택 인덱스)
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
	inline void push_function(lua_State* L, const char* name)
	{
		lua_getglobal(L, name);
	}

	inline void push_function(lua_State* L, const ref& obj)
	{
		push(L, obj);
	}

	inline void push_function_seed(lua_State* L, const char* name)
	{
		lua_getglobal(L, name);
	}

	inline void push_function_seed(lua_State* L, const std::string& name)
	{
		lua_getglobal(L, name.c_str());
	}

	inline void push_function_seed(lua_State* L, const ref& obj)
	{
		push(L, obj);
	}

	template<typename... ARGS>
	inline void push_function(lua_State* L, const std::tuple<ARGS...>& name)
	{
		push_function_seed(L, std::get<0>(name));
		for_tuple<std::tuple<ARGS...>, 1, sizeof...(ARGS)-1>::repeat_lookup(L, name);
	}

	//-------------------------------------------------------------------------
	// xpcall(글로벌/모듈 함수 이름)
	template<typename FUNC, typename ARG_TUPLE, typename RET_TUPLE>
	std::pair<bool, std::string> xpcall(
		lua_State* L,
		const FUNC& fn,
		const ARG_TUPLE& args,
		RET_TUPLE& rets,
		std::function<void(const std::string&)> errorHandler)
	{
		top_guard _(L);
		push_function(L, fn);
		return xpcall(L, -1, args, rets, errorHandler);
	}

	//-------------------------------------------------------------------------
	// pcall(글로벌/모듈 함수 이름)
	template<typename FUNC, typename ARG_TUPLE, typename RET_TUPLE>
	std::pair<bool, std::string> pcall(
		lua_State* L,
		const FUNC& fn,
		const ARG_TUPLE& args,
		RET_TUPLE& rets)
	{
		top_guard _(L);
		push_function(L, fn);
		return pcall(L, -1, args, rets);
	}

	//-------------------------------------------------------------------------
	// xpcall(ref). L을 생략하는 버전.
	template<typename ARG_TUPLE, typename RET_TUPLE>
	std::pair<bool, std::string> xpcall(
		const ref& funcRef,
		const ARG_TUPLE& args,
		RET_TUPLE& rets,
		std::function<void(const std::string&)> errorHandler)
	{
		return xpcall(funcRef.lua_state(), funcRef, args, rets, errorHandler);
	}

	//-------------------------------------------------------------------------
	// pcall(ref). L을 생략하는 버전.
	template<typename ARG_TUPLE, typename RET_TUPLE>
	std::pair<bool, std::string> pcall(
		const ref& funcRef,
		const ARG_TUPLE& args,
		RET_TUPLE& rets)
	{
		return pcall(funcRef.lua_state(), funcRef, args, rets);
	}
}
