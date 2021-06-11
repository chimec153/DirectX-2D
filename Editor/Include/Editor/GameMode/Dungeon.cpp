#include "Dungeon.h"
#include "Component/Terrain.h"
#include "Scene/Scene.h"
#include "../Object/BladePlayer.h"
#include "Component/Renderer.h"
#include "Resource/ResourceManager.h"
#include "Component/Light.h"
#include "MainMode.h"
#include "RenderManager.h"

CDungeon::CDungeon()
{
}

CDungeon::~CDungeon()
{
}

bool CDungeon::Init()
{
	if (!LoadMesh())
		return false;

	if(!CGameMode::Init())
		return false;

	if (!LoadTexture())
		return false;

	CGameMode::SetDomeTexture2D("LV_Binterrante_Env_SKY02_LHJ");
	CGameMode::SetDomeShader("Cube2");
	CGameMode::SetDomeDiffuseColor(Vector4(0.f, 0.f, 0.1f, 1.f));

	CMainMode::LoadSequence("Sequences.csv");

	std::shared_ptr<CGameObject> pTerrainObj = m_pScene->CreateObject<CGameObject>("Terrain2", m_pScene->FindLayer(TERRAIN_LAYER));

	std::shared_ptr<CComponent> pTerrain = pTerrainObj->CreateComponent<CTerrain>("Terrain2");

	pTerrainObj->SetRootComponent(pTerrain);

	std::shared_ptr<CComponent> pRenderer = pTerrainObj->CreateComponent<CSceneComponent>("Renderer");

	static_cast<CTerrain*>(pTerrain.get())->AddChild(pRenderer);

	static_cast<CTerrain*>(pTerrain.get())->BuildHeightMap("Terrain2", 512, 512, 60.f, L"Stage3.bmp");
	static_cast<CTerrain*>(pTerrain.get())->SetTiling(10.f, 10.f);
	static_cast<CTerrain*>(pTerrain.get())->SetSplatCount(5);
	static_cast<CTerrain*>(pTerrain.get())->SetWorldScale(20.f, 20.f, 20.f);

	static_cast<CSceneComponent*>(pRenderer.get())->SetShader("TerrainArray");
	static_cast<CSceneComponent*>(pRenderer.get())->SetMaterial("Color");	
	static_cast<CSceneComponent*>(pRenderer.get())->SetTexture(LINK_TYPE::DIF_ARRAY, "Terrain2ArrayDif");
	static_cast<CSceneComponent*>(pRenderer.get())->SetTexture(LINK_TYPE::NRM_ARRAY, "Terrain2ArrayNrm");
	static_cast<CSceneComponent*>(pRenderer.get())->SetTexture(LINK_TYPE::SPC_ARRAY, "Terrain2ArraySpc");
	static_cast<CSceneComponent*>(pRenderer.get())->SetTexture(LINK_TYPE::LT_CUSTOM, "Terrain2ArraySplat", 3, 40);

	std::shared_ptr<CGameObject> pPlayer = m_pScene->CreateObject<CBladePlayer>("Player", m_pScene->FindLayer(ALBEDO_LAYER));

	pPlayer->SetWorldPos(450.603f, 0.f, 9177.597f);

	std::shared_ptr<CGameObject> pLightObj = m_pScene->CreateObject<CGameObject>("DirLight", m_pScene->FindLayer(ALBEDO_LAYER));

	std::shared_ptr<CComponent> pLight = pLightObj->CreateComponent<CLight>("DirLight");

	pLightObj->SetRootComponent(pLight);

	return true;
}

bool CDungeon::LoadMesh()
{
	GET_SINGLE(CResourceManager)->CreateSphere("Cube2", 1.f, 20, 20);

	return true;
}

bool CDungeon::LoadTexture()
{
	std::vector<const TCHAR*> vecstrTexture;

	vecstrTexture.push_back(L"Terrains\\BD_Terrain_Cave_01.dds");
	vecstrTexture.push_back(L"Terrains\\BD_Terrain_Cliff05.dds");
	vecstrTexture.push_back(L"Terrains\\Terrain_Cliff_11.dds");
	vecstrTexture.push_back(L"Terrains\\Terrain_Cliff_13.dds");
	vecstrTexture.push_back(L"Terrains\\Terrain_Cliff_15_Large.dds");

	GET_SINGLE(CResourceManager)->LoadTextureArray("Terrain2ArrayDif", vecstrTexture);

	vecstrTexture.clear();

	vecstrTexture.push_back(L"Terrains\\BD_Terrain_Cave_01_NRM.bmp");
	vecstrTexture.push_back(L"Terrains\\BD_Terrain_Cliff05_NRM.bmp");
	vecstrTexture.push_back(L"Terrains\\Terrain_Cliff_11_NRM.bmp");
	vecstrTexture.push_back(L"Terrains\\Terrain_Cliff_13_NRM.bmp");
	vecstrTexture.push_back(L"Terrains\\Terrain_Cliff_15_Large_NRM.bmp");

	GET_SINGLE(CResourceManager)->LoadTextureArray("Terrain2ArrayNrm", vecstrTexture);

	vecstrTexture.clear();

	vecstrTexture.push_back(L"Terrains\\BD_Terrain_Cave_01_SPC.bmp");
	vecstrTexture.push_back(L"Terrains\\BD_Terrain_Cliff05_SPEC.bmp");
	vecstrTexture.push_back(L"Terrains\\Terrain_Cliff_11_SPEC.bmp");
	vecstrTexture.push_back(L"Terrains\\Terrain_Cliff_13_SPEC.bmp");
	vecstrTexture.push_back(L"Terrains\\Terrain_Cliff_15_Large_SPEC.bmp");

	GET_SINGLE(CResourceManager)->LoadTextureArray("Terrain2ArraySpc", vecstrTexture);

	vecstrTexture.clear();

	vecstrTexture.push_back(L"Terrains\\BattleField_Terrain_Splat_1.bmp");
	vecstrTexture.push_back(L"Terrains\\BattleField_Terrain_Splat_3.bmp");
	vecstrTexture.push_back(L"Terrains\\BattleField_Terrain_Splat_4.bmp");
	vecstrTexture.push_back(L"Terrains\\BattleField_Terrain_Splat_6.bmp");

	GET_SINGLE(CResourceManager)->LoadTextureArray("Terrain2ArraySplat", vecstrTexture);

	vecstrTexture.clear();

	GET_SINGLE(CResourceManager)->LoadTexture("LV_Binterrante_Env_SKY02_LHJ", L"Sky\\LV_Binterrante_Env_SKY02_LHJ.dds");

	return true;
}
