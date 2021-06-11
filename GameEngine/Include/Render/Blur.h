#pragma once
#include "PostProcess.h"
class CBlur :
	public CPostProcess
{
	friend class CSceneComponent;
	friend class CRenderManager;

private:
	CBlur();
	CBlur(const CBlur& pro);
	virtual ~CBlur();

private:
	int	m_iBlurCount;
	Resolution	m_tRS;
	std::shared_ptr<class CShader>	m_pVertBlurShader;
	std::shared_ptr<class CShader>	m_pHoriBlurShader;
	ID3D11ShaderResourceView*	m_pSRV;
	ID3D11UnorderedAccessView*	m_pUAV;
	class CMRT* m_pSingleRenderTarget;

public:
	bool CreateViews(float fWidth, float fHeight, DXGI_FORMAT eFmt);
	void SetRenderTarget(const std::string& strKey);

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

