#pragma once

#include "RenderState.h"

class CBlendState :
	public CRenderState
{
	friend class CRenderManager;

protected:
	CBlendState();
	virtual ~CBlendState();

private:
	std::vector<D3D11_RENDER_TARGET_BLEND_DESC>		m_vecDesc;
	UINT											m_iSampleMask;
	UINT											m_iPrevSampleMask;
	float											m_pBlendFactor[4];
	float											m_pPrevBlendFactor[4];

public:
	virtual void SetState();
	virtual void ResetState();
	void SetSampleMask(UINT iMask);

public:
	bool AddBlendInfo(bool bEnable = true, D3D11_BLEND eSrcBlend = D3D11_BLEND_SRC_ALPHA, D3D11_BLEND eDestBlend = D3D11_BLEND_INV_SRC_ALPHA, D3D11_BLEND_OP eBlendOp = D3D11_BLEND_OP_ADD,
		D3D11_BLEND eSrcBlendAlpha = D3D11_BLEND_ONE, D3D11_BLEND eDestBlendAlpha = D3D11_BLEND_ZERO, D3D11_BLEND_OP eBlendOpAlpha = D3D11_BLEND_OP_ADD, UINT8 cRenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL);
	bool CreateBlendState(bool bAlpha = false, bool bBlend = false);

public:
	virtual void ShowWindow() override;
};
