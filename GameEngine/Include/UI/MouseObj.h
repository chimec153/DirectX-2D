#pragma once

#include "UIObject.h"

class CMouseObj :
	public CUIObject
{
	friend class CScene;
	friend class CInput;

public:
	CMouseObj();
	CMouseObj(const CMouseObj& obj);
	virtual ~CMouseObj();

private:
	std::shared_ptr<class CComponent>		m_pPt;
	std::shared_ptr<class CComponent>		m_pSprt;

public:
	std::shared_ptr<class CComponent> GetCol()	const;

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

