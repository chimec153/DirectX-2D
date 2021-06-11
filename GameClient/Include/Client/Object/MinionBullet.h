#pragma once
#include "Object\GameObject.h"

class CMinionBullet :
	public CGameObject
{
	friend class CScene;

public:
	CMinionBullet();
	CMinionBullet(const CMinionBullet& bullet);
	virtual ~CMinionBullet();

private:
	std::shared_ptr<class CComponent>	m_pSprite;
	float						m_fDist;
	float						m_fLimitDist;

public:
	virtual bool Init();
	virtual void Start();
	virtual void Input(float fTime);
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

public:
	void ColInit(class CCollider* pSrc, class CCollider* pDest, float fTime);
};

