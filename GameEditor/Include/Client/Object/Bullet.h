#pragma once

#include "Object/GameObject.h"

#define CONTROL_POINT_COUNT	6

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
	std::shared_ptr<CComponent>	m_pMesh;
	std::shared_ptr<CComponent>	m_pCam;
	Vector3						m_pPoint[CONTROL_POINT_COUNT];

public:
	void SetPt(const Vector3* v);

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
	virtual std::shared_ptr<CGameObject> Clone();

public:
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);

public:
	void ColEnter(class CCollider* pSrc, class CCollider* pDest, float fTime);

public:
	static void BSplineCurve(const Vector3& pt1, const Vector3& pt2, const Vector3& pt3, const Vector3& pt4, const float fTime, Vector3& vResult);
};

