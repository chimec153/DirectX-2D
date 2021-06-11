#pragma once
#include "Collider.h"
class CColliderOBB :
	public CCollider
{
public:
	CColliderOBB();
	CColliderOBB(const CColliderOBB& obb);
	virtual ~CColliderOBB() override;

private:
	OBBInfo	m_tInfo;

public:
	void SetExtent(const Vector3& vExtent);
	const OBBInfo& GetInfo()	const;

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
	bool Collision(class CCollider* pDest);
	bool CollisionMouse(const Vector2& vMousePos);

public:
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);

public:
	virtual void ShowWindow() override;
};

