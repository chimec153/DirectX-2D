#pragma once
#include "Collider.h"
class CColliderRay :
	public CCollider
{
public:
	CColliderRay();
	CColliderRay(const CColliderRay& ray);
	virtual ~CColliderRay() override;

private:
	RAYINFO		m_tInfo;

public:
	const RAYINFO& GetInfo()	const;
	void SetOrigin(const Vector3& vOrigin);
	void SetDir(const Vector3& vDir);

public:
	virtual bool Init() override;
	virtual void Start() override;
	virtual void Update(float fTime) override;
	virtual void PostUpdate(float fTime) override;
	virtual void Collision(float fTime) override;
	virtual void PreRender(float fTime) override;
	virtual void Render(float fTime) override;
	virtual void PostRender(float fTime) override;
	virtual std::shared_ptr<CComponent> Clone() override;
	virtual bool Collision(CCollider* pDest) override;
	virtual bool CollisionMouse(const Vector2& vPos) override;
public:
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);
};

