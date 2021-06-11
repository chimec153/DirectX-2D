#pragma once

#include "resource.h"
#include "GameEngine.h"
#include <WinSock2.h>

//#pragma comment(lib, "ws2_32.lib")

#ifdef _WIN64

#ifdef _DEBUG
#pragma comment(lib, "GameEngine_Debug.lib")
#else
#pragma comment(lib, "GameEngine.lib")
#endif

#else

#ifdef _DEBUG
#pragma comment(lib, "GameEngine32_Debug.lib")
#else
#pragma comment(lib, "GameEngine32.lib")
#endif

#endif