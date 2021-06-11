#pragma once
#include "GameMode.h"
class CStartGameMode :
	public CGameMode
{
	friend class CScene;

private:
	CStartGameMode();
	virtual ~CStartGameMode();

public:
	bool Init();
	void NextScene(float fTime);
	void EndGame(float fTime);
	void LoadResource();
};

