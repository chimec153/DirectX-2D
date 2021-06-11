#pragma once

#include "../GameEngine.h"
#include "../GameMode.h"
#include "../Object/GameObject.h"
#include "../Layer/Layer.h"

class CScene
{
	friend class CSceneManager;

private:
	CScene();
	~CScene();

private:
	CGameMode* m_pGameMode;
	std::list<class CLayer*>	m_LayerList;
	SCENE_TYPE					m_eType;
	class CSceneCollision* m_pCollision;

public:
	static std::unordered_map<std::string, std::shared_ptr<class CGameObject>>	m_mapProtoType[(int)SCENE_TYPE::ST_END];

private:
	std::function<void(const std::string&)>		m_EditorCreate;
	std::function<void(const std::string&)>		m_EditorDelete;
	std::function<std::shared_ptr<class CGameObject>(int)>		m_EditorCreateObj;
	std::function<class CGameMode* (int)>		m_EditorCreateGameMode;
	static std::function<void(const std::string&)>		m_EditorProtoCreate;
	

public:
	CGameMode* GetGameMode()	const
	{
		return m_pGameMode;
	}

	SCENE_TYPE GetSceneType()	const;

	class CSceneCollision* GetCollision()	const
	{
		return m_pCollision;
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

			return false;
		}

		return m_pGameMode;
	}

public:
	template <typename T>
	std::shared_ptr<CGameObject> CreateObject(const std::string& strName, class CLayer* pLayer)
	{
		std::shared_ptr<CGameObject> pObj = std::shared_ptr<CGameObject>(new T);

		pObj->SetName(strName);
		pLayer->AddObj(pObj);

		if (!pObj->Init())
		{
			return nullptr;
		}

		if (m_EditorCreate)
			m_EditorCreate(strName);

		pObj->m_EditorDelete = m_EditorDelete;

		return pObj;
	}

	std::shared_ptr<CGameObject> CreateCloneObj(const std::string& strTag, const std::string& strProto,
		class CLayer* pLayer, SCENE_TYPE eType = SCENE_TYPE::ST_CURRENT);

	template <typename T>
	static std::shared_ptr<CGameObject> CreateProtoObj(const std::string& strTag, class CScene* pScene, SCENE_TYPE eType = SCENE_TYPE::ST_CURRENT)
	{
		std::shared_ptr<CGameObject> pObj = std::shared_ptr<CGameObject>(new T);

		pObj->m_pScene = pScene;
		pObj->SetName(strTag);

		if (!pObj->Init())
		{
			return nullptr;
		}

		if (m_EditorProtoCreate)
			m_EditorProtoCreate(strTag);

		m_mapProtoType[(int)eType].insert(std::make_pair(strTag, pObj));

		return pObj;
	}

	class CLayer* FindLayer(const std::string& strTag)	const;
	class CLayer* FindLayer(int iZOrder)	const;
	void CreateLayer(const std::string& strTag, int iZOrder = 0);

private:
	static std::shared_ptr<CGameObject> FindProtoType(const std::string& strTag, SCENE_TYPE eType)
	{
		std::unordered_map<std::string, std::shared_ptr<CGameObject>>::iterator iter = m_mapProtoType[(int)eType].find(strTag);

		if (iter == m_mapProtoType[(int)eType].end())
			return nullptr;

		return iter->second;
	}

public:
	template <typename T>
	void SetEditorCreateFunc(T* pObj, void(T::* pFunc)(const std::string&))
	{
		m_EditorCreate = std::bind(pFunc, pObj, std::placeholders::_1);
	}

	template <typename T>
	void SetEditorDeleteFunc(T* pObj, void(T::* pFunc)(const std::string&))
	{
		m_EditorDelete = std::bind(pFunc, pObj, std::placeholders::_1);
	}

	template <typename T>
	void SetEditorCreateObjFunc(T* pObj, class CGameObject* (T::* pFunc)(int))
	{
		m_EditorCreateObj = std::bind(pFunc, pObj, std::placeholders::_1);
	}

	template <typename T>
	void SetEditorCreateGameModeFunc(T* pObj, class CGameMode* (T::* pFunc)(int))
	{
		m_EditorCreateGameMode = std::bind(pFunc, pObj, std::placeholders::_1);
	}

	void EditorDelete(const std::string& strTag)
	{
		if (m_EditorDelete)
			m_EditorDelete(strTag);
	}

	std::shared_ptr<CGameObject> EditorCreateObj(int iObjType)
	{
		return m_EditorCreateObj(iObjType);
	}

	class CGameMode* EditorCreateGameMode(int iObjType)
	{
		if (m_EditorCreateGameMode)
		{
			return m_EditorCreateGameMode(iObjType);
		}

		return new CGameMode;
	}

	void DeleteEditorDelete();

public:
	bool Init();
	void Start();
	void Input(float fTime);
	void Update(float fTime);
	void PostUpdate(float fTime);
	void Collision(float fTime);
	void CheckFrustum();
	void PreRender(float fTime);
	void Render(float fTime);
	void PostRender(float fTime);

public:
	void Save(FILE* pFile);
	void SaveFromFullPath(const char* pFileName);
	void Load(FILE* pFile);
	void LoadFromFullPath(const char* pFileName);

public:
	void ChangeProto();

	public:
		void ShowWindow();

private:
	static bool SortZ(class CLayer* pSrc, class CLayer* pDst);
};

