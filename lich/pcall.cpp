#include "pcall.h"
#include "top_guard.h"

using namespace std;

namespace lich
{
	int error_handler_proxy(lua_State* L)
	{
		void* u = lua_touserdata(L, lua_upvalueindex(1));
		if (u == nullptr)
		{
			// WHAT THE FUCK???
			assert(false);
		}

		string text;
		{
			top_guard _(L);
			lua_getglobal(L, "debug");
			if (lua_istable(L, -1))
			{
				lua_getfield(L, -1, "traceback");
				lua_pushvalue(L, 1);
				lua_pushnumber(L, 2);
				lua_call(L, 2, 1);
			}
			else
			{
				lua_pop(L, 1);
			}
			const char* s = lua_tostring(L, -1);
			if (s == nullptr)
			{
				s = "(null)";
			}
			text = s;
		}

		auto& actual_handler = *reinterpret_cast<function<void(const string&)>*>(u);
		actual_handler(text);
		return 0;
	}
}
