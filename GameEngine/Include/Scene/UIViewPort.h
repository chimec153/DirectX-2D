#pragma once

#include "../Ref.h"

class CUIViewPort	:
	public CRef
{
	friend class CScene;

private:
	CUIViewPort();
	virtual ~CUIViewPort();

private:
	class CScene*					m_pScene;
	std::list<class CGameObject*>	m_UIList;

public:
	void AddObj(class CGameObject* pObj);

public:
	virtual bool Init();
	virtual void Start();
	virtual void Update(float fTime);
	virtual void PostUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void PreRender(float fTime);
	virtual void Render(float fTime);
	virtual void PostRender(float fTime);
	virtual CUIViewPort* Clone();

public:
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);

private:
	static bool SortZ(class CGameObject* pObj1, class CGameObject* pObj2);
};

