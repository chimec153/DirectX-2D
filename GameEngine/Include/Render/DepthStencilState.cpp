#include "DepthStencilState.h"
#include "../Device.h"

CDepthStencilState::CDepthStencilState()	:
	m_iPrevStencilRef(0xffffffff),
	m_iStencilRef(0xffffffff)
{
	m_eType = RENDER_STATE_TYPE::DEPTHSTENCIL;
}

CDepthStencilState::~CDepthStencilState()
{
}

void CDepthStencilState::SetState()
{
	CONTEXT->OMGetDepthStencilState((ID3D11DepthStencilState * *)& m_pPrevState, &m_iPrevStencilRef);

	CONTEXT->OMSetDepthStencilState((ID3D11DepthStencilState *)m_pState, m_iStencilRef);
}

void CDepthStencilState::ResetState()
{
	CONTEXT->OMSetDepthStencilState((ID3D11DepthStencilState*)m_pPrevState, m_iPrevStencilRef);

	SAFE_RELEASE(m_pPrevState);
}

bool CDepthStencilState::AddDepthStencilInfo(bool bEnable, D3D11_DEPTH_WRITE_MASK eMask, D3D11_COMPARISON_FUNC eFunc, bool bStencilEnable, UINT8 iReadMask, UINT8 iWriteMask, 
	const D3D11_DEPTH_STENCILOP_DESC& tFront, const D3D11_DEPTH_STENCILOP_DESC& tBack)
{
	D3D11_DEPTH_STENCIL_DESC tDesc = {};

	tDesc.DepthEnable = bEnable;
	tDesc.DepthWriteMask = eMask;
	tDesc.DepthFunc = eFunc;
	tDesc.StencilEnable = bStencilEnable;
	tDesc.StencilReadMask = iReadMask;
	tDesc.StencilWriteMask = iWriteMask;
	tDesc.FrontFace = tFront;
	tDesc.BackFace = tBack;

	if (FAILED(DEVICE->CreateDepthStencilState(&tDesc, (ID3D11DepthStencilState * *)& m_pState)))
		return false;

	return true;
}

void CDepthStencilState::SetStencilRef(UINT iRef)
{
	m_iStencilRef = iRef;
}

void CDepthStencilState::ShowWindow()
{
}
