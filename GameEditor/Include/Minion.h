#pragma once

#include "Object/GameObject.h"

class CMinion :
	public CGameObject
{
	friend class CScene;

private:
	CMinion();
	CMinion(const CMinion& minion);
	virtual ~CMinion();

private:
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

public:
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);
};

