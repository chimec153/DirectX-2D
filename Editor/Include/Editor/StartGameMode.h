#pragma once
#include "GameMode.h"
class CStartGameMode :
	public CGameMode
{
	friend class CScene;
	friend class CEditor3D;

private:
	CStartGameMode();
	virtual ~CStartGameMode();

private:
	std::shared_ptr<CGameObject>			m_pTitle;
	std::shared_ptr<CGameObject>			m_pOpening;
	float									m_fTime;
	std::shared_ptr<class CComponent>		m_pCam;
	std::shared_ptr<class CComponent>		m_pIDFont;
	std::shared_ptr<class CComponent>		m_pPWFont;

public:
	bool Init();
	bool LoadShader();
	void Update(float fTime);

public:
	bool LoadTexture();
	void Login(float fTime);
	void Button(float fTime);
	void Delete(float fTime);
	void SetIDText(const TCHAR* pID);
	void AddIDText(const TCHAR* w);
	void SetPWText(const TCHAR* pPW);
	void AddPWText(const TCHAR* w);

public:
	static void PacketFunc(const char* pPacket);

};

