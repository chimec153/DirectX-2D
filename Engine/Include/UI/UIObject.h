#pragma once
#include "../Object/GameObject.h"
class CUIObject :
	public CGameObject
{
	friend class CUIManager;

protected:
	CUIObject();
	CUIObject(const CUIObject& ui);
	virtual ~CUIObject() = 0;

protected:
	int								m_iZOrder;

public:
	int GetZOrder()	const;

public:
	virtual bool Init();
	virtual void Start();
	virtual void Update(float fTime);
	virtual void PostUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void PreRender(float fTime);
	virtual void Render(float fTime);
	virtual void PostRender(float fTime);
	virtual std::shared_ptr<class CGameObject> Clone();

public:
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);
};

