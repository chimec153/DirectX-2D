#pragma once
#include "RenderState.h"

class CRasterizerState :
	public CRenderState
{
	friend class CRenderManager;

protected:
	CRasterizerState();
	virtual ~CRasterizerState();

private:
	std::vector<D3D11_RASTERIZER_DESC>	m_vecDesc;

public:
	virtual void SetState();
	virtual void ResetState();

public:
	bool AddRasterizerInfo(D3D11_FILL_MODE eFillMode, D3D11_CULL_MODE eCullMode, bool bClock, bool bClip,
		int iDepthBias = 0, float fDepthBiasClamp = 0.f, float fSlopeScaledDepthBias = 0.f);
	bool CreateRasterizerState();

public:
	virtual void ShowWindow() override;

};
