#include "Tati.h"
#include "Component/Renderer.h"
#include "Resource/Animation.h"
#include "Component/ColliderSphere.h"
#include "MissionUI.h"
#include "Scene/Scene.h"
#include "Component/Camera.h"
#include "CameraManager.h"
#include "../../Dialog.h"
#include "../../Quest.h"
#include "Inventory3D.h"
#include "../Item.h"
#include "UI/UIImage.h"
#include "Component/Transform.h"
#include "Sound/SoundManager.h"
#include "UI/UFont.h"

CTati::CTati()	:
	m_pPrevCam(nullptr)
{
}

CTati::CTati(const CTati& npc)	:
	CNPC(npc)
	, m_pPrevCam(npc.m_pPrevCam)
{
}

CTati::~CTati()
{
}

bool CTati::Init()
{
	GET_SINGLE(CSoundManager)->LoadSound("Effect", "HighElfF_O1_01", false,
		"Village\\HighElfF_O1_01.ogg");
	GET_SINGLE(CSoundManager)->LoadSound("Effect", "HighElfF_O1_02", false,
		"Village\\HighElfF_O1_02.ogg");
	GET_SINGLE(CSoundManager)->LoadSound("Effect", "HighElfF_O1_08", false,
		"Village\\HighElfF_O1_08.ogg");
	GET_SINGLE(CSoundManager)->LoadSound("Effect", "HighElfF_O1_12", false,
		"Village\\HighElfF_O1_12.ogg");

	std::shared_ptr<CComponent> pRenderer = CreateComponent<CSceneComponent>("Renderer");

	SetRootComponent(pRenderer);

	if (!CNPC::Init())
		return false;

	static_cast<CSceneComponent*>(pRenderer.get())->SetMesh("AN_NPC_Tati_SP_AS");
	static_cast<CSceneComponent*>(pRenderer.get())->SetRelativeRotY(70.f);

	m_pAni = CAnimation::CreateAnimation<CAnimation>("TatiAni", "TatiBone", "Town\\AN_NPC_Tati_SP_AS.bne");

	m_pAni->SetSkeleton("TatiBone");

	m_pAni->AddAnimationSequence("Idle", "AN_NPC_Tati_SP_AS");

	static_cast<CSceneComponent*>(pRenderer.get())->SetAnimation(m_pAni);

	std::shared_ptr<CComponent> pCollider = CreateComponent<CColliderSphere>("NPCBody");

	static_cast<CSceneComponent*>(pRenderer.get())->AddChild(pCollider);

	static_cast<CColliderSphere*>(pCollider.get())->SetRadius(25.f);
	static_cast<CColliderSphere*>(pCollider.get())->SetRelativePosY(25.f);
	static_cast<CColliderSphere*>(pCollider.get())->SetCallBack<CNPC>(COLLISION_STATE::CS_INIT, this, &CNPC::CollisionInit);
	static_cast<CColliderSphere*>(pCollider.get())->SetCallBack<CNPC>(COLLISION_STATE::CS_STAY, this, &CNPC::CollisionStay);
	static_cast<CColliderSphere*>(pCollider.get())->SetCallBack<CNPC>(COLLISION_STATE::CS_END, this, &CNPC::CollisionEnd);

	SetWorldPos(2232.794f, 150.826f, 8061.438f);

	m_pDialog = m_pScene->CreateObject<CDialog>("Dialog", m_pScene->FindLayer(UI_LAYER));

		static_cast<CDialog*>(m_pDialog.get())->SetNameText(L"타티");
		static_cast<CDialog*>(m_pDialog.get())->SetQuest("외눈박이");

	m_pDialog->SetEnable(false);
	static_cast<CDialog*>(m_pDialog.get())->SetNPC(this);

	m_pCam = CreateComponent<CCamera>("TatiCamera");

	static_cast<CSceneComponent*>(pRenderer.get())->AddChild(m_pCam);

	static_cast<CCamera*>(m_pCam.get())->SetInheritRotX(false);
	static_cast<CCamera*>(m_pCam.get())->SetInheritRotY(false);
	static_cast<CCamera*>(m_pCam.get())->SetInheritRotZ(false);
	static_cast<CCamera*>(m_pCam.get())->AddRelativePos(20.f, 29.f, -1.f);
	static_cast<CCamera*>(m_pCam.get())->SetWorldRotY(-10.f);

	m_pName = CreateComponent<CUFont>("NameFont");

	static_cast<CSceneComponent*>(pRenderer.get())->AddChild(m_pName);

	static_cast<CUFont*>(m_pName.get())->SetRelativePosY(GetMeshSize().y * 1.1f + 10.f);
	static_cast<CUFont*>(m_pName.get())->SetRect(-100.f, 0.f, 100.f, 0.f);
	static_cast<CUFont*>(m_pName.get())->SetText("타티");
	static_cast<CUFont*>(m_pName.get())->Set3D();
	static_cast<CUFont*>(m_pName.get())->SetShadowOffset(Vector3(2.f, -2.f, 0.f));
	static_cast<CUFont*>(m_pName.get())->SetShadowColor(0.f, 0.f, 0.f, 1.f);
	return true;
}

void CTati::Start()
{
	CNPC::Start();
}

void CTati::Update(float fTime)
{
	CNPC::Update(fTime);

}

void CTati::PostUpdate(float fTime)
{
	CNPC::PostUpdate(fTime);
}

void CTati::Collision(float fTime)
{
	CNPC::Collision(fTime);
}

void CTati::PreRender(float fTime)
{
	CNPC::PreRender(fTime);
}

void CTati::Render(float fTime)
{
	CNPC::Render(fTime);
}

void CTati::PostRender(float fTime)
{
	CNPC::PostRender(fTime);
}

std::shared_ptr<CGameObject> CTati::Clone()
{
	return std::shared_ptr<CGameObject>(new CTati(*this));
}

void CTati::Save(FILE* pFile)
{
	CNPC::Save(pFile);
}

void CTati::Load(FILE* pFile)
{
	CNPC::Load(pFile);
}

void CTati::NPCWork()
{
	if (m_pDialog)
	{
		if (!m_pDialog->IsEnable())
		{
			static_cast<CDialog*>(m_pDialog.get())->Open();
			m_pPrevCam = GET_SINGLE(CCameraManager)->GetMainCam();

			GET_SINGLE(CCameraManager)->SetMainCam("TatiCamera");

			GET_SINGLE(CSoundManager)->Play("HighElfF_O1_01");
		}
	}

}

void CTati::Close()
{
	GET_SINGLE(CCameraManager)->SetMainCam(m_pPrevCam);
}

void CTati::NPCDialogEnd(bool bOk, QUEST_STATE eState)
{
	if (bOk)
	{
		switch (eState)
		{
		case QUEST_STATE::NONE:
		{
			m_pQuest->SetEnable(false);
			GET_SINGLE(CSoundManager)->Play("HighElfF_O1_12");
		}
		break;
		case QUEST_STATE::ACCEPTED:
			GET_SINGLE(CSoundManager)->Play("HighElfF_O1_08");
			break;

		case QUEST_STATE::QUALIFIED:
		{
			std::shared_ptr<CGameObject> pInven = m_pScene->FindLayer(UI_LAYER)->FindObj("Inventory");

			CItem* pItem = CItem::FindItem("세트2상의");

			if (pItem)
			{
				static_cast<CInventory3D*>(pInven.get())->AddItem(pItem);
			}
			pItem = CItem::FindItem("세트2어깨");

			if (pItem)
			{
				static_cast<CInventory3D*>(pInven.get())->AddItem(pItem);
			}
			pItem = CItem::FindItem("세트2하의");

			if (pItem)
			{
				static_cast<CInventory3D*>(pInven.get())->AddItem(pItem);
			}

			m_pQuest->SetEnable(false);
			GET_SINGLE(CSoundManager)->Play("HighElfF_O1_02");
		}
			break;
		}
	}
	Close();
}
