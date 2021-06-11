#include "Portal.h"
#include "Component/Renderer.h"
#include "Component/ColliderSphere.h"
#include "Scene/SceneManager.h"
#include "../GameMode/Town.h"
#include "Player3D.h"
#include "Sound/SoundManager.h"
#include "Resource/Mesh.h"

CPortal::CPortal()	:
	m_eGameModeType(EDITOR_GAME_MODE_TYPE::TOWN)
{
}

CPortal::CPortal(const CPortal& obj)	:
	CGameObject(obj)
	, m_eGameModeType(obj.m_eGameModeType)
{
}

CPortal::~CPortal()
{
}

bool CPortal::Init()
{
	if(!CGameObject::Init())
		return false;

	std::shared_ptr<CComponent> pRenderer = CreateComponent<CSceneComponent>("Renderer");

	SetRootComponent(pRenderer);

	static_cast<CSceneComponent*>(pRenderer.get())->SetMesh("Portal");

	std::shared_ptr<CComponent> pCollider = CreateComponent<CColliderSphere>("PortalBody", m_pScene->FindLayer(COLLIDER_LAYER));

	static_cast<CSceneComponent*>(pRenderer.get())->AddChild(pCollider);

	static_cast<CColliderSphere*>(pCollider.get())->SetRadius(50.f);
	static_cast<CColliderSphere*>(pCollider.get())->SetCallBack<CPortal>(COLLISION_STATE::CS_INIT, this, &CPortal::CollisionInit);
	static_cast<CColliderSphere*>(pCollider.get())->SetCallBack<CPortal>(COLLISION_STATE::CS_STAY, this, &CPortal::CollisionStay);
	static_cast<CColliderSphere*>(pCollider.get())->SetCallBack<CPortal>(COLLISION_STATE::CS_END, this, &CPortal::CollisionEnd);


	return true;
}

void CPortal::Start()
{
	CGameObject::Start();
}

void CPortal::Update(float fTime)
{
	CGameObject::Update(fTime);
}

void CPortal::PostUpdate(float fTime)
{
	CGameObject::PostUpdate(fTime);
}

void CPortal::Collision(float fTime)
{
	CGameObject::Collision(fTime);
}

void CPortal::PreRender(float fTime)
{
	CGameObject::PreRender(fTime);
}

void CPortal::Render(float fTime)
{
	CGameObject::Render(fTime);
}

void CPortal::PostRender(float fTime)
{
	CGameObject::PostRender(fTime);
}

std::shared_ptr<CGameObject> CPortal::Clone()
{
	return std::shared_ptr<CGameObject>(new CPortal(*this));
}

void CPortal::Save(FILE* pFile)
{
	CGameObject::Save(pFile);
}

void CPortal::Load(FILE* pFile)
{
	CGameObject::Load(pFile);
}

void CPortal::CollisionInit(CCollider* pSrc, CCollider* pDest, float fTime)
{
	if (pDest->GetName() == "PlayerBody")
	{
		switch (m_eGameModeType)
		{
		case EDITOR_GAME_MODE_TYPE::TOWN:
		{
			GET_SINGLE(CSoundManager)->Stop("NewBattle_02");
			GET_SINGLE(CSoundManager)->Stop("Amb_Forest_1");

			GET_SINGLE(CSceneManager)->CreateNextScene();

			GET_SINGLE(CSceneManager)->SetGameMode<CTown>(false);

			CScene* pNextScene = GET_SINGLE(CSceneManager)->GetNextScene();

			CLayer* pNextAlpha = pNextScene->FindLayer(ALPHA_LAYER);

			CLayer* pNextAlbedo = pNextScene->FindLayer(ALBEDO_LAYER);

			CLayer* pAlbedo = m_pScene->FindLayer(ALBEDO_LAYER);

			CLayer* pAlpha = m_pScene->FindLayer(ALPHA_LAYER);

			std::shared_ptr<CGameObject> pPlayer = pAlbedo->FindObj("Player");
			std::shared_ptr<CGameObject> pWeapon = pAlbedo->FindObj("Staff");
			std::shared_ptr<CGameObject> pRide = pAlpha->FindObj("BoneDragon");

			if (pRide)
			{
				pNextAlpha->AddObj(pRide);
			}
			pNextAlbedo->AddObj(pPlayer);
			pNextAlbedo->AddObj(pWeapon);

			CLayer* pNextUILayer = pNextScene->FindLayer(UI_LAYER);
			CLayer* pUILayer = m_pScene->FindLayer(UI_LAYER);

			std::shared_ptr<CGameObject> pUI = pUILayer->FindObj("UI");
			std::shared_ptr<CGameObject> pInventory = pUILayer->FindObj("Inventory");
			std::shared_ptr<CGameObject> pEquip = pUILayer->FindObj("Equip");
			std::shared_ptr<CGameObject> pButtonInven = pUILayer->FindObj("ButtonInven");
			std::shared_ptr<CGameObject> pButtonEquip = pUILayer->FindObj("ButtonEquip");
			std::shared_ptr<CGameObject> pMission = pUILayer->FindObj("Mission");

			pNextUILayer->AddObj(pUI);
			pNextUILayer->AddObj(pInventory);
			pNextUILayer->AddObj(pEquip);
			if (pButtonInven)
			{
				pNextUILayer->AddObj(pButtonInven);
			}
			if (pButtonEquip)
			{
				pNextUILayer->AddObj(pButtonEquip);
			}
			if (pMission)
			{
				pNextUILayer->AddObj(pMission);
			}

			pPlayer->SetWorldPos(3000.f, 0.f, 3000.f);
			static_cast<CPlayer3D*>(pPlayer.get())->SetTarget(nullptr);
		}
			break;
		}
	}
}

void CPortal::CollisionStay(CCollider* pSrc, CCollider* pDest, float fTime)
{
}

void CPortal::CollisionEnd(CCollider* pSrc, CCollider* pDest, float fTime)
{
}
