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
	CGameMode*							m_pGameMode;
	class CSceneResource*				m_pResource;
	std::list<class CGameObject*>		m_ObjList;

public:
	CGameMode* GetGameMode()	const
	{
		return m_pGameMode;
	}

	class CSceneResource* GetResource()	const
	{
		return m_pResource;
	}

public:
	template <typename T>
	bool SetGameMode()
	{
		SAFE_DELETE(m_pGameMode);

		m_pGameMode = new T;

		m_pGameMode->m_pScene = this;

		if (!m_pGameMode->Init())
		{
			SAFE_DELETE(m_pGameMode);

			return nullptr;
		}

		return m_pGameMode;
	}

public:
	template <typename T>
	T* CreateObject(const std::string& strName)
	{
		T* pObj = new T;

		pObj->SetName(strName);
		pObj->m_pScene = this;

		if (!pObj->Init())
		{
			SAFE_RELEASE(pObj);
			return nullptr;
		}

		pObj->AddRef();
		m_ObjList.push_back(pObj);

		return pObj;
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

