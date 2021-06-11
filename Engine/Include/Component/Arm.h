#pragma once
#include "SceneComponent.h"
class CArm :
	public CSceneComponent
{
public:
	CArm();
	CArm(const CArm& arm);
	virtual ~CArm() override;

private:
	Vector3 m_vOffset;
	float	m_fDist;
	float	m_fMax;
	float	m_fMin;

public:
	void SetDist(float fDist);
	void AddDist(float fDist);
	void SetMax(float fMax);
	void SetMin(float fMin);
	void SetOffset(const Vector3& vOffset);

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

public:
	virtual void Save(FILE* pFile) override;
	virtual void Load(FILE* pFile) override;
};

