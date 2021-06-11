#pragma once
#include "Object\GameObject.h"
class CBullet3D :
	public CGameObject
{
public:
	CBullet3D();
	CBullet3D(const CBullet3D& obj);
	virtual ~CBullet3D() override;

private:
	float	m_fSpeed;
	float	m_fDist;
	float	m_fLimitDist;
	Vector3 m_vDir;

public:
	virtual bool Init() override;
	virtual void Start() override;
	virtual void Update(float fTime) override;
	virtual void PostUpdate(float fTime) override;
	virtual void PreRender(float fTime) override;
	virtual void Render(float fTime) override;
	virtual void PostRender(float fTime) override;
	virtual std::shared_ptr<class CGameObject> Clone() override;

public:
	virtual void Save(FILE* pFile) override;
	virtual void Load(FILE* pFile) override;

public:
	void CollisionInit(class CCollider* pSrc, class CCollider* pDest, float fTime);
	void CollisionStay(class CCollider* pSrc, class CCollider* pDest, float fTime);
	void CollisionLast(class CCollider* pSrc, class CCollider* pDest, float fTime);

};

