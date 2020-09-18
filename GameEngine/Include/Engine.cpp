#include "Engine.h"
#include "Device.h"
#include "Scene/SceneManager.h"
#include "Timer.h"
#include "PathManager.h"
#include "Resource/ResourceManager.h"
#include "Resource/ShaderManager.h"
#include "Resource/Shader.h"
#include "Resource/Mesh2D.h"
#include "RenderManager.h"

DEFINITION_SINGLE(CEngine)

bool CEngine::m_bLoop = true;

CEngine::CEngine() :
	m_hInst(0),
	m_hWnd(0),
	m_tRS()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetBreakAlloc();
}

CEngine::~CEngine()
{
	DESTROY_SINGLE(CDevice);
	DESTROY_SINGLE(CSceneManager);
	DESTROY_SINGLE(CTimer);
	DESTROY_SINGLE(CPathManager);
	DESTROY_SINGLE(CResourceManager);
	DESTROY_SINGLE(CRenderManager);
}

bool CEngine::Init(const TCHAR* strClass, const TCHAR* strTitle, HINSTANCE hInst, int iIcon, int iSmIcon, int iWidth, int iHeight, bool bWindowMode)
{
	m_hInst = hInst;

	m_tRS.iWidth = iWidth;
	m_tRS.iHeight = iHeight;

	Register(strClass, iIcon, iSmIcon);
	Create(strClass, strTitle);

	return Init(m_hInst, m_hWnd, iWidth, iHeight, bWindowMode);
}

bool CEngine::Init(HINSTANCE hInst, HWND hWnd,
	int iWidth, int iHeight, bool bWindowMode)
{
	m_hInst = hInst;

	m_tRS.iWidth = iWidth;
	m_tRS.iHeight = iHeight;

	m_hWnd = hWnd;

	// 디바이스 초기화
	if (!GET_SINGLE(CDevice)->Init(hWnd, iWidth, iHeight, bWindowMode))
		return false;

	// 시간 관리자 초기화
	if (!GET_SINGLE(CTimer)->Init())
		return false;

	// 경로 관리자 초기화
	if (!GET_SINGLE(CPathManager)->Init())
		return false;

	// 자원 관리자 초기화
	if (!GET_SINGLE(CResourceManager)->Init())
		return false;

	// 장면 관리자 초기화
	if (!GET_SINGLE(CSceneManager)->Init())
		return false;

	// 랜더 관리자 초기화
	if (!GET_SINGLE(CRenderManager)->Init())
		return false;

	return true;
}

int CEngine::Run()
{
	MSG msg = {};

	while (m_bLoop)
	{
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			Logic();
		}
	}

	return (int)msg.wParam;
}

void CEngine::Logic()
{
	GET_SINGLE(CTimer)->Update();

	float fTime = GET_SINGLE(CTimer)->GetDeltaTime();

	Input(fTime);
	Update(fTime);
	PostUpdate(fTime);
	Collision(fTime);
	PreRender(fTime);
	Render(fTime);
	PostRender(fTime);
}

int CEngine::Input(float fTime)
{
	GET_SINGLE(CSceneManager)->Input(fTime);
	return 0;
}

int CEngine::Update(float fTime)
{
	GET_SINGLE(CSceneManager)->Update(fTime);

	return 0;
}

int CEngine::PostUpdate(float fTime)
{
	GET_SINGLE(CSceneManager)->PostUpdate(fTime);

	return 0;
}

int CEngine::Collision(float fTime)
{
	GET_SINGLE(CSceneManager)->Collision(fTime);

	return 0;
}

int CEngine::PreRender(float fTime)
{
	GET_SINGLE(CSceneManager)->PreRender(fTime);

	return 0;
}

int CEngine::Render(float fTime)
{
	GET_SINGLE(CDevice)->ClearTarget();

	//GET_SINGLE(CSceneManager)->Render(fTime);

	GET_SINGLE(CRenderManager)->Render(fTime);

	GET_SINGLE(CRenderManager)->Clear();

	GET_SINGLE(CDevice)->Render();

	return 0;
}

int CEngine::PostRender(float fTime)
{
	GET_SINGLE(CSceneManager)->PostRender(fTime);

	return 0;
}

ATOM CEngine::Register(const TCHAR* strClass, int iIcon, int iSmIcon)
{
	WNDCLASSEX tWndClass = {};

	tWndClass.cbSize = sizeof(WNDCLASSEX);
	tWndClass.cbClsExtra = 0;
	tWndClass.cbWndExtra = 0;
	tWndClass.hIcon = LoadIcon(tWndClass.hInstance, MAKEINTRESOURCE(iIcon));
	tWndClass.hIconSm = LoadIcon(tWndClass.hInstance, MAKEINTRESOURCE(iSmIcon));
	tWndClass.hInstance = m_hInst;
	tWndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	tWndClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
	tWndClass.lpfnWndProc = CEngine::WndProc;
	tWndClass.lpszClassName = strClass;
	tWndClass.lpszMenuName = strClass;
	tWndClass.style = CS_HREDRAW | CS_VREDRAW;

	return RegisterClassEx(&tWndClass);
}

BOOL CEngine::Create(const TCHAR* strClass, const TCHAR* strTitle)
{
	m_hWnd = CreateWindow(strClass, strTitle, WS_OVERLAPPEDWINDOW | WS_MINIMIZEBOX | WS_SYSMENU,
		100, 100, m_tRS.iWidth, m_tRS.iHeight, NULL, NULL, m_hInst, NULL);

	if (!m_hWnd)
		return FALSE;

	RECT tRC = { 0, 0, m_tRS.iWidth, m_tRS.iHeight };

	AdjustWindowRect(&tRC, WS_OVERLAPPEDWINDOW | WS_MINIMIZEBOX | WS_SYSMENU, false);
	SetWindowPos(m_hWnd, HWND_TOPMOST, 100, 100,
		tRC.right - tRC.left, tRC.bottom - tRC.top,
		SWP_NOZORDER | SWP_NOMOVE);

	ShowWindow(m_hWnd, SW_SHOW);
	UpdateWindow(m_hWnd);

	return TRUE;
}

LRESULT CEngine::WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	switch (iMessage)
	{
	case WM_DESTROY:
		m_bLoop = false;
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, iMessage, wParam, lParam);
	}

	return LRESULT();
}
