#pragma once
#include "Collider.h"
class CColliderTerrain :
	public CCollider
{
public:
	CColliderTerrain();
	CColliderTerrain(const CColliderTerrain& col);
	virtual ~CColliderTerrain() override;

private:
	TERRAININFO	m_tInfo;

public:
	const TERRAININFO& GetInfo()	const;
	void SetInfo(const std::vector<Vector3>& vecPos, int iWidth, int iHeight);

public:
	virtual bool Init() override;
	virtual void Start() override;
	virtual void Update(float fTime) override;
	virtual void PostUpdate(float fTime) override;
	virtual void Collision(float fTime) override;
	virtual void PreRender(float fTime) override;
	virtual void Render(float fTime) override;
	virtual void PostRender(float fTime) override;
	virtual std::shared_ptr<CComponent> Clone();
public:
	virtual bool Collision(class CCollider* pDest) override;
	virtual bool CollisionMouse(const Vector2& vPos) override;

public:
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);

};

