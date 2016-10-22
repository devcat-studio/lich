#pragma once

#include "lua.hpp"
#include "xpcall.h"
#include <utility>
#include <string>

namespace lich
{
	///////////////////////////////////////////////////////////////////////////
	// 루아 스크립트를 로드해서 스택에 푸시한다.
	// 여기에 전달한 name은 스택 트레이스 등 디버그 정보에 활용된다.
	// 컴파일 에러가 발생하면 스택에는 nil이 푸시되고,
	// <false, 에러메시지>가 리턴된다.
	std::pair<bool, std::string> push_program(
		lua_State* L,
		const char* code,
		size_t code_size,
		const char* name);

	///////////////////////////////////////////////////////////////////////////
	// 루아 스크립트를 바로 실행한다.
	// 여기에 전달한 name은 스택 트레이스 등 디버그 정보에 활용된다.
	// 루아 스택은 잠시 사용하고 원래대로 복원한다.
	// 정상 수행되면 <true, ""> 가,
	// 컴파일 에러 혹은 실행 에러가 발생하면 <false, 에러메시지>가 리턴된다.
	template<typename RET_TUPLE>
	std::pair<bool, std::string> run_program(
		lua_State* L,
		const char* code,
		size_t code_size,
		const char* name,
		RET_TUPLE& ret)
	{
		auto compile_r = push_program(L, code, code_size, name);
		if (compile_r.first == false)
		{
			return compile_r;
		}
		
		pair<bool, std::string> rv(true, string());
		auto ehandler = [&rv](const std::string& msg)
		{
			rv.first = false;
			rv.second = msg;
		};

		tuple<> args;
		xpcall(L, -1, ehandler, args, ret);
		return rv;
	}
}
