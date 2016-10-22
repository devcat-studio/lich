#include "program.h"

using namespace std;

namespace lich
{
	pair<bool, string> push_program(
		lua_State* L,
		const char* code,
		size_t code_size,
		const char* name)
	{
		int r = luaL_loadbuffer(L, code, code_size, name);
		assert(lua_gettop(L) == 1);
		if (r == 0)
		{
			return make_pair(true, string());
		}
		else
		{
			string errmsg = lua_tostring(L, -1);
			lua_pop(L, 1);
			lua_pushnil(L);
			return make_pair(false, errmsg);
		}
	}
}
