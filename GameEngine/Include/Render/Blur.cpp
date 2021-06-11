#include "Blur.h"
#include "../Device.h"
#include "../RenderManager.h"
#include "../Resource/ComputeShader.h"
#include "../Resource/ShaderManager.h"
#include "MRT.h"

CBlur::CBlur()	:
	m_iBlurCount(1)
	, m_tRS()
	, m_pSRV(nullptr)
	, m_pUAV(nullptr)
	, m_pSingleRenderTarget(nullptr)
{
}

CBlur::CBlur(const CBlur& pro)	:
	CPostProcess(pro)
{
}

CBlur::~CBlur()
{
	SAFE_RELEASE(m_pSRV);
	SAFE_RELEASE(m_pUAV);
	SAFE_RELEASE(m_pSingleRenderTarget);
}

bool CBlur::CreateViews(float fWidth, float fHeight, DXGI_FORMAT eFmt)
{
	D3D11_TEXTURE2D_DESC tDesc = {};

	tDesc.Format = eFmt;
	tDesc.Usage = D3D11_USAGE_DEFAULT;
	tDesc.Width = static_cast<UINT>(fWidth);
	tDesc.Height = static_cast<UINT>(fHeight);
	tDesc.MipLevels = 1;
	tDesc.ArraySize = 1;
	tDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
	tDesc.SampleDesc.Count = 1;
	tDesc.SampleDesc.Quality = 0;

	ID3D11Texture2D* pTex = nullptr;

	if (FAILED(DEVICE->CreateTexture2D(&tDesc, nullptr, &pTex)))
		return false;

	D3D11_SHADER_RESOURCE_VIEW_DESC tSRVDesc = {};
	tSRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	tSRVDesc.Format = eFmt;
	tSRVDesc.Texture2D.MipLevels = 1;
	tSRVDesc.Texture2D.MostDetailedMip = 0;

	if(FAILED(DEVICE->CreateShaderResourceView(pTex, &tSRVDesc, &m_pSRV)))
		return false;

	D3D11_UNORDERED_ACCESS_VIEW_DESC tUAVDesc = {};

	tUAVDesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
	tUAVDesc.Texture2D.MipSlice = 0;
	tUAVDesc.Format = eFmt;

	if(FAILED(DEVICE->CreateUnorderedAccessView(pTex, &tUAVDesc, &m_pUAV)))
		return false;

	SAFE_RELEASE(pTex);

	return true;
}

void CBlur::SetRenderTarget(const std::string& strKey)
{
	SAFE_RELEASE(m_pSingleRenderTarget);
	m_pSingleRenderTarget = GET_SINGLE(CRenderManager)->FindRenderTarget(strKey);
}

bool CBlur::Init()
{
	if (!CPostProcess::Init())
		return false;

	m_tRS = CDevice::GetInst()->GetResolution();
	
	CreateViews((float)m_tRS.iWidth, (float)m_tRS.iHeight, DXGI_FORMAT_R8G8B8A8_UNORM);

	m_pHoriBlurShader = CShaderManager::GetInst()->FindShader("Horz");
	m_pVertBlurShader = CShaderManager::GetInst()->FindShader("Vert");

	return true;
}

void CBlur::Start()
{
}

void CBlur::Update(float fTime)
{
}

void CBlur::PostUpdate(float fTime)
{
}

void CBlur::Collision(float fTime)
{
}

void CBlur::PreRender(float fTime)
{
}

void CBlur::Render(float fTime)
{
	UINT iCount = -1;

	for (int i = 0; i < m_iBlurCount; ++i)
	{
		//CRenderManager::GetInst()->SetShader("Scene", 0, (int)SHADER_CBUFFER_TYPE::CBUFFER_COMPUTE);
		//CRenderManager::GetInst()->SetShader("Scene", 103, (int)SHADER_CBUFFER_TYPE::CBUFFER_PIXEL | (int)SHADER_CBUFFER_TYPE::CBUFFER_COMPUTE);
		m_pSingleRenderTarget->SetShader(103, (int)SHADER_CBUFFER_TYPE::CBUFFER_PIXEL | (int)SHADER_CBUFFER_TYPE::CBUFFER_COMPUTE);
		CONTEXT->CSSetUnorderedAccessViews(0, 1, &m_pUAV, &iCount);
		static_cast<CComputeShader*>(m_pHoriBlurShader.get())->SetShader();
		UINT numGroupsX = (UINT)ceilf(m_tRS.iWidth / 256.0f);
		CONTEXT->Dispatch(numGroupsX, m_tRS.iHeight, 1);

		ID3D11ShaderResourceView* nullSRV[1] = { 0 };
		CONTEXT->CSSetShaderResources(0, 1, nullSRV);

		ID3D11UnorderedAccessView* nullUAV[1] = { 0 };
		CONTEXT->CSSetUnorderedAccessViews(0, 1, nullUAV, 0);

		m_pSingleRenderTarget->ResetShader(103, (int)SHADER_CBUFFER_TYPE::CBUFFER_PIXEL | (int)SHADER_CBUFFER_TYPE::CBUFFER_COMPUTE);
		//CRenderManager::GetInst()->ResetShader("Scene", 103, (int)SHADER_CBUFFER_TYPE::CBUFFER_PIXEL |(int)SHADER_CBUFFER_TYPE::CBUFFER_COMPUTE);
		CONTEXT->CSSetShaderResources(103, 1, &m_pSRV);
		//CRenderManager::GetInst()->SetUAV("Scene", 0);
		m_pSingleRenderTarget->CSSetUAV(0);
		static_cast<CComputeShader*>(m_pVertBlurShader.get())->SetShader();
		UINT numGroupsY = (UINT)ceilf(m_tRS.iHeight / 256.0f);
		CONTEXT->Dispatch(m_tRS.iWidth, numGroupsY, 1);

		CONTEXT->CSSetShaderResources(103, 1, nullSRV);
		CONTEXT->CSSetUnorderedAccessViews(0, 1, nullUAV, 0);

		//m_pSingleRenderTarget->SetShader(103, (int)SHADER_CBUFFER_TYPE::CBUFFER_PIXEL | (int)SHADER_CBUFFER_TYPE::CBUFFER_COMPUTE);
		//CRenderManager::GetInst()->SetShader("Scene", 103, (int)SHADER_CBUFFER_TYPE::CBUFFER_PIXEL |(int)SHADER_CBUFFER_TYPE::CBUFFER_COMPUTE);
	}

	CONTEXT->CSSetShader(0, 0, 0);
}

void CBlur::PostRender(float fTime)
{
}
