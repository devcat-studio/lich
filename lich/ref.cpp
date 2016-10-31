#include "ref.h"
#include "top_guard.h"
#include "program.h"
#include <cassert>
#include <cstring>

using namespace std;

namespace
{
	char master_lua_state_registry_key = ' ';
	char heap_ref_table_registry_key = ' ';
}

namespace lich
{
	ref::ref()
	{
		L = nullptr;
	}

	ref::~ref()
	{
		release();
	}

	void ref::release()
	{
		if (L == nullptr) { return; }
		top_guard _(L);

		lua_pushlightuserdata(L,&heap_ref_table_registry_key); // ~,&heap_ref_table_registry_key
		lua_rawget(L, LUA_REGISTRYINDEX);                       // ~, heap_ref_table
		assert(lua_istable(L, -1)&& "didn't call lich::enable_ref()?");
		lua_pushlightuserdata(L, this); // ~, heap_ref_table, this
		lua_pushnil(L);                 // ~, heap_ref_table, this, nil
		lua_rawset(L, -3);              // ~, heap_ref_table
		L = nullptr;
	}

	void push(lua_State* L, const ref& this_)
	{
		// 스택 뒷정리를 top_guard에게 맡기고 싶지만...
		// push는 스택을 그대로 복원하고 리턴하는 것이 아니라 값을 하나 쌓아야 하므로
		// top_guard를 쓰지 못하고 직접 한다
		int old_top = lua_gettop(L);

		lua_pushlightuserdata(L, &heap_ref_table_registry_key); // ~, &heap_ref_table_registry_key
		lua_rawget(L, LUA_REGISTRYINDEX);                       // ~, heap_ref_table
		assert(lua_istable(L, -1) && "didn't call lich::enable_ref()?");
		lua_pushlightuserdata(L, (void*)&this_);                // ~, heap_ref_table, this
		lua_rawget(L, -2);                                      // ~, heap_ref_table, v

		lua_remove(L, old_top + 1);
		assert(lua_gettop(L) == old_top + 1);
	}

	void to(lua_State* L, int idx, ref& this_)
	{
		this_.release();

		idx = unpseudo(L, idx);
		top_guard _(L);

		lua_pushlightuserdata(L, &heap_ref_table_registry_key); // ~, &heap_ref_table_registry_key
		lua_rawget(L, LUA_REGISTRYINDEX);                       // ~, heap_ref_table
		assert(lua_istable(L, -1) && "didn't call lich::enable_ref()?");
		lua_pushlightuserdata(L, (void*)&this_); // ~, heap_ref_table, this
		lua_pushvalue(L, idx);                   // ~, heap_ref_table, this, v
		lua_rawset(L, -3);                       // ~, heap_ref_table

		lua_pushlightuserdata(L, &master_lua_state_registry_key); // ~~, &master_lua_state_registry_key
		lua_rawget(L, LUA_REGISTRYINDEX);                         // ~~, master_L
		assert(lua_islightuserdata(L, -1) && "didn't call lich::enable_ref()?");
		void* u = lua_touserdata(L, -1);
		this_.L = reinterpret_cast<lua_State*>(u);
	}

	size_t count_all_ref_for_debug(lua_State* L)
	{
		top_guard _(L);

		const char* program =
			"\n"
			"\n return function(t)"
			"\n	  local n = 0"
			"\n   for k, v in pairs(t) do"
			"\n   --  print(k, v)"
			"\n     n = n + 1"
			"\n   end"
			"\n   return n"
			"\n end";

		tuple<ref> fn;
		run_program(L, program, strlen(program), "", fn);
		push(L, get<0>(fn));
		get<0>(fn).release(); // 이거 때문에 한 개 더 잡힘;;

		lua_pushlightuserdata(L, &heap_ref_table_registry_key);
		lua_rawget(L, LUA_REGISTRYINDEX);
		assert(lua_istable(L, -1) && "didn't call lich::enable_ref()?");

		lua_call(L, 1, 1);

		// ref 테이블 식별용으로 넣은 'lich' 때문에 하나를 뺀다.
		return lua_tointeger(L, -1) - 1;
	}

	void enable_ref(lua_State* L)
	{
#ifdef _DEBUG
		int old_top = lua_gettop(L);
#endif
		lua_pushlightuserdata(L, &heap_ref_table_registry_key); // ~,&heap_ref_table_registry_key
		lua_rawget(L, LUA_REGISTRYINDEX);                       // ~,&heap_ref_table_registry_key, heap_ref_table?
		if (lua_isnil(L, -1))
		{
			lua_pop(L, 1);

			lua_pushlightuserdata(L, &heap_ref_table_registry_key);    // ~,&heap_ref_table_registry_key
			lua_newtable(L);                                           // ~,&heap_ref_table_registry_key, heap_ref_table
			lua_pushstring(L, "https://github.com/lee-seungjae/lich"); // ~,&heap_ref_table_registry_key, heap_ref_table, https://...
			lua_setfield(L, -2, "lich");                               // ~,&heap_ref_table_registry_key, heap_ref_table
			lua_rawset(L, LUA_REGISTRYINDEX);                          // ~

			lua_pushlightuserdata(L, &master_lua_state_registry_key);  // ~,&master_lua_state_registry_key
			lua_pushlightuserdata(L, (void*)L);                        // ~,&master_lua_state_registry_key, master_L
			lua_rawset(L, LUA_REGISTRYINDEX);                          // ~
		}
		else if (lua_istable(L, -1))
		{
			lua_getfield(L, -1, "lich");
			const char* s = lua_tostring(L, -1);
			s = s ? s : "";
			if (strcmp(s, "https://github.com/lee-seungjae/lich") == 0)
			{
				lua_pop(L, 2);
			}
			else
			{
				// 복구 불가능한 심각한 에러.
				luaL_error(L, "lich: LUA_REGISTRY[heap_ref_table_registry_key].lich is wrong");
			}
		}
		else
		{
			// 복구 불가능한 심각한 에러.
			luaL_error(L, "lich: LUA_REGISTRY[heap_ref_table_registry_key] is not a table");
		}

#ifdef _DEBUG
		assert(old_top == lua_gettop(L));
#endif
	}
}
