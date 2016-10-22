#include "xpcall.h"

namespace lich
{
	int error_handler(lua_State* L)
	{
		(void)L;
		return 0;
	}
}
