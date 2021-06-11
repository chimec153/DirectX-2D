#pragma once
#include "Collider.h"
class CColliderCircle :
	public CCollider
{
	friend class CGameObject;

public:
	CColliderCircle();
	CColliderCircle(const CColliderCircle& col);
	virtual ~CColliderCircle();

private:
	SphereInfo			m_tInfo;

public:
	void SetRadius(float fRadius);
	SphereInfo GetInfo()	const;

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

