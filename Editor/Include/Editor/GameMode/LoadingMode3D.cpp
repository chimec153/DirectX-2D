#include "LoadingMode3D.h"
#include "Resource/ResourceManager.h"
#include "PathManager.h"
#include "Resource/ShaderManager.h"
#include "../Object/GeometryGenerator.h"
#include "Resource/Shader.h"
#include "Component/SpriteComponent.h"
#include "Scene/Scene.h"
#include "Sync.h"
#include "Thread//ThreadManager.h"
#include "Thread/Thread.h"
#include "LoadingMode3D.h"
#include "../LoadingThread3D.h"
#include "Component/Mesh2DComponent.h"
#include "Device.h"
#include "Scene/SceneManager.h"
#include "../EditorFlags.h"
#include "../Object/Player3D.h"
#include "MainMode.h"
#include "../../TownLoadingThread.h"
#include "Town.h"

float CLoadingMode3D::m_fPercent = 0.f;

CLoadingMode3D::CLoadingMode3D()	:
	m_pSection(nullptr)
	, m_pBar(nullptr)
	, m_pThread(nullptr)
	, m_eGameModeType(EDITOR_GAME_MODE_TYPE::NONE)
	, m_pMeshThread(nullptr)
	, m_pTextureThread(nullptr)
	, m_fMeshGauge(0.f)
	, m_fTextureGauge(0.f)
	, m_bCreatePlayer(false)
{
	m_iGameModeType = static_cast<int>(EDITOR_GAME_MODE_TYPE::LOADING);
	m_pSection = new CRITICAL_SECTION;

	InitializeCriticalSection(m_pSection);
}

CLoadingMode3D::~CLoadingMode3D()
{
	DeleteCriticalSection(m_pSection);

	delete m_pSection;


	SAFE_RELEASE(m_pThread);
	SAFE_RELEASE(m_pMeshThread);
	SAFE_RELEASE(m_pTextureThread);
}

void CLoadingMode3D::SetPos(float fPercent)
{
	CSync sync(m_pSection);
	m_fPercent = fPercent;
}

void CLoadingMode3D::SetGameModeType(EDITOR_GAME_MODE_TYPE eType)
{
	m_eGameModeType = eType;

	switch (m_eGameModeType)
	{
	case EDITOR_GAME_MODE_TYPE::TOWN:
	{
		m_pThread = GET_SINGLE(CThreadManager)->CreateThread<CTownLoadingThread>("TownLoadingThread");

		if (m_pThread)
		{
			static_cast<CTownLoadingThread*>(m_pThread)->SetLoadingType(LOADING_TYPE::SEQUENCE);
			static_cast<CTownLoadingThread*>(m_pThread)->SetFilePath("Sequences.csv");
			m_pThread->Start();
		}
		m_pMeshThread = GET_SINGLE(CThreadManager)->CreateThread<CTownLoadingThread>("TownLoadingMeshThread");

		if (m_pMeshThread)
		{
			static_cast<CTownLoadingThread*>(m_pMeshThread)->SetLoadingType(LOADING_TYPE::MESH);
			static_cast<CTownLoadingThread*>(m_pMeshThread)->SetFilePath("Town\\Meshs.csv");
			m_pMeshThread->Start();
		}
		m_pTextureThread = GET_SINGLE(CThreadManager)->CreateThread<CTownLoadingThread>("TownLoadingTextureThread");

		if (m_pTextureThread)
		{
			static_cast<CTownLoadingThread*>(m_pTextureThread)->SetLoadingType(LOADING_TYPE::TEXTURE);
			static_cast<CTownLoadingThread*>(m_pTextureThread)->SetFilePath("Town\\Textures.csv");
			m_pTextureThread->Start();
		}


	}
		break;
	case EDITOR_GAME_MODE_TYPE::MAIN:
	{
		m_pThread = GET_SINGLE(CThreadManager)->CreateThread<CLoadingThread3D>("LoadingThread");

		if (m_pThread)
		{
			m_pThread->Start();
		}

		m_pMeshThread = GET_SINGLE(CThreadManager)->CreateThread<CLoadingThread3D>("LoadingMeshThread");

		if (m_pMeshThread)
		{
			m_pMeshThread->SetWork(CMainMode::LoadMesh);
			m_pMeshThread->Start();
		}
	}
		break;
	}
}

void CLoadingMode3D::SetBackTexture(const std::string& strKey)
{
	static_cast<CMesh2DComponent*>(m_pBack.get())->SetTexture(LINK_TYPE::LT_DIF, strKey);
}

bool CLoadingMode3D::Init()
{
	if (!LoadTexture())
		return false;

	Resolution tRS = RESOLUTION;

	std::shared_ptr<CGameObject> pLoadingSequence = m_pScene->CreateObject<CGameObject>("LoadingSequence", m_pScene->FindLayer(ALBEDO_LAYER));

	std::shared_ptr<CComponent> _pLoading = pLoadingSequence->CreateComponent<CSpriteComponent>("LoadingSequence");

	pLoadingSequence->SetRootComponent(_pLoading);

	CSpriteComponent* pCom = static_cast<CSpriteComponent*>(_pLoading.get());

	pCom->SetWorldScale(1024.f, 1024.f, 0.f);
	pCom->AddWorldPosY(-150.f);
	pCom->SetPivot(0.5f, 0.5f, 0.f);
	pCom->AddAni2DInfo("OpeningSequence", "OpeningSequence");
	pCom->SetSceneComponentType(SCENE_COMPONENT_TYPE::SCT_UI);

	std::shared_ptr<CGameObject> pLoadingBar = m_pScene->CreateObject<CGameObject>("LoadingBar", m_pScene->FindLayer(ALBEDO_LAYER));

	/*m_pBar = pLoadingBar->CreateComponent<CSpriteComponent>("LoadingBar");

	pLoadingBar->SetRootComponent(m_pBar);

	CSpriteComponent* _pBar = static_cast<CSpriteComponent*>(m_pBar.get());

	_pBar->SetTexture(LINK_TYPE::LT_DIF, "Start_icon");
	_pBar->SetPivot(0.f, 0.5f, 0.f);
	_pBar->SetWorldPosX(-120.f);
	_pBar->SetWorldPosY(-tRS.iHeight * 2.f / 5.f);
	_pBar->SetSize(Vector2(1024.f, 1024.f));
	_pBar->SetStart(Vector2(274.f, 881.f));
	_pBar->SetEnd(Vector2(514.f, 891.f));
	_pBar->SetWorldScale(240.f, 10.f, 1.f);
	_pBar->SetSceneComponentType(SCENE_COMPONENT_TYPE::SCT_UI);

	std::shared_ptr<CComponent> pFrame = pLoadingBar->CreateComponent<CSpriteComponent>("LoadingBarFrame");

	_pBar->AddChild(pFrame);

	CSpriteComponent* _pFrame = static_cast<CSpriteComponent*>(pFrame.get());

	_pFrame->SetTexture(LINK_TYPE::LT_DIF, "Start_icon");
	_pFrame->SetPivot(0.5f, 0.5f, 0.f);
	_pFrame->SetSize(Vector2(1024.f, 1024.f));
	_pFrame->SetStart(Vector2(552.f, 841.f));
	_pFrame->SetEnd(Vector2(844.f, 857.f));
	_pFrame->SetSceneComponentType(SCENE_COMPONENT_TYPE::SCT_UI);
	_pFrame->SetInheritScale(false);
	_pFrame->AddRelativePos(120.f, 0.f, 0.f); 
	_pFrame->SetWorldScale(292.f, 16.f, 1.f);
	_pFrame->SetZOrder(2);*/

	std::shared_ptr<CGameObject> pBackImage = m_pScene->CreateObject<CGameObject>("Back Image", m_pScene->FindLayer(ALBEDO_LAYER));

	m_pBack = pBackImage->CreateComponent<CMesh2DComponent>("Back Image");

	pBackImage->SetRootComponent(m_pBack);

	CMesh2DComponent* _pBack = static_cast<CMesh2DComponent*>(m_pBack.get());

	_pBack->SetWorldScale(static_cast<float>(tRS.iWidth), static_cast<float>(tRS.iHeight), 0.f);
	_pBack->SetPivot(0.5f, 0.5f, 0.f);
	_pBack->SetSceneComponentType(SCENE_COMPONENT_TYPE::SCT_UI);
	_pBack->SetZOrder(3);

	return true;
}

void CLoadingMode3D::Update(float fTime)
{
	CGameMode::Update(fTime);

	bool bThread = false;
	if (m_pThread)
	{
		if (m_pThread->IsCompleted())
		{
			bThread = true;
		}
	}
	else
	{
		bThread = true;
	}
	bool bTextureThread = false;
	if (m_pTextureThread)
	{
		if (m_pTextureThread->IsCompleted())
		{
			bTextureThread = true;
		}
	}
	else
	{
		bTextureThread = true;
	}

	bool bMeshThread = false;
	if (m_pMeshThread)
	{
		if (m_pMeshThread->IsCompleted())
		{
			bMeshThread = true;
		}
	}
	else
	{
		bMeshThread = true;
	}

	if (bThread && bMeshThread && bTextureThread)
	{
		switch (m_eGameModeType)
		{
		case EDITOR_GAME_MODE_TYPE::MAIN:
		{
			GET_SINGLE(CSceneManager)->CreateNextScene();

			GET_SINGLE(CSceneManager)->SetGameMode<CMainMode>(false);

			CScene* pNextScene = GET_SINGLE(CSceneManager)->GetNextScene();

			CLayer* pLayer = pNextScene->FindLayer(ALBEDO_LAYER);
			CLayer* pNextAlphaLayer = pNextScene->FindLayer(ALPHA_LAYER);

			CScene* pCurrentScene = GET_SINGLE(CSceneManager)->GetScene();

			CLayer* pAlbedo = pCurrentScene->FindLayer(ALBEDO_LAYER);
			CLayer* pAlpha = pCurrentScene->FindLayer(ALPHA_LAYER);

			std::shared_ptr<CGameObject> pPlayer = pAlbedo->FindObj("Player");

			std::shared_ptr<CGameObject> pStaff = pAlbedo->FindObj("Staff");

			std::shared_ptr<CGameObject> pRide = pAlpha->FindObj("BoneDragon");

			pLayer->AddObj(pPlayer);
			pLayer->AddObj(pStaff);
			pNextAlphaLayer->AddObj(pRide);

			CLayer* pNextUILayer = pNextScene->FindLayer(UI_LAYER);
			CLayer* pUILayer = pCurrentScene->FindLayer(UI_LAYER);

			std::shared_ptr<CGameObject> pUI = pUILayer->FindObj("UI");
			std::shared_ptr<CGameObject> pInventory = pUILayer->FindObj("Inventory");
			std::shared_ptr<CGameObject> pEquip = pUILayer->FindObj("Equip");
			std::shared_ptr<CGameObject> pButtonInven = pUILayer->FindObj("ButtonInven");
			std::shared_ptr<CGameObject> pButtonEquip = pUILayer->FindObj("ButtonEquip");
			std::shared_ptr<CGameObject> pMission = pUILayer->FindObj("Mission");

			pNextUILayer->AddObj(pUI);
			pNextUILayer->AddObj(pInventory);
			pNextUILayer->AddObj(pEquip);
			pNextUILayer->AddObj(pButtonInven);
			pNextUILayer->AddObj(pButtonEquip);
			pNextUILayer->AddObj(pMission);

			pPlayer->SetWorldPos(4850.f, 0.f, 250.f);
		}
		break;
		case EDITOR_GAME_MODE_TYPE::TOWN:
		{
			GET_SINGLE(CSceneManager)->CreateNextScene();

			GET_SINGLE(CSceneManager)->SetGameMode<CTown>(false);

			if (m_bCreatePlayer)
			{
				CScene* pScene = GET_SINGLE(CSceneManager)->GetNextScene();

				CGameMode* pGameMode = pScene->GetGameMode();

				static_cast<CTown*>(pGameMode)->CreatePlayer();
			}
		}
			break;
		}
	}
}

bool CLoadingMode3D::LoadTexture()
{
	std::vector<const TCHAR*> vecPath;

	for (int i = 1; i <= 9; i += 2)
	{
		TCHAR* strNum = new TCHAR[MAX_PATH];
		memset(strNum, 0, MAX_PATH * sizeof(TCHAR));
		TCHAR _strNum[MAX_PATH] = {};
		swprintf_s(_strNum, MAX_PATH, L"%d.dds", i);
		wcscpy_s(strNum, MAX_PATH, TEXT("UI_Opening_Sequence_Resource\\Texture2D\\BLUIOpeningSequenceResource_I"));
		wcscat_s(strNum, MAX_PATH, _strNum);

		vecPath.push_back(strNum);
	}
	TCHAR* strNum = new TCHAR[MAX_PATH];
	memset(strNum, 0, MAX_PATH * sizeof(TCHAR));
	wcscpy_s(strNum, MAX_PATH, TEXT("UI_Opening_Sequence_Resource\\Texture2D\\BLUIOpeningSequenceResource_IB.dds"));
	vecPath.push_back(strNum); 
	strNum = new TCHAR[MAX_PATH];
	memset(strNum, 0, MAX_PATH * sizeof(TCHAR));
	wcscpy_s(strNum, MAX_PATH, TEXT("UI_Opening_Sequence_Resource\\Texture2D\\BLUIOpeningSequenceResource_ID.dds"));
	vecPath.push_back(strNum); 
	strNum = new TCHAR[MAX_PATH];
	memset(strNum, 0, MAX_PATH * sizeof(TCHAR));
	wcscpy_s(strNum, MAX_PATH, TEXT("UI_Opening_Sequence_Resource\\Texture2D\\BLUIOpeningSequenceResource_IF.dds"));
	vecPath.push_back(strNum);
	for (int i = 11; i <= 99; i += 2)
	{
		TCHAR* strNum = new TCHAR[MAX_PATH];
		memset(strNum, 0, MAX_PATH * sizeof(TCHAR));
		TCHAR _strNum[MAX_PATH] = {};
		swprintf_s(_strNum, MAX_PATH, L"%d.dds", i);
		wcscpy_s(strNum, MAX_PATH, TEXT("UI_Opening_Sequence_Resource\\Texture2D\\BLUIOpeningSequenceResource_I"));
		wcscat_s(strNum, MAX_PATH, _strNum);

		vecPath.push_back(strNum);
	}
	for (int i = 1; i <= 9; i += 2)
	{
		TCHAR* strNum = new TCHAR[MAX_PATH];
		memset(strNum, 0, MAX_PATH * sizeof(TCHAR));
		TCHAR _strNum[MAX_PATH] = {};
		swprintf_s(_strNum, MAX_PATH, L"A%d.dds", i);
		wcscpy_s(strNum, MAX_PATH, TEXT("UI_Opening_Sequence_Resource\\Texture2D\\BLUIOpeningSequenceResource_I"));
		wcscat_s(strNum, MAX_PATH, _strNum);

		vecPath.push_back(strNum);
	}
	strNum = new TCHAR[MAX_PATH];
	memset(strNum, 0, MAX_PATH * sizeof(TCHAR));
	wcscpy_s(strNum, MAX_PATH, TEXT("UI_Opening_Sequence_Resource\\Texture2D\\BLUIOpeningSequenceResource_IAB.dds"));
	vecPath.push_back(strNum);
	strNum = new TCHAR[MAX_PATH];
	memset(strNum, 0, MAX_PATH * sizeof(TCHAR));
	wcscpy_s(strNum, MAX_PATH, TEXT("UI_Opening_Sequence_Resource\\Texture2D\\BLUIOpeningSequenceResource_IAD.dds"));
	vecPath.push_back(strNum);
	strNum = new TCHAR[MAX_PATH];
	memset(strNum, 0, MAX_PATH * sizeof(TCHAR));
	wcscpy_s(strNum, MAX_PATH, TEXT("UI_Opening_Sequence_Resource\\Texture2D\\BLUIOpeningSequenceResource_IAF.dds"));
	vecPath.push_back(strNum);
	for (int i = 1; i <= 9; i += 2)
	{
		TCHAR* strNum = new TCHAR[MAX_PATH];
		memset(strNum, 0, MAX_PATH * sizeof(TCHAR));
		TCHAR _strNum[MAX_PATH] = {};
		swprintf_s(_strNum, MAX_PATH, L"B%d.dds", i);
		wcscpy_s(strNum, MAX_PATH, TEXT("UI_Opening_Sequence_Resource\\Texture2D\\BLUIOpeningSequenceResource_I"));
		wcscat_s(strNum, MAX_PATH, _strNum);

		vecPath.push_back(strNum);
	}
	strNum = new TCHAR[MAX_PATH];
	memset(strNum, 0, MAX_PATH * sizeof(TCHAR));
	wcscpy_s(strNum, MAX_PATH, TEXT("UI_Opening_Sequence_Resource\\Texture2D\\BLUIOpeningSequenceResource_IBB.dds"));
	vecPath.push_back(strNum);
	strNum = new TCHAR[MAX_PATH];
	memset(strNum, 0, MAX_PATH * sizeof(TCHAR));
	wcscpy_s(strNum, MAX_PATH, TEXT("UI_Opening_Sequence_Resource\\Texture2D\\BLUIOpeningSequenceResource_IBD.dds"));
	vecPath.push_back(strNum);
	strNum = new TCHAR[MAX_PATH];
	memset(strNum, 0, MAX_PATH * sizeof(TCHAR));
	wcscpy_s(strNum, MAX_PATH, TEXT("UI_Opening_Sequence_Resource\\Texture2D\\BLUIOpeningSequenceResource_IBF.dds"));
	vecPath.push_back(strNum);
	for (int i = 1; i <= 9; i += 2)
	{
		TCHAR* strNum = new TCHAR[MAX_PATH];
		memset(strNum, 0, MAX_PATH * sizeof(TCHAR));
		TCHAR _strNum[MAX_PATH] = {};
		swprintf_s(_strNum, MAX_PATH, L"C%d.dds", i);
		wcscpy_s(strNum, MAX_PATH, TEXT("UI_Opening_Sequence_Resource\\Texture2D\\BLUIOpeningSequenceResource_I"));
		wcscat_s(strNum, MAX_PATH, _strNum);

		vecPath.push_back(strNum);
	}
	strNum = new TCHAR[MAX_PATH];
	memset(strNum, 0, MAX_PATH * sizeof(TCHAR));
	wcscpy_s(strNum, MAX_PATH, TEXT("UI_Opening_Sequence_Resource\\Texture2D\\BLUIOpeningSequenceResource_ICB.dds"));
	vecPath.push_back(strNum);
	strNum = new TCHAR[MAX_PATH];
	memset(strNum, 0, MAX_PATH * sizeof(TCHAR));
	wcscpy_s(strNum, MAX_PATH, TEXT("UI_Opening_Sequence_Resource\\Texture2D\\BLUIOpeningSequenceResource_ICD.dds"));
	vecPath.push_back(strNum);
	strNum = new TCHAR[MAX_PATH];
	memset(strNum, 0, MAX_PATH * sizeof(TCHAR));
	wcscpy_s(strNum, MAX_PATH, TEXT("UI_Opening_Sequence_Resource\\Texture2D\\BLUIOpeningSequenceResource_ICF.dds"));
	vecPath.push_back(strNum);
	for (int i = 1; i <= 9; i += 2)
	{
		TCHAR* strNum = new TCHAR[MAX_PATH];
		memset(strNum, 0, MAX_PATH * sizeof(TCHAR));
		TCHAR _strNum[MAX_PATH] = {};
		swprintf_s(_strNum, MAX_PATH, L"D%d.dds", i);
		wcscpy_s(strNum, MAX_PATH, TEXT("UI_Opening_Sequence_Resource\\Texture2D\\BLUIOpeningSequenceResource_I"));
		wcscat_s(strNum, MAX_PATH, _strNum);

		vecPath.push_back(strNum);
	}
	strNum = new TCHAR[MAX_PATH];
	memset(strNum, 0, MAX_PATH * sizeof(TCHAR));
	wcscpy_s(strNum, MAX_PATH, TEXT("UI_Opening_Sequence_Resource\\Texture2D\\BLUIOpeningSequenceResource_IDB.dds"));
	vecPath.push_back(strNum);
	strNum = new TCHAR[MAX_PATH];
	memset(strNum, 0, MAX_PATH * sizeof(TCHAR));
	wcscpy_s(strNum, MAX_PATH, TEXT("UI_Opening_Sequence_Resource\\Texture2D\\BLUIOpeningSequenceResource_IDD.dds"));
	vecPath.push_back(strNum);
	strNum = new TCHAR[MAX_PATH];
	memset(strNum, 0, MAX_PATH * sizeof(TCHAR));
	wcscpy_s(strNum, MAX_PATH, TEXT("UI_Opening_Sequence_Resource\\Texture2D\\BLUIOpeningSequenceResource_IDF.dds"));
	vecPath.push_back(strNum);
	for (int i = 1; i <= 9; i += 2)
	{
		TCHAR* strNum = new TCHAR[MAX_PATH];
		memset(strNum, 0, MAX_PATH * sizeof(TCHAR));
		TCHAR _strNum[MAX_PATH] = {};
		swprintf_s(_strNum, MAX_PATH, L"E%d.dds", i);
		wcscpy_s(strNum, MAX_PATH, TEXT("UI_Opening_Sequence_Resource\\Texture2D\\BLUIOpeningSequenceResource_I"));
		wcscat_s(strNum, MAX_PATH, _strNum);

		vecPath.push_back(strNum);
	}
	strNum = new TCHAR[MAX_PATH];
	memset(strNum, 0, MAX_PATH * sizeof(TCHAR));
	wcscpy_s(strNum, MAX_PATH, TEXT("UI_Opening_Sequence_Resource\\Texture2D\\BLUIOpeningSequenceResource_IEB.dds"));
	vecPath.push_back(strNum);
	strNum = new TCHAR[MAX_PATH];
	memset(strNum, 0, MAX_PATH * sizeof(TCHAR));
	wcscpy_s(strNum, MAX_PATH, TEXT("UI_Opening_Sequence_Resource\\Texture2D\\BLUIOpeningSequenceResource_IED.dds"));
	vecPath.push_back(strNum);
	strNum = new TCHAR[MAX_PATH];
	memset(strNum, 0, MAX_PATH * sizeof(TCHAR));
	wcscpy_s(strNum, MAX_PATH, TEXT("UI_Opening_Sequence_Resource\\Texture2D\\BLUIOpeningSequenceResource_IEF.dds"));
	vecPath.push_back(strNum);
	for (int i = 1; i <= 9; i += 2)
	{
		TCHAR* strNum = new TCHAR[MAX_PATH];
		memset(strNum, 0, MAX_PATH * sizeof(TCHAR));
		TCHAR _strNum[MAX_PATH] = {};
		swprintf_s(_strNum, MAX_PATH, L"F%d.dds", i);
		wcscpy_s(strNum, MAX_PATH, TEXT("UI_Opening_Sequence_Resource\\Texture2D\\BLUIOpeningSequenceResource_I"));
		wcscat_s(strNum, MAX_PATH, _strNum);

		vecPath.push_back(strNum);
	}
	strNum = new TCHAR[MAX_PATH];
	memset(strNum, 0, MAX_PATH * sizeof(TCHAR));
	wcscpy_s(strNum, MAX_PATH, TEXT("UI_Opening_Sequence_Resource\\Texture2D\\BLUIOpeningSequenceResource_IFB.dds"));
	vecPath.push_back(strNum);
	strNum = new TCHAR[MAX_PATH];
	memset(strNum, 0, MAX_PATH * sizeof(TCHAR));
	wcscpy_s(strNum, MAX_PATH, TEXT("UI_Opening_Sequence_Resource\\Texture2D\\BLUIOpeningSequenceResource_IFD.dds"));
	vecPath.push_back(strNum);
	strNum = new TCHAR[MAX_PATH];
	memset(strNum, 0, MAX_PATH * sizeof(TCHAR));
	wcscpy_s(strNum, MAX_PATH, TEXT("UI_Opening_Sequence_Resource\\Texture2D\\BLUIOpeningSequenceResource_IFF.dds"));
	vecPath.push_back(strNum);
	for (int i = 101; i <= 177; i += 2)
	{
		TCHAR* strNum = new TCHAR[MAX_PATH];
		memset(strNum, 0, MAX_PATH * sizeof(TCHAR));
		TCHAR _strNum[MAX_PATH] = {};
		swprintf_s(_strNum, MAX_PATH, L"%d.dds", i);
		wcscpy_s(strNum, MAX_PATH, TEXT("UI_Opening_Sequence_Resource\\Texture2D\\BLUIOpeningSequenceResource_I"));
		wcscat_s(strNum, MAX_PATH, _strNum);

		vecPath.push_back(strNum);
	}
	GET_SINGLE(CResourceManager)->LoadTextureArray("OpeningSequence", vecPath);

	GET_SINGLE(CResourceManager)->CreateAni2DSequence("OpeningSequence", "OpeningSequence", 1.f);

	SAFE_DELETE_VECLIST(vecPath);

	GET_SINGLE(CResourceManager)->LoadTexture("Start_icon", TEXT("UI_Share_Etc\\Texture2D\\32_UI2_Waypoint_Manager_Start_icon.dds"));

	return true;
}
