#include "WoundedElin.h"
#include "Component/Renderer.h"
#include "Resource/Animation.h"
#include "UI/UFont.h"
#include "Component/ColliderSphere.h"
#include "Component/Camera.h"
#include "CameraManager.h"
#include "Scene/Scene.h"
#include "../../Dialog.h"
#include "../../Quest.h"
#include "Inventory3D.h"
#include "../Item.h"
#include "Sound/SoundManager.h"

CWoundedElin::CWoundedElin()	:
	m_pPrevCam(nullptr)
{
}

CWoundedElin::CWoundedElin(const CWoundedElin& npc)	:
	CNPC(npc)
	, m_pPrevCam(npc.m_pPrevCam)
{
}

CWoundedElin::~CWoundedElin()
{
}

bool CWoundedElin::Init()
{
	GET_SINGLE(CSoundManager)->LoadSound("Effect", "DialogStart_01", false,
		"Village\\DialogStart_01.ogg");
	GET_SINGLE(CSoundManager)->LoadSound("Effect", "DialogEnd_01", false,
		"Village\\DialogEnd_01.ogg");
	GET_SINGLE(CSoundManager)->LoadSound("Effect", "QuestStart_01", false,
		"Village\\QuestStart_01.ogg");
	GET_SINGLE(CSoundManager)->LoadSound("Effect", "QuestEnd_01", false,
		"Village\\QuestEnd_01.ogg");

	std::shared_ptr<CComponent> pRenderer = CreateComponent<CSceneComponent>("Renderer");

	SetRootComponent(pRenderer);

	if(!CNPC::Init())
		return false;

	static_cast<CSceneComponent*>(pRenderer.get())->SetMesh("Elin_Wounded");
	static_cast<CSceneComponent*>(pRenderer.get())->SetRelativeRotY(70.f);
	static_cast<CSceneComponent*>(pRenderer.get())->SetShader("Alpha");
	static_cast<CSceneComponent*>(pRenderer.get())->SetShader("Alpha", 0, 1);
	static_cast<CSceneComponent*>(pRenderer.get())->SetShader("Alpha", 0, 2);
	static_cast<CSceneComponent*>(pRenderer.get())->AddRenderState("NoCullRS");
	static_cast<CSceneComponent*>(pRenderer.get())->AddRenderState("NoCullRS", 0, 1);
	static_cast<CSceneComponent*>(pRenderer.get())->AddRenderState("NoCullRS", 0, 2);

	m_pAni = CAnimation::CreateAnimation<CAnimation>("ElinAni", "ElinBone", "Town\\Elin_Wounded.bne");

	m_pAni->SetSkeleton("ElinBone");
	m_pAni->AddAnimationSequence("Idle", "Elin_Wounded");

	static_cast<CSceneComponent*>(pRenderer.get())->SetAnimation(m_pAni);

	std::shared_ptr<CComponent> pCollider = CreateComponent<CColliderSphere>("NPCBody");

	static_cast<CSceneComponent*>(pRenderer.get())->AddChild(pCollider);

	static_cast<CColliderSphere*>(pCollider.get())->SetRadius(7.f);
	static_cast<CColliderSphere*>(pCollider.get())->SetRelativePosY(21.f);
	static_cast<CColliderSphere*>(pCollider.get())->SetCallBack<CNPC>(COLLISION_STATE::CS_INIT, this, &CNPC::CollisionInit);
	static_cast<CColliderSphere*>(pCollider.get())->SetCallBack<CNPC>(COLLISION_STATE::CS_STAY, this, &CNPC::CollisionStay);
	static_cast<CColliderSphere*>(pCollider.get())->SetCallBack<CNPC>(COLLISION_STATE::CS_END, this, &CNPC::CollisionEnd);

	m_pDialog = m_pScene->CreateObject<CDialog>("Dialog", m_pScene->FindLayer(UI_LAYER));
	m_pDialog->SetEnable(false);

	static_cast<CDialog*>(m_pDialog.get())->SetNameText(L"ªÛ√≥ ¿‘¿∫ ø§∏∞");
	static_cast<CDialog*>(m_pDialog.get())->SetNPC(this);
	static_cast<CDialog*>(m_pDialog.get())->SetQuest("µŒ∞≥∞Ò");

	m_pCam = CreateComponent<CCamera>("ElinCamera");

	static_cast<CSceneComponent*>(pRenderer.get())->AddChild(m_pCam);

	static_cast<CCamera*>(m_pCam.get())->SetInheritRotX(false);
	static_cast<CCamera*>(m_pCam.get())->SetInheritRotY(false);
	static_cast<CCamera*>(m_pCam.get())->SetInheritRotZ(false);
	static_cast<CCamera*>(m_pCam.get())->AddRelativePos(15.f, 29.f, 0.5f);
	static_cast<CCamera*>(m_pCam.get())->SetRelativeRotY(17.f);

	SetWorldPos(3123.949f, -17.000f, 5224.744f);

	m_pName = CreateComponent<CUFont>("NameFont");

	static_cast<CSceneComponent*>(pRenderer.get())->AddChild(m_pName);

	static_cast<CUFont*>(m_pName.get())->SetRelativePosY(GetMeshSize().y * 1.1f + 10.f);
	static_cast<CUFont*>(m_pName.get())->SetRect(-100.f, 0.f, 100.f, 0.f);
	static_cast<CUFont*>(m_pName.get())->SetText("ªÛ√≥ ¿‘¿∫ ø§∏∞");
	static_cast<CUFont*>(m_pName.get())->Set3D();
	static_cast<CUFont*>(m_pName.get())->SetShadowOffset(Vector3(2.f, -2.f, 0.f));
	static_cast<CUFont*>(m_pName.get())->SetShadowColor(0.f, 0.f, 0.f, 1.f);

	return true;
}

void CWoundedElin::Start()
{
	CNPC::Start();
}

void CWoundedElin::Update(float fTime)
{
	std::shared_ptr<CGameObject> pInven = m_pScene->FindLayer(UI_LAYER)->FindObj("Inventory");

	if (pInven)
	{
		if (static_cast<CInventory3D*>(pInven.get())->HasItem("ø‹¥´π⁄¿Ã¿« µŒ∞≥∞Ò"))
		{
			if (m_pDialog)
			{
				CQuest* pQuest = static_cast<CDialog*>(m_pDialog.get())->GetQuest();

				if (pQuest->GetState() == QUEST_STATE::ACCEPTED)
				{
					pQuest->SetState(QUEST_STATE::QUALIFIED);
				}
			}
		}
		else
		{
			if (m_pDialog)
			{
				CQuest* pQuest = static_cast<CDialog*>(m_pDialog.get())->GetQuest();

				if (pQuest->GetState() == QUEST_STATE::QUALIFIED)
				{
					pQuest->SetState(QUEST_STATE::ACCEPTED);
				}
			}
		}
	}
	CNPC::Update(fTime);
}

void CWoundedElin::PostUpdate(float fTime)
{
	CNPC::PostUpdate(fTime);
}

void CWoundedElin::Collision(float fTime)
{
	CNPC::Collision(fTime);
}

void CWoundedElin::PreRender(float fTime)
{
	CNPC::PreRender(fTime);
}

void CWoundedElin::Render(float fTime)
{
	CNPC::Render(fTime);
}

void CWoundedElin::PostRender(float fTime)
{
	CNPC::PostRender(fTime);
}

std::shared_ptr<CGameObject> CWoundedElin::Clone()
{
	return std::shared_ptr<CGameObject>(new CWoundedElin(*this));
}

void CWoundedElin::Save(FILE* pFile)
{
	CNPC::Save(pFile);
}

void CWoundedElin::Load(FILE* pFile)
{
	CNPC::Load(pFile);
}

void CWoundedElin::NPCWork()
{
	if (!m_pPrevCam)
	{
		m_pPrevCam = GET_SINGLE(CCameraManager)->GetMainCam();

		GET_SINGLE(CCameraManager)->SetMainCam("ElinCamera");

		if (m_pDialog)
		{
			static_cast<CDialog*>(m_pDialog.get())->Open();
		}

		GET_SINGLE(CSoundManager)->Play("DialogStart_01");
	}
}

void CWoundedElin::Close()
{
	if (m_pPrevCam)
	{
		GET_SINGLE(CCameraManager)->SetMainCam(m_pPrevCam);

		m_pPrevCam = nullptr;
	}
}

void CWoundedElin::NPCDialogEnd(bool bOk, QUEST_STATE eState)
{
	if (bOk)
	{
		switch (eState)
		{
		case QUEST_STATE::NONE:
		{
			m_pQuest->SetEnable(false);
			GET_SINGLE(CSoundManager)->Play("QuestStart_01");
		}
			break;
		case QUEST_STATE::QUALIFIED:
		{
			std::shared_ptr<CGameObject> pInven = m_pScene->FindLayer(UI_LAYER)->FindObj("Inventory");

			CItem* pItem = CItem::FindItem("ºº∆Æ2«Ô∏‰");

			if (pInven)
			{
				static_cast<CInventory3D*>(pInven.get())->DeleteItem("ø‹¥´π⁄¿Ã¿« µŒ∞≥∞Ò");
				static_cast<CInventory3D*>(pInven.get())->AddItem(pItem);
			}
			pItem = CItem::FindItem("ºº∆Æ2¿Â∞©");

			if (pInven)
			{
				static_cast<CInventory3D*>(pInven.get())->AddItem(pItem);
			}
			pItem = CItem::FindItem("ºº∆Æ2Ω≈πﬂ");

			if (pInven)
			{
				static_cast<CInventory3D*>(pInven.get())->AddItem(pItem);
			}
			pItem = CItem::FindItem("ºº∆Æ2∫ß∆Æ");

			if (pInven)
			{
				static_cast<CInventory3D*>(pInven.get())->AddItem(pItem);
			}
			m_pQuest->SetEnable(false);
			GET_SINGLE(CSoundManager)->Play("QuestEnd_01");
		}
			break;

		case QUEST_STATE::ACCEPTED:

			GET_SINGLE(CSoundManager)->Play("DialogEnd_01");
			break;
		}
	}

	m_pDialog->SetEnable(false);

	Close();
}
