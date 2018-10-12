#pragma once

#include "terminalapi/xplatapi.hpp"

#ifdef _WIN32
	#include "terminalapi/winapi.hpp"
#else
	#include "terminalapi/unixapi.hpp"
#endif