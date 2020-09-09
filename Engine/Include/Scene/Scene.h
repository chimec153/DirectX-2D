#pragma once

#include "../GameEngine.h"
#include "../GameMode.h"

class CScene
{
	friend class CSceneManager;

private:
	CScene();
	~CScene();

private:
	class CGameMode*					m_pGameMode;
	std::list<class CGameObject*>		m_ObjList;

public:
	class CGameMode* SetGameMode()
	{
		SAFE_DELETE(m_pGameMode);

		m_pGameMode = new CGameMode;

		if (!m_pGameMode->Init())
		{
			SAFE_DELETE(m_pGameMode);

			return nullptr;
		}

		return m_pGameMode;
	}

public:
	bool Init();
	void Start();
	void Update(float fTime);
	void PostUpdate(float fTime);
	void Collision(float fTime);
	void PreRender(float fTime);
	void Render(float fTime);
	void PostRender(float fTime);
};

