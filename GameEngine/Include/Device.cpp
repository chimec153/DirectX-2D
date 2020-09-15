#include "Device.h"

DEFINITION_SINGLE(CDevice)

CDevice::CDevice()	:
	m_pDevice(nullptr),
	m_pContext(nullptr),
	m_pSwapChain(nullptr),
	m_pTargetView(nullptr),
	m_pDepthView(nullptr),
	m_hWnd(0),
	m_tRS()
{
	memset(m_pClearColor, 0, sizeof(float) * 4);

	m_pClearColor[1] = 0.1f;
	m_pClearColor[2] = 0.1f;
}

CDevice::~CDevice()
{
	SAFE_RELEASE(m_pSwapChain);
	SAFE_RELEASE(m_pTargetView);
	SAFE_RELEASE(m_pDepthView);

	if(m_pContext)
		m_pContext->ClearState();

	SAFE_RELEASE(m_pContext);
	SAFE_RELEASE(m_pDevice);
}

bool CDevice::Init(HWND hWnd, int iWidth, int iHeight, bool bWindowMode)
{
	m_hWnd = hWnd;

	m_tRS.iWidth = iWidth;
	m_tRS.iHeight = iHeight;

	unsigned int iFlag = 0;

#ifdef _DEBUG
	iFlag = D3D11_CREATE_DEVICE_DEBUG;
#endif
	iFlag |= D3D11_CREATE_DEVICE_BGRA_SUPPORT;

	DXGI_SWAP_CHAIN_DESC tSwapChain = {};

	tSwapChain.BufferDesc.Width = iWidth;
	tSwapChain.BufferDesc.Height = iHeight;
	tSwapChain.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	tSwapChain.BufferDesc.RefreshRate.Numerator = 1;
	tSwapChain.BufferDesc.RefreshRate.Denominator = 60;
	tSwapChain.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	tSwapChain.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	tSwapChain.BufferCount = 1;
	tSwapChain.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	tSwapChain.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	tSwapChain.OutputWindow = m_hWnd;
	tSwapChain.Windowed = bWindowMode;
	tSwapChain.SampleDesc.Count = 1;
	tSwapChain.SampleDesc.Quality = 0;

	D3D_FEATURE_LEVEL eLevel = D3D_FEATURE_LEVEL_11_0;
	D3D_FEATURE_LEVEL eLevel1 = D3D_FEATURE_LEVEL_11_0;

	if (FAILED(D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE,
		0, iFlag, &eLevel, 1, D3D11_SDK_VERSION, &tSwapChain,
		&m_pSwapChain, &m_pDevice, &eLevel1,
		&m_pContext)))
		return false;

	ID3D11Texture2D* pBackBuffer = nullptr;

	m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pBackBuffer);
	
	m_pDevice->CreateRenderTargetView(pBackBuffer, nullptr, &m_pTargetView);

	SAFE_RELEASE(pBackBuffer);

	D3D11_FEATURE_DATA_THREADING tThread = {};

	HRESULT iResult = m_pDevice->CheckFeatureSupport(D3D11_FEATURE_THREADING, &tThread, sizeof(tThread));

	D3D11_TEXTURE2D_DESC  tDepthDesc = {};

	tDepthDesc.Width = iWidth;
	tDepthDesc.Height = iHeight;
	tDepthDesc.ArraySize = 1;
	tDepthDesc.MipLevels = 1;
	tDepthDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	tDepthDesc.SampleDesc.Count = 1;
	tDepthDesc.SampleDesc.Quality = 0;
	tDepthDesc.Usage = D3D11_USAGE_DEFAULT;
	tDepthDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

	ID3D11Texture2D* pDepthTex = nullptr;

	m_pDevice->CreateTexture2D(&tDepthDesc, nullptr, &pDepthTex);

	m_pDevice->CreateDepthStencilView(pDepthTex,nullptr, &m_pDepthView);

	SAFE_RELEASE(pDepthTex);

	m_pContext->OMSetRenderTargets(1,&m_pTargetView,m_pDepthView);

	D3D11_VIEWPORT tVF = {};

	tVF.Width = (float)iWidth;
	tVF.Height = (float)iHeight;
	tVF.MaxDepth = 1.f;
	
	m_pContext->RSSetViewports(1, &tVF);

	return true;
}

void CDevice::ClearTarget()
{
	m_pContext->ClearDepthStencilView(m_pDepthView, 
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);

	m_pContext->ClearRenderTargetView(m_pTargetView, m_pClearColor);
}

void CDevice::Render()
{
	m_pSwapChain->Present(0, 0);
}