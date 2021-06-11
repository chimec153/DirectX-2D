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
#include "Input.h"
#include "CameraManager.h"
#include "Collision/CollisionManager.h"
#include "UI/UIManager.h"
#include "Sound/SoundManager.h"
#include "UI/FontManager.h"
#include "UI/UFont.h"
#include "Scene/Scene.h"
#include "Thread/ThreadManager.h"
#include "UI/imgui/ImGuiManager.h"
#include "UI/imgui/imgui.h"
#include "UI/imgui/imgui_impl_win32.h"
#include "LightManager.h"
#include "LensFlares.h"
#include "Server/ClientSocket.h"

DEFINITION_SINGLE(CEngine)

bool CEngine::m_bLoop = true;

CEngine::CEngine() :
	m_hInst(0),
	m_hWnd(0),
	m_tRS(),
	m_pInstance(nullptr),
	m_bEditor(false)
#ifdef _DEBUG
	, m_bCollider(true)
#endif
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
}

CEngine::~CEngine()
{
	SAFE_DELETE(m_pInstance);
	DESTROY_SINGLE(CDevice);
	DESTROY_SINGLE(CSceneManager);
	DESTROY_SINGLE(CTimer);
	DESTROY_SINGLE(CPathManager);
	DESTROY_SINGLE(CInput);
	DESTROY_SINGLE(CCollisionManager);
	DESTROY_SINGLE(CUIManager);
	DESTROY_SINGLE(CResourceManager);
	DESTROY_SINGLE(CRenderManager);
	DESTROY_SINGLE(CSoundManager);
	DESTROY_SINGLE(CFontManager);
	DESTROY_SINGLE(CCameraManager);
	DESTROY_SINGLE(CThreadManager);
	DESTROY_SINGLE(CImGuiManager);
	DESTROY_SINGLE(CLensFlares);
	DESTROY_SINGLE(CLightManager);
}

HINSTANCE CEngine::GetWindowInstance() const
{
	return m_hInst;
}

HWND CEngine::GetWindowHandle() const
{
	return m_hWnd;
}

const Resolution& CEngine::GetResolution() const
{
	return m_tRS;
}

bool CEngine::IsExit() const
{
	return !m_bLoop;
}

bool CEngine::IsEditor() const
{
	return m_bEditor;
}

void CEngine::SetEditor()
{
	m_bEditor = true;
}

#ifdef _DEBUG
void CEngine::SetCollider(bool bCol)
{
	m_bCollider = bCol;
}
bool CEngine::IsColliderEnabled() const
{
	return m_bCollider;
}
#endif

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

	// 글꼴 관리자 초기화
	if (!GET_SINGLE(CFontManager)->Init())
		return false;

#ifdef _DEBUG
	if (_heapchk() != _HEAPOK)
		DebugBreak();
#endif
	// 자원 관리자 초기화
	if (!GET_SINGLE(CResourceManager)->Init())
		return false;

#ifdef _DEBUG
	if (_heapchk() != _HEAPOK)
		DebugBreak();
#endif
	// 충돌 관리자 초기화
	if (!GET_SINGLE(CCollisionManager)->Init())
		return false;

#ifdef _DEBUG
	if (_heapchk() != _HEAPOK)
		DebugBreak();
#endif
	// 랜더 관리자 초기화
	if (!GET_SINGLE(CRenderManager)->Init())
		return false;

	// 장면 관리자 초기화
	if (!GET_SINGLE(CSceneManager)->Init())
		return false;

	// 카메라 관리자 초기화
	if (!GET_SINGLE(CCameraManager)->Init(m_tRS))
		return false;

	// 입력 관리자 초기화
	if (!GET_SINGLE(CInput)->Init(m_hInst, m_hWnd))
		return false;

	// UI 관리자 초기화
	if (!GET_SINGLE(CUIManager)->Init())
		return false;

	// 소리 관리자 초기화
	if (!GET_SINGLE(CSoundManager)->Init())
		return false;

	// 쓰레드 관리자 초기화
	if (!GET_SINGLE(CThreadManager)->Init())
		return false;

	// ImGui 관리자 초기화
	if (!GET_SINGLE(CImGuiManager)->Init(m_hWnd, DEVICE, CONTEXT))
		return false;

	// 빛 관리자 초기화
	if (!GET_SINGLE(CLightManager)->Init())
		return false;

	// 렌즈 플레어 초기화
	if (!GET_SINGLE(CLensFlares)->Init())
		return false;

#ifdef _DEBUG
	if (_heapchk() != _HEAPOK)
		DebugBreak();
#endif
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

	GET_SINGLE(CInput)->Update(fTime);
	GET_SINGLE(CCameraManager)->Update(fTime);

	if (Input(fTime))
		return;

	if (Update(fTime))
		return;

	if (PostUpdate(fTime))
		return;

	if (Collision(fTime))
		return;

	PreRender(fTime);
	Render(fTime);
	PostRender(fTime);
}

bool CEngine::Input(float fTime)
{
#ifdef _DEBUG
	if (GetAsyncKeyState(VK_ESCAPE) & 0x01)
	{
		GET_SINGLE(CEngine)->SetCollider(!GET_SINGLE(CEngine)->IsColliderEnabled());
	}
#endif

	return GET_SINGLE(CSceneManager)->Input(fTime);
}

bool CEngine::Update(float fTime)
{
	GET_SINGLE(CSceneManager)->CheckFrustum();
	GET_SINGLE(CSoundManager)->Update(fTime);

	GET_SINGLE(CLensFlares)->Update(fTime);

	return GET_SINGLE(CSceneManager)->Update(fTime);
}

bool CEngine::PostUpdate(float fTime)
{
	return GET_SINGLE(CSceneManager)->PostUpdate(fTime);
}

bool CEngine::Collision(float fTime)
{
	GET_SINGLE(CInput)->Collision(fTime);
	return GET_SINGLE(CSceneManager)->Collision(fTime);
}

void CEngine::PreRender(float fTime)
{

	if (m_bEditor)
	{
		GET_SINGLE(CImGuiManager)->PreRender(fTime);

		GET_SINGLE(CResourceManager)->ShowWindow();

		GET_SINGLE(CLightManager)->ShowWindow();

		GET_SINGLE(CSceneManager)->ShowWindow();

		GET_SINGLE(CCameraManager)->ShowWindow();

		GET_SINGLE(CTimer)->ShowWindow();
		GET_SINGLE(CLensFlares)->ShowWindow();
		GET_SINGLE(CRenderManager)->ShowWindow();
	}
	GET_SINGLE(CSceneManager)->PreRender(fTime);

}

void CEngine::Render(float fTime)
{
	GET_SINGLE(CDevice)->ClearTarget();

	//GET_SINGLE(CSceneManager)->Render(fTime);

	GET_SINGLE(CRenderManager)->PreRender(fTime);
	
	GET_SINGLE(CInput)->PreRender(fTime);

	GET_SINGLE(CRenderManager)->Render(fTime);

	GET_SINGLE(CLensFlares)->Render(fTime);

	GET_SINGLE(CRenderManager)->Clear();	


	GET_SINGLE(CRenderManager)->PostRender(fTime);

	if (m_bEditor)
	{
		GET_SINGLE(CImGuiManager)->Render(fTime);
	}
	GET_SINGLE(CInput)->Render(fTime);

	GET_SINGLE(CDevice)->Render();
}

void CEngine::PostRender(float fTime)
{
	GET_SINGLE(CSceneManager)->PostRender(fTime);
}

void CEngine::EndGame()
{
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
	tWndClass.lpszMenuName = NULL;// strClass;
	tWndClass.style = CS_HREDRAW | CS_VREDRAW;

	return RegisterClassEx(&tWndClass);
}

BOOL CEngine::Create(const TCHAR* strClass, const TCHAR* strTitle)
{
	m_hWnd = CreateWindow(strClass, strTitle, WS_OVERLAPPEDWINDOW | WS_MINIMIZEBOX | WS_SYSMENU,
		0, 0, m_tRS.iWidth, m_tRS.iHeight, NULL, NULL, m_hInst, NULL);
	//m_hWnd = CreateWindowEx(WS_EX_APPWINDOW, strClass, strTitle, WS_POPUP,
		//0, 0, m_tRS.iWidth, m_tRS.iHeight, NULL, NULL, m_hInst, NULL);

	if (!m_hWnd)
		return FALSE;

	RECT tRC = { 0, 0, m_tRS.iWidth, m_tRS.iHeight };

	AdjustWindowRect(&tRC, WS_OVERLAPPEDWINDOW | WS_MINIMIZEBOX | WS_SYSMENU, false);
	SetWindowPos(m_hWnd, HWND_TOPMOST, 0, 0,
		tRC.right - tRC.left, tRC.bottom - tRC.top,
		SWP_NOZORDER | SWP_NOMOVE);

	ShowWindow(m_hWnd, SW_SHOW);
	UpdateWindow(m_hWnd);

	return TRUE;
}

IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT CEngine::WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	static HIMC imeID = nullptr;
	static std::wstring str;

	if (ImGui_ImplWin32_WndProcHandler(hWnd, iMessage, wParam, lParam))
		return true;

	switch (iMessage)
	{
	case WM_ACTIVATE:
	{
		if (!imeID)
		{
			imeID = ImmCreateContext();
			ImmAssociateContext(hWnd, imeID);
		}
		switch (wParam)
		{
		case WA_ACTIVE:
		case WA_CLICKACTIVE:
		{

			GET_SINGLE(CInput)->SetWinActive(true);
		}
			break;
		case WA_INACTIVE:
		{
			GET_SINGLE(CInput)->SetWinActive(false);
		}
			break;
		}
	}

		break;
	case WM_IME_STARTCOMPOSITION:
		break;
	case WM_IME_CHAR:
	{
		str.push_back(static_cast<TCHAR>(wParam));
		GET_SINGLE(CInput)->InputWord(str);
	}
		break;
	case WM_CHAR:
	{
		switch (wParam)
		{
		case '\b':
			if (!str.empty())
			{
				str.pop_back();
			}
			break;
		case VK_RETURN:
		{
			GET_SINGLE(CInput)->EnterIME(str);
			str.clear();
		}
			break;
		default:
		{
			str.push_back(static_cast<TCHAR>(wParam));
			GET_SINGLE(CInput)->InputWord(str);
		}
			break;
		}
	}
		break;
	case WM_DESTROY:
		m_bLoop = false;
		ImmDestroyContext(imeID);
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, iMessage, wParam, lParam);
	}

	return LRESULT();
}
