#include "../lich/all.h"
#include "must.h"

using namespace std;

void containers_test()
{
	lua_State* L = luaL_newstate();
	luaL_openlibs(L);
	lich::enable_ref(L);

	// map
	{
		lich::ref fn;
		string source =
			"\n"
			"\n	return function(t)"
			"\n		local rv = {}"
			"\n		for k, v in pairs(t) do"
			"\n			rv[v] = k"
			"\n		end"
			"\n		return rv"
			"\n	end";
		tuple<lich::ref&> run_result(fn);
		MUST_TRUE(lich::run_program(L, source, "", run_result).first);
		MUST_TRUE(fn.is_valid());

		map<string, int> m;
		m["사십이"] = 42;
		m["오백칠십삼"] = 573;

		tuple<map<int, string>> ret;
		MUST_TRUE(lich::pcall(fn, tie(m), ret).first);
		auto& mm = get<0>(ret);
		MUST_EQUAL(mm[42], "사십이");
		MUST_EQUAL(mm[573], "오백칠십삼");
	}

	// vector
	{
		lich::ref fn;
		string source =
			"\n"
			"\n	return function(t)"
			"\n		local rv = {}"
			"\n		for i, v in ipairs(t) do"
			"\n			rv[#t + 1 - i] = v"
			"\n		end"
			"\n		return rv"
			"\n	end";
		tuple<lich::ref&> run_result(fn);
		MUST_TRUE(lich::run_program(L, source, "", run_result).first);
		MUST_TRUE(fn.is_valid());

		vector<string> v;
		v.push_back("비워주세요");
		v.push_back("하늘을");
		v.push_back("위해");
		v.push_back("절");

		tuple<vector<string>> ret;
		MUST_TRUE(lich::pcall(fn, tie(v), ret).first);
		auto& vv = get<0>(ret);
		MUST_EQUAL(vv[0], "절");
		MUST_EQUAL(vv[1], "위해");
		MUST_EQUAL(vv[2], "하늘을");
		MUST_EQUAL(vv[3], "비워주세요");
	}

	lua_close(L);
}
