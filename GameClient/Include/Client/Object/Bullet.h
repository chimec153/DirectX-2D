#pragma once

#include "Object/GameObject.h"

class CBullet :
	public CGameObject
{
public:
	CBullet();
	CBullet(const CBullet& bullet);
	virtual ~CBullet();

private:
	float						m_fSpeed;
	float						m_fDist;
	float						m_fLimitDist;
	class CMesh2DComponent*		m_pMesh;

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
};

