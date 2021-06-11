#include "NPC.h"
#include "Resource/Animation.h"
#include "Component/Camera.h"
#include "Component/Collider.h"
#include "Input.h"
#include "../../Quest.h"
#include "UI/UIImage.h"
#include "CameraManager.h"
#include "Component/Transform.h"
#include "UI/UIImage.h"
#include "Scene/Scene.h"
#include "Resource/ResourceManager.h"
#include "../../Dialog.h"

CNPC::CNPC()	:
	m_pAni(nullptr)
{
}

CNPC::CNPC(const CNPC& npc)	:
	CGameObject(npc)
	, m_pAni(nullptr)
	, m_pCam(FindComponentByType<CCamera>())
{
}

CNPC::~CNPC()
{
	SAFE_RELEASE(m_pAni);
}

bool CNPC::Init()
{
	GET_SINGLE(CResourceManager)->LoadTexture("quest mark_question mark", L"BLUIQuestAlert\\Texture2D\\quest mark_question mark.dds");

	if(!CGameObject::Init())
		return false;

	SetShadowAll(true);
	std::shared_ptr<CComponent> pRoot = GetRootComponent();

	m_pQuest = CreateComponent<CUIImage>("Quest", m_pScene->FindLayer(ALPHA_LAYER));

	static_cast<CSceneComponent*>(pRoot.get())->AddChild(m_pQuest);

	static_cast<CUIImage*>(m_pQuest.get())->SetTexture("quest mark_question mark");
	static_cast<CUIImage*>(m_pQuest.get())->SetSize(Vector2(1024.f, 1024.f));
	static_cast<CUIImage*>(m_pQuest.get())->SetUV(Vector2(923.f, 0.f), Vector2(959.f, 36.f));
	static_cast<CUIImage*>(m_pQuest.get())->SetWorldScale(10.f, 10.f, 1.f);
	static_cast<CUIImage*>(m_pQuest.get())->SetPivot(0.5f, 0.5f, 0.f);
	static_cast<CUIImage*>(m_pQuest.get())->SetSceneComponentType(SCENE_COMPONENT_TYPE::SCT_3D);
	static_cast<CUIImage*>(m_pQuest.get())->AddRelativePosY(45.f);

	return true;
}

void CNPC::Start()
{
	CGameObject::Start();
}

void CNPC::Update(float fTime)
{
	CGameObject::Update(fTime);

	if (m_pQuest)
	{
		std::shared_ptr<CTransform> pTransform = static_cast<CUIImage*>(m_pQuest.get())->GetTransform();

		CCamera* pCam = GET_SINGLE(CCameraManager)->GetMainCam();

		Vector3 vPos = {};

		if (pCam)
		{
			vPos = pCam->GetWorldPos();
		}

		pTransform->LookAt(vPos);
	}

	if (m_pDialog)
	{
		CQuest* pQuest = static_cast<CDialog*>(m_pDialog.get())->GetQuest();

		if (pQuest)
		{
			if (pQuest->GetState() == QUEST_STATE::QUALIFIED)
			{
				if (m_pQuest)
				{
					static_cast<CUIImage*>(m_pQuest.get())->SetUV(Vector2(876.f, 0.f), Vector2(903.f, 37.f));
					m_pQuest->SetEnable(true);
				}
			}
		}
	}


	CCamera* pCam = GET_SINGLE(CCameraManager)->GetMainCam();

	if (pCam)
	{
		if (m_pName)
		{
			float fDist = (GetWorldPos() - pCam->GetWorldPos()).Length();

			if (fDist < 500.f)
			{
				Vector3 vMeshSize = GetMeshSize();

				Vector3 vBarPos = GetBoneWorldPos() + Vector3(0.f, vMeshSize.y * 1.1f, 0.f);

				Matrix matVP = pCam->GetVP();

				Vector3 vProjPos = vBarPos.TranslationCoor(matVP);

				if (vProjPos.z < 0.f || vProjPos.z>1.f)
				{
					if (m_pName->IsEnable())
					{
						m_pName->SetEnable(false);
					}
				}
				else
				{
					if (!m_pName->IsEnable())
					{
						m_pName->SetEnable(true);
					}
				}
			}
			else
			{
				if (m_pName->IsEnable())
				{
					m_pName->SetEnable(false);
				}
			}
		}
	}
}

void CNPC::PostUpdate(float fTime)
{
	CGameObject::PostUpdate(fTime);
}

void CNPC::Collision(float fTime)
{
	CGameObject::Collision(fTime);
}

void CNPC::PreRender(float fTime)
{
	CGameObject::PreRender(fTime);
}

void CNPC::Render(float fTime)
{
	CGameObject::Render(fTime);
}

void CNPC::PostRender(float fTime)
{
	CGameObject::PostRender(fTime);
}

std::shared_ptr<CGameObject> CNPC::Clone()
{
	return std::shared_ptr<CGameObject>();
}

void CNPC::Save(FILE* pFile)
{
	CGameObject::Save(pFile);
}

void CNPC::Load(FILE* pFile)
{
	CGameObject::Load(pFile);
}

void CNPC::CollisionInit(CCollider* pSrc, CCollider* pDest, float fTime)
{
	std::string strDestName = pDest->GetName();

	if (strDestName == "MouseBody")
	{
		if (GET_SINGLE(CInput)->IsUp(MOUSE_TYPE::LEFT))
		{
			NPCWork();
		}
	}
}

void CNPC::CollisionStay(CCollider* pSrc, CCollider* pDest, float fTime)
{
	std::string strDestName = pDest->GetName();

	if (strDestName == "MouseBody")
	{
		if (GET_SINGLE(CInput)->IsUp(MOUSE_TYPE::LEFT))
		{
			NPCWork();
		}
	}
}

void CNPC::CollisionEnd(CCollider* pSrc, CCollider* pDest, float fTime)
{
	std::string strDestName = pDest->GetName();

	if (strDestName == "MouseBody")
	{
		if (GET_SINGLE(CInput)->IsUp(MOUSE_TYPE::LEFT))
		{
			NPCWork();
		}
	}
}

void CNPC::NPCWork()
{
}

void CNPC::NPCDialogEnd(bool bOk, QUEST_STATE eState)
{
}
