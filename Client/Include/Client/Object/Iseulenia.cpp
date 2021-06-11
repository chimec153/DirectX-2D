#include "Iseulenia.h"
#include "Component/Renderer.h"
#include "Resource/Animation.h"
#include "Component/ColliderSphere.h"
#include "UI/UFont.h"
#include "Engine.h"
#include "DungeonUI.h"
#include "Scene/Scene.h"
#include "Component/Camera.h"
#include "CameraManager.h"
#include "../GameMode/LoadingMode3D.h"
#include "Player3D.h"
#include "Scene/SceneManager.h"
#include "Sound/SoundManager.h"

CIseulenia::CIseulenia()	:
	m_pPrevCam(nullptr)
{
}

CIseulenia::CIseulenia(const CIseulenia& npc) :
	CNPC(npc)
	, m_pPrevCam(nullptr)
{
}

CIseulenia::~CIseulenia()
{
}

bool CIseulenia::Init()
{
	GET_SINGLE(CSoundManager)->LoadSound("Effect", "PoporiF_Y5_01", false,
		"Village\\PoporiF_Y5_01.ogg");
	GET_SINGLE(CSoundManager)->LoadSound("Effect", "PoporiF_Y5_09", false,
		"Village\\PoporiF_Y5_09.ogg");



	if (!CNPC::Init())
		return false;

	std::shared_ptr<CComponent> pRenderer = CreateComponent<CSceneComponent>("Renderer");

	SetRootComponent(pRenderer);
	SetShadowAll(true);
	static_cast<CSceneComponent*>(pRenderer.get())->SetMesh("NPC_Iseulenia");
	static_cast<CSceneComponent*>(pRenderer.get())->SetShader("Alpha");
	static_cast<CSceneComponent*>(pRenderer.get())->SetShader("Alpha", 0, 1);
	static_cast<CSceneComponent*>(pRenderer.get())->SetShader("Alpha", 0, 2);
	static_cast<CSceneComponent*>(pRenderer.get())->SetShader("Alpha", 0, 3);
	static_cast<CSceneComponent*>(pRenderer.get())->SetShader("Alpha", 0, 4);
	static_cast<CSceneComponent*>(pRenderer.get())->SetShader("Alpha", 0, 5);
	static_cast<CSceneComponent*>(pRenderer.get())->SetShader("Alpha", 0, 6);
	static_cast<CSceneComponent*>(pRenderer.get())->AddRenderState(CULL_NONE);
	static_cast<CSceneComponent*>(pRenderer.get())->AddRenderState(CULL_NONE, 0, 1);
	static_cast<CSceneComponent*>(pRenderer.get())->AddRenderState(CULL_NONE, 0, 2);
	static_cast<CSceneComponent*>(pRenderer.get())->AddRenderState(CULL_NONE, 0, 3);
	static_cast<CSceneComponent*>(pRenderer.get())->AddRenderState(CULL_NONE, 0, 4);
	static_cast<CSceneComponent*>(pRenderer.get())->AddRenderState(CULL_NONE, 0, 5);
	static_cast<CSceneComponent*>(pRenderer.get())->AddRenderState(CULL_NONE, 0, 6);
	static_cast<CSceneComponent*>(pRenderer.get())->SetRelativeRotY(180.f);

	m_pAni = CAnimation::CreateAnimation<CAnimation>("IseuleniaAni", "IseuleniaBone", "Town\\NPC_Iseulenia.bne");

	m_pAni->SetSkeleton("IseuleniaBone");
	m_pAni->AddAnimationSequence("Idle", "NPC_Iseulenia");

	static_cast<CSceneComponent*>(pRenderer.get())->SetAnimation(m_pAni);

	SetWorldPos(3536.892f, -1.f, 4359.362f);
	SetWorldRotY(180.f);

	std::shared_ptr<CComponent> pCollider =  CreateComponent<CColliderSphere>("IseulBody");

	static_cast<CSceneComponent*>(pRenderer.get())->AddChild(pCollider);

	static_cast<CColliderSphere*>(pCollider.get())->SetRadius(12.5f);
	static_cast<CColliderSphere*>(pCollider.get())->SetCallBack<CNPC>(COLLISION_STATE::CS_INIT, this, &CNPC::CollisionInit);
	static_cast<CColliderSphere*>(pCollider.get())->SetCallBack<CNPC>(COLLISION_STATE::CS_STAY, this, &CNPC::CollisionStay);
	static_cast<CColliderSphere*>(pCollider.get())->SetCallBack<CNPC>(COLLISION_STATE::CS_END, this, &CNPC::CollisionEnd);
	static_cast<CColliderSphere*>(pCollider.get())->AddRelativePosY(24.5f);

	m_pDungeonUI = m_pScene->CreateObject<CDungeonUI>("DungeonUI", m_pScene->FindLayer(UI_LAYER));

	m_pDungeonUI->SetEnable(false);

	m_pCam = CreateComponent<CCamera>("IseulCamera");

	static_cast<CSceneComponent*>(pRenderer.get())->AddChild(m_pCam);
	
	static_cast<CCamera*>(m_pCam.get())->SetInheritRotX(false);
	static_cast<CCamera*>(m_pCam.get())->SetInheritRotY(false);
	static_cast<CCamera*>(m_pCam.get())->SetInheritRotZ(false);
	static_cast<CCamera*>(m_pCam.get())->AddRelativePos(15.f, 27.f, 2.f);
	static_cast<CCamera*>(m_pCam.get())->SetWorldRotY(120.f);
	CSceneComponent* _pRenderer= static_cast<CSceneComponent*>(pRenderer.get());

	m_pName = CreateComponent<CUFont>("NameFont");

	static_cast<CSceneComponent*>(pRenderer.get())->AddChild(m_pName);

	static_cast<CUFont*>(m_pName.get())->SetRelativePosY(GetMeshSize().y * 1.1f + 10.f);
	static_cast<CUFont*>(m_pName.get())->SetRect(-100.f, 0.f, 100.f, 0.f);
	static_cast<CUFont*>(m_pName.get())->SetText("던전 입장 NPC");
	static_cast<CUFont*>(m_pName.get())->Set3D();
	static_cast<CUFont*>(m_pName.get())->SetShadowOffset(Vector3(2.f, -2.f, 0.f));
	static_cast<CUFont*>(m_pName.get())->SetShadowColor(0.f, 0.f, 0.f, 1.f);

	return true;
}

void CIseulenia::Start()
{
	CNPC::Start();
}

void CIseulenia::Update(float fTime)
{
	CNPC::Update(fTime);
}

void CIseulenia::PostUpdate(float fTime)
{
	CNPC::PostUpdate(fTime);
}

void CIseulenia::Collision(float fTime)
{
	CNPC::Collision(fTime);
}

void CIseulenia::PreRender(float fTime)
{
	CNPC::PreRender(fTime);
}

void CIseulenia::Render(float fTime)
{
	CNPC::Render(fTime);
}

void CIseulenia::PostRender(float fTime)
{
	CNPC::PostRender(fTime);
}

std::shared_ptr<CGameObject> CIseulenia::Clone()
{
	return std::shared_ptr<CGameObject>(new CIseulenia(*this));
}

void CIseulenia::Save(FILE* pFile)
{
	CNPC::Save(pFile);
}

void CIseulenia::Load(FILE* pFile)
{
	CNPC::Load(pFile);
}

void CIseulenia::NPCWork()
{
	m_pDungeonUI->SetEnable(!m_pDungeonUI->IsEnable());

	if (m_pDungeonUI->IsEnable())
	{
		m_pPrevCam = GET_SINGLE(CCameraManager)->GetMainCam();

		GET_SINGLE(CCameraManager)->SetMainCam("IseulCamera");

		GET_SINGLE(CSoundManager)->Play("PoporiF_Y5_01");
	}

	else
	{
		GET_SINGLE(CCameraManager)->SetMainCam(m_pPrevCam);
		GET_SINGLE(CSoundManager)->Play("PoporiF_Y5_09");
	}

}

void CIseulenia::Teleport(DUNGEON_TYPE eType)
{
	switch (eType)
	{
	case DUNGEON_TYPE::TURTLE_LAKE:
	{
		NPCWork();

		GET_SINGLE(CSceneManager)->CreateNextScene();
		GET_SINGLE(CSceneManager)->SetGameMode<CLoadingMode3D>(false);

		CScene* pScene = GET_SINGLE(CSceneManager)->GetNextScene();

		CGameMode* pGameMode = pScene->GetGameMode();

		static_cast<CLoadingMode3D*>(pGameMode)->SetGameModeType(EDITOR_GAME_MODE_TYPE::MAIN);
		static_cast<CLoadingMode3D*>(pGameMode)->SetPos(0.f);
		static_cast<CLoadingMode3D*>(pGameMode)->SetBackTexture("UI2_Loading_E11");

		CLayer* pLayer = pScene->FindLayer(ALBEDO_LAYER);
		CLayer* pNextAlphaLayer = pScene->FindLayer(ALPHA_LAYER);

		CScene* pCurrentScene = GET_SINGLE(CSceneManager)->GetScene();

		CLayer* pAlbedo = pCurrentScene->FindLayer(ALBEDO_LAYER);
		CLayer* pAlpha = pCurrentScene->FindLayer(ALPHA_LAYER);

		std::shared_ptr<CGameObject> pPlayer = pAlbedo->FindObj("Player");

		std::shared_ptr<CGameObject> pStaff = pAlbedo->FindObj("Staff");

		std::shared_ptr<CGameObject> pRide = pAlpha->FindObj("BoneDragon");

		pLayer->AddObj(pPlayer);
		pLayer->AddObj(pStaff);
		if (pRide)
		{
			pNextAlphaLayer->AddObj(pRide);
		}

		CLayer* pNextUILayer = pScene->FindLayer(UI_LAYER);
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

		pPlayer->SetEnable(false);
		pStaff->SetEnable(false);
		pRide->SetEnable(false);
		pUI->SetEnable(false);
		pInventory->SetEnable(false);
		pEquip->SetEnable(false);
		pButtonInven->SetEnable(false);
		pButtonEquip->SetEnable(false);
		pMission->SetEnable(false);

		static_cast<CPlayer3D*>(pPlayer.get())->SetTerrain(nullptr);

		GET_SINGLE(CSoundManager)->Stop("Tria_Village");
	}
		break;
	case DUNGEON_TYPE::_2:
	{

	}
		break;
	case DUNGEON_TYPE::_3:
	{

	}
		break;
	}
}
