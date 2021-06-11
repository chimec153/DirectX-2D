#include "Client.h"
#include "Engine.h"
#include "Client/Setting.h"
#include "Client/GameMode/StartGameMode.h"
#include "Scene/SceneManager.h"
#include "Client/ClientCreateSystem.h"
#include "Client/ClientInstance.h"
#include "Client/Socket/SocketManager.h"

int wWinMain(_In_ HINSTANCE hInst,
	_In_opt_ HINSTANCE hPrevInst,
	_In_ LPWSTR lpCmdLine,
	_In_ int nCmdShow)
{
	if (!GET_SINGLE(CEngine)->Init(TEXT("GameClient"),
		TEXT("GameClient"), hInst, IDI_ICON1, IDI_ICON1,
		1280, 1080))
	{
		DESTROY_SINGLE(CEngine);
		return 0;
	}

	GET_SINGLE(CSocketManager)->Init();

	GET_SINGLE(CClientCreateSystem)->Init();

	Setting();

	GET_SINGLE(CEngine)->SetInstance<CClientInstance>();

	GET_SINGLE(CSceneManager)->SetGameMode<CStartGameMode>();

	int iRetVal = GET_SINGLE(CEngine)->Run();

	DESTROY_SINGLE(CEngine);

	DESTROY_SINGLE(CClientCreateSystem);
	DESTROY_SINGLE(CSocketManager);

	return iRetVal;
}