#pragma once

#include "push.h"
#include "to.h"
#include <cassert>

namespace lich
{
	class ref;

	//-------------------------------------------------------------------------
	void push(lua_State* L, const ref& v);
	void to(lua_State* L, int idx, ref& v);

	///////////////////////////////////////////////////////////////////////////
	// 임의의 루아 값에 대한 참조.
	// 자신의 포인터를 키로 하여, 루아 VM에 들어있는 특정 테이블에 항목을 만든다.
	class ref
	{
		lua_State* L;
	public:
		friend void push(lua_State* L, const ref& v);
		friend void to(lua_State* L, int idx, ref& v);

		ref();
		~ref();
		void release();
		bool is_valid() const { return (L != nullptr); }

		// C++ 값으로 바로 변환
		template<typename T>
		void to(T& v)
		{
			assert(L);
			lich::push(L, *this);
			lich::to(L, -1, v);
			lua_pop(L, 1);
		}

		// heap_ref를 복사하는 동작은 썩 가볍지 않으므로 구현하지 않았다.
		// 컨테이너에 넣어야 하면 unique_ptr 등을 권장.
		// 꼭 복사하고 싶으면 SOURCE.to(TARGET) 을 쓰면 된다.
		ref(const ref&) = delete;
		void operator=(const ref&) = delete;
	};

	//-------------------------------------------------------------------------
	// 루아 VM에 들어있는 ref 테이블의 개수를 센다.
	// 디버그용. 느리다.
	size_t count_all_ref_for_debug(lua_State* L);

	//-------------------------------------------------------------------------
	// 초기화. LUA_REGISTRYINDEX 에 ref 테이블을 만든다.
	// 이것을 해야 ref 관련 기능을 사용할 수 있다.
	// 여기에 전달된 lua_State는 모든 ref 인스턴스가 갖고 있게 되므로
	// lua_newstate에서 리턴받은 lua_State를 넘겨야 한다.
	void enable_ref(lua_State* L);
}
