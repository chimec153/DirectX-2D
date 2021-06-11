#pragma once

#include "Server.h"

#ifdef _DEBUG

#ifdef _WIN64
#pragma comment(lib, "Server_Debug.lib")
#else
#pragma comment(lib, "Server_Debug32.lib")
#endif

#else

#ifdef _WIN64
#pragma comment(lib, "Server.lib")
#else
#pragma comment(lib, "Server32.lib")
#endif

#endif