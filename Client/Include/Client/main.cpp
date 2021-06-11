#include "Editor.h"
#include "StartGameMode.h"
#include "Scene/SceneManager.h"
#include "Editor3D.h"
#include "EditMode.h"
#include "GameMode/LoadingMode3D.h"
#include "GameMode/MainMode.h"
#include "GameMode/Town.h"
#include "Engine.h"
#include "GameMode/Dungeon.h"
#include "Server/ClientSocket.h"

int wWinMain(_In_ HINSTANCE hInst,
	_In_opt_ HINSTANCE hPrevInst,
	_In_ LPWSTR lpCmdLine,
	_In_ int nCmdShow)
{
	WSADATA tData = {};
	WSAStartup(MAKEWORD(2, 2), &tData);

	CClientSocket* pSocket = CClientSocket::CreateInst();

	pSocket->Init(666, L"127.0.0.1");

	

#ifdef _DEBUG
	if (_heapchk() != _HEAPOK)
		DebugBreak();
#endif
	if (!GET_SINGLE(CEngine)->Init(TEXT("ASSORT30TH Editor"),
		TEXT("ASSORT30TH Editor"), hInst, IDI_ICON1, IDI_ICON1,
		1920, 1080))
	{
		DESTROY_SINGLE(CEngine);
		return 0;
	}

	//GET_SINGLE(CEngine)->SetEditor();

	if (!GET_SINGLE(CEditor3D)->Init())
		return false;

	GET_SINGLE(CSceneManager)->SetGameMode<CStartGameMode>();

	int iRetVal = GET_SINGLE(CEngine)->Run();

	DESTROY_SINGLE(CEditor3D);

	DESTROY_SINGLE(CEngine);

	CClientSocket::DestroyInst();

	WSACleanup();

	return iRetVal;
}