#pragma once

#include "Object/GameObject.h"

class CPlayer :
	public CGameObject
{
public:
	CPlayer();
	CPlayer(const CPlayer& player);
	virtual ~CPlayer();

private:
	class CMesh2DComponent*			m_pMesh;

public:
	virtual bool Init();
	virtual void Start();
	virtual void Update(float fTime);
	virtual void PostUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void PreRender(float fTime);
	virtual void Render(float fTime);
	virtual void PostRender(float fTime);
};
