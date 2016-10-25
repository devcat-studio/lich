#pragma once

#include "lua.hpp"
#include "unpseudo.h"
#include <memory>
#include <cassert>

namespace lich
{
	//-------------------------------------------------------------------------
	template<typename T>
	struct shared_ptr_userdata
	{
		std::shared_ptr<T> ptr;

		// 체크섬 용도. 뭔가 더 있었으면 좋겠지만, 아이디어가 떠오르지 않는다.
		size_t sizeOfT;

		static int gc(lua_State* L)
		{
			assert(lua_isuserdata(L, 1));
			assert(lua_objlen(L, 1) == sizeof(shared_ptr_userdata));
			void* vptr = lua_touserdata(L, 1);
			auto* s = reinterpret_cast<shared_ptr_userdata<T>*>(vptr);
			assert(s->sizeOfT == sizeof(T));
			s->~shared_ptr_userdata();
			return 0;
		}
	};

	//-------------------------------------------------------------------------
	template<typename T>
	inline void push(lua_State* L, const std::shared_ptr<T>& p)
	{
		void* ptr = lua_newuserdata(L, sizeof(shared_ptr_userdata<T>));
		auto* sptr = new (ptr) shared_ptr_userdata<T>();
		sptr->ptr = p;
		sptr->sizeOfT = sizeof(T);
		
		lua_newtable(L);
		lua_pushcfunction(L, &shared_ptr_userdata<T>::gc);
		lua_setfield(L, -2, "__gc");
		lua_setmetatable(L, -2);
	}

	//-------------------------------------------------------------------------
	template<typename T>
	inline void to(lua_State* L, int idx, std::shared_ptr<T>& p)
	{
		p.reset();
		idx = unpseudo(L, idx);
		if (!lua_isuserdata(L, 1)) { return; }
		if (lua_objlen(L, 1) != sizeof(shared_ptr_userdata<T>)) { return; }
		void* vptr = lua_touserdata(L, 1);
		auto* s = reinterpret_cast<shared_ptr_userdata<T>*>(vptr);
		if (s->sizeOfT != sizeof(T)) { return; }
		p = s->ptr;
	}
}
