#include "BlendState.h"
#include "../Device.h"

CBlendState::CBlendState()	:
	m_iSampleMask(0xffffffff),
	m_iPrevSampleMask(0xffffffff)
{
	memset(m_pBlendFactor, 0, sizeof(float) * 4);
	memset(m_pPrevBlendFactor, 0, sizeof(float) * 4);

	m_eType = RENDER_STATE_TYPE::BLEND;
}

CBlendState::~CBlendState()
{
}

void CBlendState::SetState()
{
	CONTEXT->OMGetBlendState((ID3D11BlendState**)&m_pPrevState, m_pPrevBlendFactor, &m_iPrevSampleMask);

	CONTEXT->OMSetBlendState((ID3D11BlendState*)m_pState, m_pBlendFactor, m_iSampleMask);
}

void CBlendState::ResetState()
{
	CONTEXT->OMSetBlendState((ID3D11BlendState*)m_pPrevState, m_pPrevBlendFactor, m_iPrevSampleMask);

	SAFE_RELEASE(m_pPrevState);
}

void CBlendState::SetSampleMask(UINT iMask)
{
	m_iSampleMask = iMask;
}

bool CBlendState::AddBlendInfo(bool bEnable, D3D11_BLEND eSrcBlend, D3D11_BLEND eDestBlend, D3D11_BLEND_OP eBlendOp, 
	D3D11_BLEND eSrcBlendAlpha, D3D11_BLEND eDestBlendAlpha, D3D11_BLEND_OP eBlendOpAlpha, UINT8 cRenderTargetWriteMask)
{
	D3D11_RENDER_TARGET_BLEND_DESC tDesc = {};
	tDesc.BlendEnable = bEnable;
	tDesc.SrcBlend = eSrcBlend;
	tDesc.DestBlend = eDestBlend;
	tDesc.BlendOp = eBlendOp;
	tDesc.SrcBlendAlpha = eSrcBlendAlpha;
	tDesc.DestBlendAlpha = eDestBlendAlpha;
	tDesc.BlendOpAlpha = eBlendOpAlpha;
	tDesc.RenderTargetWriteMask = cRenderTargetWriteMask;

	m_vecDesc.push_back(tDesc);

	return true;
}

bool CBlendState::CreateBlendState(bool bAlpha, bool bBlend)
{
	D3D11_BLEND_DESC tDesc = {};

	tDesc.AlphaToCoverageEnable = bAlpha;
	tDesc.IndependentBlendEnable = bBlend;

	memcpy(tDesc.RenderTarget, &m_vecDesc[0], sizeof(D3D11_RENDER_TARGET_BLEND_DESC) * m_vecDesc.size());

	if (FAILED(DEVICE->CreateBlendState(&tDesc, (ID3D11BlendState * *)& m_pState)))
		return false;

	return true;
}

void CBlendState::ShowWindow()
{
}
