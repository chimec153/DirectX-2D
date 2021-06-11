#include "Engine.h"
#include "Server/Server.h"
#include "Core.h"
#include "Input.h"
#include "Server/ServerIOCP.h"

int wWinMain(HINSTANCE hPrev, HINSTANCE hInst, TCHAR* pText, int i)
{
	WSAData tData;
	WSAStartup(MAKEWORD(2, 2), &tData);

	//if (!GET_SINGLE(CEngine)->Init(L"Server", L"Server", hInst, 0, 0, 1280, 720))
	//{
	//	DESTROY_SINGLE(CEngine);
	//	return 0;
	//}

	CServer* pServer = new CServer;

	if (!pServer->Init<CServerIOCP>(UPDATER_TYPE::IOCP, 666, L""))
	{
		SAFE_DELETE(pServer);
		//DESTROY_SINGLE(CEngine);
		WSACleanup();
		return 0;
	}

	while (true)
	{
		if (GetAsyncKeyState(VK_ESCAPE))
		{
			break;
		}
	}

	int iRetVal = 0;// GET_SINGLE(CEngine)->Run();

	//DESTROY_SINGLE(CEngine);

	SAFE_DELETE(pServer);

	WSACleanup();

	return iRetVal;
}