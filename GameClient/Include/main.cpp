#include "Client.h"
#include "Engine.h"

int wWinMain(_In_ HINSTANCE hInst,
	_In_opt_ HINSTANCE hPrevInst,
	_In_ LPWSTR lpCmdLine,
	_In_ int nCmdShow)
{
	if (!GET_SINGLE(CEngine)->Init(TEXT("GameClient"),
		TEXT("GameClient"), hInst, IDI_ICON1, IDI_ICON1,
		1280, 720))
	{
		DESTROY_SINGLE(CEngine);
		return 0;
	}

	int iRetVal = GET_SINGLE(CEngine)->Run();

	DESTROY_SINGLE(CEngine);

	return iRetVal;
}