#pragma once

#include "lua.hpp"
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
}
