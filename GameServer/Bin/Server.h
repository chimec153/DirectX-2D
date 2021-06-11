#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <WS2tcpip.h>
//#include "GameEngine.h"
//
//#ifdef _DEBUG
//#ifdef _WIN64
//#pragma comment(lib, "GameEngine_Debug.lib")
//#else
//#pragma comment(lib, "GameEngine32_Debug.lib")
//#endif
//#else
//#ifdef _WIN64
//#pragma comment(lib, "GameEngine.lib")
//#else
//#pragma comment(lib, "GameEngine32.lib")
//#endif
//#endif
#include <unordered_map>
#include <assert.h>

namespace Server
{
#define BUF_SIZE	1024

	enum class SERVER_TYPE
	{
		MAIN,
		THREAD,
		END
	};

	enum class NET_SERVER_TYPE
	{
		IOCP,
		EVENT_SELECT,
		THREAD,
		END
	};

	enum class WORK_TYPE
	{
		CONNECT,
		RECV,
		SEND
	};

	typedef struct _tagOverpart
	{
		OVERLAPPED		overlapped;
		SOCKET			Sock;
		WSABUF			Buf;
		WORK_TYPE		eType;
		DWORD			iFlag;
		char			strBuf[BUF_SIZE];
		unsigned long	iCount;
		class CUser* pUser;
	}OVERPART, * POVERPART;
}