#pragma once

#include "GameEngine.h"

class CClientCreateSystem
{
public:
	bool Init();
	std::shared_ptr<class CGameObject> CreateObject(int iObjType);
	class CGameMode* CreateGameMode(int iType);

	DECLARE_SINGLE(CClientCreateSystem)
};

