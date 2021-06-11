#include "MRT.h"
#include "../Device.h"
#include "../Resource/Texture.h"
#include "../Resource/ShaderManager.h"

CMRT::CMRT() :
	m_pTex(nullptr)
	, m_pSRV(nullptr)
	, m_pUAV(nullptr)
	, m_pTexture(nullptr)
	, m_pPrevSRV(nullptr)
	, m_pPrevRenderTarget(nullptr)
	, m_pRenderTarget()
	, m_pDepthView(nullptr)
	, m_pDepthTex(nullptr)
	, m_pPrevDepthView(nullptr)
	, m_pDepthSRV(nullptr)
#ifdef _DEBUG
	, m_matDebugWVP()
#endif
	, m_tViewPort()
	,m_tPrevViewPort()
	, m_pPrevRTV()
{
	memset(m_pClearColor, 0, 16);
	//m_pClearColor[0] = 0.05f;
	//m_pClearColor[2] = 0.05f;
}

CMRT::CMRT(const CMRT& mrt)	:
	CRef(mrt)
	, m_pTex(mrt.m_pTex)
	, m_pUAV(mrt.m_pUAV)
	, m_pTexture(mrt.m_pTexture)
	, m_pPrevSRV(mrt.m_pPrevSRV)
	, m_pPrevRenderTarget(mrt.m_pPrevRenderTarget)
	, m_pRenderTarget()
	, m_pDepthView(mrt.m_pDepthView)
	, m_pDepthTex(mrt.m_pDepthTex)
	, m_pPrevDepthView(mrt.m_pPrevDepthView)
	, m_pDepthSRV(mrt.m_pDepthSRV)
#ifdef _DEBUG
	, m_matDebugWVP()
#endif
	, m_tViewPort(mrt.m_tViewPort)
	, m_tPrevViewPort(mrt.m_tPrevViewPort)
	, m_pPrevRTV()
{
	memcpy(m_pClearColor, mrt.m_pClearColor, 16);
}

CMRT::~CMRT()
{
	SAFE_RELEASE(m_pTex);
	SAFE_RELEASE(m_pUAV);
	SAFE_RELEASE(m_pTexture);
	SAFE_RELEASE(m_pPrevSRV);
	SAFE_RELEASE(m_pPrevRenderTarget);
	SAFE_RELEASE(m_pRenderTarget);
	SAFE_RELEASE(m_pDepthView);
	SAFE_RELEASE(m_pDepthTex);
	SAFE_RELEASE(m_pPrevDepthView);
	SAFE_RELEASE(m_pDepthSRV);
	SAFE_RELEASE(m_pSRV);
}

ID3D11RenderTargetView* CMRT::GetRenderTargetView() const
{
	if (m_pRenderTarget)
	{
		m_pRenderTarget->AddRef();
	}

	return m_pRenderTarget;
}

ID3D11Texture2D* CMRT::GetTex() const
{
	if (m_pTex)
	{
		m_pTex->AddRef();
	}
	return m_pTex;
}
ID3D11DepthStencilView* CMRT::GetDepthStencilView() const
{
	if (m_pDepthView)
	{
		m_pDepthView->AddRef();
	}
	return m_pDepthView;
}
#ifdef _DEBUG
const Matrix& CMRT::GetDebugWVP() const
{
	return m_matDebugWVP;
}
#endif

bool CMRT::CreateTexture(int iWidth, int iHeight, DXGI_FORMAT eTexFmt, DXGI_FORMAT eFmt, DXGI_FORMAT eDepthFmt)
{
	D3D11_TEXTURE2D_DESC tDesc = {};

	tDesc.Height = iHeight;
	tDesc.Width = iWidth;
	tDesc.Format = eTexFmt;
	tDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET | D3D11_BIND_UNORDERED_ACCESS;
	tDesc.MipLevels = 1;
	tDesc.SampleDesc.Count = 1;
	tDesc.SampleDesc.Quality = 0;
	tDesc.Usage = D3D11_USAGE_DEFAULT;
	tDesc.ArraySize = 1;

	if (FAILED(DEVICE->CreateTexture2D(&tDesc, nullptr, &m_pTex)))
	{
		return false;
	}


	if (m_pTex)
	{
		if (FAILED(DEVICE->CreateShaderResourceView(m_pTex, nullptr, &m_pSRV)))
		{
			return false;
		}
	}

	D3D11_UNORDERED_ACCESS_VIEW_DESC tUAVDesc = {};
	tUAVDesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
	tUAVDesc.Format = eFmt;
	tUAVDesc.Texture2D.MipSlice = 0;

	DEVICE->CreateUnorderedAccessView(m_pTex, &tUAVDesc, &m_pUAV);

	if (FAILED(DEVICE->CreateRenderTargetView(m_pTex, nullptr, &m_pRenderTarget)))
	{
		return false;
	}

	m_pTexture = new CTexture;

	m_pTexture->SetTexture(m_pSRV);

	if (eDepthFmt != DXGI_FORMAT_UNKNOWN)
	{
		D3D11_TEXTURE2D_DESC tDesc = {};

		tDesc.Height = iHeight;
		tDesc.Width = iWidth;
		tDesc.Format = eDepthFmt;
		tDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL |D3D11_BIND_SHADER_RESOURCE;
		tDesc.MipLevels = 1;
		tDesc.SampleDesc.Count = 1;
		tDesc.SampleDesc.Quality = 0;
		tDesc.Usage = D3D11_USAGE_DEFAULT;
		tDesc.ArraySize = 1;

		if (FAILED(DEVICE->CreateTexture2D(&tDesc, nullptr, &m_pDepthTex)))
		{
			return false;
		}

		D3D11_DEPTH_STENCIL_VIEW_DESC tDSVDesc = {};
		tDSVDesc.Flags = 0;
		tDSVDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		tDSVDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;

		if (FAILED(DEVICE->CreateDepthStencilView(m_pDepthTex, &tDSVDesc, &m_pDepthView)))
		{
			return false;
		}

		D3D11_SHADER_RESOURCE_VIEW_DESC tSRVDesc = {};

		tSRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		tSRVDesc.Texture2D.MipLevels = 1;
		tSRVDesc.Texture2D.MostDetailedMip = 0;
		tSRVDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;

		if (FAILED(DEVICE->CreateShaderResourceView(m_pDepthTex, &tSRVDesc, &m_pDepthSRV)))
		{
			return false;
		}
	}

	m_tViewPort.TopLeftX = 0.f;
	m_tViewPort.TopLeftY = 0.f;
	m_tViewPort.Width = static_cast<float>(iWidth);
	m_tViewPort.Height = static_cast<float>(iHeight);
	m_tViewPort.MinDepth = 0.f;
	m_tViewPort.MaxDepth = 1.f;

	return true;
}
bool CMRT::CreateCubeTexture(int iCubeMapSize, DXGI_FORMAT eDepthFormat)
{
	/*D3D11_TEXTURE2D_DESC tDesc = {};

	tDesc.Width = iCubeMapSize;
	tDesc.Height = iCubeMapSize;
	tDesc.MipLevels = 0;
	tDesc.ArraySize = 6;
	tDesc.SampleDesc.Count = 1;
	tDesc.SampleDesc.Quality = 0;
	tDesc.Usage = D3D11_USAGE_DEFAULT;
	tDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	tDesc.CPUAccessFlags = 0;
	tDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS | D3D11_RESOURCE_MISC_TEXTURECUBE;

	ID3D11Texture2D* pCubeTex = nullptr;

	if (FAILED(DEVICE->CreateTexture2D(&tDesc, 0, &pCubeTex)))
		return false;

	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;

	rtvDesc.Format = tDesc.Format;
	rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DARRAY;
	rtvDesc.Texture2DArray.MipSlice = 0;
	rtvDesc.Texture2DArray.ArraySize = 6;
	rtvDesc.Texture2DArray.FirstArraySlice = 0;

	if (FAILED(DEVICE->CreateRenderTargetView(pCubeTex, &rtvDesc, &m_pRenderTarget)))
		return false;

	ID3D11ShaderResourceView* pCubeMapSRV= nullptr;

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;

	srvDesc.Format = tDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
	srvDesc.TextureCube.MostDetailedMip = 0;
	srvDesc.TextureCube.MipLevels = -1;

	if (FAILED(DEVICE->CreateShaderResourceView(pCubeTex, &srvDesc, &pCubeMapSRV)))
		return false;

	m_pSRV = ID3D11ShaderResourceView*(pCubeMapSRV);

	SAFE_RELEASE(pCubeTex);*/


	D3D11_TEXTURE2D_DESC depthTexDesc = {};
	
	depthTexDesc.Width = iCubeMapSize;
	depthTexDesc.Height = iCubeMapSize;
	depthTexDesc.MipLevels = 1;
	depthTexDesc.ArraySize = 6;
	depthTexDesc.SampleDesc.Count = 1;
	depthTexDesc.SampleDesc.Quality = 0;
	depthTexDesc.Format = DXGI_FORMAT_R32_TYPELESS;
	depthTexDesc.Usage = D3D11_USAGE_DEFAULT;
	depthTexDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	depthTexDesc.CPUAccessFlags = 0;
	depthTexDesc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;

	if (FAILED(DEVICE->CreateTexture2D(&depthTexDesc, 0, &m_pDepthTex)))
		return false;

	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
	dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DARRAY;
	dsvDesc.Texture2DArray.MipSlice = 0;
	dsvDesc.Texture2DArray.FirstArraySlice = 0;
	dsvDesc.Texture2DArray.ArraySize = 6;
	

	if (FAILED(DEVICE->CreateDepthStencilView(m_pDepthTex, &dsvDesc, &m_pDepthView)))
		return false;

	D3D11_SHADER_RESOURCE_VIEW_DESC tDepthSrvDesc = {};
	tDepthSrvDesc.Format = DXGI_FORMAT_R32_FLOAT;
	tDepthSrvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
	tDepthSrvDesc.TextureCube.MipLevels = 1;
	tDepthSrvDesc.TextureCube.MostDetailedMip = 0;

	if (FAILED(DEVICE->CreateShaderResourceView(m_pDepthTex, &tDepthSrvDesc, &m_pDepthSRV)))
		return false;

	D3D11_TEXTURE2D_DESC tTexDesc = {};
	tTexDesc.Format = DXGI_FORMAT_R32_FLOAT;
	tTexDesc.ArraySize = 6;
	tTexDesc.MipLevels = 1;
	tTexDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	tTexDesc.Height = iCubeMapSize;
	tTexDesc.Width = iCubeMapSize;
	tTexDesc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;
	tTexDesc.Usage = D3D11_USAGE_DEFAULT;
	tTexDesc.SampleDesc.Count = 1;
	tTexDesc.SampleDesc.Quality = 0;

	if (FAILED(DEVICE->CreateTexture2D(&tTexDesc, nullptr, &m_pTex)))
		return false;

	D3D11_RENDER_TARGET_VIEW_DESC tRTVDesc = {};

	tRTVDesc.Texture2DArray.ArraySize = 6;
	tRTVDesc.Texture2DArray.FirstArraySlice = 0;
	tRTVDesc.Texture2DArray.MipSlice = 0;
	tRTVDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DARRAY;
	tRTVDesc.Format = DXGI_FORMAT_R32_FLOAT;

	if (FAILED(DEVICE->CreateRenderTargetView(m_pTex, &tRTVDesc, &m_pRenderTarget)))
		return false;

	D3D11_SHADER_RESOURCE_VIEW_DESC tSRVDesc = {};

	tSRVDesc.TextureCube.MipLevels = 1;
	tSRVDesc.TextureCube.MostDetailedMip = 0;
	tSRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
	tSRVDesc.Format = DXGI_FORMAT_R32_FLOAT;

	if (FAILED(DEVICE->CreateShaderResourceView(m_pTex, &tSRVDesc, &m_pSRV)))
		return false;

	m_tViewPort.TopLeftX = 0.f;
	m_tViewPort.TopLeftY = 0.f;
	m_tViewPort.Width = static_cast<float>(iCubeMapSize);
	m_tViewPort.Height = static_cast<float>(iCubeMapSize);
	m_tViewPort.MinDepth = 0.f;
	m_tViewPort.MaxDepth = 1.f;

	return true;
}
#ifdef _DEBUG
bool CMRT::CreateDebugMatrix(const Vector3& vScale, const Vector3& vTranslation)
{
	Matrix matScale;
	Matrix matTranslation;
	Matrix matWorld;
	Matrix matProj;

	matScale.Scaling(vScale);
	matTranslation.Translate(vTranslation);
	matWorld = matScale * matTranslation;
	Resolution tRS = RESOLUTION;

	matProj = DirectX::XMMatrixOrthographicOffCenterLH(0.f, (float)tRS.iWidth, 0.f, (float)tRS.iHeight, 0.f, 1000.f);

	m_matDebugWVP = matWorld * matProj;

	m_matDebugWVP.Transpose();

	return true;
}
#endif

void CMRT::SetTarget()
{
	CDevice::Enter();
	CONTEXT->OMGetRenderTargets(1, &m_pPrevRenderTarget, &m_pPrevDepthView);

	CONTEXT->OMSetRenderTargets(1, &m_pRenderTarget, m_pDepthView);
	CDevice::Leave();
}

void CMRT::SetDepthView()
{
	ID3D11RenderTargetView* pRTV = nullptr;
	CDevice::Enter();
	CONTEXT->OMGetRenderTargets(1, &m_pPrevRenderTarget, &m_pPrevDepthView);

	CONTEXT->OMSetRenderTargets(1, &pRTV, m_pDepthView);
	CDevice::Leave();
}

void CMRT::SetCubeDepthView()
{
	ID3D11RenderTargetView* pRTV[6] = {};
	CDevice::Enter();
	CONTEXT->OMGetRenderTargets(1, m_pPrevRTV, &m_pPrevDepthView);

	CONTEXT->OMSetRenderTargets(1, pRTV, m_pDepthView);
	CDevice::Leave();
}

void CMRT::ResetTarget()
{
	//ID3D11ShaderResourceView* pSRV = m_pSRV.get();

	CDevice::Enter();
	CONTEXT->OMSetRenderTargets(1, &m_pPrevRenderTarget, m_pPrevDepthView);
	//CONTEXT->GenerateMips(pSRV);
	CDevice::Leave();

	SAFE_RELEASE(m_pPrevRenderTarget);
	SAFE_RELEASE(m_pPrevDepthView);
}

void CMRT::ClearTarget()
{
	CDevice::Enter();

	if (m_pRenderTarget)
	{
		CONTEXT->ClearRenderTargetView(m_pRenderTarget, m_pClearColor);
	}

	if (m_pDepthView)
	{
		CONTEXT->ClearDepthStencilView(m_pDepthView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);
	}
	CDevice::Leave();
}

void CMRT::SetShader(int iRegister, int iType)
{

	CDevice::Enter();
	if (iType & (int)SHADER_CBUFFER_TYPE::CBUFFER_PIXEL)
	{
		CONTEXT->PSSetShaderResources(iRegister, 1, &m_pSRV);

	}
		
	if (iType & (int)SHADER_CBUFFER_TYPE::CBUFFER_COMPUTE)
	{
		CONTEXT->CSSetShaderResources(iRegister, 1, &m_pSRV);
	}
	CDevice::Leave();
#ifdef _DEBUG
	GET_SINGLE(CShaderManager)->UpdateCBuffer("DebugWVP", &m_matDebugWVP);
#endif
}

void CMRT::ResetShader(int iRegister, int iType)
{
	ID3D11ShaderResourceView* pSRV = nullptr;

	CDevice::Enter();
	if (iType & (int)SHADER_CBUFFER_TYPE::CBUFFER_PIXEL)
	{
		CONTEXT->PSSetShaderResources(iRegister, 1, &pSRV);
	}
	if (iType & (int)SHADER_CBUFFER_TYPE::CBUFFER_COMPUTE)
	{
		CONTEXT->CSSetShaderResources(iRegister, 1, &pSRV);
	}
	CDevice::Leave();
}

void CMRT::CSSetUAV(int iRegister)
{
	UINT iCount = -1;

	CDevice::Enter();
	CONTEXT->CSSetUnorderedAccessViews(iRegister, 1, &m_pUAV, &iCount);
	CDevice::Leave();
}

void CMRT::SetDepthSRV(int iRegister, int iType)
{
	CDevice::Enter();
	if (iType & static_cast<int>(SHADER_CBUFFER_TYPE::CBUFFER_VERTEX))
		CONTEXT->VSSetShaderResources(iRegister, 1, &m_pDepthSRV);
	if (iType & static_cast<int>(SHADER_CBUFFER_TYPE::CBUFFER_PIXEL))
		CONTEXT->PSSetShaderResources(iRegister, 1, &m_pDepthSRV);
	if (iType & static_cast<int>(SHADER_CBUFFER_TYPE::CBUFFER_HULL))
		CONTEXT->HSSetShaderResources(iRegister, 1, &m_pDepthSRV);
	if (iType & static_cast<int>(SHADER_CBUFFER_TYPE::CBUFFER_DOMAIN))
		CONTEXT->DSSetShaderResources(iRegister, 1, &m_pDepthSRV);
	if (iType & static_cast<int>(SHADER_CBUFFER_TYPE::CBUFFER_GEOMETRY))
		CONTEXT->GSSetShaderResources(iRegister, 1, &m_pDepthSRV);
	if (iType & static_cast<int>(SHADER_CBUFFER_TYPE::CBUFFER_COMPUTE))
		CONTEXT->CSSetShaderResources(iRegister, 1, &m_pDepthSRV);
	CDevice::Leave();

#ifdef _DEBUG
	GET_SINGLE(CShaderManager)->UpdateCBuffer("DebugWVP", &m_matDebugWVP);
#endif
}

void CMRT::ResetDepthSRV(int iRegister, int iType)
{
	ID3D11ShaderResourceView* pSRV = nullptr;
	CDevice::Enter();
	if (iType & static_cast<int>(SHADER_CBUFFER_TYPE::CBUFFER_VERTEX))
		CONTEXT->VSSetShaderResources(iRegister, 1, &pSRV);
	if (iType & static_cast<int>(SHADER_CBUFFER_TYPE::CBUFFER_PIXEL))
		CONTEXT->PSSetShaderResources(iRegister, 1, &pSRV);
	if (iType & static_cast<int>(SHADER_CBUFFER_TYPE::CBUFFER_HULL))
		CONTEXT->HSSetShaderResources(iRegister, 1, &pSRV);
	if (iType & static_cast<int>(SHADER_CBUFFER_TYPE::CBUFFER_DOMAIN))
		CONTEXT->DSSetShaderResources(iRegister, 1, &pSRV);
	if (iType & static_cast<int>(SHADER_CBUFFER_TYPE::CBUFFER_GEOMETRY))
		CONTEXT->GSSetShaderResources(iRegister, 1, &pSRV);
	if (iType & static_cast<int>(SHADER_CBUFFER_TYPE::CBUFFER_COMPUTE))
		CONTEXT->CSSetShaderResources(iRegister, 1, &pSRV);
	CDevice::Leave();
}

void CMRT::SetViewPort()
{
	UINT iNum = 1;

	CONTEXT->RSGetViewports(&iNum, &m_tPrevViewPort);

	CONTEXT->RSSetViewports(1, &m_tViewPort);
}

void CMRT::ResetViewPort()
{
	CONTEXT->RSSetViewports(1, &m_tPrevViewPort);
}
