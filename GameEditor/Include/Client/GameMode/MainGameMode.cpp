#include "MainGameMode.h"
#include "../Object/Player.h"
#include "Scene/Scene.h"

CMainGameMode::CMainGameMode()
{
}

CMainGameMode::~CMainGameMode()
{
}

bool CMainGameMode::Init()
{
	CGameObject* pPlayer = m_pScene->CreateObject<CPlayer>("Player");

	SetPlayer(pPlayer);

	SAFE_RELEASE(pPlayer);
	
	return true;
}
