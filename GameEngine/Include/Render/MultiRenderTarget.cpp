#include "MultiRenderTarget.h"
#include "../Device.h"

CMultiRenderTarget::CMultiRenderTarget()	:
	m_pDepthTex()
	, m_pDepthView()
	, m_pPrevDepthView(nullptr)
	, m_pClearColor()
{
	//m_pClearColor[0] = 0.1f;
	//m_pClearColor[2] = 0.1f;
}

CMultiRenderTarget::~CMultiRenderTarget()
{
	Safe_Release_VecList(m_vecPrevTarget);
	Safe_Release_VecList(m_vecTarget);
	SAFE_RELEASE(m_pDepthTex);
	SAFE_RELEASE(m_pDepthView);
	SAFE_RELEASE(m_pPrevDepthView);
}

void CMultiRenderTarget::AddTarget(ID3D11RenderTargetView* pRTV)
{
	m_vecTarget.push_back(pRTV);
	m_vecPrevTarget.push_back(nullptr);
}

bool CMultiRenderTarget::CreateDepth(int iWidth, int iHeight, DXGI_FORMAT eFmt)
{
	D3D11_TEXTURE2D_DESC tTexDesc = {};

	tTexDesc.ArraySize = 1;
	tTexDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	tTexDesc.Format = eFmt;
	tTexDesc.Height = iHeight;
	tTexDesc.Width = iWidth;
	tTexDesc.MipLevels = 1;
	tTexDesc.SampleDesc.Count = 1;
	tTexDesc.SampleDesc.Quality = 0;
	tTexDesc.Usage = D3D11_USAGE_DEFAULT;

	if (FAILED(DEVICE->CreateTexture2D(&tTexDesc, nullptr, &m_pDepthTex)))
		return false;

	D3D11_DEPTH_STENCIL_VIEW_DESC tDesc = {};

	tDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	tDesc.Texture2D.MipSlice = 0;
	tDesc.Format = eFmt;

	if (FAILED(DEVICE->CreateDepthStencilView(m_pDepthTex, &tDesc, &m_pDepthView)))
		return false;

	return true;
}

void CMultiRenderTarget::SetTarget()
{
	CONTEXT->OMGetRenderTargets(static_cast<UINT>(m_vecPrevTarget.size()), &m_vecPrevTarget[0], &m_pPrevDepthView);
	CONTEXT->OMSetRenderTargets(static_cast<UINT>(m_vecTarget.size()), &m_vecTarget[0], m_pDepthView);
}

void CMultiRenderTarget::ResetTarget()
{
	size_t iSize = m_vecPrevTarget.size();
	CONTEXT->OMSetRenderTargets(static_cast<UINT>(iSize), &m_vecPrevTarget[0], m_pPrevDepthView);
	for (size_t i = 0; i < iSize; i++)
	{
		SAFE_RELEASE(m_vecPrevTarget[i]);
	}
	SAFE_RELEASE(m_pPrevDepthView);
}

void CMultiRenderTarget::Clear()
{
	size_t iSize = m_vecTarget.size();

	for (size_t i = 0; i < iSize; i++)
	{
		CONTEXT->ClearRenderTargetView(m_vecTarget[i], m_pClearColor);
	}

	CONTEXT->ClearDepthStencilView(m_pDepthView, 
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);
}
