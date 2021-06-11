#include "Device.h"
#include "CameraManager.h"
#include "Component/Camera.h"
ID3D10Multithread* CDevice::m_pThread = nullptr;
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

	//m_pClearColor[0] = 0.01f;
	//m_pClearColor[2] = 0.1f;
}

CDevice::~CDevice()
{
	SAFE_RELEASE(m_p2DFactory);
	SAFE_RELEASE(m_p2DRenderTarget);
	SAFE_RELEASE(m_pSwapChain);
	SAFE_RELEASE(m_pTargetView);
	SAFE_RELEASE(m_pDepthView);

	if(m_pContext)
		m_pContext->ClearState();

	SAFE_RELEASE(m_pContext);
	SAFE_RELEASE(m_pDevice);
	SAFE_RELEASE(m_pThread);
}

ID3D11Device* CDevice::GetDevice() const
{
	return m_pDevice;
}

ID3D11DeviceContext* CDevice::GetContext() const
{
	return m_pContext;
}

IDXGISwapChain* CDevice::GetSwapChain() const
{
	return m_pSwapChain;
}

Resolution CDevice::GetResolution() const
{
	return m_tRS;
}

Vector2 CDevice::GetWindowRatio() const
{
	RECT tRC = {};

	GetClientRect(m_hWnd, &tRC);	

	return Vector2((tRC.right - tRC.left) / (float)m_tRS.iWidth, (tRC.bottom - tRC.top) / (float)m_tRS.iHeight);
}

ID2D1Factory* CDevice::Get2DFactory() const
{
	return m_p2DFactory;
}

ID2D1RenderTarget* CDevice::Get2DRenderTarget() const
{
	return m_p2DRenderTarget;
}

ID3D10Multithread* CDevice::GetThread() const
{
	return m_pThread;
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

	D3D_FEATURE_LEVEL eLevel = D3D_FEATURE_LEVEL_11_0;
	D3D_FEATURE_LEVEL eLevel1 = D3D_FEATURE_LEVEL_11_0;

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
	tSwapChain.OutputWindow = m_hWnd;
	tSwapChain.SampleDesc.Count = 1;
	tSwapChain.SampleDesc.Quality = 0;
	tSwapChain.Windowed = bWindowMode;
	tSwapChain.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	if (FAILED(D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE,
		0, iFlag, &eLevel, 1, D3D11_SDK_VERSION, &tSwapChain,
		&m_pSwapChain, &m_pDevice, &eLevel1, &m_pContext)))
		return false;

	D3D11_FEATURE_DATA_THREADING tThread = {};

	HRESULT iResult = m_pDevice->CheckFeatureSupport(D3D11_FEATURE_THREADING, 
		&tThread, sizeof(tThread));

	ID3D11Texture2D* pBackBuffer = nullptr;

	m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pBackBuffer);
	
	m_pDevice->CreateRenderTargetView(pBackBuffer, nullptr, &m_pTargetView);

	SAFE_RELEASE(pBackBuffer);

	D3D11_TEXTURE2D_DESC  tDepthDesc = {};

	tDepthDesc.Width = iWidth;
	tDepthDesc.Height = iHeight;
	tDepthDesc.ArraySize = 1;
	tDepthDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	tDepthDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	tDepthDesc.Usage = D3D11_USAGE_DEFAULT;
	tDepthDesc.SampleDesc.Count = 1;
	tDepthDesc.SampleDesc.Quality = 0;
	tDepthDesc.MipLevels = 1;

	ID3D11Texture2D* pDepthTex = nullptr;

	m_pDevice->CreateTexture2D(&tDepthDesc, nullptr, &pDepthTex);

	m_pDevice->CreateDepthStencilView(pDepthTex,nullptr, &m_pDepthView);

	SAFE_RELEASE(pDepthTex);

	m_pContext->OMSetRenderTargets(1,&m_pTargetView,m_pDepthView);

	D3D11_VIEWPORT tVF = {};
	tVF.TopLeftX = 0.f;
	tVF.TopLeftY = 0.f;
	tVF.Width = (float)iWidth;
	tVF.Height = (float)iHeight;
	tVF.MaxDepth = 1.f;
	
	m_pContext->RSSetViewports(1, &tVF);

	IDXGISurface* pBackSurface = nullptr;

	if(FAILED(m_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackSurface))))
		return false;

	if (FAILED(D2D1CreateFactory(D2D1_FACTORY_TYPE_MULTI_THREADED, &m_p2DFactory)))
		return false;

	D2D1_RENDER_TARGET_PROPERTIES props = D2D1::RenderTargetProperties(D2D1_RENDER_TARGET_TYPE_HARDWARE,
		D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED));

	if (FAILED(m_p2DFactory->CreateDxgiSurfaceRenderTarget(pBackSurface, props, &m_p2DRenderTarget)))
		return false;

	SAFE_RELEASE(pBackSurface);

	CONTEXT->QueryInterface(&m_pThread);

	m_pThread->SetMultithreadProtected(TRUE);

	return true;
}

void CDevice::ClearTarget()
{
	m_pContext->ClearRenderTargetView(m_pTargetView, m_pClearColor);
	m_pContext->ClearDepthStencilView(m_pDepthView, 
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);
}

void CDevice::Render()
{
	m_pSwapChain->Present(0, 0);
}

void CDevice::SetTarget()
{
	m_pThread->Enter();
	m_pContext->OMSetRenderTargets(1, &m_pTargetView, m_pDepthView);
	m_pThread->Leave();
}

void CDevice::OnResize()
{
	//D3DApp::OnResize();
	///*
	//Vector2 vRatio = GetWindowRatio();
	//float fRatio = vRatio.x / vRatio.y;
	//XMMATRIX P = XMMatrixPerspectiveFovLH(0.25f * DirectX::XM_PI, 
	//	fRatio, 1.0f, 1000.f);
	//XMStoreFloat4x4(&mProj, P);*/

	//static_cast<CCamera*>(CCameraManager::GetInst()->GetMainCam().get())->SetCameraType(CAMERA_TYPE::CT_3D);
	//
	//BuildOffscreenViews();
	//mBlur.Init(m_tRS.iWidth, m_tRS.iHeight,
	//	DXGI_FORMAT_R8G8B8A8_UNORM);
}

void CDevice::Enter()
{
	m_pThread->Enter();
}

void CDevice::Leave()
{
	m_pThread->Leave();
}
