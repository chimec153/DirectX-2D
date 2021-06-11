#pragma once

#include "../Ref.h"

class CLayer	:
	public CRef
{
	friend class CScene;

private:
	CLayer();
	~CLayer();

private:
	int								m_iZOrder;
	std::list<std::shared_ptr<class CGameObject>>	m_ObjList;
	bool							m_bStart;
	class CScene*					m_pScene;
	class CLayerInstance*			m_pInstance;
	std::shared_ptr<class CGameObject>		m_pAxisObj;
	std::shared_ptr<class CGameObject>		m_pSelectObj;

private:
	std::function<std::shared_ptr<class CGameObject>(int)>	m_EditorObjCreate;
	std::function<void(const std::string&)>		m_EditorCreate;
	std::function<void(const std::string&)>		m_EditorDelete;

public:
	void SetZOrder(int iZ);
	int GetZOrder()	const;
	void AddObj(std::shared_ptr<class CGameObject> pObj);
	void DeleteEditorDelete();
	std::shared_ptr<class CGameObject> FindObj(const std::string& strKey);
	bool IsStart()	const;
	class CLayerInstance* GetInstance()	const;
	void DeleteObject(const std::string& strKey);
	void DeleteObject(const std::shared_ptr<class CGameObject>& pObj);
	class CScene* GetScene()	const;

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
	void Load(FILE* pFile);

public:
	void ShowWindow();

private:
	static bool SortY(std::shared_ptr<class CGameObject> pSrc, std::shared_ptr<class CGameObject> pDst);

private:
	void CollisionInit(class CCollider* pSrc, class CCollider* pDest, float fTime);
	void CollisionStay(class CCollider* pSrc, class CCollider* pDest, float fTime);
	void CollisionEnd(class CCollider* pSrc, class CCollider* pDest, float fTime);
};

