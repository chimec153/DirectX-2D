#pragma once
#include "Collider.h"
class CColliderOBB2D :
	public CCollider
{
	friend class CGameObject;

public:
	CColliderOBB2D();
	CColliderOBB2D(const CColliderOBB2D& col);
	virtual ~CColliderOBB2D();

private:
	OBBInfo		m_tInfo;

public:
	void SetExtend(float x,float y, float z);
	OBBInfo GetInfo()	const;

public:
	virtual bool Init();
	virtual void Start();
	virtual void Update(float fTime);
	virtual void PostUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void PreRender(float fTime);
	virtual void Render(float fTime);
	virtual void PostRender(float fTime);
	virtual std::shared_ptr<CComponent> Clone();

public:
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);

public:
	virtual bool Collision(class CCollider* pDest);
	virtual bool CollisionMouse(const Vector2& vMousePos);
};

