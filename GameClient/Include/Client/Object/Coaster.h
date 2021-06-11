#pragma once
#include "Object\GameObject.h"
class CCoaster :
	public CGameObject
{
public:
	CCoaster();
	CCoaster(const CCoaster& obj);
	virtual ~CCoaster();

private:
	std::shared_ptr<CComponent> m_pMesh;
	float			m_fClif;
	float			m_fAngle;
	Vector3						m_pPoint[6];
	float			m_fDist;
	float			m_fLimitDist;
	int				m_iTrack;

public:
	void SetPt(const Vector3* v);
	void SetDist(float fDist);

public:
	virtual bool Init();
	virtual void Start();
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
	void BSplineCurve(const Vector3& pt1, const Vector3& pt2, const Vector3& pt3, const Vector3& pt4, const float fTime, Vector3& vResult);
};

