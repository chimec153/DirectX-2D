
#include "ImGuiManager.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"
#include "imgui.h"
#include "../../Device.h"
#ifdef _DEBUG
#include "../../Engine.h"
#endif

DEFINITION_SINGLE(CImGuiManager)

CImGuiManager::CImGuiManager()
{

}

CImGuiManager::~CImGuiManager()
{
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

bool CImGuiManager::Init(HWND hWnd, ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui_ImplWin32_EnableDpiAwareness();

	ImGuiIO& io = ImGui::GetIO();

	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;	//	게임 패드로 할 때

	ImGui::StyleColorsDark();

	ImGui_ImplWin32_Init(hWnd);
	ImGui_ImplDX11_Init(pDevice, pContext);	

	return true;
}

void CImGuiManager::PreRender(float fTime)
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	//bool bOpen = true;

	//ImGui::ShowDemoWindow(&bOpen);
#ifdef _DEBUG
	if (ImGui::Begin("Debug"))
	{
		bool bEnable = GET_SINGLE(CEngine)->IsColliderEnabled();

		if (ImGui::Checkbox("Render Collider", &bEnable))
		{
			GET_SINGLE(CEngine)->SetCollider(bEnable);
		}
	}

	ImGui::End();
#endif
}

void CImGuiManager::Render(float fTime)
{
	GET_SINGLE(CDevice)->GetThread()->Enter();
	ImGui::Render();
	GET_SINGLE(CDevice)->GetThread()->Leave();

	ImGuiIO& io = ImGui::GetIO();

	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
	}

	GET_SINGLE(CDevice)->SetTarget();

	GET_SINGLE(CDevice)->GetThread()->Enter();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	GET_SINGLE(CDevice)->GetThread()->Leave();
}
