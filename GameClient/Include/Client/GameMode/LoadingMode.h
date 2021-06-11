#pragma once
#include "GameMode.h"
class CLoadingMode :
	public CGameMode
{
	friend class CScene;

private:
	CLoadingMode();
	~CLoadingMode();

private:
	std::shared_ptr<class CGameObject> m_pBar;
	class CLoadingThread* m_pThread;

public:
	std::shared_ptr<class CGameObject> GetBar()	const
	{
		return m_pBar;
	}

public:
	bool Init();
	void Update(float fTime);
	
};

