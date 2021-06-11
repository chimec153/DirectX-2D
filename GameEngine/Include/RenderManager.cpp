#include "RenderManager.h"
#include "Component/SceneComponent.h"
#include "Render/BlendState.h"
#include "Render/RasterizerState.h"
#include "Render/DepthStencilState.h"
#include "UI/UIControl.h"
#include <algorithm>
#include "Object/GameObject.h"
#include "Layer/Layer.h"
#include "LayerInstance.h"
#include "Resource/Mesh2D.h"
#include "Resource/Material.h"
#include "Tile.h"
#include "Resource/ResourceManager.h"
#include "CameraManager.h"
#include "Component/Camera.h"
#include "Component/SpriteComponent.h"
#include "Resource/Animation2DSequence.h"
#include "Resource/Texture.h"
#include "UI/UFont.h"
#include "Object/GameObject.h"
#include "Scene/Scene.h"
#include "Scene/SceneManager.h"
#include "Component/Collider.h"
#include "Render/MRT.h"
#include "Render/PostProcess.h"
#include "Resource/ShaderManager.h"
#include "Render/Blur.h"
#include "LightManager.h"
#include "Component/Renderer.h"
#include "Component/Transform.h"
#include "Render/MultiRenderTarget.h"
#include "UI/imgui/imgui.h"
#include "Scene/Scene.h"
#include "Render/RenderInstancing.h"
#include "Resource/Animation.h"
#include "Resource/Mesh.h"
#include "Timer.h"
#ifdef _DEBUG
#include "Engine.h"
#endif
#include "Component/Light.h"

DEFINITION_SINGLE(CRenderManager)

CRenderManager::CRenderManager()	:
	m_pTileInstancing(nullptr)
	, m_pBlur(nullptr)
	, m_pFullBlur(nullptr)
	, m_pAlphaBlend(nullptr)
	, m_pNoDepthWrite(nullptr)
	, m_tEquipBuffer()
	, m_bFullBlur(false)
	, m_tGlobalBuffer()
{
	m_tGlobalBuffer.fSampleDist = 1.f;
	m_tGlobalBuffer.fSampleStrength = 2.2f;
	m_tGlobalBuffer.vBlurCenter = Vector2(0.5f, 0.5f);
	//m_vec2DComponent.reserve(400);
	for (int i = 0; i < static_cast<int>(LAYER::END); i++)
	{
		m_vecComponent[i].reserve(50);
	}
}

CRenderManager::~CRenderManager()
{
	SAFE_DELETE_MAP(m_mapState);
	Safe_Delete_Map(m_mapInstancing);
	SAFE_DELETE(m_pTileInstancing);
	Safe_Release_Map(m_mapState);
	Safe_Release_Map(m_mapMRT);
	SAFE_RELEASE(m_pBlur);
	SAFE_RELEASE(m_pFullBlur);
	Safe_Release_Map(m_mapMultiRenderTarget);
	SAFE_RELEASE(m_pAlphaBlend);
	SAFE_RELEASE(m_pNoDepthWrite);
	Safe_Delete_VecList(m_RenderInstList);
#ifdef _DEBUG
	SAFE_RELEASE(m_pDebugMesh);
#endif
}

bool CRenderManager::Init()
{
	AddBlendInfo("Blend_Disable", false);
	CreateBlendState("Blend_Disable");

	AddBlendInfo(ALPHA_BLEND);
	CreateBlendState(ALPHA_BLEND);

	m_pAlphaBlend = FindState(ALPHA_BLEND);

	AddBlendInfo("LightAccBlend", true, D3D11_BLEND_ONE, D3D11_BLEND_ONE, 
		D3D11_BLEND_OP_ADD, D3D11_BLEND_ONE, D3D11_BLEND_ZERO);
	CreateBlendState("LightAccBlend");

	AddBlendInfo("DecalBlend", true, D3D11_BLEND_SRC_ALPHA, D3D11_BLEND_INV_SRC_ALPHA, D3D11_BLEND_OP_ADD,
		D3D11_BLEND_ONE, D3D11_BLEND_ZERO, D3D11_BLEND_OP_ADD, D3D11_COLOR_WRITE_ENABLE_RED |
		D3D11_COLOR_WRITE_ENABLE_GREEN | D3D11_COLOR_WRITE_ENABLE_BLUE);
	AddBlendInfo("DecalBlend", true, D3D11_BLEND_SRC_ALPHA, D3D11_BLEND_INV_SRC_ALPHA, D3D11_BLEND_OP_ADD,
		D3D11_BLEND_ONE, D3D11_BLEND_ZERO, D3D11_BLEND_OP_ADD, D3D11_COLOR_WRITE_ENABLE_RED |
		D3D11_COLOR_WRITE_ENABLE_GREEN | D3D11_COLOR_WRITE_ENABLE_BLUE);
	AddBlendInfo("DecalBlend", true, D3D11_BLEND_ONE, D3D11_BLEND_ZERO);

	CreateBlendState("DecalBlend", false, true);

	AddRasterizerInfo(CULL_NONE, D3D11_FILL_SOLID, D3D11_CULL_NONE);
	CreateRasterizerState(CULL_NONE);

	AddRasterizerInfo("Cull_Front", D3D11_FILL_SOLID, D3D11_CULL_FRONT);
	CreateRasterizerState("Cull_Front");

	AddRasterizerInfo("Depth", D3D11_FILL_SOLID, D3D11_CULL_BACK, false, true, 100000, 0.f, 1.f);
	CreateRasterizerState("Depth");

	CreateDepthStencilState(NO_DEPTH, false);

	CreateDepthStencilState(LESS_EQUAL_DSS, true, D3D11_DEPTH_WRITE_MASK_ZERO, D3D11_COMPARISON_LESS_EQUAL);
	CreateDepthStencilState("DepthNoWrite", true, D3D11_DEPTH_WRITE_MASK_ZERO);
	CreateDepthStencilState("DepthNoRead", true, D3D11_DEPTH_WRITE_MASK_ALL, D3D11_COMPARISON_ALWAYS);	

	m_pNoDepthWrite = FindState("DepthNoWrite");

	AddRasterizerInfo(WIRE_FRAME, D3D11_FILL_WIREFRAME, D3D11_CULL_NONE);
	CreateRasterizerState(WIRE_FRAME);

	AddRasterizerInfo("Shadow", D3D11_FILL_SOLID, D3D11_CULL_BACK, false, true, 1000, 0.f, 1.f);
	CreateRasterizerState("Shadow");

	AddRasterizerInfo("PointShadow", D3D11_FILL_SOLID, D3D11_CULL_BACK, false, true, 10, 0.f, 1.f);
	CreateRasterizerState("PointShadow");

	D3D11_DEPTH_STENCILOP_DESC tFront = {};

	tFront.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	tFront.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	tFront.StencilPassOp = D3D11_STENCIL_OP_REPLACE;
	tFront.StencilFunc = D3D11_COMPARISON_ALWAYS;

	CreateDepthStencilState("UI", true, D3D11_DEPTH_WRITE_MASK_ZERO,
		D3D11_COMPARISON_LESS, true, 0xff, 0xff, tFront, tFront);

	CDepthStencilState* pState = (CDepthStencilState*)FindState("UI");

	pState->SetStencilRef(1);

	D3D11_DEPTH_STENCILOP_DESC tFront2 = {};

	tFront2.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	tFront2.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	tFront2.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	tFront2.StencilFunc = D3D11_COMPARISON_EQUAL;

	CreateDepthStencilState("Draw", true, D3D11_DEPTH_WRITE_MASK_ZERO,
		D3D11_COMPARISON_LESS, true, 0xff, 0xff,
		tFront2, tFront2);

	pState = (CDepthStencilState*)FindState("Draw");

	pState->SetStencilRef(1);

	D3D11_DEPTH_STENCILOP_DESC tOutLineFront = {};
	tOutLineFront.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	tOutLineFront.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	tOutLineFront.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	tOutLineFront.StencilFunc = D3D11_COMPARISON_EQUAL;

	if (!CreateDepthStencilState("OutLine", true, D3D11_DEPTH_WRITE_MASK_ZERO, D3D11_COMPARISON_ALWAYS,
		true, 0xff, 0xff, tOutLineFront, tOutLineFront))
		return false;

	pState = static_cast<CDepthStencilState*>(FindState("OutLine"));

	pState->SetStencilRef(0);

	D3D11_DEPTH_STENCILOP_DESC tOutLineObjectFront = {};
	tOutLineObjectFront.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	tOutLineObjectFront.StencilDepthFailOp = D3D11_STENCIL_OP_REPLACE;
	tOutLineObjectFront.StencilPassOp = D3D11_STENCIL_OP_REPLACE;
	tOutLineObjectFront.StencilFunc = D3D11_COMPARISON_ALWAYS;

	if (!CreateDepthStencilState("OutLineObject", true, D3D11_DEPTH_WRITE_MASK_ALL, D3D11_COMPARISON_LESS,
		true, 0xff, 0xff, tOutLineObjectFront, tOutLineObjectFront))
		return false;

	pState = static_cast<CDepthStencilState*>(FindState("OutLineObject"));

	pState->SetStencilRef(1);

	Resolution tRS = RESOLUTION;
	if (!CreateMRT("Scene", tRS.iWidth,tRS.iHeight, DXGI_FORMAT_R32G32B32A32_FLOAT, DXGI_FORMAT_R32G32B32A32_FLOAT, DXGI_FORMAT_R24G8_TYPELESS))
	{
		return false;
	}

	if (!CreateMultiRenderTarget("GBuffer", tRS.iWidth, tRS.iHeight, DXGI_FORMAT_D24_UNORM_S8_UINT))
		return false;

	if (!CreateMRT("Albedo", tRS.iWidth, tRS.iHeight, DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_FORMAT_R24G8_TYPELESS))
		return false;
	if (!CreateMRT("GBuffer1", tRS.iWidth, tRS.iHeight, DXGI_FORMAT_R32G32B32A32_FLOAT, DXGI_FORMAT_R32G32B32A32_FLOAT, DXGI_FORMAT_R24G8_TYPELESS))
		return false;
	if (!CreateMRT("GBuffer2", tRS.iWidth, tRS.iHeight, DXGI_FORMAT_R32G32B32A32_FLOAT, DXGI_FORMAT_R32G32B32A32_FLOAT, DXGI_FORMAT_R24G8_TYPELESS))
		return false;
	if (!CreateMRT("GBuffer3", tRS.iWidth, tRS.iHeight, DXGI_FORMAT_R32G32B32A32_FLOAT, DXGI_FORMAT_R32G32B32A32_FLOAT, DXGI_FORMAT_R24G8_TYPELESS))
		return false;
	if (!CreateMRT("GBuffer4", tRS.iWidth, tRS.iHeight, DXGI_FORMAT_R32G32B32A32_FLOAT, DXGI_FORMAT_R32G32B32A32_FLOAT, DXGI_FORMAT_R24G8_TYPELESS))
		return false;
	if (!CreateMRT("GBuffer5", tRS.iWidth, tRS.iHeight, DXGI_FORMAT_R32G32B32A32_FLOAT, DXGI_FORMAT_R32G32B32A32_FLOAT, DXGI_FORMAT_R24G8_TYPELESS))
		return false;

	SetRenderTargetView("GBuffer", "Albedo");
	SetRenderTargetView("GBuffer", "GBuffer1");
	SetRenderTargetView("GBuffer", "GBuffer2");
	SetRenderTargetView("GBuffer", "GBuffer3");
	SetRenderTargetView("GBuffer", "GBuffer4");
	SetRenderTargetView("GBuffer", "GBuffer5");

	if (!CreateMultiRenderTarget("Decal", tRS.iWidth, tRS.iHeight, DXGI_FORMAT_D24_UNORM_S8_UINT))
		return false;

	if (!CreateMRT("DecalAlbedo", tRS.iWidth, tRS.iHeight, DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_FORMAT_R24G8_TYPELESS))
		return false;
	if (!CreateMRT("DecalGBuffer1", tRS.iWidth, tRS.iHeight, DXGI_FORMAT_R32G32B32A32_FLOAT, DXGI_FORMAT_R32G32B32A32_FLOAT, DXGI_FORMAT_R24G8_TYPELESS))
		return false;
	if (!CreateMRT("DecalGBuffer2", tRS.iWidth, tRS.iHeight, DXGI_FORMAT_R32G32B32A32_FLOAT, DXGI_FORMAT_R32G32B32A32_FLOAT, DXGI_FORMAT_R24G8_TYPELESS))
		return false;

	SetRenderTargetView("Decal", "DecalAlbedo");
	SetRenderTargetView("Decal", "DecalGBuffer1");
	SetRenderTargetView("Decal", "DecalGBuffer2");

	if (!CreateMultiRenderTarget("LightAcc", tRS.iWidth, tRS.iHeight, DXGI_FORMAT_D24_UNORM_S8_UINT))
		return false;

	if (!CreateMRT("LightDif", tRS.iWidth, tRS.iHeight, DXGI_FORMAT_R32G32B32A32_FLOAT, DXGI_FORMAT_R32G32B32A32_FLOAT, DXGI_FORMAT_R24G8_TYPELESS))
		return false;
	if (!CreateMRT("LightSpc", tRS.iWidth, tRS.iHeight, DXGI_FORMAT_R32G32B32A32_FLOAT, DXGI_FORMAT_R32G32B32A32_FLOAT, DXGI_FORMAT_R24G8_TYPELESS))
		return false;
	if (!CreateMRT("LightEmv", tRS.iWidth, tRS.iHeight, DXGI_FORMAT_R32G32B32A32_FLOAT, DXGI_FORMAT_R32G32B32A32_FLOAT, DXGI_FORMAT_R24G8_TYPELESS))
		return false;

	SetRenderTargetView("LightAcc", "LightDif");
	SetRenderTargetView("LightAcc", "LightSpc");
	SetRenderTargetView("LightAcc", "LightEmv");

	CreateMRT("SwordLight", tRS.iWidth, tRS.iHeight, 
		DXGI_FORMAT_R32G32B32A32_FLOAT, DXGI_FORMAT_R32G32B32A32_FLOAT, DXGI_FORMAT_R24G8_TYPELESS);

	CreateMRT("Equip", tRS.iWidth, tRS.iHeight, DXGI_FORMAT_R32G32B32A32_FLOAT, DXGI_FORMAT_R32G32B32A32_FLOAT,
		DXGI_FORMAT_R24G8_TYPELESS);

	CreateMRT("OutLine", tRS.iWidth, tRS.iHeight, DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_FORMAT_R24G8_TYPELESS);

	CreateMRT("Shadow", tRS.iWidth * 4, tRS.iHeight * 4, DXGI_FORMAT_R32G32B32A32_FLOAT,
	DXGI_FORMAT_R32G32B32A32_FLOAT, DXGI_FORMAT_R24G8_TYPELESS);

	/*CMRT* pShadow = new CMRT;

	pShadow->SetName("Shadow");

	if (!pShadow->CreateCubeTexture(1024, DXGI_FORMAT_D32_FLOAT))
	{
		SAFE_RELEASE(pShadow);
		return false;
	}

	m_mapMRT.insert(std::make_pair(pShadow->GetName(), pShadow));*/


	m_pAlphaShader = GET_SINGLE(CShaderManager)->FindShader("NoDepthAlpha");
	m_pRadialBlur = GET_SINGLE(CShaderManager)->FindShader("FullRadialBlurShader");

#ifdef _DEBUG
	CMRT* pMRT = FindRenderTarget("Scene");

	pMRT->CreateDebugMatrix(Vector3(100.f, 100.f, 0.f), Vector3(0.f, 0.f, 0.f));

	SAFE_RELEASE(pMRT);

	CreateDebugWVP("Albedo", Vector3(100.f, 100.f, 0.f), Vector3(100.f, 0.f, 0.f));
	CreateDebugWVP("GBuffer1", Vector3(100.f, 100.f, 0.f), Vector3(100.f, 100.f, 0.f));
	CreateDebugWVP("GBuffer2", Vector3(100.f, 100.f, 0.f), Vector3(100.f, 200.f, 0.f));
	CreateDebugWVP("GBuffer3", Vector3(100.f, 100.f, 0.f), Vector3(100.f, 300.f, 0.f));
	CreateDebugWVP("GBuffer4", Vector3(100.f, 100.f, 0.f), Vector3(100.f, 400.f, 0.f));
	CreateDebugWVP("GBuffer5", Vector3(100.f, 100.f, 0.f), Vector3(100.f, 500.f, 0.f));
	CreateDebugWVP("LightDif", Vector3(100.f, 100.f, 0.f), Vector3(200.f, 0.f, 0.f));
	CreateDebugWVP("LightSpc", Vector3(100.f, 100.f, 0.f), Vector3(200.f, 100.f, 0.f));
	CreateDebugWVP("LightEmv", Vector3(100.f, 100.f, 0.f), Vector3(200.f, 200.f, 0.f));
	CreateDebugWVP("DecalAlbedo", Vector3(100.f, 100.f, 0.f), Vector3(300.f, 0.f, 0.f));
	CreateDebugWVP("DecalGBuffer1", Vector3(100.f, 100.f, 0.f), Vector3(300.f, 100.f, 0.f));
	CreateDebugWVP("DecalGBuffer2", Vector3(100.f, 100.f, 0.f), Vector3(300.f, 200.f, 0.f));
	CreateDebugWVP("SwordLight", Vector3(100.f, 100.f, 0.f), Vector3(400.f, 0.f, 0.f));
	CreateDebugWVP("Equip", Vector3(100.f, 100.f, 0.f), Vector3(400.f, 100.f, 0.f));
	CreateDebugWVP("OutLine", Vector3(100.f, 100.f, 0.f), Vector3(400.f, 200.f, 0.f));
	CreateDebugWVP("Shadow", Vector3(100.f, 100.f, 0.f), Vector3(500.f, 0.f, 0.f));

	m_pDebugMesh = GET_SINGLE(CResourceManager)->GetDefaultMesh();
	m_pDebugShader = GET_SINGLE(CShaderManager)->FindShader("DebugShader");
	m_pDebugMtrl = GET_SINGLE(CResourceManager)->FindMaterial("Color");

#endif
	m_pEquipShader = GET_SINGLE(CShaderManager)->FindShader("EquipShader");

	m_pBlur = new CBlur;

	if (!m_pBlur->Init())
		return false;

	m_pBlur->SetRenderTarget("SwordLight");

	m_pFullBlur = new CBlur;

	if (!m_pFullBlur->Init())
		return false;

	m_pFullBlur->SetRenderTarget("Scene");

	m_pFullScreenShader = GET_SINGLE(CShaderManager)->FindShader("FullScreenShader");
	m_pLightAccShader = GET_SINGLE(CShaderManager)->FindShader("LightAcc");
	m_pLightCombineShader = GET_SINGLE(CShaderManager)->FindShader("LightCombine");
	m_pLensShader = GET_SINGLE(CShaderManager)->FindShader("LensFlareShader");
	m_pShader = GET_SINGLE(CShaderManager)->FindShader("Shadow");
	m_pPointShader = GET_SINGLE(CShaderManager)->FindShader("PointShadow");

	return true;
}

void CRenderManager::PreRender(float fTime)
{
	for (int i = 0; i < static_cast<int>(LAYER::END); i++)
	{
		switch (static_cast<LAYER>(i))
		{
		case LAYER::TERRAIN:
			std::sort(m_vecComponent[i].begin(), m_vecComponent[i].end(), SortVZ);
			break;
		case LAYER::OUTLINE:
			std::sort(m_vecComponent[i].begin(), m_vecComponent[i].end(), SortVZ);
			break;
		case LAYER::ALBEDO:
			std::sort(m_vecComponent[i].begin(), m_vecComponent[i].end(), SortVZ);
			break;
		case LAYER::COLLIDER:
			std::sort(m_vecComponent[i].begin(), m_vecComponent[i].end(), SortVZ);
			break;
		case LAYER::DECAL:
			std::sort(m_vecComponent[i].begin(), m_vecComponent[i].end(), SortVZInv);
			break;
		case LAYER::GBUFFER:
			std::sort(m_vecComponent[i].begin(), m_vecComponent[i].end(), SortVZ);
			break;
		case LAYER::CUBEMAP:
			std::sort(m_vecComponent[i].begin(), m_vecComponent[i].end(), SortVZ);
			break;
		case LAYER::ALPHA:
			std::sort(m_vecComponent[i].begin(), m_vecComponent[i].end(), SortVZInv);
			break;
		case LAYER::SWORD_LIGHT:
			std::sort(m_vecComponent[i].begin(), m_vecComponent[i].end(), SortVZInv);
			break;
		case LAYER::PARTICLE:
			std::sort(m_vecComponent[i].begin(), m_vecComponent[i].end(), SortVZInv);
			break;
		case LAYER::UI:
			break;
		case LAYER::EQUIP:
			break;
		}
	}
}

void CRenderManager::Render(float fTime)
{
	GET_SINGLE(CLightManager)->SetShader();

	//RenderObject(fTime);
	Render3D(fTime);
	RenderUI(fTime);
	//RenderEtc(fTime);

#ifdef _DEBUG
	SetState(NO_DEPTH);
	std::unordered_map<std::string, CMRT*>::iterator iter;
	std::unordered_map<std::string, CMRT*>::iterator iterEnd = m_mapMRT.end();

	CMRT* pShadow = FindRenderTarget("Shadow");
	for (iter = m_mapMRT.begin(); iter != iterEnd; iter++)
	{
		if (iter->second == pShadow)
			continue;

		iter->second->SetShader(0);
		m_pDebugMtrl->Render(fTime);
		m_pDebugShader->SetShader();
		m_pDebugMesh->Render(fTime);

		iter->second->ResetShader(0);
	}


	pShadow->SetDepthSRV(0);
	m_pDebugMtrl->Render(fTime);
	m_pDebugShader->SetShader();
	m_pDebugMesh->Render(fTime);
	pShadow->ResetDepthSRV(0);

	SAFE_RELEASE(pShadow);

	ResetState(NO_DEPTH);
#endif
}

void CRenderManager::PostRender(float fTime)
{

}

void CRenderManager::Clear()
{
	m_vec2DComponent.clear();
	for (int i = 0; i < static_cast<int>(LAYER::END); i++)
	{
		m_vecComponent[i].clear();
	}
	m_vecUI.clear();
	m_LayerList.clear();
	m_vecObjCom.clear();

	std::unordered_map<unsigned __int64, CLayerInstance*>::iterator iter = m_mapInstancing.begin();
	std::unordered_map<unsigned __int64, CLayerInstance*>::iterator iterEnd = m_mapInstancing.end();

	for (; iter != iterEnd; ++iter)
		iter->second->Clear();

	if(m_pTileInstancing)
	m_pTileInstancing->Clear();

	m_PostList.clear();

	std::unordered_map<std::string, CMultiRenderTarget*>::iterator iterM = m_mapMultiRenderTarget.begin();
	std::unordered_map<std::string, CMultiRenderTarget*>::iterator iterMEnd = m_mapMultiRenderTarget.end();

	for (; iterM != iterMEnd; iterM++)
	{
		iterM->second->Clear();
	}

	std::unordered_map<std::string, CMRT*>::iterator iterMRT = m_mapMRT.begin();
	std::unordered_map<std::string, CMRT*>::iterator iterMRTEnd = m_mapMRT.end();

	for (; iterMRT != iterMRTEnd; ++iterMRT)
	{
		iterMRT->second->ClearTarget();
	}
}

void CRenderManager::ShowWindow()
{
	if (ImGui::Begin("Render Manager"))
	{
		std::vector<const char*> vecstrState;

		std::unordered_map<std::string, CRenderState*>::iterator iter = m_mapState.begin();
		std::unordered_map<std::string, CRenderState*>::iterator iterEnd = m_mapState.end();

		for (; iter != iterEnd; ++iter)
		{
			vecstrState.push_back(iter->second->GetName().c_str());
		}
		static int iSel = -1;
		if (m_mapState.size() > 0)
		{
			ImGui::ListBox("Render State", &iSel, &vecstrState[0], static_cast<int>(m_mapState.size()));

			if (iSel != -1 && m_mapState.size() > iSel)
			{
				CRenderState* pState = FindState(vecstrState[iSel]);

				if (pState)
				{
					pState->ShowWindow();

					SAFE_RELEASE(pState);
				}
			}
		}

		if (ImGui::SliderFloat("SampleDist", &m_tGlobalBuffer.fSampleDist, 0.5f, 5.f))
		{
			SetSampleDist(m_tGlobalBuffer.fSampleDist);
		}
		if (ImGui::SliderFloat("SampleStrength", &m_tGlobalBuffer.fSampleStrength, 1.f, 5.f))
		{
			SetSampleStrength(m_tGlobalBuffer.fSampleStrength);
		}
		if (ImGui::InputFloat2("Center", &m_tGlobalBuffer.vBlurCenter.x))
		{
			SetBlurCenter(m_tGlobalBuffer.vBlurCenter);
		}
	}
	ImGui::End();
}

void CRenderManager::SetEquipBuffer(const TransformCBuffer& tCBuffer)
{
	m_tEquipBuffer = tCBuffer;
}

bool CRenderManager::AddComponent(CSceneComponent* pComponent)
{
	if (pComponent->IsInstance())
	{
		SCENE_COMPONENT_TYPE eType = pComponent->GetSceneComponentType();

		switch (eType)
		{
		case SCENE_COMPONENT_TYPE::SCT_2D:
		{
			if (pComponent->GetSceneComponentClassType() == (int)SCENECOMPONENT_CLASS_TYPE::SCT_SPRITE)
				return false;

			else if (pComponent->GetSceneComponentClassType() == (int)SCENECOMPONENT_CLASS_TYPE::SCT_COLLIDER_RECT ||
				pComponent->GetSceneComponentClassType() == (int)SCENECOMPONENT_CLASS_TYPE::SCT_COLLIDER_LINE ||
				pComponent->GetSceneComponentClassType() == (int)SCENECOMPONENT_CLASS_TYPE::SCT_COLLIDER_CIRCLE ||
				pComponent->GetSceneComponentClassType() == (int)SCENECOMPONENT_CLASS_TYPE::SCT_COLLIDER_OBB2D ||
				pComponent->GetSceneComponentClassType() == (int)SCENECOMPONENT_CLASS_TYPE::SCT_COLLIDER_PIXEL ||
				pComponent->GetSceneComponentClassType() == (int)SCENECOMPONENT_CLASS_TYPE::COLLIDER_POINT)
				return false;

			std::shared_ptr<CMaterial> pMtrl = pComponent->GetMaterial();

			class CMesh* pMesh = pComponent->GetMesh();

			unsigned __int64 iMtrlKey = GetHashingKey(pMtrl->GetName());
			unsigned __int64 iMeshKey = GetHashingKey(pMesh->GetName());

			unsigned __int64 iKey = iMtrlKey << 32 | iMeshKey;

			CLayerInstance* pInstance = FindInstancingGeometry(iKey);

			if (!pInstance)
			{
				pInstance = new CLayerInstance;

				std::shared_ptr<CMaterial> pClone = pMtrl->Clone();

				pClone->SetShader("Inst");

				pInstance->Init(pMesh, pClone, sizeof(INSTANCINGDATA));

				pInstance->SetLayer(pComponent->GetLayer());

				m_mapInstancing.insert(std::make_pair(iKey, pInstance));
			}

			PINSTANCINGDATA pData = new INSTANCINGDATA;

			CCamera* pCam = GET_SINGLE(CCameraManager)->GetMainCam();

			pData->matWVP = pComponent->GetMatWorld() * pCam->GetVP();

			pData->matWVP.Transpose();
			pData->vMeshSize = pComponent->GetMeshSize();
			pData->vPivot = pComponent->GetPivot();

			pInstance->AddInstancingData(pData);

			pInstance->AddCom(pComponent);

			delete pData;

			return true;
		}
			break;
		case SCENE_COMPONENT_TYPE::SCT_3D:
		{
			int iType = pComponent->GetSceneComponentClassType();

			if (iType == static_cast<int>(SCENECOMPONENT_CLASS_TYPE::NONE))
			{
				bool bCreate = true;

				class CMesh* pMesh = pComponent->GetMesh();

				CRenderInstancing* pInstancing = nullptr;

				std::list<CRenderInstancing*>::iterator iter = m_RenderInstList.begin();
				std::list<CRenderInstancing*>::iterator iterEnd = m_RenderInstList.end();

				for (; iter != iterEnd; ++iter)
				{
					if ((*iter)->GetMesh() == pMesh)
					{
						bCreate = false;
						pInstancing = *iter;
						break;
					}
				}

				if (bCreate && pMesh)
				{
					pInstancing = new CRenderInstancing;
					if (pMesh->GetMeshType() == MESH_TYPE::MT_ANIMATION_MESH)
					{
						CAnimation* pAni = pComponent->GetAnimation();

						CSkeleton* pSkeleton = pAni->GetSkeleton();

						int iBoneCount = pSkeleton->GetBoneCount();

						SAFE_RELEASE(pSkeleton);

						SAFE_RELEASE(pAni);

						pInstancing->Init(pMesh, iBoneCount);

						pInstancing->CreateBoneBuffer();
					}
					else
					{

						pInstancing->Init(pMesh, 0);
					}

					m_RenderInstList.push_back(pInstancing);
				}

				if (pInstancing)
				{
					pComponent->SetInstIndex(pInstancing->GetCount());
					pComponent->SetInstancing(pInstancing);
					pInstancing->AddComponent(pComponent);
				}
			}
#ifdef _DEBUG
			else if(iType == static_cast<int>(SCENECOMPONENT_CLASS_TYPE::COLLIDER_SPHERE))
			{
				if (GET_SINGLE(CEngine)->IsColliderEnabled())
				{
					std::list<CRenderInstancing*>::iterator iter = m_RenderInstList.begin();
					std::list<CRenderInstancing*>::iterator iterEnd = m_RenderInstList.end();

					class CMesh* pMesh = static_cast<CCollider*>(pComponent)->GetDebugMesh();

					bool bCreate = true;
					for (; iter != iterEnd; ++iter)
					{

						if ((*iter)->GetMesh() == pMesh)
						{
							bCreate = false;
							(*iter)->AddComponent(pComponent);
							break;
						}
					}

					if (bCreate)
					{
						CRenderInstancing* pInst = new CRenderInstancing;

						pInst->Init(pMesh, 0);

						pInst->AddComponent(pComponent);

						pInst->CreateInstBuffer(1000, 272);
						pInst->SetShader("ColliderInst3D");
						//CRenderState* pDepth = FindState(NO_DEPTH);
						//pInst->SetDepthState(pDepth);
						CRenderState* pRasterizer = FindState(WIRE_FRAME);
						pInst->SetRasterizer(pRasterizer);
						//SAFE_RELEASE(pDepth);
						SAFE_RELEASE(pRasterizer);

						m_RenderInstList.push_back(pInst);
					}
				}
			}
#endif
		}
			break;
		case SCENE_COMPONENT_TYPE::SCT_UI:
		{

		}
			break;
		}

	}

	else if (pComponent->IsPostProcess())
	{
		m_PostList.push_back(pComponent);
	}

	else
	{
		switch (pComponent->GetSceneComponentType())
		{
		case SCENE_COMPONENT_TYPE::SCT_2D:
			m_vec2DComponent.push_back(pComponent);
			break;
		case SCENE_COMPONENT_TYPE::SCT_3D:
		{
			CLayer* pLayer = pComponent->GetLayer();

			int iZOrder = pLayer->GetZOrder();

			m_vecComponent[iZOrder].push_back(pComponent);
		}	
			break;
		case SCENE_COMPONENT_TYPE::SCT_UI:
			m_vecUI.push_back(pComponent);
			break;
		}

		if (pComponent->IsUIEnabled())
		{
			m_vecComponent[static_cast<int>(LAYER::EQUIP)].push_back(pComponent);
		}
	}

	return true;
}

bool CRenderManager::AddAnim2DInstancing(CSpriteComponent* pComponent)
{
	std::shared_ptr<CMaterial> pMtrl = pComponent->GetMaterial();

	class CMesh* pMesh = pComponent->GetMesh();

	unsigned __int64 iMtrlKey = GetHashingKey(pMtrl->GetName());
	unsigned __int64 iMeshKey = GetHashingKey(pMesh->GetName());

	unsigned __int64 iKey = iMtrlKey << 32 | iMeshKey;

	CLayerInstance* pInstance = FindInstancingGeometry(iKey);

	if (!pInstance)
	{
		pInstance = new CLayerInstance;

		std::shared_ptr<CMaterial> pClone = pMtrl->Clone();

		pClone->SetShader("Anim2DInst");

		Ani2DInfo* pInfo = ((CSpriteComponent*)pComponent)->GetCurrentAni();

		std::shared_ptr<CTexture> pTex = pInfo->pSequence->GetTexture();

		pClone.get()->SetTexture(LINK_TYPE::LT_DIF, pTex);

		pInstance->Init(pMesh, pClone, sizeof(INSTANCINGANIM2D));

		pInstance->SetLayer(pComponent->GetLayer());

		m_mapInstancing.insert(std::make_pair(iKey, pInstance));
	}

	PINSTANCINGANIM2D pData = new INSTANCINGANIM2D;

	CCamera* pCam = GET_SINGLE(CCameraManager)->GetMainCam();

	pData->matWVP = pComponent->GetMatWorld() * pCam->GetView() * pCam->GetProj();

	pData->matWVP.Transpose();
	pData->vMeshSize = pComponent->GetMeshSize();
	pData->vPivot = pComponent->GetPivot();

	Ani2DInfo* pInfo = ((CSpriteComponent*)pComponent)->GetCurrentAni();

	std::shared_ptr<CTexture> pTex = pInfo->pSequence->GetTexture();

	pData->vTexSize = Vector2((float)pTex->GetWidth(), (float)pTex->GetHeight());

	int iCnt = pInfo->pSequence->GetFrameCount();

	if (iCnt > 0)
	{
		FrameInfo tInfo = ((CSpriteComponent*)pComponent)->GetFrameInfo();

		pData->vStart = tInfo.vStart;
		pData->vEnd = tInfo.vEnd;
	}

	else
	{
		pData->vStart = Vector2(0.f, 0.f);
		pData->vEnd = pData->vTexSize;
	}

	pInstance->AddInstancingData(pData);

	pInstance->AddCom(pComponent);

	delete pData;

	return true;
}

bool CRenderManager::AddColliderInstancing(CCollider* pComponent)
{
	std::shared_ptr<CMaterial> pMtrl = pComponent->GetMaterial();

	if (!pMtrl)
		return false;

	class CMesh* pMesh = pComponent->GetMesh();

	if (!pMesh)
		return false;
	
	unsigned __int64 iMtrlKey = GetHashingKey(pMtrl->GetName());
	unsigned __int64 iMeshKey = GetHashingKey(pMesh->GetName());

	unsigned __int64 iKey = iMtrlKey << 32 | iMeshKey;

	CLayerInstance* pInstance = FindInstancingGeometry(iKey);

	if (!pInstance)
	{
		pInstance = new CLayerInstance;

		std::shared_ptr<CMaterial> pClone = pMtrl->Clone();

		pClone->SetShader("ColliderInst");

		pInstance->Init(pMesh, pClone, sizeof(INSTANCINGCOLLIDER));

		pInstance->SetLayer(pComponent->GetLayer());

		m_mapInstancing.insert(std::make_pair(iKey, pInstance));
	}

	PINSTANCINGCOLLIDER pData = new INSTANCINGCOLLIDER;

	CCamera* pCam = GET_SINGLE(CCameraManager)->GetMainCam();

	pData->matWVP = pComponent->GetMatWorld() * pCam->GetView() * pCam->GetProj();

	pData->matWVP.Transpose();
	pData->vMeshSize = pComponent->GetMeshSize();
	pData->vPivot = pComponent->GetPivot();
	pData->vColor = pMtrl->GetDiffuseColor();

	pInstance->AddInstancingData(pData);

	pInstance->AddCom(pComponent);

	delete pData;

	return true;
}

unsigned int CRenderManager::GetHashingKey(const std::string& strKey)
{
	const char* pKey = strKey.c_str();

	unsigned int iKey = 1;

	int iLength = (int)strKey.length();

	for (int i = 0; i < iLength; ++i)
	{
		iKey += 1;

		iKey <<= 3;

		iKey *= pKey[i] + 10;
	}

	return iKey;
}

CMRT* CRenderManager::FindRenderTarget(const std::string& strKey)
{
	std::unordered_map<std::string, CMRT*>::iterator iter = m_mapMRT.find(strKey);

	if (iter == m_mapMRT.end())
		return nullptr;

	iter->second->AddRef();

	return iter->second;
}

void CRenderManager::SetTarget(const std::string& strKey)
{
	CMRT* pMRT = FindRenderTarget(strKey);

	if (!pMRT)
	{
		return;
	}

	pMRT->SetTarget();

	pMRT->Release();
}

void CRenderManager::ResetTarget(const std::string& strKey)
{
	CMRT* pMRT = FindRenderTarget(strKey);

	if (!pMRT)
	{
		return;
	}

	pMRT->ResetTarget();

	pMRT->Release();
}

void CRenderManager::SetShader(const std::string& strKey, int iRegister, int iType)
{
	CMRT* pMRT = FindRenderTarget(strKey);

	if (!pMRT)
	{
		return;
	}

	pMRT->SetShader(iRegister, iType);

	pMRT->Release();
}

void CRenderManager::ResetShader(const std::string& strKey, int iRegister,int iType)
{
	CMRT* pMRT = FindRenderTarget(strKey);

	if (!pMRT)
	{
		return;
	}

	pMRT->ResetShader(iRegister, iType);

	pMRT->Release();
}

void CRenderManager::SetUAV(const std::string& strKey, int iRegister)
{
	CMRT* pMRT = FindRenderTarget(strKey);

	if (!pMRT)
	{
		return;
	}

	pMRT->CSSetUAV(iRegister);

	pMRT->Release();
}

#ifdef _DEBUG
void CRenderManager::CreateDebugWVP(const std::string& strKey, const Vector3& vScale, const Vector3& vTranslation)
{
	CMRT* pMRT = FindRenderTarget(strKey);

	if (!pMRT)
	{
		return;
	}

	pMRT->CreateDebugMatrix(vScale, vTranslation);

	pMRT->Release();
}
#endif

void CRenderManager::ClearTarget(const std::string& strKey)
{
	CMRT* pMRT = FindRenderTarget(strKey);

	if (!pMRT)
	{
		return;
	}

	pMRT->ClearTarget();

	pMRT->Release();
}

bool CRenderManager::CreateMRT(const std::string& strKey, int iWidth, int iHeight, DXGI_FORMAT eTexFmt, DXGI_FORMAT eFmt, DXGI_FORMAT eDepthFmt)
{
	CMRT* pMRT = FindRenderTarget(strKey);

	if (pMRT)
	{
		SAFE_RELEASE(pMRT);
		return false;
	}

	pMRT = new CMRT;

	pMRT->SetName(strKey);

	if (!pMRT->CreateTexture(iWidth, iHeight, eTexFmt, eFmt, eDepthFmt))
	{
		SAFE_RELEASE(pMRT);
		return false;
	}

	m_mapMRT.insert(std::make_pair(strKey, pMRT));

	return true;
}

void CRenderManager::SetRenderTargetView(const std::string& strKey, const std::string& strMRT)
{
	CMultiRenderTarget* pMRT = FindMRT(strKey);

	if (!pMRT)
		return;

	CMRT* _pMRT = FindRenderTarget(strMRT);

	if (!_pMRT)
	{
		SAFE_RELEASE(pMRT);
		return;
	}

	ID3D11RenderTargetView* pRTV = _pMRT->GetRenderTargetView();

	pMRT->AddTarget(pRTV);

	SAFE_RELEASE(_pMRT);
	SAFE_RELEASE(pMRT);
}

bool CRenderManager::CreateMultiRenderTarget(const std::string& strKey, int iWidth, int iHeight, DXGI_FORMAT eFmt)
{
	CMultiRenderTarget* pMRT = FindMRT(strKey);

	if (pMRT)
	{
		SAFE_RELEASE(pMRT);
		return false;
	}

	pMRT = new CMultiRenderTarget;

	if (!pMRT->CreateDepth(iWidth, iHeight, eFmt))
	{
		SAFE_RELEASE(pMRT);
		return false;
	}

	m_mapMultiRenderTarget.insert(std::make_pair(strKey, pMRT));

	return true;
}

void CRenderManager::SetMRT(const std::string& strKey)
{
	CMultiRenderTarget* pMRT = FindMRT(strKey);

	if (!pMRT)
		return;

	pMRT->SetTarget();

	SAFE_RELEASE(pMRT);
}

void CRenderManager::ResetMRT(const std::string& strKey)
{
	CMultiRenderTarget* pMRT = FindMRT(strKey);

	if (!pMRT)
		return;

	pMRT->ResetTarget();

	SAFE_RELEASE(pMRT);
}

void CRenderManager::ClearMRT(const std::string& strKey)
{
	CMultiRenderTarget* pMRT = FindMRT(strKey);

	if (!pMRT)
		return;

	pMRT->Clear();

	SAFE_RELEASE(pMRT);
}

CMultiRenderTarget* CRenderManager::FindMRT(const std::string& strKey)
{
	std::unordered_map<std::string, CMultiRenderTarget*>::iterator iter = m_mapMultiRenderTarget.find(strKey);

	if (iter == m_mapMultiRenderTarget.end())
		return nullptr;

	iter->second->AddRef();

	return iter->second;
}

bool CRenderManager::SortVZ(CSceneComponent* pSrc, CSceneComponent* pDest)
{
	CCamera* pCam = GET_SINGLE(CCameraManager)->GetMainCam();

	Matrix matView = pCam->GetView();

	Vector3 vSrc = pSrc->GetWorldPos();

	Vector3 vSrcView = vSrc.TranslationCoor(matView);

	Vector3 vDest = pDest->GetWorldPos();

	Vector3 vDestView = vDest.TranslationCoor(matView);

	return vSrcView.z < vDestView.z;
}

bool CRenderManager::SortVZInv(CSceneComponent* pSrc, CSceneComponent* pDest)
{
	CCamera* pCam = GET_SINGLE(CCameraManager)->GetMainCam();

	Matrix matView = pCam->GetView();

	Vector3 vSrc = pSrc->GetWorldPos();

	Vector3 vSrcView = vSrc.TranslationCoor(matView);

	Vector3 vDest = pDest->GetWorldPos();

	Vector3 vDestView = vDest.TranslationCoor(matView);

	return vSrcView.z > vDestView.z;
}

bool CRenderManager::SortZOrder(CSceneComponent* pSrc, CSceneComponent* pDest)
{
	return pSrc->GetZOrder() < pDest->GetZOrder();
}

void CRenderManager::SetFullBlur(bool bBlur)
{
	m_bFullBlur = bBlur;
}

void CRenderManager::SetBlurCenter(const Vector2& vCenter)
{
	m_tGlobalBuffer.vBlurCenter = vCenter;
}

void CRenderManager::SetSampleDist(float fDist)
{
	m_tGlobalBuffer.fSampleDist = fDist;
}

void CRenderManager::SetSampleStrength(float fStrength)
{
	m_tGlobalBuffer.fSampleStrength = fStrength;
}

void CRenderManager::AddObj(CUIObject* pObj)
{
}

void CRenderManager::ChangeScene()
{
	SAFE_DELETE_MAP(m_mapInstancing);
	SAFE_DELETE(m_pTileInstancing);
	SAFE_DELETE_VECLIST(m_RenderInstList);
}

void CRenderManager::AddObjCom(CObjectComponent* pObjCom)
{
	m_vecObjCom.push_back(pObjCom);
}

bool CRenderManager::AddBlendInfo(const std::string& strTag, bool bEnable, D3D11_BLEND eSrcBlend, D3D11_BLEND eDestBlend, 
	D3D11_BLEND_OP eBlendOp, D3D11_BLEND eSrcBlendAlpha, D3D11_BLEND eDestBlendAlpha, D3D11_BLEND_OP eBlendOpAlpha, UINT8 cRenderTargetWriteMask)
{
	CBlendState* pState = (CBlendState*)FindState(strTag);

	if (!pState)
	{
		pState = new CBlendState;
		pState->SetName(strTag);
		m_mapState.insert(std::make_pair(strTag, pState));
	}

	return pState->AddBlendInfo(bEnable, eSrcBlend, eDestBlend, eBlendOp, eSrcBlendAlpha, eDestBlendAlpha, eBlendOpAlpha, cRenderTargetWriteMask);
}

bool CRenderManager::CreateBlendState(const std::string& strTag, bool bAlpha, bool bBlend)
{
	CBlendState* pState = (CBlendState*)FindState(strTag);

	if (!pState)
		return false;

	if (!pState->CreateBlendState(bAlpha, bBlend))
	{
		SAFE_DELETE(pState);

		return false;
	}

	m_mapState.insert(std::make_pair(strTag, pState));

	return true;
}

bool CRenderManager::AddRasterizerInfo(const std::string& strTag, D3D11_FILL_MODE eFillMode, D3D11_CULL_MODE eCullMode, bool bClock, bool bClip,
	int iDepthBias, float fDepthBiasClamp, float fSlopeScaledDepthBias)
{
	CRasterizerState* pState = (CRasterizerState*)FindState(strTag);

	if (pState)
		return false;

	pState = new CRasterizerState;

	pState->SetName(strTag);

	if (!pState->AddRasterizerInfo(eFillMode, eCullMode, bClock, bClip, 
		iDepthBias, fDepthBiasClamp, fSlopeScaledDepthBias))
	{
		SAFE_DELETE(pState);
		return false;
	}

	m_mapState.insert(std::make_pair(strTag, pState));

	return true;
}

bool CRenderManager::CreateRasterizerState(const std::string& strTag)
{
	CRasterizerState* pState = (CRasterizerState*)FindState(strTag);

	if (!pState)
		return false;

	if(!pState->CreateRasterizerState())
	{
		SAFE_DELETE(pState);
		return false;
	}

	m_mapState.insert(std::make_pair(strTag, pState));

	return true;
}

bool CRenderManager::CreateDepthStencilState(const std::string& strTag, bool bEnable, D3D11_DEPTH_WRITE_MASK eMask, 
	D3D11_COMPARISON_FUNC eFunc, bool bStencilEnable, UINT8 iReadMask, UINT8 iWriteMask, const D3D11_DEPTH_STENCILOP_DESC& tFront, const D3D11_DEPTH_STENCILOP_DESC& tBack)
{
	CDepthStencilState* pState = (CDepthStencilState*)FindState(strTag);

	if (pState)
		return false;

	pState = new CDepthStencilState;
	pState->SetName(strTag);

	if (!pState->AddDepthStencilInfo(bEnable, eMask, eFunc, bStencilEnable, iReadMask, iWriteMask, tFront, tBack))
	{
		SAFE_DELETE(pState);
		return false;
	}

	m_mapState.insert(std::make_pair(strTag, pState));

	return true;
}

void CRenderManager::Render3D(float fTime)
{
	RenderShadow(fTime);
	RenderTerrain(fTime);
	RenderCubeMap(fTime);
	//RenderOutLine(fTime);
	RenderAlbedo(fTime);
	RenderDecal(fTime);
	RenderGBuffer(fTime);
	RenderLightAcc(fTime);
	RenderLightCombine(fTime);
	RenderScene(fTime);
	PostProcess2D(fTime);
	RenderSwordLight(fTime);
	RenderAlpha(fTime);
	RenderParticle(fTime);
	RenderCollider(fTime);
	//RenderMinimap(fTime);
}

void CRenderManager::RenderUI(float fTime)
{
	SetState("AlphaBlend");
	SetState("NoDepth");
	size_t iSz = m_vecUI.size();

	if(iSz > 1)
		qsort(&m_vecUI[0], iSz, sizeof(CSceneComponent*), SortZ);

	for (size_t i = 0; i < iSz; ++i)
	{
		m_vecUI[i]->Render(fTime);
	}
	ResetState("AlphaBlend");
	ResetState("NoDepth");

	RenderEtc(fTime);
}

void CRenderManager::RenderObject(float fTime)
{
	ClearTarget("Scene");
	SetTarget("Scene");

	SetState("NoCullRS");
	SetState("AlphaBlend");
	SetState("NoDepth");

	RenderTileInstancing();

	std::list<class CLayer*>::iterator iter = m_LayerList.begin();
	std::list<class CLayer*>::iterator iterEnd = m_LayerList.end();

	for (; iter != iterEnd; ++iter)
	{
		std::unordered_map<unsigned __int64, CLayerInstance*>::iterator iterInst = m_mapInstancing.begin();
		std::unordered_map<unsigned __int64, CLayerInstance*>::iterator iterInstEnd = m_mapInstancing.end();

		int i = 0;

		for (; iterInst != iterInstEnd; ++iterInst)
		{
			if (iterInst->second->m_pLayer == (*iter))
			{
				if (iterInst->second->GetComSize() <= 10)
				{
					const std::vector<CSceneComponent*>* vecCom = iterInst->second->GetComponents();

					std::vector<CSceneComponent*>::const_iterator iterCom = vecCom->begin();
					std::vector<CSceneComponent*>::const_iterator iterComEnd = vecCom->end();

					for (; iterCom != iterComEnd; ++iterCom)
					{
						switch ((*iterCom)->GetSceneComponentType())
						{
						case SCENE_COMPONENT_TYPE::SCT_2D:
							m_vec2DComponent.push_back((*iterCom));
							break;
						case SCENE_COMPONENT_TYPE::SCT_3D:
						{
							CLayer* pLayer = (*iterCom)->GetLayer();

							int iZOrder = pLayer->GetZOrder();

							m_vecComponent[iZOrder].push_back((*iterCom));
						}							
							break;
						case SCENE_COMPONENT_TYPE::SCT_UI:
							m_vecUI.push_back((*iterCom));
							break;
						}
					}
				}
				else
				{
					iterInst->second->Render();
					iterInst->second->RenderFont(i++);
				}
			}
		}

		size_t iSize = m_vec2DComponent.size();

		for (size_t i = 0; i < iSize; ++i)
		{
			if ((*iter) == m_vec2DComponent[i]->GetLayer())
			{
				m_vec2DComponent[i]->Render(fTime);
			}
		}
	}

	ResetState("NoCullRS");
	ResetState("AlphaBlend");
	ResetState("NoDepth");

	ResetTarget("Scene");

	RenderTarget2D(fTime);
	PostProcess2D(fTime);
}

void CRenderManager::RenderTarget2D(float fTime)
{
	//m_pBlur->Render(fTime);

	SetShader("Scene", 0, (int)SHADER_CBUFFER_TYPE::CBUFFER_COMPUTE | (int)SHADER_CBUFFER_TYPE::CBUFFER_PIXEL);
	SetState("NoCullRS");
	SetState("NoDepth");

	m_pFullScreenShader->SetShader();

	UINT iOffset = 0;

	CONTEXT->IASetInputLayout(nullptr);
	CONTEXT->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	CONTEXT->IASetVertexBuffers(0, 0, nullptr, 0, &iOffset);
	CONTEXT->IASetIndexBuffer(nullptr, DXGI_FORMAT_UNKNOWN, 0);
	CONTEXT->Draw(4, 0);

	ResetState("NoCullRS");
	ResetState("NoDepth");
	ResetShader("Scene", 0, (int)SHADER_CBUFFER_TYPE::CBUFFER_COMPUTE | (int)SHADER_CBUFFER_TYPE::CBUFFER_PIXEL);
}

void CRenderManager::PostProcess2D(float fTime)
{
	SetShader("Scene", 103, (int)SHADER_CBUFFER_TYPE::CBUFFER_COMPUTE | (int)SHADER_CBUFFER_TYPE::CBUFFER_PIXEL);
	SetState("NoDepth");
	std::list<CSceneComponent*>::iterator iter;
	std::list<CSceneComponent*>::iterator iterEnd = m_PostList.end();

	for (iter = m_PostList.begin(); iter != iterEnd; ++iter)
	{
		(*iter)->Render(fTime);
	}

	ResetState("NoDepth");
	ResetShader("Scene", 103, (int)SHADER_CBUFFER_TYPE::CBUFFER_COMPUTE | (int)SHADER_CBUFFER_TYPE::CBUFFER_PIXEL);
}

void CRenderManager::RenderShadow(float fTime)
{
	CMRT* pShadow = FindRenderTarget("Shadow");

	pShadow->SetDepthView();
	pShadow->SetViewPort();

	m_pShader->SetShader();
	//m_pPointShader->SetShader();

	CRenderState* pRS = FindState("Shadow");
	//CRenderState* pRS = FindState("PointShadow");

	pRS->SetState();

	size_t iSize = m_vecComponent[static_cast<int>(LAYER::TERRAIN)].size();

	for (size_t i = 0; i < iSize; i++)
	{
		m_vecComponent[static_cast<int>(LAYER::TERRAIN)][i]->RenderShadow(fTime);
	}

	iSize = m_vecComponent[static_cast<int>(LAYER::ALBEDO)].size();

	for (size_t i = 0; i < iSize; i++)
	{
		m_vecComponent[static_cast<int>(LAYER::ALBEDO)][i]->RenderShadow(fTime);
	}

	iSize = m_vecComponent[static_cast<int>(LAYER::ALPHA)].size();

	for (size_t i = 0; i < iSize; i++)
	{
		m_vecComponent[static_cast<int>(LAYER::ALPHA)][i]->RenderShadow(fTime);
	}

	std::list<CRenderInstancing*>::iterator iter = m_RenderInstList.begin();
	std::list<CRenderInstancing*>::iterator iterEnd = m_RenderInstList.end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->RenderShadow(fTime);
	}

	pRS->ResetState();
	SAFE_RELEASE(pRS);

	pShadow->ResetTarget();
	pShadow->ResetViewPort();
	SAFE_RELEASE(pShadow);
}

void CRenderManager::RenderTerrain(float fTime)
{
	CMultiRenderTarget* pMRT = FindMRT("GBuffer");
	//CMRT* pShadow = FindRenderTarget("Shadow");

	//pShadow->SetDepthSRV(105);
	pMRT->SetTarget();
	size_t iSize = m_vecComponent[static_cast<int>(LAYER::TERRAIN)].size();

	for (size_t i = 0; i < iSize; ++i)
	{
		m_vecComponent[static_cast<int>(LAYER::TERRAIN)][i]->Render(fTime);
	}

	pMRT->ResetTarget();
	SAFE_RELEASE(pMRT);
	//pShadow->ResetDepthSRV(105);
	//SAFE_RELEASE(pShadow);
}

void CRenderManager::RenderOutLine(float fTime)
{
	CMRT* pMRT = FindRenderTarget("OutLine");

	pMRT->SetTarget();

	SetState("OutLine");

	size_t iSize = m_vecComponent[static_cast<int>(LAYER::OUTLINE)].size();

	for (size_t i = 0; i < iSize; i++)
	{
		m_vecComponent[static_cast<int>(LAYER::OUTLINE)][i]->Render(fTime);
	}

	ResetState("OutLine");

	pMRT->ResetTarget();

	m_pBlur->SetRenderTarget("OutLine");

	m_pBlur->Render(fTime);
	SAFE_RELEASE(pMRT);

	CMultiRenderTarget* pGBuffer = FindMRT("GBuffer");

	pGBuffer->SetTarget();

	CMRT* pOutLine = FindRenderTarget("OutLine");
	pOutLine->SetShader(20);

	m_pFullScreenShader->SetShader();
	SetState(NO_DEPTH);
	m_pAlphaBlend->SetState();
	CONTEXT->IASetInputLayout(nullptr);
	ID3D11Buffer* pBuffer1 = nullptr;
	UINT iOffset = 0;
	CONTEXT->IASetVertexBuffers(0, 1, &pBuffer1, &iOffset, &iOffset);
	CONTEXT->IASetIndexBuffer(nullptr, DXGI_FORMAT_UNKNOWN, iOffset);
	CONTEXT->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	CONTEXT->Draw(4, 0);
	m_pAlphaBlend->ResetState();
	ResetState(NO_DEPTH);
	pOutLine->ResetShader(20);
	SAFE_RELEASE(pOutLine);

	pGBuffer->ResetTarget();
	SAFE_RELEASE(pGBuffer);

}

void CRenderManager::RenderCollider(float fTime)
{
	//CMRT* pMRT = FindRenderTarget("Scene");

	//pMRT->SetTarget();

	size_t iSize = m_vecComponent[static_cast<int>(LAYER::COLLIDER)].size();

	for (size_t i = 0; i < iSize; ++i)
	{
		m_vecComponent[static_cast<int>(LAYER::COLLIDER)][i]->Render(fTime);
	}
	//pMRT->ResetTarget();
	//SAFE_RELEASE(pMRT);
}

void CRenderManager::RenderCubeMap(float fTime)
{
	CMRT* pMRT = FindRenderTarget("Scene");

	pMRT->SetTarget();

	size_t iSize = m_vecComponent[static_cast<int>(LAYER::CUBEMAP)].size();

	for (size_t i = 0; i < iSize; ++i)
	{
		m_vecComponent[static_cast<int>(LAYER::CUBEMAP)][i]->Render(fTime);
	}

	pMRT->ResetTarget();
	SAFE_RELEASE(pMRT);
}

void CRenderManager::RenderAlbedo(float fTime)
{
	CMultiRenderTarget* pMRT = FindMRT("GBuffer");
	//CMRT* pShadow = FindRenderTarget("Shadow");

	if (pMRT)
	{
		pMRT->SetTarget();
	}
	//pShadow->SetDepthSRV(105);


	size_t iSize = m_vecComponent[static_cast<int>(LAYER::ALBEDO)].size();

	for (size_t i = 0; i < iSize; ++i)
	{
		m_vecComponent[static_cast<int>(LAYER::ALBEDO)][i]->Render(fTime);
	}

	std::list<CRenderInstancing*>::iterator iter = m_RenderInstList.begin();
	std::list<CRenderInstancing*>::iterator iterEnd = m_RenderInstList.end();

	for (; iter != iterEnd; iter++)
	{
		(*iter)->Render(fTime);
	}

	//pShadow->ResetDepthSRV(105);
	//SAFE_RELEASE(pShadow);
	if (pMRT)
	{
		pMRT->ResetTarget();
		SAFE_RELEASE(pMRT);
	}




}

void CRenderManager::RenderAlpha(float fTime)
{
	CMRT* pMRT = FindRenderTarget("GBuffer2");

	CMRT* pShadow = FindRenderTarget("Shadow");

	pShadow->SetDepthSRV(20);
	if (pMRT)
	{
		pMRT->SetShader(12);
	}

	m_pAlphaBlend->SetState();

	size_t iSize = m_vecComponent[static_cast<int>(LAYER::ALPHA)].size();

	for (size_t i = 0; i < iSize; ++i)
	{
		m_vecComponent[static_cast<int>(LAYER::ALPHA)][i]->Render(fTime);
	}

	if (pMRT)
	{
		pMRT->ResetShader(12);
		SAFE_RELEASE(pMRT);
	}
	pShadow->ResetDepthSRV(20);
	SAFE_RELEASE(pShadow);

	CMRT* pSwordLight = FindRenderTarget("SwordLight");
	pSwordLight->SetShader(20);
	UINT iStride = 0;
	CONTEXT->IASetIndexBuffer(nullptr, DXGI_FORMAT_UNKNOWN, 0);
	CONTEXT->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	ID3D11Buffer* pBuffer = nullptr;
	CONTEXT->IASetVertexBuffers(0, 1, &pBuffer, &iStride, &iStride);
	CONTEXT->IASetInputLayout(nullptr);

	m_pFullScreenShader->SetShader();

	SetState("OutLine");

	CONTEXT->Draw(4, 0);

	ResetState("OutLine");

	pSwordLight->ResetShader(20);
	SAFE_RELEASE(pSwordLight);

	m_pAlphaBlend->ResetState();

	//pScene->ResetTarget();
	//pScene->Release();
}

void CRenderManager::RenderSwordLight(float fTime)
{
	CMRT* pMRT = FindRenderTarget("SwordLight");
	CMRT* pGBuffer2 = FindRenderTarget("GBuffer2");

	pMRT->SetTarget();
	pGBuffer2->SetShader(12);

	std::vector<CSceneComponent*>::iterator iter = m_vecComponent[static_cast<int>(LAYER::SWORD_LIGHT)].begin();
	std::vector<CSceneComponent*>::iterator iterEnd = m_vecComponent[static_cast<int>(LAYER::SWORD_LIGHT)].end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->Render(fTime);
	}
	pGBuffer2->ResetShader(12);
	pGBuffer2->Release();
	pMRT->ResetTarget();

	pMRT->Release();
	m_pBlur->SetRenderTarget("SwordLight");


	m_pBlur->Render(fTime);

	ID3D11ShaderResourceView* pSRV = nullptr;
	CONTEXT->PSSetShaderResources(103, 1, &pSRV);
	CONTEXT->CSSetShaderResources(103, 1, &pSRV);
}

void CRenderManager::RenderDecal(float fTime)
{
	CMultiRenderTarget* pMRT = FindMRT("Decal");

	if (pMRT)
	{
		pMRT->Clear();
		pMRT->SetTarget();
	}

	CMRT* pAlbedo = FindRenderTarget("Albedo");
	CMRT* pGBuffer1 = FindRenderTarget("GBuffer1");
	CMRT* pGBuffer2 = FindRenderTarget("GBuffer2");
	CMRT* pGBuffer3 = FindRenderTarget("GBuffer3");
	CMRT* pGBuffer4 = FindRenderTarget("GBuffer4");
	CMRT* pGBuffer5 = FindRenderTarget("GBuffer5");

	pAlbedo->SetShader(10);
	pGBuffer1->SetShader(11);
	pGBuffer2->SetShader(12);
	pGBuffer3->SetShader(13);
	pGBuffer4->SetShader(14);
	pGBuffer5->SetShader(15);

	SetState(CULL_NONE);
	SetState(NO_DEPTH);
	//SetState("DecalBlend");

	size_t iSize = m_vecComponent[static_cast<int>(LAYER::DECAL)].size();

	for (size_t i = 0; i < iSize; ++i)
	{
		m_vecComponent[static_cast<int>(LAYER::DECAL)][i]->Render(fTime);
	}

	//ResetState("DecalBlend");
	ResetState(NO_DEPTH);
	ResetState(CULL_NONE);

	pAlbedo->ResetShader(10);
	pGBuffer1->ResetShader(11);
	pGBuffer2->ResetShader(12);
	pGBuffer3->ResetShader(13);
	pGBuffer4->ResetShader(14);
	pGBuffer5->ResetShader(15);

	if (pMRT)
	{
		pMRT->ResetTarget();

		SAFE_RELEASE(pMRT);
	}
	SAFE_RELEASE(pAlbedo);
	SAFE_RELEASE(pGBuffer1);
	SAFE_RELEASE(pGBuffer2);
	SAFE_RELEASE(pGBuffer3);
	SAFE_RELEASE(pGBuffer4);
	SAFE_RELEASE(pGBuffer5);

}

void CRenderManager::RenderGBuffer(float fTime)
{
	size_t iSize = m_vecComponent[static_cast<int>(LAYER::GBUFFER)].size();

	for (size_t i = 0; i < iSize; ++i)
	{
		m_vecComponent[static_cast<int>(LAYER::GBUFFER)][i]->Render(fTime);
	}
}

void CRenderManager::RenderLightAcc(float fTime)
{
	CMultiRenderTarget* pMRT = FindMRT("LightAcc");
	CMRT* pShadow = FindRenderTarget("Shadow");
	pMRT->Clear();
	pMRT->SetTarget();
	pShadow->SetDepthSRV(20);
	m_pLightAccShader->SetShader();
	SetState(NO_DEPTH);
	SetState("LightAccBlend");

	SetShader("Albedo", 10);
	SetShader("GBuffer1", 11);
	SetShader("GBuffer2", 12);
	SetShader("GBuffer3", 13);
	SetShader("DecalAlbedo", 17);
	SetShader("DecalGBuffer1", 18);
	SetShader("DecalGBuffer2", 19);
	
	GET_SINGLE(CLightManager)->Render(fTime);

	ResetShader("Albedo", 10);
	ResetShader("GBuffer1", 11);
	ResetShader("GBuffer2", 12);
	ResetShader("GBuffer3", 13);
	ResetShader("DecalAlbedo", 17);
	ResetShader("DecalGBuffer1", 18);
	ResetShader("DecalGBuffer2", 19);

	ResetState("LightAccBlend");
	ResetState(NO_DEPTH);
	pShadow->ResetDepthSRV(20);
	SAFE_RELEASE(pShadow);
	pMRT->ResetTarget();
	SAFE_RELEASE(pMRT);
}

void CRenderManager::RenderLightCombine(float fTime)
{
	CMRT* pMRT = FindRenderTarget("Scene");
	//pMRT->ClearTarget();
	pMRT->SetTarget();
	m_pLightCombineShader->SetShader();

	SetState(NO_DEPTH);
	SetState(ALPHA_BLEND);

	SetShader("Albedo", 10);
	SetShader("LightDif", 14);
	SetShader("LightSpc", 15);
	SetShader("LightEmv", 16);
	SetShader("DecalAlbedo", 17);
	SetShader("DecalGBuffer1", 18);
	SetShader("DecalGBuffer2", 19);

	UINT iOffset = 0;

	CONTEXT->IASetInputLayout(nullptr);
	CONTEXT->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	CONTEXT->IASetVertexBuffers(0, 0, nullptr, 0, &iOffset);
	CONTEXT->IASetIndexBuffer(nullptr, DXGI_FORMAT_UNKNOWN, 0);
	CONTEXT->Draw(4, 0);

	ResetShader("Albedo", 10);
	ResetShader("LightDif", 14);
	ResetShader("LightSpc", 15);
	ResetShader("LightEmv", 16);
	ResetShader("DecalAlbedo", 17);
	ResetShader("DecalGBuffer1", 18);
	ResetShader("DecalGBuffer2", 19);

	ResetState(ALPHA_BLEND);
	ResetState(NO_DEPTH);
	pMRT->ResetTarget();
	SAFE_RELEASE(pMRT);
}

void CRenderManager::RenderScene(float fTime)
{
	SetShader("Scene", 20, (int)SHADER_CBUFFER_TYPE::CBUFFER_COMPUTE | (int)SHADER_CBUFFER_TYPE::CBUFFER_PIXEL);
	//SetState("NoCullRS");
	SetState("NoDepth");
	SetState(ALPHA_BLEND);

	if (m_bFullBlur)
	{
		//m_pFullBlur->Render(fTime);
		m_tGlobalBuffer.fAccTime = GET_SINGLE(CTimer)->GetTotalTime();
		GET_SINGLE(CShaderManager)->UpdateCBuffer("Global", &m_tGlobalBuffer);
		m_pRadialBlur->SetShader();
	}
	else
	{
		m_pFullScreenShader->SetShader();
	}

	UINT iOffset = 0;

	CONTEXT->IASetInputLayout(nullptr);
	CONTEXT->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	CONTEXT->IASetVertexBuffers(0, 0, nullptr, 0, &iOffset);
	CONTEXT->IASetIndexBuffer(nullptr, DXGI_FORMAT_UNKNOWN, 0);
	CONTEXT->Draw(4, 0);

	ResetState(ALPHA_BLEND);
	//ResetState("NoCullRS");
	ResetState("NoDepth");
	ResetShader("Scene", 20, (int)SHADER_CBUFFER_TYPE::CBUFFER_COMPUTE | (int)SHADER_CBUFFER_TYPE::CBUFFER_PIXEL);
}

void CRenderManager::RenderParticle(float fTime)
{
	CMRT* pMRT = FindRenderTarget("GBuffer2");

	pMRT->SetShader(12);

	m_pAlphaBlend->SetState();
	SetState(NO_DEPTH);

	CLight* pLight = GET_SINGLE(CLightManager)->GetMainLight();

	if (pLight)
	{
		pLight->SetShader();
	}

	size_t iSize = m_vecComponent[static_cast<int>(LAYER::PARTICLE)].size();

	for (size_t i = 0; i < iSize; i++)
	{
		m_vecComponent[static_cast<int>(LAYER::PARTICLE)][i]->Render(fTime);
	}
	ResetState(NO_DEPTH);
	m_pAlphaBlend->ResetState();

	pMRT->ResetShader(12);
	pMRT->Release();
}

void CRenderManager::RenderEtc(float fTime)
{
	CCamera* pCam = GET_SINGLE(CCameraManager)->GetMainCam();
	GET_SINGLE(CCameraManager)->SetMainCam("EquipCam");

	m_pAlphaShader->SetShader();

	CMRT* pMRT = FindRenderTarget("Equip");

	pMRT->SetTarget();

	size_t iSize = m_vecComponent[static_cast<int>(LAYER::EQUIP)].size();

	for (size_t i = 0; i < iSize; i++)
	{
		std::shared_ptr<CTransform> pTransform = m_vecComponent[static_cast<int>(LAYER::EQUIP)][i]->GetTransform();

		if (pTransform)
			pTransform->SetTransform();

		CAnimation* pAnimation = m_vecComponent[static_cast<int>(LAYER::EQUIP)][i]->GetAnimation();

		if (pAnimation)
		{
			pAnimation->SetShaderResource();
		}

		class CMesh* pMesh = m_vecComponent[static_cast<int>(LAYER::EQUIP)][i]->GetMesh();

		if (pMesh)
		{
			const std::vector<PMESHCONTAINER>* pvecContaioner = pMesh->GetMeshContainer();

			size_t iContainer = pvecContaioner->size();

			for (size_t i = 0; i < iContainer; i++)
			{
				size_t iSub = (*pvecContaioner)[i]->vecIdx.size();

				for (size_t j = 0; j < iSub; j++)
				{
					(*pvecContaioner)[i]->vecMaterial[j]->SetMaterialNoShader();

					pMesh->Render(fTime, static_cast<int>(i), static_cast<int>(j));
				}
			}
		}
		if (pAnimation)
		{
			pAnimation->ResetShaderResource();
		}

		SAFE_RELEASE(pAnimation);
	}
	pMRT->ResetTarget();



	m_pAlphaBlend->SetState();
	GET_SINGLE(CCameraManager)->SetMainCam(pCam);
	SetState(NO_DEPTH);

	if (iSize > 0)
	{
		pMRT->SetShader(20);

		GET_SINGLE(CShaderManager)->UpdateCBuffer("Transform", &m_tEquipBuffer);

		m_pEquipShader->SetShader();

		UINT iOffset = 0;
		ID3D11Buffer* pBuffer = nullptr;
		CONTEXT->IASetVertexBuffers(0, 1, &pBuffer, &iOffset, &iOffset);
		CONTEXT->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
		CONTEXT->Draw(4, 0);

		pMRT->ResetShader(20);
	}
	m_pAlphaBlend->ResetState();

	ResetState(NO_DEPTH);

	SAFE_RELEASE(pMRT);
}

void CRenderManager::RenderMinimap(float fTime)
{
	CCamera* pCam = GET_SINGLE(CCameraManager)->GetMainCam();
	GET_SINGLE(CCameraManager)->SetMainCam("Minimap");
	SetState(NO_DEPTH);

	size_t iSize = m_vecComponent[static_cast<int>(LAYER::TERRAIN)].size();

	for (size_t i = 0; i < iSize; ++i)
	{
		m_vecComponent[static_cast<int>(LAYER::TERRAIN)][i]->Render(fTime);
	}

	ResetState(NO_DEPTH);
	GET_SINGLE(CCameraManager)->SetMainCam(pCam);
}

void CRenderManager::SetState(const std::string& strTag)
{
	CRenderState* pState = FindState(strTag);

	if (!pState)
		return;

	pState->SetState();
}

void CRenderManager::ResetState(const std::string& strTag)
{
	CRenderState* pState = FindState(strTag);

	if (!pState)
		return;

	pState->ResetState();
}

void CRenderManager::AddTileInstancing(CTile* pTile)
{
	if (!m_pTileInstancing)
	{
		m_pTileInstancing = new CLayerInstance;

		class CMesh* pMesh = GET_SINGLE(CResourceManager)->FindMesh("Basic");

		std::shared_ptr<CMaterial> pMtrl = GET_SINGLE(CResourceManager)->FindMaterial("Inst");

		m_pTileInstancing->Init(pMesh, pMtrl, sizeof(INSTANCINGDATA), 10000);

		m_pTileInstancing->m_pMaterial->SetTexture(LINK_TYPE::LT_CUSTOM, "Tile", (int)SHADER_CBUFFER_TYPE::CBUFFER_PIXEL, 9);
	}

	CCamera* pCam = GET_SINGLE(CCameraManager)->GetMainCam();

	INSTANCINGDATA tData;

	tData.matWVP = pTile->GetMatWorld() * pCam->GetView() * pCam->GetProj();
	tData.vMeshSize = pTile->GetMeshSize();
	tData.vPivot = pTile->GetPivot();

	tData.matWVP.Transpose();

	tData.iFrame = (int)pTile->GetTileIndex();
	tData.vColor = pTile->GetMaterial().get()->GetDiffuseColor();

	m_pTileInstancing->AddInstancingData(&tData);

	m_pTileInstancing->AddCom(pTile);
}

void CRenderManager::RenderTileInstancing()
{
	if (!m_pTileInstancing)
		return;

	if (m_pTileInstancing->GetComSize() <= 10)
	{
		const std::vector<CSceneComponent*>* vecCom = m_pTileInstancing->GetComponents();
		
		size_t iSz = m_pTileInstancing->GetComSize();

		for (size_t i = 0; i < iSz; ++i)
		{
			m_vec2DComponent.push_back((*vecCom)[i]);
		}
	}

	else
	{
		SetState("AlphaBlend");
		SetState("NoDepth");
		m_pTileInstancing->Render();
		m_pTileInstancing->RenderFont(-1);
		ResetState("AlphaBlend");
		ResetState("NoDepth");
	}

}

void CRenderManager::ClearTileInstancing()
{
	if (!m_pTileInstancing)
		return;

	m_pTileInstancing->Clear();
}

void CRenderManager::SetTileInstTex(const std::string& strKey, bool bArray)
{
	if (!m_pTileInstancing)
		return;

	m_pTileInstancing->m_pMaterial->ClearTexture();
	m_pTileInstancing->m_pMaterial->SetTexture(LINK_TYPE::LT_CUSTOM, strKey, (int)SHADER_CBUFFER_TYPE::CBUFFER_PIXEL, 9 * bArray);
}

PInstancingBuffer CRenderManager::CreateInstancingBuffer(int iSize, int iCount)
{
	PInstancingBuffer pBuffer = new InstancingBuffer;

	pBuffer->pData = new char[(size_t)iSize * (size_t)iCount];
	pBuffer->eUsg = D3D11_USAGE_DYNAMIC;
	pBuffer->iSize = iSize;
	pBuffer->iCount = iCount;

	D3D11_BUFFER_DESC tDesc = {};

	tDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	tDesc.ByteWidth = iSize * iCount;
	tDesc.Usage = D3D11_USAGE_DYNAMIC;
	tDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	if (FAILED(DEVICE->CreateBuffer(&tDesc, nullptr, &pBuffer->pBuffer)))
		return nullptr;

	return pBuffer;
}

CLayerInstance* CRenderManager::FindInstancingGeometry(unsigned __int64 iKey)
{
	std::unordered_map<unsigned __int64, CLayerInstance*>::iterator iter = m_mapInstancing.find(iKey);

	if (iter == m_mapInstancing.end())
		return nullptr;

	return iter->second;
}

void CRenderManager::ResizeInstancingBuffer(PInstancingBuffer pBuffer, int iCount)
{
	SAFE_DELETE_ARRAY(pBuffer->pData);
	SAFE_RELEASE(pBuffer->pBuffer);
	pBuffer->iCount = iCount;
	pBuffer->pData = new char[(size_t)pBuffer->iSize * (size_t)iCount];

	D3D11_BUFFER_DESC	tDesc = {};
	tDesc.ByteWidth = pBuffer->iSize * pBuffer->iCount;
	tDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	tDesc.Usage = D3D11_USAGE_DYNAMIC;
	tDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	if (FAILED(DEVICE->CreateBuffer(&tDesc, nullptr, &pBuffer->pBuffer)))
		return;
}

void CRenderManager::CopyInstancingData(PInstancingBuffer pBuffer, int iCount)
{
	D3D11_MAPPED_SUBRESOURCE	tMap = {};

	CONTEXT->Map(pBuffer->pBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &tMap);

	memcpy(tMap.pData, pBuffer->pData, (size_t)pBuffer->iSize * (size_t)iCount);

	CONTEXT->Unmap(pBuffer->pBuffer, 0);
}

void CRenderManager::ComputeInstancing()
{
	std::unordered_map<unsigned __int64, CLayerInstance*>::iterator iterIns = m_mapInstancing.begin();
	std::unordered_map<unsigned __int64, CLayerInstance*>::iterator iterInsEnd = m_mapInstancing.end();

	for (; iterIns != iterInsEnd; ++iterIns)
	{
		if (iterIns->first == 0xffffffffffffffff)
			continue;

		iterIns->second->m_bAnimation = false;
		//iterIns->second->Clear();
	}
/*
	iterEnd = m_ObjList.end();
	for (iter = m_ObjList.begin(); iter != itreEnd; ++iter)
	{
		iterObjEnd = (*iter)->ObjList.end();
		for (iterObj = (*iter)->ObjList.begin(); iterObj != iterObjEnd; iterObj++)
		{
			CGameObject* pObj = *iterObj;

			if (!pObj->IsActive() || !pObj->IsEnable())
				continue;

			CRenderer* pRenderer = pObj->FindComponentByType<CRenderer>("");
		}
	}*/
}

void CRenderManager::SetTilesTexture(const std::string& strKey)
{
	CLayerInstance* pInst = FindInstancingGeometry(0xffffffffffffffff);

	if (!pInst)
		return;

	pInst->m_pMaterial->SetTexture(LINK_TYPE::LT_CUSTOM, strKey, (int)SHADER_CBUFFER_TYPE::CBUFFER_PIXEL, 9);
}

void CRenderManager::SetTilesTexture(std::shared_ptr<CTexture> pTex)
{
	CLayerInstance* pInst = FindInstancingGeometry(0xffffffffffffffff);

	if (!pInst)
		return;

	pInst->m_pMaterial.get()->SetTexture(LINK_TYPE::LT_CUSTOM, pTex, (int)SHADER_CBUFFER_TYPE::CBUFFER_PIXEL, 9);
}

void CRenderManager::AddRenderObject(CGameObject* pObj)
{
	CLayer* pLayer = pObj->GetLayer();

	std::list<CGameObject*>::iterator iterEnd = m_ObjList.end();
	for (std::list<CGameObject*>::iterator iter = m_ObjList.begin(); iter != iterEnd; ++iter)
	{
		if ((*iter)->m_pLayer == pLayer)
		{
			pObj->AddRef();
			//(*iter)->ObjList.push_back(pObj);
			break;
		}
	}

}

void CRenderManager::AddLayer(CLayer* pLayer)
{
	m_LayerList.push_back(pLayer);
}

CRenderState* CRenderManager::FindState(const std::string& strTag)
{
	std::unordered_map<std::string, CRenderState*>::iterator iter = m_mapState.find(strTag);

	if (iter == m_mapState.end())
		return nullptr;

	iter->second->AddRef();

	return iter->second;
}

bool CRenderManager::SortY(CSceneComponent* pCom1, CSceneComponent* pCom2)
{
	return pCom1->GetWorldPos().y < pCom2->GetWorldPos().y;
}

int CRenderManager::SortZ(const void* pSrc, const void* pDst)
{
	int iZSrc = (*((CUIControl**)pSrc))->GetZOrder();
	int iZDst = (*((CUIControl**)pDst))->GetZOrder();

	if (iZSrc > iZDst) return -1;
	else if (iZSrc == iZDst) return 0;
	else return 1;
}
