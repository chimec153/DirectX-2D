#pragma once
#include "PostProcess.h"
class CGray :
	public CPostProcess
{
	friend class CRenderManager;

protected:
	CGray();
	CGray(const CGray& pro);
	virtual ~CGray();

private:
	std::shared_ptr<class CShader> m_pGrayShader;
	Resolution	m_tRS;
	ID3D11UnorderedAccessView* m_pUAV;

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

