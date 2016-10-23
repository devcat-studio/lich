#pragma once

#include <iostream>

#ifdef _WIN32
	#define WIN32_LEAN_AND_MEAN
	#include <windows.h>
#else
	#define _CrtDbgBreak()
#endif

#define MUST_TRUE(cond)	\
	if (!(cond)) { \
		cerr << "FAILED: MUST_TRUE(" << #cond << ")" << endl; \
		_CrtDbgBreak(); \
		exit(1); \
	}

#define MUST_EQUAL(a, b) \
	if ((a) != (b)) { \
		cerr << "FAILED: MUST_EQUAL(" << #a << ", " << #b << ")" << endl; \
		_CrtDbgBreak(); \
		exit(1); \
	}

