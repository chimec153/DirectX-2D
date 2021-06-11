#pragma once

#include "GameMode.h"


class CMainGameMode :
	public CGameMode
{
public:
	CMainGameMode();
	~CMainGameMode();

private:
	static std::shared_ptr<CGameObject>	m_pRideWindow;
	static std::shared_ptr<CGameObject>	m_pContructWindow;

public:
	bool Init();
	bool SetUI();
	void CreateButton();
	void CreatePanel();
	void CreateProto();

public:
	void LoadTexture();
	void LoadAnimation2D();

public:
	static void ShowRideWindow(float fTime);
	static void ShowConstructWindow(float fTime);
	static void CloseRideWindow(float fTime);
	void StartScene(float);
	void LoadExcel(const char* pFileName, IMAGE_TYPE eType = IMAGE_TYPE::IT_ATLAS);
	void LoadSeq(const char* pFileName);

public:
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);
};

