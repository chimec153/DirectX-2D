#include "MainMode.h"
#include "Object/GameObject.h"
#include "Scene/Scene.h"
#include "Component/Renderer.h"
#include "Component/Mesh2DComponent.h"
#include "Resource/ResourceManager.h"
#include "../Object/Player3D.h"
#include "../Object/UI.h"
#include "Resource/Material.h"
#include "Resource/Animation.h"
#include "Resource/Texture.h"
#include "Component/Transform.h"
#include "../Object/GeometryGenerator.h"
#include "CameraManager.h"
#include "Component/Camera.h"
#include "Component/Light.h"
#include "Device.h"
#include "Resource/ShaderManager.h"
#include "Resource/GraphicShader.h"
#include "../Object/Monster3D.h"
#include "Component/Terrain.h"
#include "PathManager.h"
#include "Component/ColliderTerrain.h"
#include "../Object/Tree.h"
#include "../Object/Rock.h"
#include "../Object/Bullet3D.h"
#include "Component/Decal.h"
#include "Scene/SceneManager.h"
#include "LoadingMode3D.h"
#include "LensFlares.h"
#include "Component/Particle.h"
#include "../Object/Inventory3D.h"
#include "../Object/Equip.h"
#include "../Object/InvenButton.h"
#include "../Object/EquipButton.h"
#include "../Object/TargetUI.h"
#include "../Object/Cyclops.h"
#include "../Object/Particle3D.h"
#include "../Object/Goblin.h"
#include "../Object/GoblinRock.h"
#include "../Object/Ride.h"
#include "../Object/BlueFlame.h"
#include "../Item.h"
#include "../Object/LootingWindow.h"
#include "../Object/Portal.h"
#include "../Object/Coin.h"
#include "../Object/BladePlayer.h"
#include "../Object/StaffPlayer.h"
#include "../../Sequence.h"
#include "LightManager.h"
#include "Sound/SoundManager.h"
#include "Component/PaperBurn.h"
#include "Render/Distortion.h"

CMainMode::CMainMode()
{
	m_iGameModeType = static_cast<int>(EDITOR_GAME_MODE_TYPE::MAIN);
}

CMainMode::~CMainMode()
{
}
// 상수버퍼 반지름 clip
// 스킬 만들기
// 스킬 트레일
// 스킬 데칼
// 스킬 파티클
// 몬스터 추가
// 무기 장착 /해제 시퀀스
// 사운드 작업
// 미니맵
// fxaa
// 마을 오브젝트 배치하기
// 상점 꾸미기
// 방사형 블러
// 카메라를 가리는 물체가 있을 경우 처리하기
// 인스턴싱 자동으로 만들기
// 디퍼드 랜더링 멀티쓰레드
// 물만들기
// 높이맵 테셀레이션 적용
// 깊이 안개 효과 적용하기
// DoF
// 림 라이트
// 크레이지 범프, 수업시간에 한것 다적용, 그림자 캐릭터만 적용, 랜더링 기술 (fxaa)
bool CMainMode::Init()
{
	if (!LoadSequence())
		return false;

	CItem::LoadItemTable("Items.csv");

	if (!LoadShader())
		return false;
	LoadMesh();
	if (!LoadTexture())
		return false;
	if (!CreateParticleProto())
		return false;
	if (!CreateProtoType())
	return false;

	std::shared_ptr<CGameObject> pTerrain = m_pScene->CreateObject<CGameObject>("TerrainObj", m_pScene->FindLayer(TERRAIN_LAYER));


	const std::shared_ptr<CComponent> __pHeight = pTerrain->CreateComponent<CTerrain>("Terrain");

	pTerrain->SetRootComponent(__pHeight);
	static_cast<CTerrain*>(__pHeight.get())->BuildHeightMap("TerrainG", 512, 512, 25.5f, TEXT("Stage1.bmp"));

	static_cast<CTerrain*>(__pHeight.get())->SetTiling(60.f, 60.f);
	static_cast<CTerrain*>(__pHeight.get())->SetSplatCount(5);
	static_cast<CTerrain*>(__pHeight.get())->SetSceneComponentType(SCENE_COMPONENT_TYPE::SCT_3D);
	static_cast<CSceneComponent*>(__pHeight.get())->SetWorldScale(10.f, 10.f, 10.f);
	static_cast<CSceneComponent*>(__pHeight.get())->SetMesh("TerrainG");
	static_cast<CSceneComponent*>(__pHeight.get())->SetShader("TerrainArray");
	static_cast<CSceneComponent*>(__pHeight.get())->SetMaterial("Color");
	static_cast<CSceneComponent*>(__pHeight.get())->SetTexture(LINK_TYPE::LT_CUSTOM, "Terrain1Dif", 3, 30);
	static_cast<CSceneComponent*>(__pHeight.get())->SetTexture(LINK_TYPE::LT_CUSTOM, "Terrain1Nrm", 3, 31);
	//static_cast<CSceneComponent*>(_pHeight.get())->SetTexture(LINK_TYPE::LT_CUSTOM, "Terrain1Spc", 3, 32);
	static_cast<CSceneComponent*>(__pHeight.get())->SetTexture(LINK_TYPE::LT_CUSTOM, "Alphas", 3, 40);
	static_cast<CSceneComponent*>(__pHeight.get())->GetMaterial()->SetEnableBump(true);
	static_cast<CSceneComponent*>(__pHeight.get())->GetMaterial()->SetDecal(true);
	//static_cast<CSceneComponent*>(_pHeight.get())->AddRenderState(WIRE_FRAME);

	std::shared_ptr<CComponent> pTerrainCol = pTerrain->CreateComponent<CColliderTerrain>("TerrainBody");
	const std::vector<Vector3>& vecPos = static_cast<CTerrain*>(__pHeight.get())->GetVecPos();
	static_cast<CColliderTerrain*>(pTerrainCol.get())->SetWorldScale(1.f, 1.f, 1.f);
	static_cast<CColliderTerrain*>(pTerrainCol.get())->SetInfo(vecPos, 256, 256);
	//static_cast<CSceneComponent*>(_pHeight.get())->AddChild(pTerrainCol);

	//static_cast<CTerrain*>(__pHeight.get())->SetCollider(pTerrainCol);

	if (!LoadObjMap(m_pScene, TEXT("ObjMap.bmp")))
		return false;
	if (!CreateRock())
		return false;
	if (!CreateLOD())
		return false;
	if (!CreateCampA())
		return false;

	//std::shared_ptr<CGameObject> pRide = m_pScene->CreateObject<CRide>("BoneDragon", m_pScene->FindLayer(ALPHA_LAYER));

	//pRide->SetEnable(false);

	//CLayer* pAlbedo = m_pScene->FindLayer(ALBEDO_LAYER);

	//std::shared_ptr<CGameObject> pPlayer = m_pScene->CreateObject<CStaffPlayer>("Player", m_pScene->FindLayer(ALBEDO_LAYER));

	//pPlayer->SetWorldPos(750.f, 0.f, 2600.f);
	//pPlayer->SetWorldPos(4640.f, 0.f, 540.f);

	//if (!CreateUI())
		//return false;

	std::shared_ptr<CGameObject> pTargetUI = m_pScene->CreateObject<CTargetUI>("Target UI", m_pScene->FindLayer(UI_LAYER));
	pTargetUI->SetEnable(false);

	if (!CGameMode::Init())
		return false;

	std::shared_ptr<CTexture> pCubeTex = GET_SINGLE(CResourceManager)->FindTexture("CubeMap2");

	CDevice::Enter();
	CONTEXT->PSSetShaderResources(51, 1, &pCubeTex->GetInfo()->pSRV);
	CDevice::Leave();

	std::shared_ptr<CTexture> pProjCubeTex = GET_SINGLE(CResourceManager)->FindTexture("ProjCubeMap");

	CDevice::Enter();
	CONTEXT->PSSetShaderResources(52, 1, &pProjCubeTex->GetInfo()->pSRV);
	CDevice::Leave();

	std::shared_ptr<CTexture> pProjTex = GET_SINGLE(CResourceManager)->FindTexture("ProjTex");

	CDevice::Enter();
	CONTEXT->PSSetShaderResources(104, 1, &pProjTex->GetInfo()->pSRV);
	CDevice::Leave();

	GET_SINGLE(CResourceManager)->LoadTexture("Sky", TEXT("DefaultSkybox.dds"));

	SetDomeTexture("Sky");


	std::shared_ptr<CGameObject> pPointLight = m_pScene->CreateObject<CGameObject>("PointLight", m_pScene->FindLayer(ALBEDO_LAYER));

	
	std::shared_ptr<CComponent> _pPointLight = pPointLight->CreateComponent<CLight>("PointLight");

	pPointLight->SetRootComponent(_pPointLight);

	CLight* __pPointLight = static_cast<CLight*>(_pPointLight.get());

	__pPointLight->SetLightType(LIGHT_TYPE::POINT);
	__pPointLight->SetDifColor(Vector4(0.18f, 0.001f, 0.56f, 1.f));
	__pPointLight->SetAmbColor(Vector4(173.f/ 255.f, 62.f / 255.f, 202.f / 255.f, 1.f));
	__pPointLight->SetSpcColor(Vector4(86.f / 255.f, 34.f / 255.f, 144.f / 255.f, 1.f));
	__pPointLight->SetEmiColor(Vector4(157.f / 255.f, 33.f / 255.f, 210.f / 255.f, 1.f));
	__pPointLight->SetRange(128.f);
	__pPointLight->SetWorldPos(0.f, 0.f, 0.f);
	__pPointLight->SetWorldPos(4500.f, 1.f, 1050.f);
	__pPointLight->SetAttn(0.f, 0.07f, 0.f);
	GET_SINGLE(CLightManager)->SetMainLight(__pPointLight);

	/*std::shared_ptr<CGameObject> pWhiteLight = m_pScene->CreateObject<CGameObject>("WhiteLight", m_pScene->FindLayer(ALBEDO_LAYER));


	std::shared_ptr<CComponent> _pWhiteLight = pPointLight->CreateComponent<CLight>("WhiteLight");

	pWhiteLight->SetRootComponent(_pWhiteLight);

	CLight* __pWhiteLight = static_cast<CLight*>(_pWhiteLight.get());

	__pWhiteLight->SetLightType(LIGHT_TYPE::POINT);
	__pWhiteLight->SetDifColor(Vector4::White);
	__pWhiteLight->SetRange(512.f);
	__pWhiteLight->SetWorldPos(16.f, 60.f, 0.f);
	__pWhiteLight->SetWorldPos(4500.f, 100.f, 550.f);
	GET_SINGLE(CLightManager)->SetMainLight(__pWhiteLight);*/

	std::shared_ptr<CGameObject> pDirLight = m_pScene->CreateObject<CGameObject>("DirLight", m_pScene->FindLayer(ALBEDO_LAYER));

	
	std::shared_ptr<CComponent> _pDirLight = pPointLight->CreateComponent<CLight>("DirLight");

	pDirLight->SetRootComponent(_pDirLight);

	CLight* __pDirLight = static_cast<CLight*>(_pDirLight.get());

	__pDirLight->SetLightType(LIGHT_TYPE::DIRECTIONAL);
	__pDirLight->SetDifColor(Vector4(150.f / 255.f, 150.f / 255.f, 150.f / 255.f, 1.f));
	__pDirLight->SetAmbColor(Vector4(150.f / 255.f, 150.f / 255.f, 150.f / 255.f, 1.f));
	__pDirLight->SetSpcColor(Vector4(150.f / 255.f, 150.f / 255.f, 150.f / 255.f, 1.f));
	__pDirLight->AddWorldRotX(22.5f);
	__pDirLight->AddWorldRotY(-135.f);
	__pDirLight->SetWorldPos(5120.f, 1000.f, 5120.f);
	__pDirLight->SetFrustum(7500, 7500, 0.f, 10000.f);
	GET_SINGLE(CLightManager)->SetMainLight(__pDirLight);


	std::shared_ptr<CGameObject> pMountain = m_pScene->CreateObject<CGameObject>("Mountain", m_pScene->FindLayer(ALBEDO_LAYER));

	std::shared_ptr<CComponent> pMountainCom = pMountain->CreateComponent<CSceneComponent>("Mountain");

	pMountain->SetRootComponent(pMountainCom);

	CSceneComponent* _pMountain = static_cast<CSceneComponent*>(pMountainCom.get());

	_pMountain->SetMesh("Mountain");
	_pMountain->SetWorldPos(5000.f , -500.f, -2500.f);
	_pMountain->SetWorldScale(20.f, 20.f, 20.f);

	std::shared_ptr<CGameObject> pDecal = m_pScene->CreateProtoObj<CGameObject>("Crack", m_pScene, m_pScene->GetSceneType());

	std::shared_ptr<CComponent> pCom = pDecal->CreateComponent<CDecal>("Crack");

	pDecal->SetRootComponent(pCom);

	static_cast<CDecal*>(pCom.get())->SetWorldScale(100.f, 100.f, 100.f);
	static_cast<CDecal*>(pCom.get())->SetPivot(0.5f, 0.5f, 0.5f);

	static_cast<CDecal*>(pCom.get())->SetMesh("OBBMesh");
	static_cast<CDecal*>(pCom.get())->SetMaterial("Color");
	static_cast<CDecal*>(pCom.get())->SetShader("DecalAlphaMask");
	static_cast<CDecal*>(pCom.get())->SetTexture(LINK_TYPE::LT_DIF, "FX_Crack_004_TEX_PYS");
	static_cast<CDecal*>(pCom.get())->SetTexture(LINK_TYPE::LT_NOR, "FX_CrackNormal_001_TEX_PYS");
	static_cast<CDecal*>(pCom.get())->SetTexture(LINK_TYPE::LT_EMI, "FX_Crater_007_Tex_Pys");

	static_cast<CDecal*>(pCom.get())->SetAlpha(1.f);
	static_cast<CDecal*>(pCom.get())->SetFadeOut(1.f);
	static_cast<CDecal*>(pCom.get())->SetDuration(1.f);

	std::shared_ptr<CGameObject> pGoblin = m_pScene->CreateCloneObj("Goblin1", "Goblin", m_pScene->FindLayer(ALBEDO_LAYER), m_pScene->GetSceneType());
	pGoblin->SetWorldPos(600.f, 0.f, 1000.f);
	pGoblin = m_pScene->CreateCloneObj("Goblin2", "Goblin", m_pScene->FindLayer(ALBEDO_LAYER), m_pScene->GetSceneType());
	pGoblin->SetWorldPos(600.f, 0.f, 1100.f);
	pGoblin = m_pScene->CreateCloneObj("Goblin3", "Goblin", m_pScene->FindLayer(ALBEDO_LAYER), m_pScene->GetSceneType());
	pGoblin->SetWorldPos(700.f, 0.f, 1000.f);

	pGoblin = m_pScene->CreateCloneObj("Goblin4", "Goblin", m_pScene->FindLayer(ALBEDO_LAYER), m_pScene->GetSceneType());
	pGoblin->SetWorldPos(843.972f, 0.f, 667.107f);
	pGoblin = m_pScene->CreateCloneObj("Goblin5", "Goblin", m_pScene->FindLayer(ALBEDO_LAYER), m_pScene->GetSceneType());
	pGoblin->SetWorldPos(843.972f, 0.f, 767.107f);
	pGoblin = m_pScene->CreateCloneObj("Goblin6", "Goblin", m_pScene->FindLayer(ALBEDO_LAYER), m_pScene->GetSceneType());
	pGoblin->SetWorldPos(743.972f, 0.f, 667.107f);

	pGoblin = m_pScene->CreateCloneObj("Goblin7", "Goblin", m_pScene->FindLayer(ALBEDO_LAYER), m_pScene->GetSceneType());
	pGoblin->SetWorldPos(1543.489f, 0.f, 864.932f);
	pGoblin = m_pScene->CreateCloneObj("Goblin8", "Goblin", m_pScene->FindLayer(ALBEDO_LAYER), m_pScene->GetSceneType());
	pGoblin->SetWorldPos(1543.489f, 0.f, 964.932f);
	pGoblin = m_pScene->CreateCloneObj("Goblin9", "Goblin", m_pScene->FindLayer(ALBEDO_LAYER), m_pScene->GetSceneType());
	pGoblin->SetWorldPos(1443.489f, 0.f, 864.932f);

	pGoblin = m_pScene->CreateCloneObj("Goblin10", "Goblin", m_pScene->FindLayer(ALBEDO_LAYER), m_pScene->GetSceneType());
	pGoblin->SetWorldPos(2091.657f, 0.f, 1222.319f);
	pGoblin = m_pScene->CreateCloneObj("Goblin11", "Goblin", m_pScene->FindLayer(ALBEDO_LAYER), m_pScene->GetSceneType());
	pGoblin->SetWorldPos(2091.657f, 0.f, 1122.319f);
	pGoblin = m_pScene->CreateCloneObj("Goblin12", "Goblin", m_pScene->FindLayer(ALBEDO_LAYER), m_pScene->GetSceneType());
	pGoblin->SetWorldPos(1991.657f, 0.f, 1222.319f);

	pGoblin = m_pScene->CreateCloneObj("Goblin13", "Goblin", m_pScene->FindLayer(ALBEDO_LAYER), m_pScene->GetSceneType());
	pGoblin->SetWorldPos(2829.799f, 0.f, 866.845f);
	pGoblin = m_pScene->CreateCloneObj("Goblin14", "Goblin", m_pScene->FindLayer(ALBEDO_LAYER), m_pScene->GetSceneType());
	pGoblin->SetWorldPos(2829.799f, 0.f, 966.845f);
	pGoblin = m_pScene->CreateCloneObj("Goblin15", "Goblin", m_pScene->FindLayer(ALBEDO_LAYER), m_pScene->GetSceneType());
	pGoblin->SetWorldPos(2729.799f, 0.f, 866.845f);

	pGoblin = m_pScene->CreateCloneObj("Goblin16", "Goblin", m_pScene->FindLayer(ALBEDO_LAYER), m_pScene->GetSceneType());
	pGoblin->SetWorldPos(3584.369f, 0.f, 1251.721f);
	pGoblin = m_pScene->CreateCloneObj("Goblin17", "Goblin", m_pScene->FindLayer(ALBEDO_LAYER), m_pScene->GetSceneType());
	pGoblin->SetWorldPos(3584.369f, 0.f, 1351.721f);
	pGoblin = m_pScene->CreateCloneObj("Goblin18", "Goblin", m_pScene->FindLayer(ALBEDO_LAYER), m_pScene->GetSceneType());
	pGoblin->SetWorldPos(3484.369f, 0.f, 1251.721f);

	pGoblin = m_pScene->CreateCloneObj("Goblin19", "Goblin", m_pScene->FindLayer(ALBEDO_LAYER), m_pScene->GetSceneType());
	pGoblin->SetWorldPos(4397.220f, 0.f, 1153.438f);
	pGoblin = m_pScene->CreateCloneObj("Goblin20", "Goblin", m_pScene->FindLayer(ALBEDO_LAYER), m_pScene->GetSceneType());
	pGoblin->SetWorldPos(4397.220f, 0.f, 1053.438f);
	pGoblin = m_pScene->CreateCloneObj("Goblin21", "Goblin", m_pScene->FindLayer(ALBEDO_LAYER), m_pScene->GetSceneType());
	pGoblin->SetWorldPos(4297.220f, 0.f, 1153.438f);

	pGoblin = m_pScene->CreateCloneObj("Goblin22", "Goblin", m_pScene->FindLayer(ALBEDO_LAYER), m_pScene->GetSceneType());
	pGoblin->SetWorldPos(4068.008f, 0.f, 1859.252f);
	pGoblin = m_pScene->CreateCloneObj("Goblin23", "Goblin", m_pScene->FindLayer(ALBEDO_LAYER), m_pScene->GetSceneType());
	pGoblin->SetWorldPos(4068.008f, 0.f, 1759.252f);
	pGoblin = m_pScene->CreateCloneObj("Goblin24", "Goblin", m_pScene->FindLayer(ALBEDO_LAYER), m_pScene->GetSceneType());
	pGoblin->SetWorldPos(3968.008f, 0.f, 1859.252f);

	pGoblin = m_pScene->CreateCloneObj("Goblin25", "Goblin", m_pScene->FindLayer(ALBEDO_LAYER), m_pScene->GetSceneType());
	pGoblin->SetWorldPos(4347.465f, 0.f, 2527.197f);
	pGoblin = m_pScene->CreateCloneObj("Goblin26", "Goblin", m_pScene->FindLayer(ALBEDO_LAYER), m_pScene->GetSceneType());
	pGoblin->SetWorldPos(4347.465f, 0.f, 2427.197f);
	pGoblin = m_pScene->CreateCloneObj("Goblin27", "Goblin", m_pScene->FindLayer(ALBEDO_LAYER), m_pScene->GetSceneType());
	pGoblin->SetWorldPos(4247.465f, 0.f, 2527.197f);

	pGoblin = m_pScene->CreateCloneObj("Goblin28", "Goblin", m_pScene->FindLayer(ALBEDO_LAYER), m_pScene->GetSceneType());
	pGoblin->SetWorldPos(4434.641f, 0.f, 4051.423f);
	pGoblin = m_pScene->CreateCloneObj("Goblin29", "Goblin", m_pScene->FindLayer(ALBEDO_LAYER), m_pScene->GetSceneType());
	pGoblin->SetWorldPos(4434.641f, 0.f, 3951.423f);
	pGoblin = m_pScene->CreateCloneObj("Goblin30", "Goblin", m_pScene->FindLayer(ALBEDO_LAYER), m_pScene->GetSceneType());
	pGoblin->SetWorldPos(4534.641f, 0.f, 4051.423f);

	std::shared_ptr<CGameObject> pPortal = m_pScene->CreateObject<CPortal>("Portal", m_pScene->FindLayer(ALBEDO_LAYER));

	pPortal->SetWorldPos(5000.f, 0.f, 200.f);
	pPortal->SetWorldRotY(-45.f);

	std::shared_ptr<CGameObject> pSequence = m_pScene->CreateObject<CSequence>("Sequence", m_pScene->FindLayer(ALBEDO_LAYER));

	pSequence->SetWorldPos(750.f, 0.f, 2200.f);

	for (int i = 0; i < 250; i++)
	{
		static_cast<CSequence*>(pSequence.get())->AddPos(
			Vector3(cosf(static_cast<float>(i/250.f - 0.5f) * XM_PI) * 200.f, 
				i /250.f * 150.f, sinf(static_cast<float>(i / 250.f - 0.5f) * XM_PI) * 200.f));
	}

	static_cast<CSequence*>(pSequence.get())->SetMaxTime(4.f);
	static_cast<CSequence*>(pSequence.get())->SetStartCallBack(this, &CMainMode::BossSpawn);	

	GET_SINGLE(CSoundManager)->LoadSound("BGM","Amb_Forest_1", true, "Dungeon\\Amb_Forest_1.ogg");
	GET_SINGLE(CSoundManager)->Play("Amb_Forest_1");


	return true;
}

bool CMainMode::LoadTexture()
{
	GET_SINGLE(CResourceManager)->LoadTexture("CubeMap2", TEXT("Texture2D\\Texture1.dds"));
		
	GET_SINGLE(CResourceManager)->LoadTexture("ProjCubeMap", TEXT("Texture2D\\Texture1.dds"));
		
	GET_SINGLE(CResourceManager)->LoadTexture("ProjTex", TEXT("Texture2D\\LV_DesertValley_CubeMap_TexPosZ_JSA.dds"));
		
	GET_SINGLE(CResourceManager)->LoadTexture("BLUIInventory_I4F", L"BLUIInventory\\Texture2D\\BLUIInventory_I4F.dds");
		
	GET_SINGLE(CResourceManager)->LoadTexture("BLUIPC_I9", TEXT("BLUIPC\\Texture2D\\BLUIPC_I9.dds"), TEXTURE_PATH);
		

	GET_SINGLE(CResourceManager)->LoadTexture("BLUIPC_I22", TEXT("BLUIPC\\Texture2D\\BLUIPC_I22.dds"), TEXTURE_PATH);
		

	GET_SINGLE(CResourceManager)->LoadTexture("icon_sheet_009", TEXT("IconSheet\\Texture2D\\icon_sheet_009.dds"), TEXTURE_PATH);
		

	GET_SINGLE(CResourceManager)->LoadTexture("icon_sheet_076", 
		TEXT("IconSheet\\Texture2D\\icon_sheet_076.dds"), TEXTURE_PATH);
	GET_SINGLE(CResourceManager)->LoadTexture("icon_sheet_017",
		TEXT("IconSheet\\Texture2D\\icon_sheet_017.dds"), TEXTURE_PATH);
		

	GET_SINGLE(CResourceManager)->LoadTexture("icon_sheet_751", TEXT("IconSheet\\Texture2D\\icon_sheet_751.dds"), TEXTURE_PATH);
		

	GET_SINGLE(CResourceManager)->LoadTexture("icon_sheet_1026", TEXT("IconSheet\\Texture2D\\icon_sheet_1026.dds"), TEXTURE_PATH);
		

	GET_SINGLE(CResourceManager)->LoadTexture("BLUIEXPBar_I3", TEXT("BLUIEXPBar\\Texture2D\\BLUIEXPBar_I3.dds"), TEXTURE_PATH);
		

	GET_SINGLE(CResourceManager)->LoadTexture("UI2_Shared_AutoSort_Line_Img", TEXT("BLUIInventory\\Texture2D\\UI2_Shared_AutoSort_Line_Img.dds"), TEXTURE_PATH);
		

	GET_SINGLE(CResourceManager)->LoadTexture("UI2_Slot_Lock_icon", TEXT("UI_Share_Slot\\Texture2D\\UI2_Slot_Lock_icon.dds"), TEXTURE_PATH);
		

	GET_SINGLE(CResourceManager)->LoadTexture("BLUIEquip_I2C", TEXT("BLUIEquip\\Texture2D\\BLUIEquip_I2C.dds"), TEXTURE_PATH);
		

	GET_SINGLE(CResourceManager)->LoadTexture("BLUITarget_I11", TEXT("BLUITarget\\Texture2D\\BLUITarget_I11.dds"), TEXTURE_PATH);
		

	GET_SINGLE(CResourceManager)->LoadTexture("BLUILooting_I4", TEXT("BLUILooting\\Texture2D\\BLUILooting_I4.dds"), TEXTURE_PATH);
		

	GET_SINGLE(CResourceManager)->LoadTexture("FX_SmokeSubUV_002_Tex_Pys", TEXT("FX_Resource_Pys_01\\Texture2D\\FX_SmokeSubUV_002_Tex_Pys.dds"), TEXTURE_PATH);
		

	GET_SINGLE(CResourceManager)->LoadTexture("Fx_StoneChip_001_Tex_Pys", TEXT("FX_Resource_Pys_01\\Texture2D\\Fx_StoneChip_001_Tex_Pys.dds"), TEXTURE_PATH);
		

	GET_SINGLE(CResourceManager)->LoadTexture("FX_Crack_004_TEX_PYS", TEXT("FX_Resource_Pys_01\\Texture2D\\FX_Crack_004_TEX_PYS.dds"), TEXTURE_PATH);
		

	GET_SINGLE(CResourceManager)->LoadTexture("FX_CrackNormal_001_TEX_PYS", TEXT("FX_Resource_Pys_01\\Texture2D\\FX_CrackNormal_001_TEX_PYS.dds"), TEXTURE_PATH);
		

	GET_SINGLE(CResourceManager)->LoadTexture("FX_Crater_007_Tex_Pys", TEXT("FX_Resource_Pys_01\\Texture2D\\FX_Crater_007_Tex_Pys.dds"), TEXTURE_PATH);
		

	GET_SINGLE(CResourceManager)->LoadTexture("FX_SmokeTile_005_Tex_Pys", TEXT("FX_Resource_Pys_01\\Texture2D\\FX_SmokeTile_005_Tex_Pys.dds"), TEXTURE_PATH);
		

	GET_SINGLE(CResourceManager)->LoadTexture("FX_Explosion_Sub_08_Yellow", TEXT("FX_Resource_KKJ_05\\Texture2D\\FX_Explosion_Sub_08_Yellow.dds"), TEXTURE_PATH);
		

	GET_SINGLE(CResourceManager)->LoadTexture("FX_fireaura_001_TEX_KJS", TEXT("FX_Resource_KJS_01\\Texture2D\\FX_fireaura_001_TEX_KJS.dds"), TEXTURE_PATH);
		

	GET_SINGLE(CResourceManager)->LoadTexture("FX_fumefie_005_TEX_KJS", TEXT("FX_Resource_KJS_01\\Texture2D\\FX_fumefie_005_TEX_KJS.dds"), TEXTURE_PATH);
		

	GET_SINGLE(CResourceManager)->LoadTexture("FX_blood_001_TEX_KJS", TEXT("FX_Resource_KJS_01\\Texture2D\\FX_blood_001_TEX_KJS.dds"), TEXTURE_PATH);
		

	GET_SINGLE(CResourceManager)->LoadTexture("FX_blood_002_TEX_KJS", TEXT("FX_Resource_KJS_01\\Texture2D\\FX_blood_002_TEX_KJS.dds"), TEXTURE_PATH);
		

	GET_SINGLE(CResourceManager)->LoadTexture("FX_blood_002_NTEX_KJS", TEXT("FX_Resource_KJS_01\\Texture2D\\FX_blood_002_NTEX_KJS.dds"), TEXTURE_PATH);


	GET_SINGLE(CResourceManager)->LoadTexture("FX_Com_LowHP_TEX_KYH", TEXT("FX_Com_Screen\\Texture2D\\FX_Com_LowHP_TEX_KYH.dds"), TEXTURE_PATH);
	GET_SINGLE(CResourceManager)->LoadTexture("FX_SparkYellow_001_TEX_HKB", TEXT("FX_Resource_KKJ_05\\Texture2D\\FX_SparkYellow_001_TEX_HKB.dds"), TEXTURE_PATH);

	GET_SINGLE(CResourceManager)->LoadTexture("B_Fire001_emis",
		TEXT("FX_B\\Texture2D\\B_Fire001_emis.tga"), TEXTURE_PATH);
		

	GET_SINGLE(CResourceManager)->LoadTexture("B_Fire004_emis",
		TEXT("FX_B\\Texture2D\\B_Fire004_emis.tga"), TEXTURE_PATH);
		

	GET_SINGLE(CResourceManager)->LoadTexture("B_Fire008_emis",
		TEXT("FX_B\\Texture2D\\B_Fire008_emis.tga"), TEXTURE_PATH);
		

	GET_SINGLE(CResourceManager)->LoadTexture("B_Fire009_emis",
		TEXT("FX_B\\Texture2D\\B_Fire009_emis.tga"), TEXTURE_PATH);

	GET_SINGLE(CResourceManager)->LoadTexture("FX_Flame_001_Tex_CJH",
		L"FX_Resource_CJH_01\\Texture2D\\FX_Flame_001_Tex_CJH.dds");
	GET_SINGLE(CResourceManager)->LoadTexture("FX_trail_007_TEX_CJH",
		L"FX_Resource_CJH_01\\Texture2D\\FX_trail_007_TEX_CJH.dds");
	GET_SINGLE(CResourceManager)->LoadTexture("FX_NormalTile_017_TEX_KKJ",
		L"FX_Resource_KKJ_01\\Texture2D\\FX_NormalTile_017_TEX_KKJ.dds");
	GET_SINGLE(CResourceManager)->LoadTexture("FX_fumefire_003_TEX_KJS",
		L"FX_Resource_KJS_01\\Texture2D\\FX_fumefire_003_TEX_KJS.dds");
	GET_SINGLE(CResourceManager)->LoadTexture("FX_explosion_002_TEX_KJS",
		L"FX_Resource_KJS_01\\Texture2D\\FX_explosion_002_TEX_KJS.dds");
	GET_SINGLE(CResourceManager)->LoadTexture("FX_Explosion_Sub_08_White6",
		L"FX_Resource_KKJ_05\\Texture2D\\FX_Explosion_Sub_08_White6.dds");
	GET_SINGLE(CResourceManager)->LoadTexture("FX_FlameFlip_002_TEX_HKB",
		L"FX_Resource_KKJ_05\\Texture2D\\FX_FlameFlip_002_TEX_HKB.dds");
	GET_SINGLE(CResourceManager)->LoadTexture("FX_FlameFlip_004_TEX_HKB",
		L"FX_Resource_KKJ_05\\Texture2D\\FX_FlameFlip_004_TEX_HKB.dds");

	GET_SINGLE(CResourceManager)->LoadTexture("FX_IceFloor_003_Tex_PHS",
		TEXT("FX_Resource_PHS_01\\Texture2D\\FX_IceFloor_003_Tex_PHS.dds"), TEXTURE_PATH);
	GET_SINGLE(CResourceManager)->LoadTexture("FX_IceWhipDecal_001_Tex_PHS",
		TEXT("FX_Resource_PHS_01\\Texture2D\\FX_IceWhipDecal_001_Tex_PHS.dds"), TEXTURE_PATH);
	GET_SINGLE(CResourceManager)->LoadTexture("FX_IceWhipDecal_NRM_001_Tex_PHS",
		TEXT("FX_Resource_PHS_01\\Texture2D\\FX_IceWhipDecal_NRM_001_Tex_PHS.dds"), TEXTURE_PATH);

	GET_SINGLE(CResourceManager)->LoadTexture("FX_Smoke_001_TEX_KKJ_Clamp",
		L"FX_Resource_KKJ_01\\Texture2D\\FX_Smoke_001_TEX_KKJ_Clamp.dds");

	GET_SINGLE(CResourceManager)->LoadTexture("FX_dissolve_004_TEX_PYS",
		L"FX_Resource_Pys_01\\Texture2D\\FX_dissolve_004_TEX_PYS.dds");

	GET_SINGLE(CResourceManager)->LoadTexture("FX_T_EFX_Ring_Thin",
		L"FX_Resource_CJS_01\\Texture2D\\FX_T_EFX_Ring_Thin.dds");

	std::vector<const TCHAR*> vecPath;

	vecPath.push_back(TEXT("Terrains\\FarmLand.bmp"));
	vecPath.push_back(TEXT("Terrains\\Rock.bmp"));
	vecPath.push_back(TEXT("Terrains\\Load.bmp"));
	vecPath.push_back(TEXT("Terrains\\Flower.bmp"));

	GET_SINGLE(CResourceManager)->LoadTextureArray("Alphas", vecPath);
		

	vecPath.clear();

	vecPath.push_back(TEXT("LV_TurtleLake_Tile_A\\Texture2D\\LV_TurtleLake_Tile_Forest_D_KJM.dds"));
	vecPath.push_back(TEXT("LV_TurtleLake_Tile_A\\Texture2D\\LV_TurtleLake_Tile_FarmLand_D_KJM.dds"));
	vecPath.push_back(TEXT("LV_TurtleLake_Tile_A\\Texture2D\\LV_TurtleLake_Tile_Rock01_D_HMJ.dds"));
	vecPath.push_back(TEXT("LV_TurtleLake_Tile_A\\Texture2D\\LV_TurtleLake_Tile_Road01_D_HMJ.dds"));
	vecPath.push_back(TEXT("LV_TurtleLake_Tile_A\\Texture2D\\LV_TurtleLake_Tile_WildFlower_D_KJM.dds"));

	GET_SINGLE(CResourceManager)->LoadTextureArray("Terrain1Dif", vecPath, MESH_PATH);
		

	vecPath.clear();

	vecPath.push_back(TEXT("LV_TurtleLake_Tile_A\\Texture2D\\LV_TurtleLake_Tile_Forest_N_KJM.dds"));
	vecPath.push_back(TEXT("LV_TurtleLake_Tile_A\\Texture2D\\LV_TurtleLake_Tile_FarmLand_N_KJM.dds"));
	vecPath.push_back(TEXT("LV_TurtleLake_Tile_A\\Texture2D\\LV_TurtleLake_Tile_Rock01_N_HMJ.dds"));
	vecPath.push_back(TEXT("LV_TurtleLake_Tile_A\\Texture2D\\LV_TurtleLake_Tile_Forest_N_KJM.dds"));
	vecPath.push_back(TEXT("LV_TurtleLake_Tile_A\\Texture2D\\LV_TurtleLake_Tile_FarmLand_N_KJM.dds"));

	GET_SINGLE(CResourceManager)->LoadTextureArray("Terrain1Nrm", vecPath, MESH_PATH);
		

	vecPath.clear();

	GET_SINGLE(CSoundManager)->LoadSound("BGM", "NewBattle_02", true,
		"Dungeon\\NewBattle_02.ogg");
	GET_SINGLE(CSoundManager)->LoadSound("Effect", "Cyclops_atk01", false,
		"Cyclops\\Cyclops_atk01.ogg");
	GET_SINGLE(CSoundManager)->LoadSound("Effect", "Cyclops_damage", false,
		"Cyclops\\Cyclops_damage.ogg");
	GET_SINGLE(CSoundManager)->LoadSound("Effect", "Cyclops_death_00", false,
		"Cyclops\\Cyclops_death_00.ogg");
	GET_SINGLE(CSoundManager)->LoadSound("Effect", "Cyclops_death_01", false,
		"Cyclops\\Cyclops_death_01.ogg");
	GET_SINGLE(CSoundManager)->LoadSound("Effect", "Cyclops_heabyatk01", false,
		"Cyclops\\Cyclops_heabyatk01.ogg");
	GET_SINGLE(CSoundManager)->LoadSound("Effect", "Cyclops_Laugh_01", false,
		"Cyclops\\Cyclops_Laugh_01.ogg");
	GET_SINGLE(CSoundManager)->LoadSound("Effect", "Cyclops_longatk01", false,
		"Cyclops\\Cyclops_longatk01.ogg");
	GET_SINGLE(CSoundManager)->LoadSound("Effect", "Cyclops_longatk02_00", false,
		"Cyclops\\Cyclops_longatk02_00.ogg");
	GET_SINGLE(CSoundManager)->LoadSound("Effect", "Cyclops_longatk02_01", false,
		"Cyclops\\Cyclops_longatk02_01.ogg");
	GET_SINGLE(CSoundManager)->LoadSound("Effect", "Cyclops_longmove_00", false,
		"Cyclops\\Cyclops_longmove_00.ogg");
	GET_SINGLE(CSoundManager)->LoadSound("Effect", "Cyclops_longmove_01", false,
		"Cyclops\\Cyclops_longmove_01.ogg");
	GET_SINGLE(CSoundManager)->LoadSound("Effect", "Cyclops_modearalm", false,
		"Cyclops\\Cyclops_modearalm.ogg");
	GET_SINGLE(CSoundManager)->LoadSound("Effect", "Cyclops_ultraatk01_00", false,
		"Cyclops\\Cyclops_ultraatk01_00.ogg");
	GET_SINGLE(CSoundManager)->LoadSound("Effect", "Cyclops_ultraatk01_01", false,
		"Cyclops\\Cyclops_ultraatk01_01.ogg");
	GET_SINGLE(CSoundManager)->LoadSound("Effect", "Cyclops_ultraatk02", false,
		"Cyclops\\Cyclops_ultraatk02.ogg");
	GET_SINGLE(CSoundManager)->LoadSound("Effect", "Cyclops_wait", false,
		"Cyclops\\Cyclops_wait.ogg");
	GET_SINGLE(CSoundManager)->LoadSound("Effect", "Cyclops_walk_00", false,
		"Cyclops\\Cyclops_walk_00.ogg");
	GET_SINGLE(CSoundManager)->LoadSound("Effect", "Cyclops_walk_01", false,
		"Cyclops\\Cyclops_walk_01.ogg");
	GET_SINGLE(CSoundManager)->LoadSound("Effect", "WatchManBlack_SkillVox_01", false,
		"Cyclops\\WatchManBlack_SkillVox_01.ogg");
	GET_SINGLE(CSoundManager)->LoadSound("Effect", "roar_05", false,
		"Cyclops\\roar_05.ogg");
	GET_SINGLE(CSoundManager)->LoadSound("Effect", "Sorcerer_Inferno_Explo_1", false,
		"Cyclops\\Sorcerer_Inferno_Explo_1.ogg");

	return true;
}

void CMainMode::LoadMesh()
{
	GET_SINGLE(CResourceManager)->CreateSphere("Cube2", 1.f, 20, 20);
	GET_SINGLE(CResourceManager)->LoadMesh(MESH_TYPE::MT_STATIC_MESH, "Sphere", TEXT("sphere.msh"));
	GET_SINGLE(CResourceManager)->LoadMesh(MESH_TYPE::MT_STATIC_MESH, "Portal", TEXT("Obj\\portal.msh"));

	GET_SINGLE(CResourceManager)->LoadMesh(MESH_TYPE::MT_STATIC_MESH, "IceWave1", TEXT("Skill\\IceWave001.msh"));
	GET_SINGLE(CResourceManager)->LoadMesh(MESH_TYPE::MT_STATIC_MESH, "IceWave2", TEXT("Skill\\IceWave002.msh"));
	GET_SINGLE(CResourceManager)->LoadMesh(MESH_TYPE::MT_STATIC_MESH, "IceWave3", TEXT("Skill\\IceWave003.msh"));
	GET_SINGLE(CResourceManager)->LoadMesh(MESH_TYPE::MT_STATIC_MESH, "IceWave4", TEXT("Skill\\IceWave004.msh"));
	GET_SINGLE(CResourceManager)->LoadMesh(MESH_TYPE::MT_STATIC_MESH, "IceWave5", TEXT("Skill\\IceWave005.msh"));
	GET_SINGLE(CResourceManager)->LoadMesh(MESH_TYPE::MT_STATIC_MESH, "IceWave6", TEXT("Skill\\IceWave006.msh"));
	GET_SINGLE(CResourceManager)->LoadMesh(MESH_TYPE::MT_STATIC_MESH, "IceWave7", TEXT("Skill\\IceWave007.msh"));

	GET_SINGLE(CResourceManager)->LoadMesh(MESH_TYPE::MT_ANIMATION_MESH, "Goblin", TEXT("Goblin\\GoblinMesh.msh"));

	GET_SINGLE(CResourceManager)->LoadMesh(MESH_TYPE::MT_STATIC_MESH, "GoblinRock", TEXT("Goblin\\GoblinRock.msh"));
	GET_SINGLE(CResourceManager)->LoadMesh(MESH_TYPE::MT_STATIC_MESH, "Coin", TEXT("Coin\\Coin.msh"));


	GET_SINGLE(CResourceManager)->LoadMesh(MESH_TYPE::MT_ANIMATION_MESH, "BoneDragon", TEXT("Vehicle\\BoneDragon.msh"));

	
	GET_SINGLE(CResourceManager)->LoadMesh(MESH_TYPE::MT_STATIC_MESH, "Bush2", TEXT("Obj\\Bush2.msh"));
	
	GET_SINGLE(CResourceManager)->LoadMesh(MESH_TYPE::MT_STATIC_MESH, "Bush3", TEXT("Obj\\Bush3.msh"));
	
	GET_SINGLE(CResourceManager)->LoadMesh(MESH_TYPE::MT_STATIC_MESH, "Bush4", TEXT("Obj\\Bush4.msh"));
	
	GET_SINGLE(CResourceManager)->LoadMesh(MESH_TYPE::MT_STATIC_MESH, "BushTree", TEXT("Obj\\BushTree.msh"));
	
	GET_SINGLE(CResourceManager)->LoadMesh(MESH_TYPE::MT_STATIC_MESH, "Cattail1", TEXT("Obj\\Cattail1.msh"));
	
	GET_SINGLE(CResourceManager)->LoadMesh(MESH_TYPE::MT_STATIC_MESH, "Cattail2", TEXT("Obj\\Cattail2.msh"));
	
	GET_SINGLE(CResourceManager)->LoadMesh(MESH_TYPE::MT_STATIC_MESH, "cave01", TEXT("Obj\\cave01.msh"));
	
	GET_SINGLE(CResourceManager)->LoadMesh(MESH_TYPE::MT_STATIC_MESH, "cave02", TEXT("Obj\\cave02.msh"));
	
	GET_SINGLE(CResourceManager)->LoadMesh(MESH_TYPE::MT_STATIC_MESH, "cave03", TEXT("Obj\\cave03.msh"));
	
	GET_SINGLE(CResourceManager)->LoadMesh(MESH_TYPE::MT_STATIC_MESH, "cave04", TEXT("Obj\\cave04.msh"));
	
	GET_SINGLE(CResourceManager)->LoadMesh(MESH_TYPE::MT_STATIC_MESH, "caveTent01", TEXT("Obj\\caveTent01.msh"));
	
	GET_SINGLE(CResourceManager)->LoadMesh(MESH_TYPE::MT_STATIC_MESH, "caveTent02", TEXT("Obj\\caveTent02.msh"));
	
	GET_SINGLE(CResourceManager)->LoadMesh(MESH_TYPE::MT_STATIC_MESH, "Crop1", TEXT("Obj\\Crop1.msh"));
	
	GET_SINGLE(CResourceManager)->LoadMesh(MESH_TYPE::MT_STATIC_MESH, "Crop2", TEXT("Obj\\Crop2.msh"));
	
	//GET_SINGLE(CResourceManager)->LoadMesh(MESH_TYPE::MT_STATIC_MESH, "Deco01", TEXT("Obj\\Deco01.msh"));
	
	//GET_SINGLE(CResourceManager)->LoadMesh(MESH_TYPE::MT_STATIC_MESH, "Deco02", TEXT("Obj\\Deco02.msh"));
	
	//GET_SINGLE(CResourceManager)->LoadMesh(MESH_TYPE::MT_STATIC_MESH, "Deco03", TEXT("Obj\\Deco03.msh"));
	
	//GET_SINGLE(CResourceManager)->LoadMesh(MESH_TYPE::MT_STATIC_MESH, "Deco04", TEXT("Obj\\Deco04.msh"));
	
	GET_SINGLE(CResourceManager)->LoadMesh(MESH_TYPE::MT_STATIC_MESH, "Duckweed1", TEXT("Obj\\Duckweed1.msh"));
	
	GET_SINGLE(CResourceManager)->LoadMesh(MESH_TYPE::MT_STATIC_MESH, "Duckweed2", TEXT("Obj\\Duckweed2.msh"));
	
	GET_SINGLE(CResourceManager)->LoadMesh(MESH_TYPE::MT_STATIC_MESH, "Fence01", TEXT("Obj\\Fence01.msh"));
	
	GET_SINGLE(CResourceManager)->LoadMesh(MESH_TYPE::MT_STATIC_MESH, "Fence01B", TEXT("Obj\\Fence01B.msh"));
	
	GET_SINGLE(CResourceManager)->LoadMesh(MESH_TYPE::MT_STATIC_MESH, "Fence01C", TEXT("Obj\\Fence01C.msh"));
	
	GET_SINGLE(CResourceManager)->LoadMesh(MESH_TYPE::MT_STATIC_MESH, "Fence01D", TEXT("Obj\\Fence01D.msh"));
	
	GET_SINGLE(CResourceManager)->LoadMesh(MESH_TYPE::MT_STATIC_MESH, "Fence01E", TEXT("Obj\\Fence01E.msh"));
	
	GET_SINGLE(CResourceManager)->LoadMesh(MESH_TYPE::MT_STATIC_MESH, "Fence01F", TEXT("Obj\\Fence01F.msh"));
	
	GET_SINGLE(CResourceManager)->LoadMesh(MESH_TYPE::MT_STATIC_MESH, "Fence02A", TEXT("Obj\\Fence02A.msh"));
	
	GET_SINGLE(CResourceManager)->LoadMesh(MESH_TYPE::MT_STATIC_MESH, "Fence02B", TEXT("Obj\\Fence02B.msh"));
	
	GET_SINGLE(CResourceManager)->LoadMesh(MESH_TYPE::MT_STATIC_MESH, "Ferry3", TEXT("Obj\\Ferry3.msh"));
	
	GET_SINGLE(CResourceManager)->LoadMesh(MESH_TYPE::MT_STATIC_MESH, "FishCage", TEXT("Obj\\FishCage.msh"));
	
	GET_SINGLE(CResourceManager)->LoadMesh(MESH_TYPE::MT_STATIC_MESH, "FishDryingRack01", TEXT("Obj\\FishDryingRack01.msh"));
	
	GET_SINGLE(CResourceManager)->LoadMesh(MESH_TYPE::MT_STATIC_MESH, "FishDryingRack02", TEXT("Obj\\FishDryingRack02.msh"));
	
	GET_SINGLE(CResourceManager)->LoadMesh(MESH_TYPE::MT_STATIC_MESH, "Gate02", TEXT("Obj\\Gate02.msh"));
	
	GET_SINGLE(CResourceManager)->LoadMesh(MESH_TYPE::MT_STATIC_MESH, "GoblinRock1", TEXT("Obj\\GoblinRock1.msh"));
	
	GET_SINGLE(CResourceManager)->LoadMesh(MESH_TYPE::MT_STATIC_MESH, "GoblinRock2", TEXT("Obj\\GoblinRock2.msh"));
	
	GET_SINGLE(CResourceManager)->LoadMesh(MESH_TYPE::MT_STATIC_MESH, "GoblinRock3", TEXT("Obj\\GoblinRock3.msh"));
	
	GET_SINGLE(CResourceManager)->LoadMesh(MESH_TYPE::MT_STATIC_MESH, "GoblinRock4", TEXT("Obj\\GoblinRock4.msh"));
	
	GET_SINGLE(CResourceManager)->LoadMesh(MESH_TYPE::MT_STATIC_MESH, "GoblinRock5", TEXT("Obj\\GoblinRock5.msh"));
	
	GET_SINGLE(CResourceManager)->LoadMesh(MESH_TYPE::MT_STATIC_MESH, "GuardPost", TEXT("Obj\\GuardPost.msh"));
	
	GET_SINGLE(CResourceManager)->LoadMesh(MESH_TYPE::MT_STATIC_MESH, "HutFrame", TEXT("Obj\\HutFrame.msh"));
	
	GET_SINGLE(CResourceManager)->LoadMesh(MESH_TYPE::MT_STATIC_MESH, "HutRoof", TEXT("Obj\\HutRoof.msh"));
	
	GET_SINGLE(CResourceManager)->LoadMesh(MESH_TYPE::MT_STATIC_MESH, "Ivy1", TEXT("Obj\\Ivy1.msh"));
	
	GET_SINGLE(CResourceManager)->LoadMesh(MESH_TYPE::MT_STATIC_MESH, "Ivy2", TEXT("Obj\\Ivy2.msh"));
	
	GET_SINGLE(CResourceManager)->LoadMesh(MESH_TYPE::MT_STATIC_MESH, "Ivy3", TEXT("Obj\\Ivy3.msh"));
	
	GET_SINGLE(CResourceManager)->LoadMesh(MESH_TYPE::MT_STATIC_MESH, "Ladder", TEXT("Obj\\Ladder.msh"));
	
	GET_SINGLE(CResourceManager)->LoadMesh(MESH_TYPE::MT_STATIC_MESH, "LogBridge", TEXT("Obj\\LogBridge.msh"));
	
	GET_SINGLE(CResourceManager)->LoadMesh(MESH_TYPE::MT_STATIC_MESH, "LookOut01", TEXT("Obj\\LookOut01.msh"));
	
	GET_SINGLE(CResourceManager)->LoadMesh(MESH_TYPE::MT_STATIC_MESH, "MadroneTree1", TEXT("Obj\\MadroneTree1.msh"));
	
	GET_SINGLE(CResourceManager)->LoadMesh(MESH_TYPE::MT_STATIC_MESH, "MadroneTree2", TEXT("Obj\\MadroneTree2.msh"));
	
	GET_SINGLE(CResourceManager)->LoadMesh(MESH_TYPE::MT_STATIC_MESH, "MadroneTree3", TEXT("Obj\\MadroneTree3.msh"));
	
	GET_SINGLE(CResourceManager)->LoadMesh(MESH_TYPE::MT_STATIC_MESH, "ManzanitaTree2", TEXT("Obj\\ManzanitaTree2.msh"));
	
	GET_SINGLE(CResourceManager)->LoadMesh(MESH_TYPE::MT_STATIC_MESH, "ManzanitaTree3", TEXT("Obj\\ManzanitaTree3.msh"));
	
	GET_SINGLE(CResourceManager)->LoadMesh(MESH_TYPE::MT_STATIC_MESH, "MassiveLod1", TEXT("Obj\\MassiveLod1.msh"));
	
	GET_SINGLE(CResourceManager)->LoadMesh(MESH_TYPE::MT_STATIC_MESH, "MassiveLod2", TEXT("Obj\\MassiveLod2.msh"));
	
	//GET_SINGLE(CResourceManager)->LoadMesh(MESH_TYPE::MT_STATIC_MESH, "Mat", TEXT("Obj\\Mat.msh"));
	
	GET_SINGLE(CResourceManager)->LoadMesh(MESH_TYPE::MT_STATIC_MESH, "MG1", TEXT("Obj\\MG1.msh"));
	
	GET_SINGLE(CResourceManager)->LoadMesh(MESH_TYPE::MT_STATIC_MESH, "MG2", TEXT("Obj\\MG2.msh"));
	
	GET_SINGLE(CResourceManager)->LoadMesh(MESH_TYPE::MT_STATIC_MESH, "Pgrass", TEXT("Obj\\Pgrass.msh"));
	
	GET_SINGLE(CResourceManager)->LoadMesh(MESH_TYPE::MT_STATIC_MESH, "PiledFishes", TEXT("Obj\\PiledFishes.msh"));
	
	GET_SINGLE(CResourceManager)->LoadMesh(MESH_TYPE::MT_STATIC_MESH, "PotatoDryingRack", TEXT("Obj\\PotatoDryingRack.msh"));
	
	GET_SINGLE(CResourceManager)->LoadMesh(MESH_TYPE::MT_STATIC_MESH, "Shelter01", TEXT("Obj\\Shelter01.msh"));
	
	GET_SINGLE(CResourceManager)->LoadMesh(MESH_TYPE::MT_STATIC_MESH, "StoneStair", TEXT("Obj\\StoneStair.msh"));
	
	GET_SINGLE(CResourceManager)->LoadMesh(MESH_TYPE::MT_STATIC_MESH, "Stump1", TEXT("Obj\\Stump1.msh"));
	
	GET_SINGLE(CResourceManager)->LoadMesh(MESH_TYPE::MT_STATIC_MESH, "Stump2", TEXT("Obj\\Stump2.msh"));
	
	GET_SINGLE(CResourceManager)->LoadMesh(MESH_TYPE::MT_STATIC_MESH, "TurtleRock2", TEXT("Obj\\TurtleRock2.msh"));
	
	GET_SINGLE(CResourceManager)->LoadMesh(MESH_TYPE::MT_STATIC_MESH, "TurtleRock3", TEXT("Obj\\TurtleRock3.msh"));
	
	GET_SINGLE(CResourceManager)->LoadMesh(MESH_TYPE::MT_STATIC_MESH, "TurtleRock4", TEXT("Obj\\TurtleRock4.msh"));
	
	GET_SINGLE(CResourceManager)->LoadMesh(MESH_TYPE::MT_STATIC_MESH, "Vine2", TEXT("Obj\\Vine2.msh"));
	
	GET_SINGLE(CResourceManager)->LoadMesh(MESH_TYPE::MT_STATIC_MESH, "WallChii", TEXT("Obj\\WallChii.msh"));
	
	GET_SINGLE(CResourceManager)->LoadMesh(MESH_TYPE::MT_STATIC_MESH, "Watercoin", TEXT("Obj\\Watercoin.msh"));
	
	GET_SINGLE(CResourceManager)->LoadMesh(MESH_TYPE::MT_STATIC_MESH, "Watercoin2", TEXT("Obj\\Watercoin2.msh"));
	
	GET_SINGLE(CResourceManager)->LoadMesh(MESH_TYPE::MT_STATIC_MESH, "WildFlower", TEXT("Obj\\WildFlower.msh"));
	
	GET_SINGLE(CResourceManager)->LoadMesh(MESH_TYPE::MT_STATIC_MESH, "WoodBridge1", TEXT("Obj\\WoodBridge1.msh"));
	
	GET_SINGLE(CResourceManager)->LoadMesh(MESH_TYPE::MT_STATIC_MESH, "WoodBridge2", TEXT("Obj\\WoodBridge2.msh"));
	
	GET_SINGLE(CResourceManager)->LoadMesh(MESH_TYPE::MT_STATIC_MESH, "WoodBridge3", TEXT("Obj\\WoodBridge3.msh"));
	
	GET_SINGLE(CResourceManager)->LoadMesh(MESH_TYPE::MT_STATIC_MESH, "Tree", TEXT("Obj\\FlowerTree.msh"));
	
	GET_SINGLE(CResourceManager)->LoadMesh(MESH_TYPE::MT_STATIC_MESH, "Rock", TEXT("Obj\\LakeRock1.msh"));
	
	GET_SINGLE(CResourceManager)->LoadMesh(MESH_TYPE::MT_STATIC_MESH, "Mountain", TEXT("Obj\\Mountain.msh"), MESH_PATH, true);
	
	GET_SINGLE(CResourceManager)->LoadMesh(MESH_TYPE::MT_STATIC_MESH, "SmallRock1", TEXT("Obj\\SmallRock1.msh"));
	
	GET_SINGLE(CResourceManager)->LoadMesh(MESH_TYPE::MT_STATIC_MESH, "SmallRock2", TEXT("Obj\\SmallRock2.msh"));
	
	GET_SINGLE(CResourceManager)->LoadMesh(MESH_TYPE::MT_STATIC_MESH, "SmallRock3", TEXT("Obj\\SmallRock3.msh"));
	
	GET_SINGLE(CResourceManager)->LoadMesh(MESH_TYPE::MT_STATIC_MESH, "TurtleRock1", TEXT("Obj\\TurtleRock1.msh"));
	
	//GET_SINGLE(CResourceManager)->LoadMesh(MESH_TYPE::MT_STATIC_MESH, "bedding", TEXT("Obj\\bedding.msh"));
	
	GET_SINGLE(CResourceManager)->LoadMesh(MESH_TYPE::MT_STATIC_MESH, "Boat", TEXT("Obj\\Boat.msh"));
	
	GET_SINGLE(CResourceManager)->LoadMesh(MESH_TYPE::MT_STATIC_MESH, "bone01", TEXT("Obj\\bone01.msh"));
	
	GET_SINGLE(CResourceManager)->LoadMesh(MESH_TYPE::MT_STATIC_MESH, "bone02", TEXT("Obj\\bone02.msh"));
	
	GET_SINGLE(CResourceManager)->LoadMesh(MESH_TYPE::MT_STATIC_MESH, "BrokenTree1", TEXT("Obj\\BrokenTree1.msh"));
	
	GET_SINGLE(CResourceManager)->LoadMesh(MESH_TYPE::MT_STATIC_MESH, "BrokenTree2", TEXT("Obj\\BrokenTree2.msh"));
	
	GET_SINGLE(CResourceManager)->LoadMesh(MESH_TYPE::MT_STATIC_MESH, "Bush1", TEXT("Obj\\Bush1.msh"));
	
	GET_SINGLE(CResourceManager)->LoadMesh(MESH_TYPE::MT_STATIC_MESH, "Gate01", TEXT("Obj\\Gate01.msh"));
	
	GET_SINGLE(CResourceManager)->LoadMesh(MESH_TYPE::MT_STATIC_MESH, "LakeRock2", TEXT("Obj\\LakeRock2.msh"));
	
	GET_SINGLE(CResourceManager)->LoadMesh(MESH_TYPE::MT_STATIC_MESH, "LakeRock3", TEXT("Obj\\LakeRock3.msh"));
	
	GET_SINGLE(CResourceManager)->LoadMesh(MESH_TYPE::MT_STATIC_MESH, "LakeStone1", TEXT("Obj\\LakeStone1.msh"));
	
	GET_SINGLE(CResourceManager)->LoadMesh(MESH_TYPE::MT_STATIC_MESH, "LakeStone2", TEXT("Obj\\LakeStone2.msh"));
	
	GET_SINGLE(CResourceManager)->LoadMesh(MESH_TYPE::MT_STATIC_MESH, "LakeStone3", TEXT("Obj\\LakeStone3.msh"));
	
	GET_SINGLE(CResourceManager)->LoadMesh(MESH_TYPE::MT_STATIC_MESH, "ManzanitaTree1", TEXT("Obj\\ManzanitaTree1.msh"));
	
	GET_SINGLE(CResourceManager)->LoadMesh(MESH_TYPE::MT_STATIC_MESH, "Vine1", TEXT("Obj\\Vine1.msh"));
	//람다

	return;
}

bool CMainMode::LoadShader()
{
	
	return true;
}

bool CMainMode::LoadSequence()
{
	if (!LoadSequence("Sequences.csv"))
		return false;

	GET_SINGLE(CResourceManager)->LoadAnimationSequence("MonsterWalk", true, TEXT("Cyclops_Anim\\Walk.sqc"));
	
	GET_SINGLE(CResourceManager)->LoadAnimationSequence("MonsterDeath", false, TEXT("Cyclops_Anim\\Death.sqc"));
	
	GET_SINGLE(CResourceManager)->LoadAnimationSequence("MonsterAttack", false, TEXT("Cyclops_Anim\\atk01.sqc"));
	
	GET_SINGLE(CResourceManager)->LoadAnimationSequence("Cyclops_modeAlarm_Start", false, TEXT("Cyclops_Anim\\modeAlarm_Start.sqc"));
	
	GET_SINGLE(CResourceManager)->LoadAnimationSequence("Cyclops_modeAlarm_Loop", true, TEXT("Cyclops_Anim\\modeAlarm_Loop.sqc"));
	
	GET_SINGLE(CResourceManager)->LoadAnimationSequence("Cyclops_modeAlarm_End", false, TEXT("Cyclops_Anim\\modeAlarm_End.sqc"));
	
	GET_SINGLE(CResourceManager)->LoadAnimationSequence("Cyclops_HeavyAtk01", false, TEXT("Cyclops_Anim\\HeavyAtk01.sqc"));
	
	GET_SINGLE(CResourceManager)->LoadAnimationSequence("Cyclops_longAtk01_Start", false, TEXT("Cyclops_Anim\\longAtk01_Start.sqc"));
	
	GET_SINGLE(CResourceManager)->LoadAnimationSequence("Cyclops_LongAtk01_Loop", true, TEXT("Cyclops_Anim\\LongAtk01_Loop.sqc"));
	
	GET_SINGLE(CResourceManager)->LoadAnimationSequence("Cyclops_LongAtk01_End_A", false, TEXT("Cyclops_Anim\\LongAtk01_End_A.sqc"));

	GET_SINGLE(CResourceManager)->LoadAnimationSequence("Cyclops_LongAtk02_End_B", false, TEXT("Cyclops_Anim\\LongAtk02_End_B.sqc"));

	GET_SINGLE(CResourceManager)->LoadAnimationSequence("Cyclops_LongAtk02_End_B_Ball", false, TEXT("Cyclops_Anim\\LongAtk02_End_B_Ball.sqc"),MESH_PATH,true);
	
	GET_SINGLE(CResourceManager)->LoadAnimationSequence("Cyclops_Wait", false, TEXT("Cyclops_Anim\\Wait.sqc"));
	
	GET_SINGLE(CResourceManager)->LoadAnimationSequence("Cyclops_activeMove_End", false, TEXT("Cyclops_Anim\\activeMove_End.sqc"));
	
	GET_SINGLE(CResourceManager)->LoadAnimationSequence("Cyclops_activeMove_Loop", true, TEXT("Cyclops_Anim\\activeMove_Loop.sqc"));
	
	GET_SINGLE(CResourceManager)->LoadAnimationSequence("Cyclops_activeMove_Start", false, TEXT("Cyclops_Anim\\activeMove_Start.sqc"));
	
	//GET_SINGLE(CResourceManager)->LoadAnimationSequence("Cyclops_atk02", false, TEXT("Cyclops_Anim\\atk02.sqc"));
	
	//GET_SINGLE(CResourceManager)->LoadAnimationSequence("Cyclops_BackMove", false, TEXT("Cyclops_Anim\\BackMove.sqc"));
	
	GET_SINGLE(CResourceManager)->LoadAnimationSequence("Cyclops_deathwait", false, TEXT("Cyclops_Anim\\deathwait.sqc"));
	
	GET_SINGLE(CResourceManager)->LoadAnimationSequence("Cyclops_flinch", false, TEXT("Cyclops_Anim\\flinch.sqc"));
	
	//GET_SINGLE(CResourceManager)->LoadAnimationSequence("Cyclops_Groggy", false, TEXT("Cyclops_Anim\\Groggy.sqc"));
	
	GET_SINGLE(CResourceManager)->LoadAnimationSequence("Cyclops_longMove_End", false, TEXT("Cyclops_Anim\\longMove_End.sqc"));
	
	GET_SINGLE(CResourceManager)->LoadAnimationSequence("Cyclops_LongMove_Move", false, TEXT("Cyclops_Anim\\LongMove_Move.sqc"));
	
	GET_SINGLE(CResourceManager)->LoadAnimationSequence("Cyclops_longMove_Start", false, TEXT("Cyclops_Anim\\longMove_Start.sqc"));
	
	GET_SINGLE(CResourceManager)->LoadAnimationSequence("Cyclops_pullAtk_End", false, TEXT("Cyclops_Anim\\pullAtk_End.sqc"));
	
	GET_SINGLE(CResourceManager)->LoadAnimationSequence("Cyclops_pullAtk_Loop", true, TEXT("Cyclops_Anim\\pullAtk_Loop.sqc"));
	
	GET_SINGLE(CResourceManager)->LoadAnimationSequence("Cyclops_pullAtk_Start", false, TEXT("Cyclops_Anim\\pullAtk_Start.sqc"));
	
	//GET_SINGLE(CResourceManager)->LoadAnimationSequence("Cyclops_Reaction_End", false, TEXT("Cyclops_Anim\\Reaction_End.sqc"));
	
	//GET_SINGLE(CResourceManager)->LoadAnimationSequence("Cyclops_Reaction_Loop", true, TEXT("Cyclops_Anim\\Reaction_Loop.sqc"));
	
	//GET_SINGLE(CResourceManager)->LoadAnimationSequence("Cyclops_Reaction_Start", false, TEXT("Cyclops_Anim\\Reaction_Start.sqc"));
	
	//GET_SINGLE(CResourceManager)->LoadAnimationSequence("Cyclops_reactionAtk", false, TEXT("Cyclops_Anim\\reactionAtk.sqc"));
	
	//GET_SINGLE(CResourceManager)->LoadAnimationSequence("Cyclops_roundAtk01", false, TEXT("Cyclops_Anim\\roundAtk01.sqc"));
	
	//GET_SINGLE(CResourceManager)->LoadAnimationSequence("Cyclops_roundAtk02", false, TEXT("Cyclops_Anim\\roundAtk02.sqc"));
	
	//GET_SINGLE(CResourceManager)->LoadAnimationSequence("Cyclops_roundmove01", false, TEXT("Cyclops_Anim\\roundmove01.sqc"));
	
	//GET_SINGLE(CResourceManager)->LoadAnimationSequence("Cyclops_roundmove02", false, TEXT("Cyclops_Anim\\roundmove02.sqc"));
	
	//GET_SINGLE(CResourceManager)->LoadAnimationSequence("Cyclops_runaway", false, TEXT("Cyclops_Anim\\runaway.sqc"));
	
	GET_SINGLE(CResourceManager)->LoadAnimationSequence("Cyclops_UltraAtk01", false, TEXT("Cyclops_Anim\\UltraAtk01.sqc"));
	
	GET_SINGLE(CResourceManager)->LoadAnimationSequence("Cyclops_UltraAtk02", false, TEXT("Cyclops_Anim\\UltraAtk02.sqc"));
	
	///GET_SINGLE(CResourceManager)->LoadAnimationSequence("Cyclops_unarmedwait", false, TEXT("Cyclops_Anim\\unarmedwait.sqc"));
	
	GET_SINGLE(CResourceManager)->LoadAnimationSequence("Walk_Ball", false, TEXT("Cyclops_Anim\\Walk_Ball.sqc"));
	
	GET_SINGLE(CResourceManager)->LoadAnimationSequence("longAtk01_Start_Ball", false, TEXT("Cyclops_Anim\\longAtk01_Start_Ball.sqc"));
	
	GET_SINGLE(CResourceManager)->LoadAnimationSequence("LongAtk01_Loop_Ball", false, TEXT("Cyclops_Anim\\LongAtk01_Loop_Ball.sqc"));
	
	GET_SINGLE(CResourceManager)->LoadAnimationSequence("LongAtk01_End_A_Ball", false, TEXT("Cyclops_Anim\\LongAtk01_End_A_Ball.sqc"));

	GET_SINGLE(CResourceManager)->LoadAnimationSequence("Goblin_Idle_C", true, TEXT("Goblin\\Idle_C.sqc"));
	
	GET_SINGLE(CResourceManager)->LoadAnimationSequence("Goblin_Die", false, TEXT("Goblin\\Die.sqc"));
	
	GET_SINGLE(CResourceManager)->LoadAnimationSequence("Goblin_Walk_F", true, TEXT("Goblin\\Walk_F.sqc"));
	
	GET_SINGLE(CResourceManager)->LoadAnimationSequence("Goblin_Run_F", true, TEXT("Goblin\\Run_F.sqc"));
	
	GET_SINGLE(CResourceManager)->LoadAnimationSequence("Goblin_DMG_F", false, TEXT("Goblin\\DMG_F.sqc"));
	
	GET_SINGLE(CResourceManager)->LoadAnimationSequence("Goblin_ATK_01", false, TEXT("Goblin\\ATK_01.sqc"));
	
	GET_SINGLE(CResourceManager)->LoadAnimationSequence("Goblin_ATK_02", false, TEXT("Goblin\\ATK_02.sqc"));

	GET_SINGLE(CResourceManager)->LoadAnimationSequence("Goblin_DeadBody", false, TEXT("Goblin\\DeadBody.sqc"));

	//if (!GET_SINGLE(CResourceManager)->LoadSequence("Knight", true, TEXT("Player_Default.sqc")))
		//return false;
	//if (!GET_SINGLE(CResourceManager)->LoadAnimationSequence("Face_Jump_Up_WP", true, TEXT("BPlayer\\Face\\Jump_Up_WP.FBX")))
		//return false;
	return true;
}

bool CMainMode::CreateTree()
{
	std::shared_ptr<CGameObject> pTreeProto = m_pScene->CreateProtoObj<CTree>("Tree", m_pScene, m_pScene->GetSceneType());

	std::shared_ptr<CGameObject> pTree = m_pScene->CreateCloneObj("Tree1", "Tree", m_pScene->FindLayer(ALPHA_LAYER), m_pScene->GetSceneType());

	pTree->SetWorldPos(1290.f, 169.f, 250.f);

	pTree = m_pScene->CreateCloneObj("Tree2", "Tree", m_pScene->FindLayer(ALPHA_LAYER), m_pScene->GetSceneType());

	pTree->SetWorldPos(1650.f, 200.f, 430.f);

	pTree = m_pScene->CreateCloneObj("Tree3", "Tree", m_pScene->FindLayer(ALPHA_LAYER), m_pScene->GetSceneType());

	pTree->SetWorldPos(1300.f, 225.f, 1200.f);

	pTree = m_pScene->CreateCloneObj("Tree4", "Tree", m_pScene->FindLayer(ALPHA_LAYER), m_pScene->GetSceneType());

	pTree->SetWorldPos(1520.f, 200.f, 1330.f);

	return true;
}

bool CMainMode::CreateRock()
{
	std::shared_ptr<CGameObject> pObj = m_pScene->CreateObject<CGameObject>("Boat", m_pScene->FindLayer(ALBEDO_LAYER));
	std::shared_ptr<CComponent> pCom = pObj->CreateComponent<CSceneComponent>("Renderer");
	pObj->SetRootComponent(pCom);
	static_cast<CSceneComponent*>(pCom.get())->SetMesh("Boat");
	pObj->SetWorldPos(100.f, 250.f, 2800.f);

	pObj = m_pScene->CreateObject<CGameObject>("bone01", m_pScene->FindLayer(ALBEDO_LAYER));
	pCom = pObj->CreateComponent<CSceneComponent>("Renderer");
	pObj->SetRootComponent(pCom);
	static_cast<CSceneComponent*>(pCom.get())->SetMesh("bone01");
	pObj->SetWorldPos(1000.f, 0.1f, 3200.f);

	pObj = m_pScene->CreateObject<CGameObject>("bone02", m_pScene->FindLayer(ALBEDO_LAYER));
	pCom = pObj->CreateComponent<CSceneComponent>("Renderer");
	pObj->SetRootComponent(pCom);
	static_cast<CSceneComponent*>(pCom.get())->SetMesh("bone02");
	pObj->SetWorldPos(2400.f,2.f, 3500.f);

	pObj = m_pScene->CreateObject<CGameObject>("BrokenTree1", m_pScene->FindLayer(ALBEDO_LAYER));
	pCom = pObj->CreateComponent<CSceneComponent>("Renderer");
	pObj->SetRootComponent(pCom);
	static_cast<CSceneComponent*>(pCom.get())->SetMesh("BrokenTree1");
	pObj->SetWorldPos(2000.f, 100.f, 4800.f);
	pObj->SetWorldRotY(90.f);

	pObj = m_pScene->CreateObject<CGameObject>("BrokenTree2", m_pScene->FindLayer(ALBEDO_LAYER));
	pCom = pObj->CreateComponent<CSceneComponent>("Renderer");
	pObj->SetRootComponent(pCom);
	static_cast<CSceneComponent*>(pCom.get())->SetMesh("BrokenTree2");
	pObj->SetWorldPos(3300.f, 200.f, 3500.f);


	pObj = m_pScene->CreateObject<CGameObject>("Gate01", m_pScene->FindLayer(ALBEDO_LAYER));
	pCom = pObj->CreateComponent<CSceneComponent>("Renderer");
	pObj->SetRootComponent(pCom);
	static_cast<CSceneComponent*>(pCom.get())->SetMesh("Gate01");
	pObj->SetWorldPos(3400.f, 50.f, 4500.f);
	pObj->SetWorldRotY(90.f);


	pObj = m_pScene->CreateObject<CGameObject>("Deco01", m_pScene->FindLayer(ALPHA_LAYER));
	pCom = pObj->CreateComponent<CSceneComponent>("Renderer");
	pObj->SetRootComponent(pCom);
	static_cast<CSceneComponent*>(pCom.get())->SetMesh("Deco01");
	pObj->SetWorldPos(359.342f, 21.865f, 2958.125f);
	pObj->SetWorldRotY(90.f);

	pObj = m_pScene->CreateObject<CGameObject>("Deco02", m_pScene->FindLayer(ALPHA_LAYER));
	pCom = pObj->CreateComponent<CSceneComponent>("Renderer");
	pObj->SetRootComponent(pCom);
	static_cast<CSceneComponent*>(pCom.get())->SetMesh("Deco02");
	pObj->SetWorldPos(2117.114f, 15.388f, 4691.926f);
	pObj->SetWorldRotY(90.f);

	pObj = m_pScene->CreateObject<CGameObject>("Deco03", m_pScene->FindLayer(ALPHA_LAYER));
	pCom = pObj->CreateComponent<CSceneComponent>("Renderer");
	pObj->SetRootComponent(pCom);
	static_cast<CSceneComponent*>(pCom.get())->SetMesh("Deco03");
	pObj->SetWorldPos(1494.677f, -5.448f, 2437.713f);

	pObj = m_pScene->CreateObject<CGameObject>("Deco04", m_pScene->FindLayer(ALPHA_LAYER));
	pCom = pObj->CreateComponent<CSceneComponent>("Renderer");
	pObj->SetRootComponent(pCom);
	static_cast<CSceneComponent*>(pCom.get())->SetMesh("Deco04");
	pObj->SetWorldPos(516.907f, 0.382f, 3973.354f);


	pObj = m_pScene->CreateObject<CGameObject>("Ladder", m_pScene->FindLayer(ALBEDO_LAYER));
	pCom = pObj->CreateComponent<CSceneComponent>("Renderer");
	pObj->SetRootComponent(pCom);
	static_cast<CSceneComponent*>(pCom.get())->SetMesh("Ladder");
	pObj->SetWorldPos(800.f, 200.f, 4912.f);
	pObj->SetWorldRotX(40.f);
	//pObj->SetWorldRotY(90.f);

	pObj = m_pScene->CreateObject<CGameObject>("Mat", m_pScene->FindLayer(ALBEDO_LAYER));
	pCom = pObj->CreateComponent<CSceneComponent>("Renderer");
	pObj->SetRootComponent(pCom);
	static_cast<CSceneComponent*>(pCom.get())->SetMesh("Mat");
	pObj->SetWorldPos(1000.f, 0.f, 2600.f);


	pObj = m_pScene->CreateObject<CGameObject>("StoneStair", m_pScene->FindLayer(ALBEDO_LAYER));
	pCom = pObj->CreateComponent<CSceneComponent>("Renderer");
	pObj->SetRootComponent(pCom);
	static_cast<CSceneComponent*>(pCom.get())->SetMesh("StoneStair");
	pObj->SetWorldPos(900.f, 0.f, 4750.f);

	pObj = m_pScene->CreateObject<CGameObject>("TurtleRock3", m_pScene->FindLayer(ALBEDO_LAYER));
	pCom = pObj->CreateComponent<CSceneComponent>("TurtleRock3");
	pObj->SetRootComponent(pCom);
	static_cast<CSceneComponent*>(pCom.get())->SetMesh("TurtleRock3");
	pObj->SetWorldPos(-800.f, 600.f, 2600.f);
	pObj->SetWorldRot(0.f, 0.f, 90.f);

	pObj = m_pScene->CreateObject<CGameObject>("TurtleRock4", m_pScene->FindLayer(ALBEDO_LAYER));
	pCom = pObj->CreateComponent<CSceneComponent>("TurtleRock4");
	pObj->SetRootComponent(pCom);
	static_cast<CSceneComponent*>(pCom.get())->SetMesh("TurtleRock4");
	pObj->SetWorldPos(-250.f, 600.f, 4200.f);
	pObj->SetWorldRot(0.f, 90.f, 90.f);


	return true;
}

bool CMainMode::CreateUI()
{
	std::shared_ptr<CGameObject> pUI = m_pScene->CreateObject<CUI>("UI", m_pScene->FindLayer(UI_LAYER));

	CLayer* pLayer = m_pScene->FindLayer(ALBEDO_LAYER);

	if (pLayer)
	{
		std::shared_ptr<CGameObject> pPlayer = pLayer->FindObj("Player");

		static_cast<CUI*>(pUI.get())->SetPlayer(static_cast<CPlayer3D*>(pPlayer.get()));
	}
	std::shared_ptr<CGameObject> pInven = m_pScene->CreateObject<CInventory3D>("Inventory", m_pScene->FindLayer(UI_LAYER));
	pInven->SetEnable(false);
	std::shared_ptr<CGameObject> pEquip = m_pScene->CreateObject<CEquip>("Equip", m_pScene->FindLayer(UI_LAYER));
	pEquip->SetEnable(false);
	std::shared_ptr<CGameObject> pButton = m_pScene->CreateObject<CInvenButton>("Inventory Button", m_pScene->FindLayer(UI_LAYER));
	std::shared_ptr<CGameObject> pEquipButton = m_pScene->CreateObject<CEquipButton>("Equip Button", m_pScene->FindLayer(UI_LAYER));
	std::shared_ptr<CGameObject> pTargetUI = m_pScene->CreateObject<CTargetUI>("Target UI", m_pScene->FindLayer(UI_LAYER));
	pTargetUI->SetEnable(false);

	return true;
}

bool CMainMode::CreateLOD()
{
	std::shared_ptr<CGameObject> pObj = m_pScene->CreateObject<CGameObject>("MassiveLod1", m_pScene->FindLayer(ALBEDO_LAYER));

	std::shared_ptr<CComponent> pCom = pObj->CreateComponent<CSceneComponent>("Renderer");

	pObj->SetRootComponent(pCom);

	static_cast<CSceneComponent*>(pCom.get())->SetMesh("MassiveLod1");

	pObj->SetWorldPos(9000.f, 1000.000f, 2500.f);

	pObj = m_pScene->CreateObject<CGameObject>("MassiveLod2", m_pScene->FindLayer(ALBEDO_LAYER));

	pCom = pObj->CreateComponent<CSceneComponent>("Renderer");

	pObj->SetRootComponent(pCom);

	static_cast<CSceneComponent*>(pCom.get())->SetMesh("MassiveLod2");

	pObj->SetWorldPos(3000.f, 750.f, 5600.f);
	pObj->SetWorldRotY(-30.f);

	pObj = m_pScene->CreateObject<CGameObject>("MassiveLod1_1", m_pScene->FindLayer(ALBEDO_LAYER));

	pCom = pObj->CreateComponent<CSceneComponent>("MassiveLod1_1");

	pObj->SetRootComponent(pCom);

	static_cast<CSceneComponent*>(pCom.get())->SetMesh("MassiveLod1");

	pObj->SetWorldPos(-3000.f, 1600.f, 6600.f);
	pObj->SetWorldRotY(240.000f);

	return true;
}

bool CMainMode::CreateCampA()
{
	std::shared_ptr<CGameObject> pObj = m_pScene->CreateObject<CGameObject>("cave01", m_pScene->FindLayer(ALBEDO_LAYER));
	std::shared_ptr<CComponent> pCom = pObj->CreateComponent<CSceneComponent>("Renderer");
	pObj->SetRootComponent(pCom);
	static_cast<CSceneComponent*>(pCom.get())->SetMesh("cave01");
	pObj->SetWorldPos(1800.f, 250.f, 5150.f);

	 pObj = m_pScene->CreateObject<CGameObject>("cave02", m_pScene->FindLayer(ALBEDO_LAYER));
	pCom = pObj->CreateComponent<CSceneComponent>("Renderer");
	pObj->SetRootComponent(pCom);
	static_cast<CSceneComponent*>(pCom.get())->SetMesh("cave02");
	pObj->SetWorldPos(4850.f, 250.f, 1600.f);
	pObj->SetWorldRotY(90.f);

	 pObj = m_pScene->CreateObject<CGameObject>("cave03", m_pScene->FindLayer(ALBEDO_LAYER));
	pCom = pObj->CreateComponent<CSceneComponent>("Renderer");
	pObj->SetRootComponent(pCom);
	static_cast<CSceneComponent*>(pCom.get())->SetMesh("cave03");
	pObj->SetWorldPos(50.f, 250.f, 1000.f);
	pObj->SetWorldRotY(-90.f);

	 pObj = m_pScene->CreateObject<CGameObject>("cave04", m_pScene->FindLayer(ALBEDO_LAYER));
	pCom = pObj->CreateComponent<CSceneComponent>("Renderer");
	pObj->SetRootComponent(pCom);
	static_cast<CSceneComponent*>(pCom.get())->SetMesh("cave04");
	pObj->SetWorldPos(800.f, 250.f, 0.f);
	pObj->SetWorldRotY(180.f);

	 pObj = m_pScene->CreateObject<CGameObject>("caveTent01", m_pScene->FindLayer(ALBEDO_LAYER));
	pCom = pObj->CreateComponent<CSceneComponent>("Renderer");
	pObj->SetRootComponent(pCom);
	static_cast<CSceneComponent*>(pCom.get())->SetMesh("caveTent01");
	pObj->SetWorldPos(1850.f, 250.f, 5080.f);

	 pObj = m_pScene->CreateObject<CGameObject>("caveTent02", m_pScene->FindLayer(ALBEDO_LAYER));
	pCom = pObj->CreateComponent<CSceneComponent>("Renderer");
	pObj->SetRootComponent(pCom);
	static_cast<CSceneComponent*>(pCom.get())->SetMesh("caveTent02");
	pObj->SetWorldPos(4680.f, 250.f, 1620.f);
	pObj->SetWorldRotY(90.f);

	 pObj = m_pScene->CreateObject<CGameObject>("Fence01", m_pScene->FindLayer(ALBEDO_LAYER));
	pCom = pObj->CreateComponent<CSceneComponent>("Renderer");
	pObj->SetRootComponent(pCom);
	static_cast<CSceneComponent*>(pCom.get())->SetMesh("Fence01");
	pObj->SetWorldPos(1100.f, 260.f, 1500.f);

	 pObj = m_pScene->CreateObject<CGameObject>("Fence01B", m_pScene->FindLayer(ALBEDO_LAYER));
	pCom = pObj->CreateComponent<CSceneComponent>("Renderer");
	pObj->SetRootComponent(pCom);
	static_cast<CSceneComponent*>(pCom.get())->SetMesh("Fence01B");
	pObj->SetWorldPos(1400.f, 250.f, 1300.f);

	 pObj = m_pScene->CreateObject<CGameObject>("Fence01C", m_pScene->FindLayer(ALBEDO_LAYER));
	pCom = pObj->CreateComponent<CSceneComponent>("Renderer");
	pObj->SetRootComponent(pCom);
	static_cast<CSceneComponent*>(pCom.get())->SetMesh("Fence01C");
	pObj->SetWorldPos(5050.f, 40.f, 100.f);
	pObj->SetWorldRotY(-90.f);

	 pObj = m_pScene->CreateObject<CGameObject>("Fence01D", m_pScene->FindLayer(ALBEDO_LAYER));
	pCom = pObj->CreateComponent<CSceneComponent>("Renderer");
	pObj->SetRootComponent(pCom);
	static_cast<CSceneComponent*>(pCom.get())->SetMesh("Fence01D");
	pObj->SetWorldPos(300.f, 250.f, 1000.f);

	 pObj = m_pScene->CreateObject<CGameObject>("Fence01E", m_pScene->FindLayer(ALBEDO_LAYER));
	pCom = pObj->CreateComponent<CSceneComponent>("Renderer");
	pObj->SetRootComponent(pCom);
	static_cast<CSceneComponent*>(pCom.get())->SetMesh("Fence01E");
	pObj->SetWorldPos(1140.f, 210.f, 1100.f);

	 pObj = m_pScene->CreateObject<CGameObject>("Fence01F", m_pScene->FindLayer(ALBEDO_LAYER));
	pCom = pObj->CreateComponent<CSceneComponent>("Renderer");
	pObj->SetRootComponent(pCom);
	static_cast<CSceneComponent*>(pCom.get())->SetMesh("Fence01F");
	pObj->SetWorldPos(3600.f, 210.f, 1700.f);

	 pObj = m_pScene->CreateObject<CGameObject>("Fence02A", m_pScene->FindLayer(ALBEDO_LAYER));
	pCom = pObj->CreateComponent<CSceneComponent>("Renderer");
	pObj->SetRootComponent(pCom);
	static_cast<CSceneComponent*>(pCom.get())->SetMesh("Fence02A");
	pObj->SetWorldPos(1100.f, 70.f, 2100.f);
	//pObj->SetWorldRotY(45.f);

	 pObj = m_pScene->CreateObject<CGameObject>("Fence02B", m_pScene->FindLayer(ALBEDO_LAYER));
	pCom = pObj->CreateComponent<CSceneComponent>("Renderer");
	pObj->SetRootComponent(pCom);
	static_cast<CSceneComponent*>(pCom.get())->SetMesh("Fence02B");
	pObj->SetWorldPos(1900.f, 190.f, 700.f);
	pObj->SetWorldRotY(-45.f);

	 pObj = m_pScene->CreateObject<CGameObject>("Ferry3", m_pScene->FindLayer(ALBEDO_LAYER));
	pCom = pObj->CreateComponent<CSceneComponent>("Renderer");
	pObj->SetRootComponent(pCom);
	static_cast<CSceneComponent*>(pCom.get())->SetMesh("Ferry3");
	pObj->SetWorldPos(3000.f, 170.f, 350.f);

	 pObj = m_pScene->CreateObject<CGameObject>("FishCage", m_pScene->FindLayer(ALPHA_LAYER));
	pCom = pObj->CreateComponent<CSceneComponent>("Renderer");
	pObj->SetRootComponent(pCom);
	static_cast<CSceneComponent*>(pCom.get())->SetMesh("FishCage");
	pObj->SetWorldPos(200.f, 250.f, 850.f);

	 pObj = m_pScene->CreateObject<CGameObject>("FishDryingRack01", m_pScene->FindLayer(ALBEDO_LAYER));
	pCom = pObj->CreateComponent<CSceneComponent>("Renderer");
	pObj->SetRootComponent(pCom);
	static_cast<CSceneComponent*>(pCom.get())->SetMesh("FishDryingRack01");
	pObj->SetWorldPos(2100.f, 230.f, 700.f);

	 pObj = m_pScene->CreateObject<CGameObject>("FishDryingRack02", m_pScene->FindLayer(ALBEDO_LAYER));
	pCom = pObj->CreateComponent<CSceneComponent>("Renderer");
	pObj->SetRootComponent(pCom);
	static_cast<CSceneComponent*>(pCom.get())->SetMesh("FishDryingRack02");
	pObj->SetWorldPos(150.f, 250.f, 900.f);

	 pObj = m_pScene->CreateObject<CGameObject>("Gate02", m_pScene->FindLayer(ALBEDO_LAYER));
	pCom = pObj->CreateComponent<CSceneComponent>("Renderer");
	pObj->SetRootComponent(pCom);
	static_cast<CSceneComponent*>(pCom.get())->SetMesh("Gate02");
	pObj->SetWorldPos(700.f, 50.f, 1800.f);
	//pObj->SetWorldRotY(45.f);

	 pObj = m_pScene->CreateObject<CGameObject>("GuardPost", m_pScene->FindLayer(ALBEDO_LAYER));
	pCom = pObj->CreateComponent<CSceneComponent>("Renderer");
	pObj->SetRootComponent(pCom);
	static_cast<CSceneComponent*>(pCom.get())->SetMesh("GuardPost");
	pObj->SetWorldPos(220.f, 280.f, 550.f);
	pObj->SetWorldRotY(90.f);

	 pObj = m_pScene->CreateObject<CGameObject>("HutFrame", m_pScene->FindLayer(ALBEDO_LAYER));
	pCom = pObj->CreateComponent<CSceneComponent>("Renderer");
	pObj->SetRootComponent(pCom);
	static_cast<CSceneComponent*>(pCom.get())->SetMesh("HutFrame");
	pObj->SetWorldPos(1300.f, 250.f, 1500.f);

	 pObj = m_pScene->CreateObject<CGameObject>("HutRoof", m_pScene->FindLayer(ALBEDO_LAYER));
	pCom = pObj->CreateComponent<CSceneComponent>("Renderer");
	pObj->SetRootComponent(pCom);
	static_cast<CSceneComponent*>(pCom.get())->SetMesh("HutRoof");
	pObj->SetWorldPos(1300.f, 370.f, 1500.f);

	 pObj = m_pScene->CreateObject<CGameObject>("PiledFishes", m_pScene->FindLayer(ALBEDO_LAYER));
	pCom = pObj->CreateComponent<CSceneComponent>("Renderer");
	pObj->SetRootComponent(pCom);
	static_cast<CSceneComponent*>(pCom.get())->SetMesh("PiledFishes");
	pObj->SetWorldPos(200.f, 245.f, 950.f);

	 pObj = m_pScene->CreateObject<CGameObject>("PotatoDryingRack", m_pScene->FindLayer(ALPHA_LAYER));
	pCom = pObj->CreateComponent<CSceneComponent>("Renderer");
	pObj->SetRootComponent(pCom);
	static_cast<CSceneComponent*>(pCom.get())->SetMesh("PotatoDryingRack");
	pObj->SetWorldPos(500.f, 0.f, 900.f);

	 pObj = m_pScene->CreateObject<CGameObject>("Shelter01", m_pScene->FindLayer(ALBEDO_LAYER));
	pCom = pObj->CreateComponent<CSceneComponent>("Renderer");
	pObj->SetRootComponent(pCom);
	static_cast<CSceneComponent*>(pCom.get())->SetMesh("Shelter01");
	pObj->SetWorldPos(100.f, 260.f, 1700.f);
	pObj->SetWorldRotY(-90.f);

	 pObj = m_pScene->CreateObject<CGameObject>("WoodBridge1", m_pScene->FindLayer(ALBEDO_LAYER));
	pCom = pObj->CreateComponent<CSceneComponent>("Renderer");
	pObj->SetRootComponent(pCom);
	static_cast<CSceneComponent*>(pCom.get())->SetMesh("WoodBridge1");
	pObj->SetWorldPos(700.f, 250.f, 1200.f);
	pObj->SetWorldRotY(90.f);

	 pObj = m_pScene->CreateObject<CGameObject>("WoodBridge2", m_pScene->FindLayer(ALBEDO_LAYER));
	pCom = pObj->CreateComponent<CSceneComponent>("Renderer");
	pObj->SetRootComponent(pCom);
	static_cast<CSceneComponent*>(pCom.get())->SetMesh("WoodBridge2");
	pObj->SetWorldPos(700.f, 300.f, 1280.f);
	pObj->SetWorldRotY(90.f);

	 pObj = m_pScene->CreateObject<CGameObject>("WoodBridge3", m_pScene->FindLayer(ALBEDO_LAYER));
	pCom = pObj->CreateComponent<CSceneComponent>("Renderer");
	pObj->SetRootComponent(pCom);
	static_cast<CSceneComponent*>(pCom.get())->SetMesh("WoodBridge3");
	pObj->SetWorldPos(430.f, 230.f, 1200.f);
	pObj->SetWorldRotY(90.f);

	return true;
}

bool CMainMode::CreateParticleProto()
{
	std::shared_ptr<CGameObject> pRockParticle = m_pScene->CreateProtoObj<CParticle3D>("RockParticle", m_pScene, m_pScene->GetSceneType());

	std::shared_ptr<CComponent> pCom = pRockParticle->CreateComponent<CParticle>("SmokeParticle");

	pRockParticle->SetRootComponent(pCom);

	static_cast<CParticle3D*>(pRockParticle.get())->SetLimitTime(3.f);

	static_cast<CParticle*>(pCom.get())->SetTexture(LINK_TYPE::LT_DIF, "FX_SmokeSubUV_002_Tex_Pys");
	static_cast<CParticle*>(pCom.get())->SetCountX(8);
	static_cast<CParticle*>(pCom.get())->SetCountY(4);
	static_cast<CParticle*>(pCom.get())->SetImageSize(Vector2(512.f, 256.f));
	static_cast<CParticle*>(pCom.get())->SetMaxFrame(32);
	static_cast<CParticle*>(pCom.get())->SetMaxScale(Vector3(48.f, 48.f, 1.f));
	static_cast<CParticle*>(pCom.get())->SetMinScale(Vector3(48.f, 48.f, 1.f));
	static_cast<CParticle*>(pCom.get())->SetMaxTime(3.f);
	static_cast<CParticle*>(pCom.get())->SetMinTime(3.f);
	static_cast<CParticle*>(pCom.get())->SetMaxColor(Vector4(1.f, 1.f, 1.f, 1.f));
	static_cast<CParticle*>(pCom.get())->SetMinColor(Vector4(1.f, 1.f, 1.f, 1.f));
	static_cast<CParticle*>(pCom.get())->SetMaxSpawnTime(3.1f);
	static_cast<CParticle*>(pCom.get())->SetSceneComponentType(SCENE_COMPONENT_TYPE::SCT_3D);

	std::shared_ptr<CComponent> pRock = pRockParticle->CreateComponent<CParticle>("RockParticle");

	static_cast<CParticle*>(pCom.get())->AddChild(pRock);

	static_cast<CParticle*>(pRock.get())->SetTexture(LINK_TYPE::LT_DIF, "Fx_StoneChip_001_Tex_Pys");
	static_cast<CParticle*>(pRock.get())->SetCountX(2);
	static_cast<CParticle*>(pRock.get())->SetCountY(2);
	static_cast<CParticle*>(pRock.get())->SetImageSize(Vector2(256.f, 256.f));
	static_cast<CParticle*>(pRock.get())->SetMaxFrame(-4);
	static_cast<CParticle*>(pRock.get())->SetMaxScale(Vector3(8.f, 8.f, 1.f));
	static_cast<CParticle*>(pRock.get())->SetMinScale(Vector3(8.f, 8.f, 1.f));
	static_cast<CParticle*>(pRock.get())->SetMaxTime(3.f);
	static_cast<CParticle*>(pRock.get())->SetMinTime(3.f);
	static_cast<CParticle*>(pRock.get())->SetMaxColor(Vector4(1.f, 1.f, 1.f, 1.f));
	static_cast<CParticle*>(pRock.get())->SetMinColor(Vector4(1.f, 1.f, 1.f, 1.f));
	static_cast<CParticle*>(pRock.get())->SetMaxSpawnTime(3.f);
	static_cast<CParticle*>(pRock.get())->SetSceneComponentType(SCENE_COMPONENT_TYPE::SCT_3D);
	static_cast<CParticle*>(pRock.get())->SetMove(true);
	static_cast<CParticle*>(pRock.get())->SetMaxSpeed(120.f);
	static_cast<CParticle*>(pRock.get())->SetMinSpeed(120.f);
	static_cast<CParticle*>(pRock.get())->SetAccel(Vector3(0.f, -100.f, 0.f));
	static_cast<CParticle*>(pRock.get())->SetAngleAccel(0.01f);
	static_cast<CParticle*>(pRock.get())->SetRange(Vector3(50.f, 50.f, 50.f));
	static_cast<CParticle*>(pRock.get())->SetSpawnCount(128);
	static_cast<CParticle*>(pRock.get())->SetMaxCount(6400);
	static_cast<CParticle*>(pRock.get())->SetThreadGroupSize(16);
	static_cast<CParticle*>(pRock.get())->SetUpdateShader("RockUpdateShader");

	std::shared_ptr<CComponent> _pCom = pRockParticle->CreateComponent<CParticle>("SmokeParticle2");

	static_cast<CParticle*>(pCom.get())->AddChild(_pCom);

	static_cast<CParticle*>(_pCom.get())->SetTexture(LINK_TYPE::LT_DIF, "FX_SmokeTile_005_Tex_Pys");
	static_cast<CParticle*>(_pCom.get())->SetCountX(2);
	static_cast<CParticle*>(_pCom.get())->SetCountY(2);
	static_cast<CParticle*>(_pCom.get())->SetImageSize(Vector2(512.f, 512.f));
	static_cast<CParticle*>(_pCom.get())->SetMaxFrame(-4);
	static_cast<CParticle*>(_pCom.get())->SetMaxScale(Vector3(128.f, 128.f, 1.f));
	static_cast<CParticle*>(_pCom.get())->SetMinScale(Vector3(8.f, 8.f, 1.f));
	static_cast<CParticle*>(_pCom.get())->SetMaxTime(3.f);
	static_cast<CParticle*>(_pCom.get())->SetMinTime(3.f);
	static_cast<CParticle*>(_pCom.get())->SetMaxColor(Vector4(1.f, 1.f, 1.f, 0.f));
	static_cast<CParticle*>(_pCom.get())->SetMinColor(Vector4(1.f, 1.f, 1.f, 0.5f));
	static_cast<CParticle*>(_pCom.get())->SetMaxSpawnTime(3.f);
	static_cast<CParticle*>(_pCom.get())->SetSceneComponentType(SCENE_COMPONENT_TYPE::SCT_3D);
	static_cast<CParticle*>(_pCom.get())->SetSpawnCount(4);
	static_cast<CParticle*>(_pCom.get())->SetRange(Vector3(50.f, 50.f, 50.f));

	std::shared_ptr<CGameObject> pParticle = m_pScene->CreateProtoObj<CParticle3D>("DustParticle", m_pScene, m_pScene->GetSceneType());

	pCom = pParticle->CreateComponent<CParticle>("DustParticle");

	pParticle->SetRootComponent(pCom);

	static_cast<CParticle3D*>(pParticle.get())->SetLimitTime(3.f);

	static_cast<CParticle*>(pCom.get())->SetTexture(LINK_TYPE::LT_DIF, "FX_SmokeSubUV_002_Tex_Pys");
	static_cast<CParticle*>(pCom.get())->SetCountX(8);
	static_cast<CParticle*>(pCom.get())->SetCountY(4);
	static_cast<CParticle*>(pCom.get())->SetImageSize(Vector2(512.f, 256.f));
	static_cast<CParticle*>(pCom.get())->SetMaxFrame(32);
	static_cast<CParticle*>(pCom.get())->SetMaxScale(Vector3(32.f, 32.f, 1.f));
	static_cast<CParticle*>(pCom.get())->SetMinScale(Vector3(32.f, 32.f, 1.f));
	static_cast<CParticle*>(pCom.get())->SetMaxTime(3.f);
	static_cast<CParticle*>(pCom.get())->SetMinTime(3.f);
	static_cast<CParticle*>(pCom.get())->SetMaxColor(Vector4(1.f, 1.f, 1.f, 1.f));
	static_cast<CParticle*>(pCom.get())->SetMinColor(Vector4(1.f, 1.f, 1.f, 1.f));
	static_cast<CParticle*>(pCom.get())->SetMaxSpawnTime(3.f);
	static_cast<CParticle*>(pCom.get())->SetSceneComponentType(SCENE_COMPONENT_TYPE::SCT_3D);
	

	pParticle = m_pScene->CreateProtoObj<CParticle3D>("DarkDustParticle", m_pScene, m_pScene->GetSceneType());

	pCom = pParticle->CreateComponent<CParticle>("DarkDustParticle");

	pParticle->SetRootComponent(pCom);

	static_cast<CParticle3D*>(pParticle.get())->SetLimitTime(3.f);

	static_cast<CParticle*>(pCom.get())->SetTexture(LINK_TYPE::LT_DIF, "FX_SmokeTile_005_Tex_Pys");
	static_cast<CParticle*>(pCom.get())->SetCountX(2);
	static_cast<CParticle*>(pCom.get())->SetCountY(2);
	static_cast<CParticle*>(pCom.get())->SetImageSize(Vector2(512.f, 512.f));
	static_cast<CParticle*>(pCom.get())->SetMaxFrame(-4);
	static_cast<CParticle*>(pCom.get())->SetMaxScale(Vector3(128.f, 128.f, 1.f));
	static_cast<CParticle*>(pCom.get())->SetMinScale(Vector3(64.f, 64.f, 1.f));
	static_cast<CParticle*>(pCom.get())->SetMaxTime(3.f);
	static_cast<CParticle*>(pCom.get())->SetMinTime(3.f);
	static_cast<CParticle*>(pCom.get())->SetMaxColor(Vector4(1.f, 1.f, 1.f, 0.f));
	static_cast<CParticle*>(pCom.get())->SetMinColor(Vector4(1.f, 1.f, 1.f, 1.f));
	static_cast<CParticle*>(pCom.get())->SetMaxSpawnTime(3.f);
	static_cast<CParticle*>(pCom.get())->SetSceneComponentType(SCENE_COMPONENT_TYPE::SCT_3D);
	static_cast<CParticle*>(pCom.get())->SetSpawnCount(4);
	static_cast<CParticle*>(pCom.get())->SetRange(Vector3(100.f, 100.f, 100.f));
	static_cast<CParticle*>(pCom.get())->SetMaxCount(25600);
	static_cast<CParticle*>(pCom.get())->SetSpawnCount(256);

	pParticle = m_pScene->CreateProtoObj<CParticle3D>("BloodParticle", m_pScene, m_pScene->GetSceneType());

	pCom = pParticle->CreateComponent<CParticle>("BloodParticle");

	pParticle->SetRootComponent(pCom);

	static_cast<CParticle3D*>(pParticle.get())->SetLimitTime(0.8f);

	static_cast<CParticle*>(pCom.get())->SetShader("ParticleLight");
	static_cast<CParticle*>(pCom.get())->SetTexture(LINK_TYPE::LT_DIF, "FX_blood_002_TEX_KJS");
	static_cast<CParticle*>(pCom.get())->SetTexture(LINK_TYPE::LT_NOR, "FX_blood_002_NTEX_KJS");
	static_cast<CParticle*>(pCom.get())->SetCountX(4);
	static_cast<CParticle*>(pCom.get())->SetCountY(4);
	static_cast<CParticle*>(pCom.get())->SetImageSize(Vector2(512.f, 512.f));
	static_cast<CParticle*>(pCom.get())->SetMaxFrame(16);
	static_cast<CParticle*>(pCom.get())->SetMaxScale(Vector3(32.f, 32.f, 1.f));
	static_cast<CParticle*>(pCom.get())->SetMinScale(Vector3(32.f, 32.f, 1.f));
	static_cast<CParticle*>(pCom.get())->SetMaxTime(0.8f);
	static_cast<CParticle*>(pCom.get())->SetMinTime(0.8f);
	static_cast<CParticle*>(pCom.get())->SetMaxColor(Vector4(1.f, 1.f, 1.f, 1.f));
	static_cast<CParticle*>(pCom.get())->SetMinColor(Vector4(1.f, 1.f, 1.f, 1.f));
	static_cast<CParticle*>(pCom.get())->SetMaxSpawnTime(0.f);
	static_cast<CParticle*>(pCom.get())->SetSceneComponentType(SCENE_COMPONENT_TYPE::SCT_3D);
	static_cast<CParticle*>(pCom.get())->SetSpawnCount(1);
	static_cast<CParticle*>(pCom.get())->SetRange(Vector3(20.f, 20.f, 20.f));
	std::shared_ptr<CMaterial> pBloodMtrl = static_cast<CParticle*>(pCom.get())->GetMaterial();

	if (pBloodMtrl)
	{
		pBloodMtrl->SetDiffuseColor(Vector4(150.f / 255.f, 150.f / 255.f, 150.f / 255.f, 1.f));
		pBloodMtrl->SetAmbientColor(Vector4(150.f / 255.f, 150.f / 255.f, 150.f / 255.f, 1.f));
		pBloodMtrl->SetSpecularColor(Vector4(0.f, 0.f, 0.f, 2.f));
		pBloodMtrl->SetEmissiveColor(Vector4(0.f, 0.f, 0.f, 1.f));
	}

	pParticle = m_pScene->CreateProtoObj<CParticle3D>("BloodParticle2", m_pScene, m_pScene->GetSceneType());

	pCom = pParticle->CreateComponent<CParticle>("BloodParticle2");

	pParticle->SetRootComponent(pCom);

	static_cast<CParticle3D*>(pParticle.get())->SetLimitTime(0.8f);

	static_cast<CParticle*>(pCom.get())->SetShader("ParticleLight");
	static_cast<CParticle*>(pCom.get())->SetTexture(LINK_TYPE::LT_DIF, "FX_blood_002_TEX_KJS");
	static_cast<CParticle*>(pCom.get())->SetTexture(LINK_TYPE::LT_NOR, "FX_blood_002_NTEX_KJS");
	static_cast<CParticle*>(pCom.get())->SetCountX(4);
	static_cast<CParticle*>(pCom.get())->SetCountY(4);
	static_cast<CParticle*>(pCom.get())->SetImageSize(Vector2(512.f, 512.f));
	static_cast<CParticle*>(pCom.get())->SetMaxFrame(16);
	static_cast<CParticle*>(pCom.get())->SetMaxScale(Vector3(32.f, 32.f, 1.f));
	static_cast<CParticle*>(pCom.get())->SetMinScale(Vector3(32.f, 32.f, 1.f));
	static_cast<CParticle*>(pCom.get())->SetMaxTime(0.8f);
	static_cast<CParticle*>(pCom.get())->SetMinTime(0.8f);
	static_cast<CParticle*>(pCom.get())->SetMaxColor(Vector4(1.f, 1.f, 1.f, 1.f));
	static_cast<CParticle*>(pCom.get())->SetMinColor(Vector4(1.f, 1.f, 1.f, 1.f));
	static_cast<CParticle*>(pCom.get())->SetMaxSpawnTime(0.f);
	static_cast<CParticle*>(pCom.get())->SetSceneComponentType(SCENE_COMPONENT_TYPE::SCT_3D);
	static_cast<CParticle*>(pCom.get())->SetSpawnCount(1);
	static_cast<CParticle*>(pCom.get())->SetRange(Vector3(10.f, 10.f, 10.f));

	pBloodMtrl = static_cast<CParticle*>(pCom.get())->GetMaterial();

	if (pBloodMtrl)
	{
		pBloodMtrl->SetDiffuseColor(Vector4(150.f / 255.f, 150.f / 255.f, 150.f / 255.f, 1.f));
		pBloodMtrl->SetAmbientColor(Vector4(150.f / 255.f, 150.f / 255.f, 150.f / 255.f, 1.f));
		pBloodMtrl->SetSpecularColor(Vector4(0.f, 0.f, 0.f, 2.f));
		pBloodMtrl->SetEmissiveColor(Vector4(0.f, 0.f, 0.f, 1.f));
	}
	GET_SINGLE(CResourceManager)->LoadTexture("T_UI_BaseA", L"UI_Base\\Texture2D\\T_UI_BaseA.dds");

	std::shared_ptr<CGameObject> pDamage = m_pScene->CreateProtoObj<CParticle3D>("PlayerDamageFont", m_pScene, m_pScene->GetSceneType());

	std::shared_ptr<CComponent> pChar = pDamage->CreateComponent<CParticle>("Char", m_pScene->FindLayer(PARTICLE_LAYER));

	pDamage->SetRootComponent(pChar);

	static_cast<CParticle3D*>(pDamage.get())->SetLimitTime(3.f);
	static_cast<CParticle*>(pChar.get())->SetMove(true);
	static_cast<CParticle*>(pChar.get())->SetMaxFrame(20);
	static_cast<CParticle*>(pChar.get())->SetMaxTime(3.f);
	static_cast<CParticle*>(pChar.get())->SetMaxSpeed(20.f);
	static_cast<CParticle*>(pChar.get())->SetMinScale(Vector3(4.f, 4.f, 1.f));
	static_cast<CParticle*>(pChar.get())->SetMaxScale(Vector3(8.f, 8.f, 1.f));
	static_cast<CParticle*>(pChar.get())->SetMaxColor(Vector4(0.5f, 0.01f, 0.65f, 1.f));
	static_cast<CParticle*>(pChar.get())->SetMinColor(Vector4(0.65f, 0.01f, 0.5f, 1.f));
	static_cast<CParticle*>(pChar.get())->SetSceneComponentType(SCENE_COMPONENT_TYPE::SCT_3D);
	static_cast<CParticle*>(pChar.get())->SetTexture(LINK_TYPE::LT_DIF, "T_UI_BaseA");
	static_cast<CParticle*>(pChar.get())->SetMaxSpawnTime(10.f);
	static_cast<CParticle*>(pChar.get())->SetShader("CharShader");
	static_cast<CParticle*>(pChar.get())->SetUpdateShader("CharUpdateShader");


	std::shared_ptr<CGameObject> pLight = m_pScene->CreateProtoObj<CParticle3D>("Hit Particle", m_pScene, m_pScene->GetSceneType());

	pCom = pLight->CreateComponent<CParticle>("Particle", m_pScene->FindLayer(PARTICLE_LAYER));
	static_cast<CParticle3D*>(pLight.get())->SetLimitTime(3.f);
	static_cast<CParticle3D*>(pLight.get())->AddChild(pCom);

	static_cast<CParticle*>(pCom.get())->SetShader("ParticleBlackClip");
	static_cast<CParticle*>(pCom.get())->SetMaxScale(Vector3(1.f, 1.f, 1.f));
	static_cast<CParticle*>(pCom.get())->SetMinScale(Vector3(1.f, 1.f, 1.f));

	static_cast<CParticle*>(pCom.get())->SetMaxColor(Vector4(1.f, 1.f, 1.f, 1.f));
	static_cast<CParticle*>(pCom.get())->SetMinColor(Vector4(1.f, 1.f, 1.f, 1.f));
	static_cast<CParticle*>(pCom.get())->SetSceneComponentType(SCENE_COMPONENT_TYPE::SCT_3D);
	static_cast<CParticle*>(pCom.get())->SetSpawnCount(1000);
	static_cast<CParticle*>(pCom.get())->SetMaxSpawnTime(0.f);
	static_cast<CParticle*>(pCom.get())->SetMaxCount(1000);
	static_cast<CParticle*>(pCom.get())->SetMaxSpeed(150.f);
	static_cast<CParticle*>(pCom.get())->SetMinSpeed(150.f);
	static_cast<CParticle*>(pCom.get())->SetAccel(Vector3(0.f, -100.f, 0.f));
	static_cast<CParticle*>(pCom.get())->SetMaxTime(2.f);
	static_cast<CParticle*>(pCom.get())->SetMinTime(2.f);
	static_cast<CParticle*>(pCom.get())->SetMove(true);
	static_cast<CParticle*>(pCom.get())->SetCountX(1);
	static_cast<CParticle*>(pCom.get())->SetCountY(1);
	static_cast<CParticle*>(pCom.get())->SetMaxFrame(1);
	static_cast<CParticle*>(pCom.get())->SetImageSize(Vector2(512.f, 512.f));
	static_cast<CParticle*>(pCom.get())->SetTexture(LINK_TYPE::LT_DIF, "FX_SparkYellow_001_TEX_HKB");


	std::shared_ptr<CGameObject> pParticleObj = m_pScene->CreateProtoObj<CParticle3D>("IceSmoke", 
		m_pScene,m_pScene->GetSceneType());

	static_cast<CParticle3D*>(pParticleObj.get())->SetLimitTime(1.5f);

	pCom = pParticleObj->CreateComponent<CParticle>("IceSmoke");

	pParticleObj->SetRootComponent(pCom);

	static_cast<CParticle*>(pCom.get())->SetShader("AlphaParticle");
	static_cast<CParticle*>(pCom.get())->SetTexture(LINK_TYPE::LT_EMI, "FX_Smoke_001_TEX_KKJ_Clamp");
	std::shared_ptr<CMaterial> pMtrl = static_cast<CParticle*>(pCom.get())->GetMaterial();

	if (pMtrl)
	{
		pMtrl->SetDiffuseColor(1.f, 1.f, 1.f, 1.f);
	}
	static_cast<CParticle*>(pCom.get())->SetUpdateShader("RockUpdateShader");
	static_cast<CParticle*>(pCom.get())->SetMove(true);
	static_cast<CParticle*>(pCom.get())->SetMaxScale(Vector3(50.f, 50.f, 1.f));
	static_cast<CParticle*>(pCom.get())->SetMinScale(Vector3(30.f, 30.f, 1.f));
	static_cast<CParticle*>(pCom.get())->SetMaxTime(1.5f);
	static_cast<CParticle*>(pCom.get())->SetMinTime(1.5f);
	static_cast<CParticle*>(pCom.get())->SetMaxSpeed(5.f);
	static_cast<CParticle*>(pCom.get())->SetMinSpeed(5.f);
	static_cast<CParticle*>(pCom.get())->SetSpawnCount(25600);
	static_cast<CParticle*>(pCom.get())->SetMaxCount(51200);
	static_cast<CParticle*>(pCom.get())->SetMaxSpawnTime(1.5f);
	static_cast<CParticle*>(pCom.get())->SetRange(Vector3(500.f, 5.f, 500.f));
	static_cast<CParticle*>(pCom.get())->SetMaxColor(Vector4(1.f, 1.f, 1.f, 0.f));
	static_cast<CParticle*>(pCom.get())->SetMinColor(Vector4(1.f, 1.f, 1.f, 1.f));
	static_cast<CParticle*>(pCom.get())->AddRenderState(NO_DEPTH);

	/*std::shared_ptr<CComponent> pBurn = pParticleObj->CreateComponent<CPaperBurn>("IceResolve");

	static_cast<CParticle*>(pCom.get())->AddObjComponent(pBurn);

	static_cast<CPaperBurn*>(pBurn.get())->SetBurnTexture("FX_dissolve_004_TEX_PYS");
	static_cast<CPaperBurn*>(pBurn.get())->ToggleBurn();
	static_cast<CPaperBurn*>(pBurn.get())->SetLoop(true);
	static_cast<CPaperBurn*>(pBurn.get())->SetColor(1.f, 1.f, 1.f);
	static_cast<CPaperBurn*>(pBurn.get())->SetInColor(0.f, 200.f / 255.f, 1.f);*/


	return true;
}

bool CMainMode::CreateProtoType()
{
	m_pScene->CreateProtoObj<CCyclops>("Cyclops", m_pScene, m_pScene->GetSceneType());
	std::shared_ptr<CGameObject> pBullet = m_pScene->CreateProtoObj<CBullet3D>("Bullet", m_pScene, m_pScene->GetSceneType());

	pBullet = m_pScene->CreateProtoObj<CBlueFlame>("BlueBullet", m_pScene, m_pScene->GetSceneType());
	std::shared_ptr<CGameObject> pGoblin = m_pScene->CreateProtoObj<CGoblin>("Goblin", m_pScene, m_pScene->GetSceneType());

	m_pScene->CreateProtoObj<CGoblinRock>("GoblinRock", m_pScene, m_pScene->GetSceneType());

	std::shared_ptr<CGameObject> pDamage = m_pScene->CreateProtoObj<CParticle3D>("MonsterDamageFont", m_pScene, m_pScene->GetSceneType());

	std::shared_ptr<CComponent> pChar = pDamage->CreateComponent<CParticle>("Char", m_pScene->FindLayer(PARTICLE_LAYER));

	pDamage->SetRootComponent(pChar);

	static_cast<CParticle3D*>(pDamage.get())->SetLimitTime(3.f);
	static_cast<CParticle*>(pChar.get())->SetMove(true);
	static_cast<CParticle*>(pChar.get())->SetMaxFrame(20);
	static_cast<CParticle*>(pChar.get())->SetMaxTime(3.f);
	static_cast<CParticle*>(pChar.get())->SetMaxSpeed(20.f);
	static_cast<CParticle*>(pChar.get())->SetMinScale(Vector3(4.f, 4.f, 1.f));
	static_cast<CParticle*>(pChar.get())->SetMaxScale(Vector3(8.f, 8.f, 1.f));
	static_cast<CParticle*>(pChar.get())->SetMaxColor(Vector4(0.5f, 0.01f, 0.02f, 1.f));
	static_cast<CParticle*>(pChar.get())->SetMinColor(Vector4(0.5f, 0.01f, 0.02f, 1.f));
	static_cast<CParticle*>(pChar.get())->SetSceneComponentType(SCENE_COMPONENT_TYPE::SCT_3D);
	static_cast<CParticle*>(pChar.get())->SetTexture(LINK_TYPE::LT_DIF, "T_UI_BaseA");
	static_cast<CParticle*>(pChar.get())->SetMaxSpawnTime(10.f);
	static_cast<CParticle*>(pChar.get())->SetShader("CharShader");
	static_cast<CParticle*>(pChar.get())->SetUpdateShader("CharUpdateShader");

	std::shared_ptr<CGameObject> pLootingWindow = m_pScene->CreateProtoObj<CLootingWindow>(
		"Looting", m_pScene, m_pScene->GetSceneType());

	std::shared_ptr<CGameObject> pCoin = m_pScene->CreateProtoObj<CCoin>(
		"Coin", m_pScene, m_pScene->GetSceneType());
	std::shared_ptr<CGameObject> pTree = m_pScene->CreateProtoObj<CGameObject>("ManzanitaTree1", m_pScene, m_pScene->GetSceneType());
	std::shared_ptr<CComponent> pCom = pTree->CreateComponent<CSceneComponent>("Renderer");
	pTree->SetRootComponent(pCom);
	static_cast<CSceneComponent*>(pCom.get())->SetMesh("ManzanitaTree1");
	static_cast<CSceneComponent*>(pCom.get())->AddRenderState(CULL_NONE, 0, 1);
	static_cast<CSceneComponent*>(pCom.get())->AddRenderState("DepthNoWrite", 0, 1);
	static_cast<CSceneComponent*>(pCom.get())->SetInstance(true);

	pTree = m_pScene->CreateProtoObj<CGameObject>("ManzanitaTree2", m_pScene, m_pScene->GetSceneType());
	pCom = pTree->CreateComponent<CSceneComponent>("Renderer");
	pTree->SetRootComponent(pCom);
	static_cast<CSceneComponent*>(pCom.get())->SetMesh("ManzanitaTree2");
	static_cast<CSceneComponent*>(pCom.get())->AddRenderState(CULL_NONE, 0, 1);
	static_cast<CSceneComponent*>(pCom.get())->AddRenderState("DepthNoWrite", 0, 1);
	static_cast<CSceneComponent*>(pCom.get())->SetInstance(true);

	pTree = m_pScene->CreateProtoObj<CGameObject>("ManzanitaTree3", m_pScene, m_pScene->GetSceneType());
	pCom = pTree->CreateComponent<CSceneComponent>("Renderer");
	pTree->SetRootComponent(pCom);
	static_cast<CSceneComponent*>(pCom.get())->SetMesh("ManzanitaTree3");
	static_cast<CSceneComponent*>(pCom.get())->AddRenderState(CULL_NONE, 0, 1);
	static_cast<CSceneComponent*>(pCom.get())->AddRenderState("DepthNoWrite", 0, 1);
	static_cast<CSceneComponent*>(pCom.get())->SetInstance(true);

	pTree = m_pScene->CreateProtoObj<CGameObject>("MadroneTree1", m_pScene, m_pScene->GetSceneType());
	pCom = pTree->CreateComponent<CSceneComponent>("Renderer");
	pTree->SetRootComponent(pCom);
	static_cast<CSceneComponent*>(pCom.get())->SetMesh("MadroneTree1");
	static_cast<CSceneComponent*>(pCom.get())->AddRenderState(CULL_NONE, 0, 1);
	static_cast<CSceneComponent*>(pCom.get())->AddRenderState("DepthNoWrite", 0, 1);
	static_cast<CSceneComponent*>(pCom.get())->SetInstance(true);

	pTree = m_pScene->CreateProtoObj<CGameObject>("MadroneTree2", m_pScene, m_pScene->GetSceneType());
	pCom = pTree->CreateComponent<CSceneComponent>("Renderer");
	pTree->SetRootComponent(pCom);
	static_cast<CSceneComponent*>(pCom.get())->SetMesh("MadroneTree2");
	static_cast<CSceneComponent*>(pCom.get())->AddRenderState(CULL_NONE, 0, 1);
	static_cast<CSceneComponent*>(pCom.get())->AddRenderState("DepthNoWrite", 0, 1);
	static_cast<CSceneComponent*>(pCom.get())->SetInstance(true);

	pTree = m_pScene->CreateProtoObj<CGameObject>("MadroneTree3", m_pScene, m_pScene->GetSceneType());
	pCom = pTree->CreateComponent<CSceneComponent>("Renderer");
	pTree->SetRootComponent(pCom);
	static_cast<CSceneComponent*>(pCom.get())->SetMesh("MadroneTree3");
	static_cast<CSceneComponent*>(pCom.get())->AddRenderState(CULL_NONE, 0, 1);
	static_cast<CSceneComponent*>(pCom.get())->AddRenderState("DepthNoWrite", 0, 1);
	static_cast<CSceneComponent*>(pCom.get())->SetInstance(true);

	std::shared_ptr<CGameObject> pRock = m_pScene->CreateProtoObj<CRock>("LakeRock1",
		m_pScene,m_pScene->GetSceneType());

	pRock = m_pScene->CreateProtoObj<CRock>("SmallRock1",
		m_pScene, m_pScene->GetSceneType());
	pCom = pRock->CreateComponent<CSceneComponent>("Renderer");
	pRock->SetRootComponent(pCom);
	static_cast<CSceneComponent*>(pCom.get())->SetMesh("SmallRock1");
	static_cast<CSceneComponent*>(pCom.get())->SetInstance(true);

	pRock = m_pScene->CreateProtoObj<CGameObject>("SmallRock2",
		m_pScene, m_pScene->GetSceneType());
	pCom = pRock->CreateComponent<CSceneComponent>("Renderer");
	pRock->SetRootComponent(pCom);
	static_cast<CSceneComponent*>(pCom.get())->SetMesh("SmallRock2");
	static_cast<CSceneComponent*>(pCom.get())->SetInstance(true);

	pRock = m_pScene->CreateProtoObj<CGameObject>("SmallRock3",
		m_pScene, m_pScene->GetSceneType());
	pCom = pRock->CreateComponent<CSceneComponent>("Renderer");
	pRock->SetRootComponent(pCom);
	static_cast<CSceneComponent*>(pCom.get())->SetMesh("SmallRock3");
	static_cast<CSceneComponent*>(pCom.get())->SetInstance(true);

	pRock = m_pScene->CreateProtoObj<CGameObject>("TurtleRock1",
		m_pScene, m_pScene->GetSceneType());
	pCom = pRock->CreateComponent<CSceneComponent>("Renderer");
	pRock->SetRootComponent(pCom);
	static_cast<CSceneComponent*>(pCom.get())->SetMesh("TurtleRock1");
	static_cast<CSceneComponent*>(pCom.get())->SetInstance(true);

	pRock = m_pScene->CreateProtoObj<CGameObject>("LakeRock2",
		m_pScene, m_pScene->GetSceneType());
	pCom = pRock->CreateComponent<CSceneComponent>("Renderer");
	pRock->SetRootComponent(pCom);
	static_cast<CSceneComponent*>(pCom.get())->SetMesh("LakeRock2");
	static_cast<CSceneComponent*>(pCom.get())->SetInstance(true);

	pRock = m_pScene->CreateProtoObj<CGameObject>("LakeRock3",
		m_pScene, m_pScene->GetSceneType());
	pCom = pRock->CreateComponent<CSceneComponent>("Renderer");
	pRock->SetRootComponent(pCom);
	static_cast<CSceneComponent*>(pCom.get())->SetMesh("LakeRock3");
	static_cast<CSceneComponent*>(pCom.get())->SetInstance(true);

	pRock = m_pScene->CreateProtoObj<CGameObject>("LakeStone1",
		m_pScene, m_pScene->GetSceneType());
	pCom = pRock->CreateComponent<CSceneComponent>("Renderer");
	pRock->SetRootComponent(pCom);
	static_cast<CSceneComponent*>(pCom.get())->SetMesh("LakeStone1");
	static_cast<CSceneComponent*>(pCom.get())->SetInstance(true);

	pRock = m_pScene->CreateProtoObj<CGameObject>("LakeStone2",
		m_pScene, m_pScene->GetSceneType());
	pCom = pRock->CreateComponent<CSceneComponent>("Renderer");
	pRock->SetRootComponent(pCom);
	static_cast<CSceneComponent*>(pCom.get())->SetMesh("LakeStone2");
	static_cast<CSceneComponent*>(pCom.get())->SetInstance(true);

	pRock = m_pScene->CreateProtoObj<CGameObject>("LakeStone3",
		m_pScene, m_pScene->GetSceneType());
	pCom = pRock->CreateComponent<CSceneComponent>("Renderer");
	pRock->SetRootComponent(pCom);
	static_cast<CSceneComponent*>(pCom.get())->SetMesh("LakeStone3");
	static_cast<CSceneComponent*>(pCom.get())->SetInstance(true);

	pRock = m_pScene->CreateProtoObj<CGameObject>("GoblinRock1",
		m_pScene, m_pScene->GetSceneType());
	pCom = pRock->CreateComponent<CSceneComponent>("Renderer");
	pRock->SetRootComponent(pCom);
	static_cast<CSceneComponent*>(pCom.get())->SetMesh("GoblinRock1");
	static_cast<CSceneComponent*>(pCom.get())->SetInstance(true);

	pRock = m_pScene->CreateProtoObj<CGameObject>("GoblinRock2",
		m_pScene, m_pScene->GetSceneType());
	pCom = pRock->CreateComponent<CSceneComponent>("Renderer");
	pRock->SetRootComponent(pCom);
	static_cast<CSceneComponent*>(pCom.get())->SetMesh("GoblinRock2");
	static_cast<CSceneComponent*>(pCom.get())->SetInstance(true);

	pRock = m_pScene->CreateProtoObj<CGameObject>("GoblinRock3",
		m_pScene, m_pScene->GetSceneType());
	pCom = pRock->CreateComponent<CSceneComponent>("Renderer");
	pRock->SetRootComponent(pCom);
	static_cast<CSceneComponent*>(pCom.get())->SetMesh("GoblinRock3");
	static_cast<CSceneComponent*>(pCom.get())->SetInstance(true);

	pRock = m_pScene->CreateProtoObj<CGameObject>("GoblinRock4",
		m_pScene, m_pScene->GetSceneType());
	pCom = pRock->CreateComponent<CSceneComponent>("Renderer");
	pRock->SetRootComponent(pCom);
	static_cast<CSceneComponent*>(pCom.get())->SetMesh("GoblinRock4");
	static_cast<CSceneComponent*>(pCom.get())->SetInstance(true);

	pRock = m_pScene->CreateProtoObj<CGameObject>("GoblinRock5",
		m_pScene, m_pScene->GetSceneType());
	pCom = pRock->CreateComponent<CSceneComponent>("Renderer");
	pRock->SetRootComponent(pCom);
	static_cast<CSceneComponent*>(pCom.get())->SetMesh("GoblinRock5");
	static_cast<CSceneComponent*>(pCom.get())->SetInstance(true);

	pRock = m_pScene->CreateProtoObj<CGameObject>("TurtleRock2",
		m_pScene, m_pScene->GetSceneType());
	pCom = pRock->CreateComponent<CSceneComponent>("Renderer");
	pRock->SetRootComponent(pCom);
	static_cast<CSceneComponent*>(pCom.get())->SetMesh("TurtleRock2");
	static_cast<CSceneComponent*>(pCom.get())->SetInstance(true);

	pRock = m_pScene->CreateProtoObj<CGameObject>("TurtleRock3",
		m_pScene, m_pScene->GetSceneType());
	pCom = pRock->CreateComponent<CSceneComponent>("Renderer");
	pRock->SetRootComponent(pCom);
	static_cast<CSceneComponent*>(pCom.get())->SetMesh("TurtleRock3");
	static_cast<CSceneComponent*>(pCom.get())->SetInstance(true);

	pRock = m_pScene->CreateProtoObj<CGameObject>("TurtleRock4",
		m_pScene, m_pScene->GetSceneType());
	pCom = pRock->CreateComponent<CSceneComponent>("Renderer");
	pRock->SetRootComponent(pCom);
	static_cast<CSceneComponent*>(pCom.get())->SetMesh("TurtleRock4");
	static_cast<CSceneComponent*>(pCom.get())->SetInstance(true);

	std::shared_ptr<CGameObject> pGrass = m_pScene->CreateProtoObj<CGameObject>("Bush1",
		m_pScene, m_pScene->GetSceneType());
	pCom = pGrass->CreateComponent<CSceneComponent>("Renderer");
	pGrass->SetRootComponent(pCom);
	static_cast<CSceneComponent*>(pCom.get())->SetMesh("Bush1");
	static_cast<CSceneComponent*>(pCom.get())->SetInstance(true);

	pGrass = m_pScene->CreateProtoObj<CGameObject>("Vine1",
		m_pScene, m_pScene->GetSceneType());
	pCom = pGrass->CreateComponent<CSceneComponent>("Renderer");
	pGrass->SetRootComponent(pCom);
	static_cast<CSceneComponent*>(pCom.get())->SetMesh("Vine1");
	static_cast<CSceneComponent*>(pCom.get())->SetInstance(true);

	pGrass = m_pScene->CreateProtoObj<CGameObject>("Bush2",
		m_pScene, m_pScene->GetSceneType());
	pCom = pGrass->CreateComponent<CSceneComponent>("Renderer");
	pGrass->SetRootComponent(pCom);
	static_cast<CSceneComponent*>(pCom.get())->SetMesh("Bush2");
	static_cast<CSceneComponent*>(pCom.get())->SetInstance(true);

	pGrass = m_pScene->CreateProtoObj<CGameObject>("Bush3",
		m_pScene, m_pScene->GetSceneType());
	pCom = pGrass->CreateComponent<CSceneComponent>("Renderer");
	pGrass->SetRootComponent(pCom);
	static_cast<CSceneComponent*>(pCom.get())->SetMesh("Bush3");
	static_cast<CSceneComponent*>(pCom.get())->SetInstance(true);

	pGrass = m_pScene->CreateProtoObj<CGameObject>("Bush4",
		m_pScene, m_pScene->GetSceneType());
	pCom = pGrass->CreateComponent<CSceneComponent>("Renderer");
	pGrass->SetRootComponent(pCom);
	static_cast<CSceneComponent*>(pCom.get())->SetMesh("Bush4");
	static_cast<CSceneComponent*>(pCom.get())->SetInstance(true);

	pGrass = m_pScene->CreateProtoObj<CGameObject>("BushTree",
		m_pScene, m_pScene->GetSceneType());
	pCom = pGrass->CreateComponent<CSceneComponent>("Renderer");
	pGrass->SetRootComponent(pCom);
	static_cast<CSceneComponent*>(pCom.get())->SetMesh("BushTree");
	static_cast<CSceneComponent*>(pCom.get())->SetInstance(true);

	pGrass = m_pScene->CreateProtoObj<CGameObject>("Cattail1",
		m_pScene, m_pScene->GetSceneType());
	pCom = pGrass->CreateComponent<CSceneComponent>("Renderer");
	pGrass->SetRootComponent(pCom);
	static_cast<CSceneComponent*>(pCom.get())->SetMesh("Cattail1");
	static_cast<CSceneComponent*>(pCom.get())->SetInstance(true);

	pGrass = m_pScene->CreateProtoObj<CGameObject>("Cattail2",
		m_pScene, m_pScene->GetSceneType());
	pCom = pGrass->CreateComponent<CSceneComponent>("Renderer");
	pGrass->SetRootComponent(pCom);
	static_cast<CSceneComponent*>(pCom.get())->SetMesh("Cattail2");
	static_cast<CSceneComponent*>(pCom.get())->SetInstance(true);

	pGrass = m_pScene->CreateProtoObj<CGameObject>("Crop1",
		m_pScene, m_pScene->GetSceneType());
	pCom = pGrass->CreateComponent<CSceneComponent>("Renderer");
	pGrass->SetRootComponent(pCom);
	static_cast<CSceneComponent*>(pCom.get())->SetMesh("Crop1");
	static_cast<CSceneComponent*>(pCom.get())->SetInstance(true);

	pGrass = m_pScene->CreateProtoObj<CGameObject>("Crop2",
		m_pScene, m_pScene->GetSceneType());
	pCom = pGrass->CreateComponent<CSceneComponent>("Renderer");
	pGrass->SetRootComponent(pCom);
	static_cast<CSceneComponent*>(pCom.get())->SetMesh("Crop2");
	static_cast<CSceneComponent*>(pCom.get())->SetInstance(true);

	pGrass = m_pScene->CreateProtoObj<CGameObject>("Duckweed1",
		m_pScene, m_pScene->GetSceneType());
	pCom = pGrass->CreateComponent<CSceneComponent>("Renderer");
	pGrass->SetRootComponent(pCom);
	static_cast<CSceneComponent*>(pCom.get())->SetMesh("Duckweed1");
	static_cast<CSceneComponent*>(pCom.get())->SetInstance(true);

	pGrass = m_pScene->CreateProtoObj<CGameObject>("Duckweed2",
		m_pScene, m_pScene->GetSceneType());
	pCom = pGrass->CreateComponent<CSceneComponent>("Renderer");
	pGrass->SetRootComponent(pCom);
	static_cast<CSceneComponent*>(pCom.get())->SetMesh("Duckweed2");
	static_cast<CSceneComponent*>(pCom.get())->SetInstance(true);

	pGrass = m_pScene->CreateProtoObj<CGameObject>("Ivy1",
		m_pScene, m_pScene->GetSceneType());
	pCom = pGrass->CreateComponent<CSceneComponent>("Renderer");
	pGrass->SetRootComponent(pCom);
	static_cast<CSceneComponent*>(pCom.get())->SetMesh("Ivy1");
	static_cast<CSceneComponent*>(pCom.get())->SetInstance(true);

	pGrass = m_pScene->CreateProtoObj<CGameObject>("Ivy2",
		m_pScene, m_pScene->GetSceneType());
	pCom = pGrass->CreateComponent<CSceneComponent>("Renderer");
	pGrass->SetRootComponent(pCom);
	static_cast<CSceneComponent*>(pCom.get())->SetMesh("Ivy2");
	static_cast<CSceneComponent*>(pCom.get())->SetInstance(true);

	pGrass = m_pScene->CreateProtoObj<CGameObject>("Ivy3",
		m_pScene, m_pScene->GetSceneType());
	pCom = pGrass->CreateComponent<CSceneComponent>("Renderer");
	pGrass->SetRootComponent(pCom);
	static_cast<CSceneComponent*>(pCom.get())->SetMesh("Ivy3");
	static_cast<CSceneComponent*>(pCom.get())->SetInstance(true);

	pGrass = m_pScene->CreateProtoObj<CGameObject>("MG1",
		m_pScene, m_pScene->GetSceneType());
	pCom = pGrass->CreateComponent<CSceneComponent>("Renderer");
	pGrass->SetRootComponent(pCom);
	static_cast<CSceneComponent*>(pCom.get())->SetMesh("MG1");
	static_cast<CSceneComponent*>(pCom.get())->SetInstance(true);

	pGrass = m_pScene->CreateProtoObj<CGameObject>("Pgrass",
		m_pScene, m_pScene->GetSceneType());
	pCom = pGrass->CreateComponent<CSceneComponent>("Renderer");
	pGrass->SetRootComponent(pCom);
	static_cast<CSceneComponent*>(pCom.get())->SetMesh("Pgrass");
	static_cast<CSceneComponent*>(pCom.get())->SetInstance(true);

	pGrass = m_pScene->CreateProtoObj<CGameObject>("Stump1",
		m_pScene, m_pScene->GetSceneType());
	pCom = pGrass->CreateComponent<CSceneComponent>("Renderer");
	pGrass->SetRootComponent(pCom);
	static_cast<CSceneComponent*>(pCom.get())->SetMesh("Stump1");
	static_cast<CSceneComponent*>(pCom.get())->SetInstance(true);

	pGrass = m_pScene->CreateProtoObj<CGameObject>("Stump2",
		m_pScene, m_pScene->GetSceneType());
	pCom = pGrass->CreateComponent<CSceneComponent>("Renderer");
	pGrass->SetRootComponent(pCom);
	static_cast<CSceneComponent*>(pCom.get())->SetMesh("Stump2");
	static_cast<CSceneComponent*>(pCom.get())->SetInstance(true);

	pGrass = m_pScene->CreateProtoObj<CGameObject>("Vine2",
		m_pScene, m_pScene->GetSceneType());
	pCom = pGrass->CreateComponent<CSceneComponent>("Renderer");
	pGrass->SetRootComponent(pCom);
	static_cast<CSceneComponent*>(pCom.get())->SetMesh("Vine2");
	static_cast<CSceneComponent*>(pCom.get())->SetInstance(true);

	pGrass = m_pScene->CreateProtoObj<CGameObject>("WallChii",
		m_pScene, m_pScene->GetSceneType());
	pCom = pGrass->CreateComponent<CSceneComponent>("Renderer");
	pGrass->SetRootComponent(pCom);
	static_cast<CSceneComponent*>(pCom.get())->SetMesh("WallChii");
	static_cast<CSceneComponent*>(pCom.get())->SetInstance(true);

	pGrass = m_pScene->CreateProtoObj<CGameObject>("Watercoin",
		m_pScene, m_pScene->GetSceneType());
	pCom = pGrass->CreateComponent<CSceneComponent>("Renderer");
	pGrass->SetRootComponent(pCom);
	static_cast<CSceneComponent*>(pCom.get())->SetMesh("Watercoin");
	static_cast<CSceneComponent*>(pCom.get())->SetInstance(true);

	pGrass = m_pScene->CreateProtoObj<CGameObject>("Watercoin2",
		m_pScene, m_pScene->GetSceneType());
	pCom = pGrass->CreateComponent<CSceneComponent>("Renderer");
	pGrass->SetRootComponent(pCom);
	static_cast<CSceneComponent*>(pCom.get())->SetMesh("Watercoin2");
	static_cast<CSceneComponent*>(pCom.get())->SetInstance(true);

	pGrass = m_pScene->CreateProtoObj<CGameObject>("WildFlower",
		m_pScene, m_pScene->GetSceneType());
	pCom = pGrass->CreateComponent<CSceneComponent>("Renderer");
	pGrass->SetRootComponent(pCom);
	static_cast<CSceneComponent*>(pCom.get())->SetMesh("WildFlower");
	static_cast<CSceneComponent*>(pCom.get())->SetInstance(true);


	return true;
}

void CMainMode::Start()
{
	CGameMode::Start();

	CLayer* pAlbedo = m_pScene->FindLayer(ALBEDO_LAYER);
	CLayer* pUILayer = m_pScene->FindLayer(UI_LAYER);

	std::shared_ptr<CGameObject> pPlayer = pAlbedo->FindObj("Player");
	std::shared_ptr<CGameObject> pStaff = pAlbedo->FindObj("Staff");
	std::shared_ptr<CGameObject> pUI = pUILayer->FindObj("UI");
	std::shared_ptr<CGameObject> pInventory = pUILayer->FindObj("Inventory");
	std::shared_ptr<CGameObject> pEquip = pUILayer->FindObj("Equip");
	std::shared_ptr<CGameObject> pButtonInven = pUILayer->FindObj("ButtonInven");
	std::shared_ptr<CGameObject> pButtonEquip = pUILayer->FindObj("ButtonEquip");


	if (pPlayer)
	{
		pPlayer->SetEnable(true);

		static_cast<CPlayer3D*>(pPlayer.get())->SetNameAndHPBar(true);
	}
	if (pStaff)
		pStaff->SetEnable(true);
	if (pUI)
		pUI->SetEnable(true);
	if (pButtonInven)
		pButtonInven->SetEnable(true);
	if (pButtonEquip)
	pButtonEquip->SetEnable(true);

	GET_SINGLE(CLensFlares)->Start();

}

bool CMainMode::LoadSequence(const char* pFilePath)
{
	FILE* pFile = nullptr;

	fpos_t iPos;
	const char* pPath = GET_SINGLE(CPathManager)->FindMultiBytePath(DATA_PATH);

	char strFullPath[MAX_PATH] = {};

	strcpy_s(strFullPath, pPath);

	strcat_s(strFullPath, pFilePath);

	fopen_s(&pFile, strFullPath, "rt");

	if (pFile)
	{
		fseek(pFile, 0L, SEEK_END);
		long iSize = ftell(pFile);
		rewind(pFile);
		char strName[MAX_PATH] = {};

		fgets(strName, MAX_PATH, pFile);

		while (true)
		{
			fgets(strName, MAX_PATH, pFile);

			char* pContext = nullptr;

			char* pResult = strtok_s(strName, ", ", &pContext);

			if (!strcmp(pResult, "") || !strcmp(pContext, ""))
			{
				break;
			}

			char* pLoop = strtok_s(nullptr, ", ", &pContext);

			if (!pLoop)
				break;

			bool bLoop = atoi(pLoop);

			char* pFilePath = strtok_s(nullptr, ", ", &pContext);

			if (!pFilePath)
				break;

			if (pFilePath[strlen(pFilePath) - 1] == '\n')
				pFilePath[strlen(pFilePath) - 1] = 0;

			TCHAR strFilePath[MAX_PATH] = {};
#ifdef UNICODE
			MultiByteToWideChar(CP_ACP, 0, pFilePath, -1, strFilePath, static_cast<int>(strlen(pFilePath)));
#else
			strcpy_s(strFilePath, pContext);
#endif

			GET_SINGLE(CResourceManager)->LoadAnimationSequence(pResult, bLoop, strFilePath);
			fgetpos(pFile, &iPos);

			CScene* pScene = GET_SINGLE(CSceneManager)->GetScene();

			if (pScene)
			{
				CGameMode* pGameMode = pScene->GetGameMode();

				if (pGameMode)
				{
					if (typeid(CLoadingMode3D) == typeid(*pGameMode))
					{
						CLoadingMode3D* pMode = static_cast<CLoadingMode3D*>(pGameMode);

						if (pMode)
						{
							pMode->SetPos(iPos / static_cast<float>(iSize));
						}
					}
				}
			}
		}

		fclose(pFile);
		return true;
	}

	return false;
}

bool CMainMode::LoadObjMap(class CScene* pScene, 
	const TCHAR* pFilePath, const std::string& strPathKey)
{
	CTerrain* pTerrain = CTerrain::GetMainTerrain();

	TCHAR strFullPath[MAX_PATH] = {};

	const TCHAR* pPath = GET_SINGLE(CPathManager)->FindPath(strPathKey);

	if (pPath)
		lstrcpy(strFullPath, pPath);

	lstrcat(strFullPath, pFilePath);

	ScratchImage* pImage = new ScratchImage;

	if (FAILED(LoadFromWICFile(strFullPath, WIC_FLAGS_NONE, nullptr, *pImage)))
		return false;

	uint8_t* pPixels = pImage->GetPixels();
	int iWidth = static_cast<int>(pImage->GetMetadata().width);
	int iHeight = static_cast<int>(pImage->GetMetadata().height);

	for (int i = 0; i < iHeight; i++)
	{
		for (int j = 0; j < iWidth; j++)
		{
			char strNum[MAX_PATH] = {};

			int iIdx = 4 * (i * iWidth + j);

			sprintf_s(strNum, MAX_PATH, "-%d", iIdx / 4 + 1);
			std::shared_ptr<CGameObject> pObj = nullptr;

			char strObj[MAX_PATH] = {};
			char strProto[MAX_PATH] = {};

			switch (static_cast<ROCK_TYPE>(pPixels[iIdx]))
			{
			case ROCK_TYPE::LAKEROCK1:
			{
				sprintf_s(strObj, "LakeRock1");
				sprintf_s(strProto, "LakeRock1");
				strcat_s(strObj, strNum);
			}
			break;
			case ROCK_TYPE::SMALLROCK1:
			{
				sprintf_s(strObj, "SmallRock1");
				sprintf_s(strProto, "SmallRock1");
				strcat_s(strObj, strNum);
			}
			break;
			case ROCK_TYPE::SMALLROCK2:
			{
				sprintf_s(strObj, "SmallRock2");
				sprintf_s(strProto, "SmallRock2");
				strcat_s(strObj, strNum);
			}
			break;
			case ROCK_TYPE::SMALLROCK3:
			{
				sprintf_s(strObj, "SmallRock3");
				sprintf_s(strProto, "SmallRock3");
				strcat_s(strObj, strNum);
			}
			break;
			case ROCK_TYPE::TURTLEROCK1:
			{
				sprintf_s(strObj, "SmallRock3");
				sprintf_s(strProto, "SmallRock3");
				strcat_s(strObj, strNum);
				//sprintf_s(strObj, "TurtleRock1");
				//sprintf_s(strProto, "TurtleRock1");
				//strcat_s(strObj, strNum);
			}
			break;
			case ROCK_TYPE::LAKEROCK2:
			{
				sprintf_s(strObj, "LakeRock2");
				sprintf_s(strProto, "LakeRock2");
				strcat_s(strObj, strNum);
			}
			break;
			case ROCK_TYPE::LAKEROCK3:
			{
				sprintf_s(strObj, "LakeRock3");
				sprintf_s(strProto, "LakeRock3");
				strcat_s(strObj, strNum);
			}
			break;
			case ROCK_TYPE::LAKESTONE1:
			{
				sprintf_s(strObj, "LakeStone1");
				sprintf_s(strProto, "LakeStone1");
				strcat_s(strObj, strNum);
			}
			break;
			case ROCK_TYPE::LAKESTONE2:
			{
				sprintf_s(strObj, "LakeStone2");
				sprintf_s(strProto, "LakeStone2");
				strcat_s(strObj, strNum);
			}
			break;
			case ROCK_TYPE::LAKESTONE3:
			{
				sprintf_s(strObj, "LakeStone3");
				sprintf_s(strProto, "LakeStone3");
				strcat_s(strObj, strNum);
			}
			break;
			case ROCK_TYPE::GOBLINROCK1:
			{
				sprintf_s(strObj, "GoblinRock1");
				sprintf_s(strProto, "GoblinRock1");
				strcat_s(strObj, strNum);
			}
			break;
			case ROCK_TYPE::GOBLINROCK2:
			{
				sprintf_s(strObj, "GoblinRock2");
				sprintf_s(strProto, "GoblinRock2");
				strcat_s(strObj, strNum);
			}
			break;
			case ROCK_TYPE::GOBLINROCK3:
			{
				sprintf_s(strObj, "GoblinRock3");
				sprintf_s(strProto, "GoblinRock3");
				strcat_s(strObj, strNum);
			}
			break;
			case ROCK_TYPE::GOBLINROCK4:
			{
				sprintf_s(strObj, "GoblinRock4");
				sprintf_s(strProto, "GoblinRock4");
				strcat_s(strObj, strNum);
			}
			break;
			case ROCK_TYPE::GOBLINROCK5:
			{
				sprintf_s(strObj, "GoblinRock5");
				sprintf_s(strProto, "GoblinRock5");
				strcat_s(strObj, strNum);
			}
			break;
			case ROCK_TYPE::TURTLEROCK2:
			{
				sprintf_s(strObj, "LakeStone1");
				sprintf_s(strProto, "LakeStone1");
				//sprintf_s(strObj, "TurtleRock2");
				//sprintf_s(strProto, "TurtleRock2");
				strcat_s(strObj, strNum);
			}
			break;
			case ROCK_TYPE::TURTLEROCK3:
			{
				sprintf_s(strObj, "LakeStone2");
				sprintf_s(strProto, "LakeStone2");
				//sprintf_s(strObj, "TurtleRock3");
				//sprintf_s(strProto, "TurtleRock3");
				strcat_s(strObj, strNum);
			}
			break;
			case ROCK_TYPE::TURTLEROCK4:
			{
				sprintf_s(strObj, "LakeStone3");
				sprintf_s(strProto, "LakeStone3");
				//sprintf_s(strObj, "TurtleRock4");
				//sprintf_s(strProto, "TurtleRock4");
				strcat_s(strObj, strNum);
			}
			break;
			}

			switch (static_cast<TREE_TYPE>(pPixels[iIdx + 1]))
			{
			case TREE_TYPE::MANZANITATREE1:
			{
				sprintf_s(strObj, "ManzanitaTree1");
				sprintf_s(strProto, "ManzanitaTree1");
				strcat_s(strObj, strNum);
			}
			break;
			case TREE_TYPE::MANZANITATREE2:
			{
				sprintf_s(strObj, "ManzanitaTree2");
				sprintf_s(strProto, "ManzanitaTree2");
				strcat_s(strObj, strNum);
			}
			break;
			case TREE_TYPE::MANZANITATREE3:
			{
				sprintf_s(strObj, "ManzanitaTree3");
				sprintf_s(strProto, "ManzanitaTree3");
				strcat_s(strObj, strNum);
			}
			break;
			case TREE_TYPE::MADRONETREE1:
			{
				sprintf_s(strObj, "MadroneTree1");
				sprintf_s(strProto, "MadroneTree1");
				strcat_s(strObj, strNum);
			}
			break;
			case TREE_TYPE::MADRONETREE2:
			{
				sprintf_s(strObj, "MadroneTree2");
				sprintf_s(strProto, "MadroneTree2");
				strcat_s(strObj, strNum);
			}
			break;
			case TREE_TYPE::MADRONETREE3:
			{
				sprintf_s(strObj, "MadroneTree3");
				sprintf_s(strProto, "MadroneTree3");
				strcat_s(strObj, strNum);
			}
			break;
			case TREE_TYPE::STUMP1:
			{
				sprintf_s(strObj, "Stump1");
				sprintf_s(strProto, "Stump1");
				strcat_s(strObj, strNum);
			}
			break;
			case TREE_TYPE::STUMP2:
			{
				sprintf_s(strObj, "Stump2");
				sprintf_s(strProto, "Stump2");
				strcat_s(strObj, strNum);
			}
			break;
			}

			switch (static_cast<GRASS_TYPE>(pPixels[iIdx + 2]))
			{
			case GRASS_TYPE::BUSH1:
			{
				sprintf_s(strObj, "Bush1");
				sprintf_s(strProto, "Bush1");
				strcat_s(strObj, strNum);
			}
			break;
			case GRASS_TYPE::VINE1:
			{
				sprintf_s(strObj, "Vine1");
				sprintf_s(strProto, "Vine1");
				strcat_s(strObj, strNum);
			}
			break;
			case GRASS_TYPE::BUSH2:
			{
				sprintf_s(strObj, "Bush2");
				sprintf_s(strProto, "Bush2");
				strcat_s(strObj, strNum);
			}
			break;
			case GRASS_TYPE::BUSH3:
			{
				sprintf_s(strObj, "Bush3");
				sprintf_s(strProto, "Bush3");
				strcat_s(strObj, strNum);
			}
			break;
			case GRASS_TYPE::BUSH4:
			{
				sprintf_s(strObj, "Bush4");
				sprintf_s(strProto, "Bush4");
				strcat_s(strObj, strNum);
			}
			break;
			case GRASS_TYPE::BUSHTREE:
			{
				sprintf_s(strObj, "BushTree");
				sprintf_s(strProto, "BushTree");
				strcat_s(strObj, strNum);
			}
			break;
			case GRASS_TYPE::CATTAIL1:
			{
				sprintf_s(strObj, "Cattail1");
				sprintf_s(strProto, "Cattail1");
				strcat_s(strObj, strNum);
			}
			break;
			case GRASS_TYPE::CATTAIL2:
			{
				sprintf_s(strObj, "Cattail2");
				sprintf_s(strProto, "Cattail2");
				strcat_s(strObj, strNum);
			}
			break;
			case GRASS_TYPE::CROP1:
			{
				sprintf_s(strObj, "Crop1");
				sprintf_s(strProto, "Crop1");
				strcat_s(strObj, strNum);
			}
			break;
			case GRASS_TYPE::CROP2:
			{
				sprintf_s(strObj, "Crop2");
				sprintf_s(strProto, "Crop2");
				strcat_s(strObj, strNum);
			}
			break;
			case GRASS_TYPE::DUCKWEED1:
			{
				sprintf_s(strObj, "Duckweed1");
				sprintf_s(strProto, "Duckweed1");
				strcat_s(strObj, strNum);
			}
			break;
			case GRASS_TYPE::DUCKWEED2:
			{
				sprintf_s(strObj, "Duckweed2");
				sprintf_s(strProto, "Duckweed2");
				strcat_s(strObj, strNum);
			}
			break;
			case GRASS_TYPE::IVY1:
			{
				sprintf_s(strObj, "Ivy1");
				sprintf_s(strProto, "Ivy1");
				strcat_s(strObj, strNum);
			}
			break;
			case GRASS_TYPE::IVY2:
			{
				sprintf_s(strObj, "Ivy2");
				sprintf_s(strProto, "Ivy2");
				strcat_s(strObj, strNum);
			}
			break;
			case GRASS_TYPE::IVY3:
			{
				sprintf_s(strObj, "Ivy3");
				sprintf_s(strProto, "Ivy3");
				strcat_s(strObj, strNum);
			}
			break;
			case GRASS_TYPE::MG1:
			{
				sprintf_s(strObj, "MG1");
				sprintf_s(strProto, "MG1");
				strcat_s(strObj, strNum);
			}
			break;
			case GRASS_TYPE::PGRASS:
			{
				sprintf_s(strObj, "Pgrass");
				sprintf_s(strProto, "Pgrass");
				strcat_s(strObj, strNum);
			}
			break;
			case GRASS_TYPE::VINE2:
			{
				sprintf_s(strObj, "Vine2");
				sprintf_s(strProto, "Vine2");
				strcat_s(strObj, strNum);
			}
			break;
			case GRASS_TYPE::WALLCHII:
			{
				sprintf_s(strObj, "WallChii");
				sprintf_s(strProto, "WallChii");
				strcat_s(strObj, strNum);
			}
			break;
			case GRASS_TYPE::WATERCOIN:
			{
				sprintf_s(strObj, "Watercoin");
				sprintf_s(strProto, "Watercoin");
				strcat_s(strObj, strNum);
			}
			break;
			case GRASS_TYPE::WATERCOIN2:
			{
				sprintf_s(strObj, "Watercoin2");
				sprintf_s(strProto, "Watercoin2");
				strcat_s(strObj, strNum);
			}
			break;
			case GRASS_TYPE::WILDFLOWER:
			{
				sprintf_s(strObj, "WildFlower");
				sprintf_s(strProto, "WildFlower");
				strcat_s(strObj, strNum);
			}
			break;
			}

			pObj = pScene->CreateCloneObj(
				strObj, strProto,
				pScene->FindLayer(ALBEDO_LAYER), pScene->GetSceneType());

			if (pObj)
			{
				float fHeight = pTerrain->GetHeight(Vector3(j * 10.f, 0.f, i * 10.f));
				pObj->SetWorldPos(j * 10.f, fHeight, i * 10.f);

				if (pPixels[iIdx] > 237)
				{
					pObj->SetWorldRotX((rand() % 1800) / 10.f);
					pObj->SetWorldRotY((rand() % 1800) / 10.f);
					pObj->SetWorldRotZ((rand() % 1800) / 10.f);
				}
				else if (pPixels[iIdx + 2] > 233)
				{
					static_cast<CSceneComponent*>(pObj->GetRootComponent().get())->SetShader("EmvGClip");
				}
			}
			else
			{
				ROCK_TYPE eType1 = static_cast<ROCK_TYPE>(pPixels[iIdx]);
				TREE_TYPE eType2 = static_cast<TREE_TYPE>(pPixels[iIdx + 1]);
				GRASS_TYPE eType3 = static_cast<GRASS_TYPE>(pPixels[iIdx + 2]);

				if (eType2 == TREE_TYPE::NONE &&
					eType3 == GRASS_TYPE::NONE)
				{
					continue;
				}
				else
				{
					assert(false);
				}
			}
		}
	}

	delete pImage;

	return true;
}

void CMainMode::Save(FILE* pFile)
{
	CGameMode::Save(pFile);
}

void CMainMode::Load(FILE* pFile)
{
	CGameMode::Load(pFile);

	/*m_pScene->CreateLayer(PARTICLE_LAYER, 7);

	CreateUI();*/
}

void CMainMode::BossSpawn()
{
	std::shared_ptr<CGameObject> pMonster = m_pScene->CreateCloneObj("Cyclops","Cyclops",
		m_pScene->FindLayer(ALBEDO_LAYER), m_pScene->GetSceneType());
	pMonster->SetWorldPos(1300.f, 8000.f, 2450.f);

}
