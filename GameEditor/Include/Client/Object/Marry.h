#pragma once
#include "Object\GameObject.h"
class CMarry :
	public CGameObject
{
	friend class CScene;
	
public:
	CMarry();
	CMarry(const CMarry& obj);
	virtual ~CMarry();

private:
	std::shared_ptr<CComponent> m_pAni;
	std::shared_ptr<CComponent>	m_pSnd;

public:
	virtual bool Init();
	virtual void Start();
	virtual void Update(float fTime);
	virtual void PostUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void PreRender(float fTime);
	virtual void Render(float fTime);
	virtual void PostRender(float fTime);
	virtual std::shared_ptr<CGameObject> Clone();

public:
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);
};

