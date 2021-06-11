#pragma once
#include "PostProcess.h"
class CDistortion :
	public CPostProcess
{
	friend class CSceneComponent;

private:
	CDistortion();
	CDistortion(const CDistortion& pro);
	virtual ~CDistortion();

private:
	std::shared_ptr<class CMaterial>	m_pMaterial;
	DISTORTCBUFFER						m_tCBuffer;

public:
	void SetMaterial(const std::string& strMtrl);
	void SetWeight(float fWeight);
	void SetSpeed(float fSpeed);
	void SetFrequency(float fFre);

public:
	virtual bool Init();
	virtual void Start();
	virtual void Update(float fTime);
	virtual void PostUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void PreRender(float fTime);
	virtual void Render(float fTime);
	virtual void PostRender(float fTime);

};

