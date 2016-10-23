#pragma once

#include "push.h"
#include "to.h"

namespace lich
{
	///////////////////////////////////////////////////////////////////////////
	// ������ ��� ���� ���� ����.
	// �ڽ��� �����͸� Ű�� �Ͽ�, ��� VM�� ����ִ� Ư�� ���̺� �׸��� �����.
	class ref
	{
		lua_State* L = nullptr;
	public:
		friend void push(lua_State* L, const ref& v);
		friend void to(lua_State* L, int idx, ref& v);

		ref();
		~ref();
		void release();

		// heap_ref�� �����ϴ� ������ �� ������ �����Ƿ� �Ϻη� ���� �� ��.
		// ���� �����ϰ� ������ push�ؼ� �������� ������ �� �� �ν��Ͻ��� ������.
		ref(const ref&) = delete;
		void operator=(const ref&) = delete;
	};

	//-------------------------------------------------------------------------
	void push(lua_State* L, const ref& v);
	void to(lua_State* L, int idx, ref& v);

	//-------------------------------------------------------------------------
	// ��� VM�� ����ִ� ref ���̺��� ������ ����.
	// ����׿�. ������.
	size_t count_all_ref_for_debug(lua_State* L);

	//-------------------------------------------------------------------------
	// �ʱ�ȭ. LUA_REGISTRYINDEX �� ref ���̺��� �����.
	// ���⿡ ���޵� lua_State�� ��� ref �ν��Ͻ��� ���� �ְ� �ǹǷ�
	// lua_newstate���� ���Ϲ��� lua_State�� �Ѱܾ� �Ѵ�.
	void enable_ref(lua_State* L);
}
