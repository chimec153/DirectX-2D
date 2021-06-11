#include "Client.h"
#include "Client/StartGameMode.h"
#include "Scene/SceneManager.h"

int wWinMain(_In_ HINSTANCE hInst,
	_In_opt_ HINSTANCE hPrevInst,
	_In_ LPWSTR lpCmdLine,
	_In_ int nCmdShow)
{
	if (!GET_SINGLE(CEngine)->Init(TEXT("Client"),
		TEXT("Client"), hInst, IDI_ICON1, IDI_ICON1,
		1280, 1080))
	{
		DESTROY_SINGLE(CEngine);
		return 0;
	}

	GET_SINGLE(CSceneManager)->SetGameMode<CStartGameMode>();

	int iRetVal = GET_SINGLE(CEngine)->Run();

	DESTROY_SINGLE(CEngine);

	return iRetVal;
}