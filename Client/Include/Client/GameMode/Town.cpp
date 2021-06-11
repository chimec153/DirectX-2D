#include "Town.h"
#include "Component/Terrain.h"
#include "Scene/Scene.h"
#include "Component/Renderer.h"
#include "Resource/ResourceManager.h"
#include "../Object/Player3D.h"
#include "MainMode.h"
#include "Component/Light.h"
#include "Resource/Material.h"
#include "Component/ColliderTerrain.h"
#include "Sound/SoundManager.h"
#include "LensFlares.h"
#include "../Object/Tanara.h"
#include "../Object/Tati.h"
#include "../Object/WoundedElin.h"
#include "PathManager.h"
#include "Resource/ShaderManager.h"
#include "Resource/GraphicShader.h"
#include "../Object/Iseulenia.h"
#include "../Object/UI.h"
#include "../Object/Inventory3D.h"
#include "../Object/Equip.h"
#include "../Object/EquipButton.h"
#include "../Object/InvenButton.h"
#include "../Item.h"
#include "../Object/GoblinSword.h"
#include "../Object/BladePlayer.h"
#include "../Object/Ocean.h"
#include "../Object/Bullet3D.h"
#include "../Object/BlueFlame.h"
#include "Scene/SceneManager.h"
#include "LoadingMode3D.h"
#include "../Object/Ride.h"
#include "../Object/StaffPlayer.h"
#include "../Object/MissionUI.h"
#include "LightManager.h"

CTown::CTown()
{
}

CTown::~CTown()
{
}

bool CTown::Init()
{
	/*GET_SINGLE(CResourceManager)->LoadAnimationSequence(
		"PPlayer_Wait", false,
		L"PPlayer\\Wait.sqc", MESH_PATH, true);*/
	//GET_SINGLE(CResourceManager)->LoadMesh(MESH_TYPE::MT_ANIMATION_MESH,
		//"Elf_MS_ANI",
		//L"BPlayer\\Ani\\Elf_MS_ANI.FBX");
	/*GET_SINGLE(CResourceManager)->LoadMesh(MESH_TYPE::MT_ANIMATION_MESH,
		"AN_PC_Elf_F_STF_AS2",
		L"BPlayer\\test.FBX");
	GET_SINGLE(CResourceManager)->LoadSkeleton("UpperBone", "BPlayer\\test.bne");*/

	std::shared_ptr<CGameObject> _pOcean = m_pScene->CreateObject<COcean>("Ocean", m_pScene->FindLayer(ALPHA_LAYER));


	GET_SINGLE(CResourceManager)->LoadAnimationSequence(
		"Iseul_Idle", true,
		L"Town\\Iseul\\Idle.sqc");
	GET_SINGLE(CResourceManager)->LoadAnimationSequence(
		"Iseul_talk1", true,
		L"Town\\Iseul\\talk1.sqc");
	GET_SINGLE(CResourceManager)->LoadAnimationSequence(
		"Iseul_v_spell", false,
		L"Town\\Iseul\\v_spell.sqc");

	CItem::LoadItemTable("Items.csv");

	LoadTextures("Town\\Textures.csv");
	if (!LoadShader())
		return false;

	LoadMeshs("Town\\Meshs.csv");

	CMainMode::LoadSequence("Sequences.csv");
	

	LoadProtoTypes(m_pScene, "Town\\ProtoTypes.csv");
	LoadObjects(m_pScene, "Town\\Objects.csv");
	LoadCloneObjects(m_pScene, "Town\\Clones.csv");

	GET_SINGLE(CResourceManager)->LoadAnimationSequence(
		"AN_NPC_Tanara_01", true,
		L"Town\\AN_NPC_Tanara_01_Weapon_SP_AS.sqc");

	GET_SINGLE(CResourceManager)->LoadAnimationSequence(
		"AN_NPC_Tati_SP_AS", true,
		L"Town\\AN_NPC_Tati_Stick_President.sqc");

	GET_SINGLE(CResourceManager)->LoadAnimationSequence(
		"Elin_Wounded", true,
		L"Town\\Elin_Wounded_anim.sqc");
	GET_SINGLE(CResourceManager)->LoadAnimationSequence(
		"NPC_Iseulenia", true,
		L"Town\\NPC_Iseulenia_anim.sqc");

	GET_SINGLE(CResourceManager)->CreateSphere("Cube2", 1.f, 20, 20);
	if (!CGameMode::Init())
		return false;
	GET_SINGLE(CResourceManager)->LoadTexture(
		"FX_Trail_012_TEX_PYS",
		L"FX_Resource_Pys_01\\Texture2D\\FX_Trail_012_TEX_PYS.dds");

	//std::shared_ptr<CGameObject> pBladePlayer = m_pScene->CreateObject<CBladePlayer>("Player", m_pScene->FindLayer(ALBEDO_LAYER));

	//pBladePlayer->SetWorldPos(3240.822f, 1200.000f, 7822.507f);

	GET_SINGLE(CResourceManager)->LoadTexture(
		"LV_Elrano_Tile_Floor01_D_KSV",
		L"LV_ElRano_Tile_A\\Texture2D\\LV_Elrano_Tile_Floor01_D_KSV.dds",
		MESH_PATH);
	GET_SINGLE(CResourceManager)->LoadTexture(
		"LV_Elrano_Tile_Floor01_N_KSV",
		L"LV_ElRano_Tile_A\\Texture2D\\LV_Elrano_Tile_Floor01_N_KSV.dds",
		MESH_PATH);
	GET_SINGLE(CResourceManager)->LoadTexture(
		"LV_BasicSky_SKY_HSI",
		L"LV_Common_Sky_A\\Texture2D\\LV_BasicSky_SKY_HSI.dds");

	GET_SINGLE(CSoundManager)->LoadSound("BGM","Tria_Village", true, "Village\\Tria_Village.ogg");
	GET_SINGLE(CSoundManager)->Play("Tria_Village");

	CGameMode::SetDomeTexture2D("LV_BasicSky_SKY_HSI");
	SetDomeShader("Cube2");
	SetDomeDiffuseColor(Vector4(0.027f, 0.023f, 0.18f, 1.f));

	std::shared_ptr<CGameObject> pObj = m_pScene->CreateObject<CGameObject>("TerrainObj", 
		m_pScene->FindLayer(TERRAIN_LAYER));

	std::shared_ptr<CComponent> pTerrain = pObj->CreateComponent<CTerrain>("TerrainTown");

	pObj->SetRootComponent(pTerrain);

	static_cast<CTerrain*>(pTerrain.get())->BuildHeightMap("Terrain", 512, 512, 255.f, L"TownHeightMap2.bmp");
	static_cast<CTerrain*>(pTerrain.get())->SetTiling(60.f, 60.f);
	static_cast<CTerrain*>(pTerrain.get())->SetWorldScale(10.f, 10.f, 20.f);
	static_cast<CTerrain*>(pTerrain.get())->SetMaterial("Color");
	static_cast<CTerrain*>(pTerrain.get())->SetEditorMode(true);
	static_cast<CTerrain*>(pTerrain.get())->SetEditorMode(false);
	static_cast<CTerrain*>(pTerrain.get())->SetBrushTexture("Brush1", L"brush1.bmp");
	static_cast<CTerrain*>(pTerrain.get())->SetBrushTexture("Brush4x4", L"brush4x4.bmp");
	static_cast<CTerrain*>(pTerrain.get())->SetBrushTexture("Brush8x8", L"brush8x8.bmp");
	static_cast<CTerrain*>(pTerrain.get())->SetBrushTexture("circle_brush16x16", L"circle_brush16x16.bmp");
	static_cast<CTerrain*>(pTerrain.get())->SetBrushTexture("CircleBrush64", L"CircleBrush64.png");
	static_cast<CTerrain*>(pTerrain.get())->SetBrushTexture("Brush16x16", L"brush16x16.bmp");
	static_cast<CTerrain*>(pTerrain.get())->SetBrushTexture("Brush32x32", L"brush32x32.bmp");
	static_cast<CTerrain*>(pTerrain.get())->SetBrushTexture("Brush64x64", L"brush64x64.bmp");
	static_cast<CTerrain*>(pTerrain.get())->SetBrushTexture("Brush128x128", L"brush128x128.bmp");
	static_cast<CTerrain*>(pTerrain.get())->SetShader("Terrain");
	CTerrain::SetMainTerrain("TerrainTown");

	std::shared_ptr<CMaterial> pMtrl = static_cast<CSceneComponent*>(pTerrain.get())->GetMaterial();

	if (pMtrl)
	{
		pMtrl->SetDecal(true);
		pMtrl->SetTexture(LINK_TYPE::LT_DIF, "LV_Elrano_Tile_Floor01_D_KSV");
		pMtrl->SetTexture(LINK_TYPE::LT_NOR, "LV_Elrano_Tile_Floor01_N_KSV");
		pMtrl->SetSpecularColor(0.3f, 0.3f, 0.3f, 1.f);
	}

	std::shared_ptr<CComponent> pColliderTerrain = pObj->CreateComponent<CColliderTerrain>("TerrainBody");

	std::vector<Vector3> vecPos = static_cast<CTerrain*>(pTerrain.get())->GetVecPos();

	static_cast<CColliderTerrain*>(pColliderTerrain.get())->SetInfo(vecPos, 512, 512);

	static_cast<CTerrain*>(pTerrain.get())->AddChild(pColliderTerrain);
	CTerrain* _pTerrain = static_cast<CTerrain*>(pTerrain.get());
	static_cast<CColliderTerrain*>(pColliderTerrain.get())->SetCallBack(
		COLLISION_STATE::CS_INIT, _pTerrain, &CTerrain::ColInit);
	static_cast<CColliderTerrain*>(pColliderTerrain.get())->SetCallBack(
		COLLISION_STATE::CS_STAY, _pTerrain, &CTerrain::ColStay);
	static_cast<CColliderTerrain*>(pColliderTerrain.get())->SetCallBack(
		COLLISION_STATE::CS_LAST, _pTerrain, &CTerrain::ColLast);


	pObj = m_pScene->CreateObject<CGameObject>("MassiveLod09",
		m_pScene->FindLayer(ALBEDO_LAYER));

	std::shared_ptr<CComponent> pRenderer = pObj->CreateComponent<CSceneComponent>("MassiveLOD09");

	pObj->SetRootComponent(pRenderer);
	pObj->SetWorldPos(2600.f, 355.000f + 1200.f, 5500.f);
	pObj->SetWorldScale(0.5f, 0.5f, 0.5f);
	static_cast<CSceneComponent*>(pRenderer.get())->SetMesh("LV_Elrano_MassiveLOD09_Town01_SM_KWY");
	static_cast<CSceneComponent*>(pRenderer.get())->GetMaterial()->SetEnableBump(false);
	static_cast<CSceneComponent*>(pRenderer.get())->SetShader("EmvGClip");
	static_cast<CSceneComponent*>(pRenderer.get())->AddRenderState(CULL_NONE);


	pObj = m_pScene->CreateObject<CGameObject>("Light", m_pScene->FindLayer(ALBEDO_LAYER));

	std::shared_ptr<CComponent> pLight = pObj->CreateComponent<CLight>("Light");

	pObj->SetRootComponent(pLight);

	static_cast<CLight*>(pLight.get())->SetWorldRotX(135.f);
	static_cast<CLight*>(pLight.get())->SetAmbColor(1.f, 1.f, 1.f, 1.f);
	static_cast<CLight*>(pLight.get())->SetFrustum(4096, 4096, 0.f, 5000.f);
	static_cast<CLight*>(pLight.get())->SetWorldPos(2500.f, 2500.f, 7000.f);

	GET_SINGLE(CLensFlares)->SetSunPos(Vector3(2500.f, 3700.f, 10000.f));

	CScene* pScene = GET_SINGLE(CSceneManager)->GetScene();

	std::shared_ptr<CGameObject> pPlayer = pScene->FindLayer(ALBEDO_LAYER)->FindObj("Player");
	if (!pPlayer)
	{
		pPlayer = pScene->FindLayer(ALPHA_LAYER)->FindObj("Player");

		if (!pPlayer)
		{
			//CreatePlayer();
		}
	}

	m_pScene->CreateObject<CTati>("Tati", m_pScene->FindLayer(ALBEDO_LAYER))->AddWorldPosY(1200.f);

	m_pScene->CreateObject<CTanara>("Tanara", m_pScene->FindLayer(ALBEDO_LAYER))->AddWorldPosY(1200.f);

	m_pScene->CreateObject<CWoundedElin>("Elin", m_pScene->FindLayer(ALPHA_LAYER))->AddWorldPosY(1200.f);

	m_pScene->CreateObject<CIseulenia>("Iseulenia", m_pScene->FindLayer(ALPHA_LAYER))->AddWorldPosY(1200.f);

	return true;
}

void CTown::Start()
{
	CGameMode::Start();

	GET_SINGLE(CLensFlares)->Start();

	CLight* pLight = GET_SINGLE(CLightManager)->GetMainLight();

	pLight->SetWorldRotY(180.f);
	pLight->SetWorldRotX(45.f);

	std::shared_ptr<CGameObject> pPlayer = m_pScene->FindLayer(ALBEDO_LAYER)->FindObj("Player");

	static_cast<CPlayer3D*>(pPlayer.get())->SetNameAndHPBar(false);
}

bool CTown::CreateProtoType()
{
	m_pScene->CreateProtoObj<CBullet3D>("Bullet", m_pScene, m_pScene->GetSceneType());

	m_pScene->CreateProtoObj<CBlueFlame>("BlueBullet", m_pScene, m_pScene->GetSceneType());

	return true;
}


bool CTown::CreateUI()
{// 카메라 쉐이크
	m_pScene->CreateObject<CUI>("UI", m_pScene->FindLayer(UI_LAYER));
	std::shared_ptr<CGameObject> pInven = m_pScene->CreateObject<CInventory3D>("Inventory", m_pScene->FindLayer(UI_LAYER));
	pInven->SetEnable(false);
	std::shared_ptr<CGameObject> pEquip = m_pScene->CreateObject<CEquip>("Equip", m_pScene->FindLayer(UI_LAYER));
	pEquip->SetEnable(false);
	m_pScene->CreateObject<CInvenButton>("ButtonInven", m_pScene->FindLayer(UI_LAYER));
	m_pScene->CreateObject<CEquipButton>("ButtonEquip", m_pScene->FindLayer(UI_LAYER));
	std::shared_ptr<CGameObject> pMission = m_pScene->CreateObject<CMissionUI>("Mission", m_pScene->FindLayer(UI_LAYER));
	pMission->SetEnable(false);

	return true;
}

bool CTown::LoadShader()
{

	return true;
}

void CTown::CreatePlayer()
{
	CreateUI();
	std::shared_ptr<CGameObject> pPlayer = m_pScene->CreateObject<CStaffPlayer>("Player", m_pScene->FindLayer(ALBEDO_LAYER));
	
	pPlayer->SetWorldPos(2640.822f, 120.000f, 7422.507f);

	std::shared_ptr<CGameObject> pRide = m_pScene->CreateObject<CRide>("BoneDragon", m_pScene->FindLayer(ALPHA_LAYER));
	pRide->SetEnable(false);
}

void CTown::LoadMeshs(const char* pFilePath, const std::string& strKey)
{
	fpos_t iPos;

	char strFullPath[MAX_PATH] = {};

	const char* pPath = GET_SINGLE(CPathManager)->FindMultiBytePath(strKey);

	if (pPath)
		strcpy_s(strFullPath, pPath);
	
	strcat_s(strFullPath, pFilePath);

	FILE* pFile = nullptr;

	fopen_s(&pFile, strFullPath, "rt");

	if (pFile)
	{
		fseek(pFile, 0L, SEEK_END);
	 	long lSize = ftell(pFile);
		rewind(pFile);

		char strText[MAX_PATH] = {};

		fgets(strText, MAX_PATH, pFile);
		
		while (true)
		{
			fgets(strText, MAX_PATH, pFile);

			if (!strcmp(strText, ""))
				break;

			char* pContext = nullptr;
			char* pResult = strtok_s(strText, ",", &pContext);

			if (!pResult)
				break;

			int iType = atoi(pResult);

			char* pName = strtok_s(nullptr, ",", &pContext);
			
			if (!pName)
				break;

			char* pPath = strtok_s(nullptr, ",", &pContext);

			TCHAR strFilePath[MAX_PATH] = {};

			if (!pPath)
				break;
#ifdef UNICODE
			MultiByteToWideChar(CP_ACP, 0, pPath, -1, strFilePath, static_cast<int>(strlen(pPath)));
#else
			strcpy_s(strFilePath, MAX_PATH, pPath);
#endif

			if (!GET_SINGLE(CResourceManager)->LoadMesh(static_cast<MESH_TYPE>(iType), pName, strFilePath))
			{
				OutputDebugStringA("Mesh Load Failed. Name: ");
				OutputDebugStringA(pName);
				OutputDebugStringA("\n");
			}

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
							pMode->SetMeshGauge(iPos / static_cast<float>(lSize));
						}
					}
				}
			}
		}

		fclose(pFile);
	}
}

void CTown::LoadTextures(const char* pFilePath, const std::string& strKey)
{
	fpos_t iPos;

	char strFullPath[MAX_PATH] = {};

	const char* pPath = GET_SINGLE(CPathManager)->FindMultiBytePath(strKey);

	if (pPath)
		strcpy_s(strFullPath, pPath);

	strcat_s(strFullPath, pFilePath);

	FILE* pFile = nullptr;

	fopen_s(&pFile, strFullPath, "rt");

	if (pFile)
	{
		fseek(pFile, 0L, SEEK_END);
		long lSize = ftell(pFile);
		rewind(pFile);
		char strText[MAX_PATH] = {};

		fgets(strText, MAX_PATH, pFile);

		while (true)
		{
			fgets(strText, MAX_PATH, pFile);

			if (!strcmp(strText, ""))
				break;

			char* pContext = nullptr;
			char* pName = strtok_s(strText, ",", &pContext);

			if (!pName)
				break;

			char* strPath = strtok_s(nullptr, ",", &pContext);

			if (!strPath)
				break;

			if(strPath[strlen(strPath) - 1]== '\n')
				strPath[strlen(strPath) - 1] = 0;

			TCHAR strTexturePath[MAX_PATH] = {};

#ifdef UNICODE
			MultiByteToWideChar(CP_ACP, 0, strPath, -1, strTexturePath, static_cast<int>(strlen(strPath)));
#else
			strcpy_s(strTexturePath, MAX_PATH, strPath);
#endif
#ifdef _DEBUG
			if (!GET_SINGLE(CResourceManager)->LoadTexture(pName, strTexturePath))
			{
				OutputDebugStringA("Texture Load Failed. Name: ");
				OutputDebugStringA(pName);
				OutputDebugStringA("\n");
			}
#else
			GET_SINGLE(CResourceManager)->LoadTexture(pName, strTexturePath);
#endif
			fgetpos(pFile, &iPos);

			CScene* pScene = GET_SINGLE(CSceneManager)->GetScene();

			if (pScene)
			{
				CGameMode* pGameMode = pScene->GetGameMode();
				if (pGameMode)
				{
					if (typeid(*pGameMode) == typeid(CLoadingMode3D))
					{
						static_cast<CLoadingMode3D*>(pGameMode)->SetTextureGauge(iPos / static_cast<float>(lSize));
					}
				}
			}
		}

		fclose(pFile);
	}
}

void CTown::LoadProtoTypes(class CScene* pScene, const char* pFilePath, const std::string& strKey)
{
	char strFullPath[MAX_PATH] = {};

	const char* pPath = GET_SINGLE(CPathManager)->FindMultiBytePath(strKey);

	if (pPath)
		strcpy_s(strFullPath, pPath);

	strcat_s(strFullPath, pFilePath);

	FILE* pFile = nullptr;

	fopen_s(&pFile, strFullPath, "rt");

	if (pFile)
	{
		char strText[MAX_PATH] = {};

		fgets(strText, MAX_PATH, pFile);

		while (true)
		{
			fgets(strText, MAX_PATH, pFile);

			char* pContext = nullptr;
			char* pResult = strtok_s(strText, ", ", &pContext);

			int iType = atoi(pResult);

			char* pName = strtok_s(nullptr, ", ", &pContext);

			if (!pName)
				break;

			std::shared_ptr<CGameObject> pObj = nullptr;

			switch (static_cast<OBJECT_CLASS_TYPE>(iType))
			{
			case OBJECT_CLASS_TYPE::OBJECT:
			{
				pObj = pScene->CreateProtoObj<CGameObject>(pName, pScene, pScene->GetSceneType());
			}
				break;
			case OBJECT_CLASS_TYPE::OCT_PLAYER:
				break;
			case OBJECT_CLASS_TYPE::OCT_BULLET:
				break;
			case OBJECT_CLASS_TYPE::OCT_MINION:
				break;
			case OBJECT_CLASS_TYPE::MINION_BULLET:
				break;
			case OBJECT_CLASS_TYPE::TILEOBJ:
				break;
			case OBJECT_CLASS_TYPE::EFFECT:
				break;
			case OBJECT_CLASS_TYPE::MARRY:
				break;
			case OBJECT_CLASS_TYPE::RIDE_WINDOW:
				break;
			case OBJECT_CLASS_TYPE::PATH_WINDOW:
				break;
			case OBJECT_CLASS_TYPE::CONSTRUCTION_WINDOW:
				break;
			case OBJECT_CLASS_TYPE::BAR:
				break;
			case OBJECT_CLASS_TYPE::CHECKBOX:
				break;
			case OBJECT_CLASS_TYPE::CLIENTBUTTON:
				break;
			case OBJECT_CLASS_TYPE::RADIOBOX:
				break;
			case OBJECT_CLASS_TYPE::SLIDERCONTROL:
				break;
			case OBJECT_CLASS_TYPE::UIPANEL:
				break;
			}

			std::shared_ptr<CComponent> pCom = pObj->CreateComponent<CSceneComponent>("Renderer");

			pObj->SetRootComponent(pCom);

			char* strMesh = strtok_s(nullptr, ", ", &pContext);

			if (!strMesh)
				break;

			static_cast<CSceneComponent*>(pCom.get())->SetMesh(strMesh);

			char* strState = strtok_s(nullptr, ", ", &pContext);

			if (!strState)
				break;

			if (strcmp(strState,""))
			{
				static_cast<CSceneComponent*>(pCom.get())->AddRenderState(strState);
			}
			strState = strtok_s(nullptr, ", ", &pContext);
			if (!strState)
				break;

			if (strcmp(strState, ""))
			{
				static_cast<CSceneComponent*>(pCom.get())->AddRenderState(strState);
			}
			strState = strtok_s(nullptr, ", ", &pContext);
			if (!strState)
				break;

			if (strcmp(strState, ""))
			{
				static_cast<CSceneComponent*>(pCom.get())->AddRenderState(strState);
			}

			pResult = strtok_s(nullptr, ", ", &pContext);

			if (!pResult)
				break;

			int iInstance = atoi(pResult);

			static_cast<CSceneComponent*>(pCom.get())->SetInstance(iInstance);
			
		}


		fclose(pFile);
	}
}

void CTown::LoadCloneObjects(CScene* pScene, const char* pFilePath, const std::string& strKey)
{
	char strFullPath[MAX_PATH] = {};

	const char* pPath = GET_SINGLE(CPathManager)->FindMultiBytePath(strKey);

	if (pPath)
		strcpy_s(strFullPath, pPath);

	strcat_s(strFullPath, pFilePath);

	FILE* pFile = nullptr;

	fopen_s(&pFile, strFullPath, "rt");

	if (pFile)
	{
		char strText[MAX_PATH] = {};

		fgets(strText, MAX_PATH, pFile);

		while (true)
		{
			fgets(strText, MAX_PATH, pFile);

			char* pContext = nullptr;

			char* pName = strtok_s(strText, ", ", &pContext);

			if (!pName)
				break;

			char* pProto = strtok_s(nullptr, ", ", &pContext);

			if (!pProto)
				break;

			char* pLayer = strtok_s(nullptr, ", ", &pContext);

			if (!pLayer)
				break;

			std::shared_ptr<CGameObject> pObj = pScene->CreateCloneObj(
				pName, pProto, pScene->FindLayer(pLayer), pScene->GetSceneType());

			char* pX = strtok_s(nullptr, ", ", &pContext);

			if (!pX)
				break;

			float fX = static_cast<float>(atof(pX));

			char* pY = strtok_s(nullptr, ", ", &pContext);

			if (!pY)
				break;

			float fY = static_cast<float>(atof(pY));

			char* pZ = strtok_s(nullptr, ", ", &pContext);

			if (!pZ)
				break;

			float fZ = static_cast<float>(atof(pZ));

			pObj->SetWorldPos(fX, fY, fZ);

			char* pRotX = strtok_s(nullptr, ", ", &pContext);
			if (!pRotX)
				break;
			float fRotX = static_cast<float>(atof(pRotX));
			char* pRotY = strtok_s(nullptr, ", ", &pContext);
			if (!pRotY)
				break;
			float fRotY = static_cast<float>(atof(pRotY));
			char* pRotZ = strtok_s(nullptr, ", ", &pContext);
			if (!pRotZ)
				break;
			float fRotZ = static_cast<float>(atof(pRotZ));

			pObj->SetWorldRot(fRotX, fRotY, fRotZ);
		}

		fclose(pFile);
	}
}

void CTown::LoadObjects(CScene* pScene, const char* pFilePath, const std::string& strKey)
{
	char strFullPath[MAX_PATH] = {};

	const char* pPath = GET_SINGLE(CPathManager)->FindMultiBytePath(strKey);

	if (pPath)
		strcpy_s(strFullPath, pPath);

	strcat_s(strFullPath, pFilePath);

	FILE* pFile = nullptr;

	fopen_s(&pFile, strFullPath, "rt");

	if (pFile)
	{
		char strText[MAX_PATH] = {};

		fgets(strText, MAX_PATH, pFile);

		while (true)
		{
			fgets(strText, MAX_PATH, pFile);

			char* pContext = nullptr;

			char* pType = strtok_s(strText, ", ", &pContext);

			if (!pType)
				break;
			
			int iType = atoi(pType);

			char* pName = strtok_s(nullptr, ", ", &pContext);
			if (!pName)
				break;
			char* pLayer = strtok_s(nullptr, ", ", &pContext);
			if (!pLayer)
				break;
			char* pMesh = strtok_s(nullptr, ", ", &pContext);
			if (!pMesh)
				break;
			char* pState1 = strtok_s(nullptr, ", ", &pContext);
			if (!pState1)
				break;
			char* pX = strtok_s(nullptr, ", ", &pContext);
			if (!pX)
				break;
			char* pY = strtok_s(nullptr, ", ", &pContext);
			if (!pY)
				break;
			char* pZ = strtok_s(nullptr, ", ", &pContext);
			if (!pZ)
				break;
			char* pRotX = strtok_s(nullptr, ", ", &pContext);
			if (!pRotX)
				break;
			char* pRotY = strtok_s(nullptr, ", ", &pContext);
			if (!pRotY)
				break;
			char* pRotZ = strtok_s(nullptr, ", ", &pContext);
			if (!pRotZ)
				break;

			float fX = static_cast<float>(atof(pX));
			float fY = static_cast<float>(atof(pY));
			float fZ = static_cast<float>(atof(pZ));
			float fRotX = static_cast<float>(atof(pRotX));
			float fRotY = static_cast<float>(atof(pRotY));
			float fRotZ = static_cast<float>(atof(pRotZ));

			std::shared_ptr<CGameObject> pObj;

			switch (static_cast<EDITOR_OBJECT_TYPE>(iType))
			{
			case EDITOR_OBJECT_TYPE::NONE:
				pObj = pScene->CreateObject<CGameObject>(
					pName,
					pScene->FindLayer(pLayer));
				break;
			case EDITOR_OBJECT_TYPE::PLAYER:
				break;
			case EDITOR_OBJECT_TYPE::BULLET:
				break;
			case EDITOR_OBJECT_TYPE::MONSTER:
				break;
			case EDITOR_OBJECT_TYPE::TREE:
				break;
			case EDITOR_OBJECT_TYPE::ROCK:
				break;
			case EDITOR_OBJECT_TYPE::EQUIP:
				break;
			case EDITOR_OBJECT_TYPE::EQUIPBUTTON:
				break;
			case EDITOR_OBJECT_TYPE::INVENTORYBUTTON:
				break;
			case EDITOR_OBJECT_TYPE::INVENTORY:
				break;
			case EDITOR_OBJECT_TYPE::TARGETUI:
				break;
			case EDITOR_OBJECT_TYPE::UI:
				break;
			}

			std::shared_ptr<CComponent> pCom = pObj->CreateComponent<CSceneComponent>("Renderer");
			pObj->SetRootComponent(pCom);
			static_cast<CSceneComponent*>(pCom.get())->SetMesh(pMesh);
			static_cast<CSceneComponent*>(pCom.get())->AddRenderState(pState1);
			pObj->SetWorldPos(fX, fY, fZ);
			pObj->SetWorldRot(fRotX, fRotY, fRotZ);
		}

		fclose(pFile);
	}
}
