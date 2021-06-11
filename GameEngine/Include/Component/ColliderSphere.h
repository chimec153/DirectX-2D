#pragma once
#include "Collider.h"
class CColliderSphere :
	public CCollider
{
public:
	CColliderSphere();
	CColliderSphere(const CColliderSphere& col);
	virtual ~CColliderSphere() override;

private:
	SphereInfo	m_tInfo;

public:
	const SphereInfo& GetInfo()	const;
	void SetRadius(float fRadius);

public:
	virtual bool Init() override;
	virtual void Start() override;
	virtual void Update(float fTime) override;
	virtual void PostUpdate(float fTime) override;
	virtual void Collision(float fTime) override;
	virtual void PreRender(float fTime) override;
	virtual void Render(float fTime) override;
	virtual void PostRender(float fTime) override;
	virtual std::shared_ptr<class CComponent> Clone();

public:
	virtual void Save(FILE* pFile) override;
	virtual void Load(FILE* pFile) override;

public:
	virtual bool Collision(class CCollider* pDest);
	virtual bool CollisionMouse(const Vector2& vPos);

public:
	virtual void ShowWindow() override;

};

