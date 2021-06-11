#pragma once
#include "GameMode.h"
class CEditMode :
	public CGameMode
{
	friend class CScene;
	friend class CEditor3D;

private:
	CEditMode();
	virtual ~CEditMode();

private:
	static std::shared_ptr<class CComponent> m_pCam;

public:
	virtual bool Init();

public:
	static void MoveX(const std::string& strKey, float, float);
	static void MoveY(const std::string& strKey, float, float);
	static void MoveZ(const std::string& strKey, float, float);
	static void RotY(const std::string& strKey, float, float);
};

