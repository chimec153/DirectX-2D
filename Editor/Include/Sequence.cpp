#include "Sequence.h"
#include "Component/ColliderSphere.h"
#include "Component/Camera.h"
#include "CameraManager.h"
#include "Scene/Scene.h"
#include "Editor/Object/Player3D.h"
#include "Editor/Object/Cyclops.h"
#include "RenderManager.h"
#include "Sound/SoundManager.h"

CSequence::CSequence()	:
	m_pCam(nullptr)
	, m_iMaxFrame(0)
	, m_fSequenceTime(0.f)
	, m_fMaxTime(0.f)
	, m_bPlay(false)
	, m_pTarget(nullptr)
	, m_pPrevCam(nullptr)
	, m_eBossSceneType(BOSS_SCENE_TYPE::FIND1)
	, m_fSceneTime(0.f)
	, m_fSceneTimeMax(0.f)
	, m_fIdleTime(0.f)
	, m_fIdleTimeMax(1.f)
{
}

CSequence::CSequence(const CSequence& seq)	:
	CGameObject(seq)
	, m_pCam(seq.m_pCam)
	, m_iMaxFrame(seq.m_iMaxFrame)
	, m_fSequenceTime(seq.m_fSequenceTime)
	, m_fMaxTime(seq.m_fMaxTime)
	, m_bPlay(seq.m_bPlay)
	, m_pTarget(seq.m_pTarget)
	, m_pPrevCam(seq.m_pPrevCam)
	, m_fIdleTime(seq.m_fIdleTime)
	, m_fIdleTimeMax(seq.m_fIdleTimeMax)
{
}

CSequence::~CSequence()
{
}

void CSequence::SetTarget(CGameObject* pObj)
{
	m_pTarget = pObj;

	if (m_pTarget)
	{
		std::shared_ptr<CComponent> pRoot = m_pTarget->GetRootComponent();

		CSceneComponent* pCom = static_cast<CSceneComponent*>(pRoot.get());

		static_cast<CCamera*>(m_pCam.get())->SetTarget(pCom);
	}
}

void CSequence::AddPos(const Vector3& vPos)
{
	m_vecPos.push_back(vPos);
	m_iMaxFrame = static_cast<int>(m_vecPos.size()) - 1;
}

void CSequence::SetMaxTime(float fMaxTime)
{
	m_fMaxTime = fMaxTime;
}

bool CSequence::Init()
{
	if(!CGameObject::Init())
		return false;

	std::shared_ptr<CComponent> pCollider = CreateComponent<CColliderSphere>("EventBody", m_pScene->FindLayer(COLLIDER_LAYER));

	SetRootComponent(pCollider);

	static_cast<CColliderSphere*>(pCollider.get())->SetRadius(200.f);
	static_cast<CColliderSphere*>(pCollider.get())->SetCallBack<CSequence>(COLLISION_STATE::CS_INIT, this, &CSequence::CollisionInit);
	static_cast<CColliderSphere*>(pCollider.get())->SetCallBack<CSequence>(COLLISION_STATE::CS_STAY, this, &CSequence::CollisionStay);
	static_cast<CColliderSphere*>(pCollider.get())->SetCallBack<CSequence>(COLLISION_STATE::CS_LAST, this, &CSequence::CollisionEnd);

	m_pCam = CreateComponent<CCamera>("EventCamera");

	GET_SINGLE(CCameraManager)->AddCam(static_cast<CCamera*>(m_pCam.get()));

	m_pPlayerCam1 = CreateComponent<CCamera>("EventPlayerCam1");
	static_cast<CCamera*>(m_pPlayerCam1.get())->SetWorldPos(1000.f, 160.f, 2250.f);
	static_cast<CCamera*>(m_pPlayerCam1.get())->SetPositionTargetMode(true);
	static_cast<CCamera*>(m_pPlayerCam1.get())->SetPositionTarget(Vector3(1025.f, 0.f, 2400.f));

	GET_SINGLE(CCameraManager)->AddCam(static_cast<CCamera*>(m_pPlayerCam1.get()));

	m_pPlayerCam2 = CreateComponent<CCamera>("EventPlayerCam2");

	GET_SINGLE(CCameraManager)->AddCam(static_cast<CCamera*>(m_pPlayerCam2.get()));
	static_cast<CCamera*>(m_pPlayerCam2.get())->SetWorldPos(850.f, 170.f, 2480.f);
	static_cast<CCamera*>(m_pPlayerCam2.get())->SetPositionTargetMode(true);
	static_cast<CCamera*>(m_pPlayerCam2.get())->SetPositionTarget(Vector3(1085.f, 0.f, 2510.f));

	m_pBossCam1 = CreateComponent<CCamera>("BossCam1");
	GET_SINGLE(CCameraManager)->AddCam(static_cast<CCamera*>(m_pBossCam1.get()));
	static_cast<CCamera*>(m_pBossCam1.get())->SetRelativeRotX(90.f);
	m_pBossCam2 = CreateComponent<CCamera>("BossCam2");
	GET_SINGLE(CCameraManager)->AddCam(static_cast<CCamera*>(m_pBossCam2.get()));

	static_cast<CSceneComponent*>(m_pBossCam2.get())->SetWorldPos(950.f, 60.f, 2450.f);
	static_cast<CSceneComponent*>(m_pBossCam2.get())->SetWorldRotY(90.f);

	return true;
}

void CSequence::Start()
{
	CGameObject::Start();
}

void CSequence::Update(float fTime)
{
	CGameObject::Update(fTime);

	if (m_bPlay)
	{
		m_fSceneTime += fTime;

		if (m_fSceneTime >= m_fSceneTimeMax)
		{
			m_fSceneTime -= m_fSceneTimeMax;

			switch (m_eBossSceneType)
			{

			case BOSS_SCENE_TYPE::NONE:
			{

				m_eBossSceneType = static_cast<BOSS_SCENE_TYPE>(static_cast<int>(m_eBossSceneType) + 1);
			}
				break;
			case BOSS_SCENE_TYPE::FIND1:
			{
				m_eBossSceneType = static_cast<BOSS_SCENE_TYPE>(static_cast<int>(m_eBossSceneType) + 1);
				GET_SINGLE(CCameraManager)->SetMainCam(m_pPlayerCam2->GetName());
			}
				break;
			case BOSS_SCENE_TYPE::FIND2:
			{
				m_eBossSceneType = static_cast<BOSS_SCENE_TYPE>(static_cast<int>(m_eBossSceneType) + 1);
				GET_SINGLE(CCameraManager)->SetMainCam(m_pBossCam1->GetName());
				GET_SINGLE(CRenderManager)->SetFullBlur(true);
				GET_SINGLE(CRenderManager)->SetSampleDist(1.7f);
				GET_SINGLE(CRenderManager)->SetSampleStrength(2.5f);
				m_fSceneTimeMax = 3.f;

			}
				break;
			case BOSS_SCENE_TYPE::BOSSFALL:
			{
				GET_SINGLE(CRenderManager)->SetFullBlur(false);
				m_eBossSceneType = static_cast<BOSS_SCENE_TYPE>(static_cast<int>(m_eBossSceneType) + 1);
				GET_SINGLE(CCameraManager)->SetMainCam(m_pBossCam2->GetName());
			}
				break;
			case BOSS_SCENE_TYPE::BOSSLAND:
			{
			}
				break;
			case BOSS_SCENE_TYPE::END:
				break;
			}

		}

		switch (m_eBossSceneType)
		{
		case BOSS_SCENE_TYPE::FIND1:
		{
		}
			break;
		case BOSS_SCENE_TYPE::FIND2:
		{
		}
			break;
		case BOSS_SCENE_TYPE::BOSSFALL:
		{

		}
			break;
		case BOSS_SCENE_TYPE::BOSSLAND:
		{
			if (!static_cast<CCyclops*>(m_pTarget)->IsAir())
			{
				m_eBossSceneType = static_cast<BOSS_SCENE_TYPE>(static_cast<int>(m_eBossSceneType) + 1);
			}
		}
			break;
		case BOSS_SCENE_TYPE::END:
		{
			if (m_fIdleTime < m_fIdleTimeMax)
			{
				m_fIdleTime += fTime;

				if (m_fIdleTime >= m_fIdleTimeMax)
				{
					static_cast<CCyclops*>(m_pTarget)->MotionStart();
					GET_SINGLE(CCameraManager)->SetMainCam("EventCamera");
				}

			}

			else
			{
				m_fSequenceTime += fTime;

				float fFrameTime = m_fMaxTime / m_iMaxFrame;

				int iFrame = static_cast<int>(m_fSequenceTime / fFrameTime);

				float fRatio = m_fSequenceTime / fFrameTime - iFrame;

				int iNextFrame = iFrame + 1;

				if (iNextFrame > m_iMaxFrame)
				{
					static_cast<CCyclops*>(m_pTarget)->SetSequenceMode(false);
					m_bPlay = false;
					GET_SINGLE(CCameraManager)->SetMainCam(m_pPrevCam);
					m_pPrevCam = nullptr;
					if (m_EndCallBack)
					{
						m_EndCallBack();
					}
					static_cast<CPlayer3D*>(m_pPlayer.get())->SetSequenceMode(false);
					m_pScene->FindLayer(UI_LAYER)->SetEnable(true);
					static_cast<CCyclops*>(m_pTarget)->MotionEnd();
					static_cast<CPlayer3D*>(m_pPlayer.get())->SetNameAndHPBar(true);
					Destroy();
				}
				else
				{
					Vector3 vPos = XMVectorLerp(m_vecPos[iFrame].Convert(), m_vecPos[iNextFrame].Convert(), fRatio);
					if (m_pTarget)
					{
						Matrix matRot = m_pTarget->GetMatRot();

						vPos = vPos.TranslationCoor(matRot);

						static_cast<CSceneComponent*>(m_pCam.get())->SetWorldPos(vPos + m_pTarget->GetWorldPos());
					}
					else
					{
						static_cast<CSceneComponent*>(m_pCam.get())->SetWorldPos(vPos);
					}

				}
			}
		}
			break;
		}
	}
}

void CSequence::PostUpdate(float fTime)
{
	CGameObject::PostUpdate(fTime);
}

void CSequence::Collision(float fTime)
{
	CGameObject::Collision(fTime);
}

void CSequence::PreRender(float fTime)
{
	CGameObject::PreRender(fTime);

	if (m_bPlay)
	{
		switch (m_eBossSceneType)
		{
		case BOSS_SCENE_TYPE::FIND1:
		{
			if (m_fSceneTime > 0.15f)
			{
				static_cast<CPlayer3D*>(m_pPlayer.get())->MoveX(0.25f, fTime);
			}
		}
		break;
		case BOSS_SCENE_TYPE::FIND2:
		{
			static_cast<CPlayer3D*>(m_pPlayer.get())->MoveX(0.25f, fTime);
		}
		}
	}
}

void CSequence::Render(float fTime)
{
	CGameObject::Render(fTime);
}

void CSequence::PostRender(float fTime)
{
	CGameObject::PostRender(fTime);
}

std::shared_ptr<CGameObject> CSequence::Clone()
{
	return std::shared_ptr<CGameObject>(new CSequence(*this));
}

void CSequence::Save(FILE* pFile)
{
	CGameObject::Save(pFile);
}

void CSequence::Load(FILE* pFile)
{
	CGameObject::Load(pFile);
}

void CSequence::CollisionInit(CCollider* pSrc, CCollider* pDest, float fTime)
{
	if (pDest->GetName() == "PlayerBody")
	{
		GET_SINGLE(CSoundManager)->Stop("Amb_Forest_1");
		m_bPlay = true;
		if (m_StartCallBack)
		{
			m_StartCallBack();
		}
		m_pPrevCam = GET_SINGLE(CCameraManager)->GetMainCam();
		GET_SINGLE(CCameraManager)->SetMainCam(m_pPlayerCam1->GetName());

		m_pTarget = m_pScene->FindLayer(ALBEDO_LAYER)->FindObj("Cyclops").get();

		static_cast<CSceneComponent*>(m_pTarget->GetRootComponent().get())->AddChild(m_pBossCam1);

		static_cast<CCamera*>(m_pCam.get())->SetTarget(static_cast<CSceneComponent*>(m_pTarget->GetRootComponent().get()));

		m_pTarget->SetWorldRotY(180.f);
		static_cast<CCyclops*>(m_pTarget)->SetSequenceMode(true);

		m_pPlayer = m_pScene->FindLayer(ALBEDO_LAYER)->FindObj("Player");

		static_cast<CCamera*>(m_pBossCam1.get())->SetTarget(static_cast<CSceneComponent*>(m_pPlayer->GetRootComponent().get()));

		m_pPlayer->SetWorldPos(925.f, 0.f, 2335.f);
		m_pPlayer->SetWorldRotY(311.761f);
		static_cast<CPlayer3D*>(m_pPlayer.get())->SetSequenceMode(true);
		static_cast<CPlayer3D*>(m_pPlayer.get())->SetRotY(2.4f);
		static_cast<CPlayer3D*>(m_pPlayer.get())->SetNameAndHPBar(false);

		m_pScene->FindLayer(UI_LAYER)->SetEnable(false);
	}
}

void CSequence::CollisionStay(CCollider* pSrc, CCollider* pDest, float fTime)
{
}

void CSequence::CollisionEnd(CCollider* pSrc, CCollider* pDest, float fTime)
{
}

void CSequence::ShowWindow()
{
	CGameObject::ShowWindow();

	std::string strName = GetName();

	if (strName == "")
	{
		strName = "None";
	}

	if (ImGui::Begin(strName.c_str()))
	{
		if (m_pPlayerCam1)
		{
			m_pPlayerCam1->ShowWindow();
		}
		if (m_pPlayerCam2)
		{
			m_pPlayerCam2->ShowWindow();
		}
		if (m_pBossCam1)
		{
			m_pBossCam1->ShowWindow();
		}
		if (m_pBossCam2)
		{
			m_pBossCam2->ShowWindow();
		}

		switch (m_eBossSceneType)
		{
		case BOSS_SCENE_TYPE::FIND1:
			ImGui::Text("Find1");
			break;
		case BOSS_SCENE_TYPE::FIND2:
			ImGui::Text("FIND2");
			break;
		case BOSS_SCENE_TYPE::BOSSFALL:
			ImGui::Text("BOSSFALL");
			break;
		case BOSS_SCENE_TYPE::BOSSLAND:
			ImGui::Text("BOSSLAND");
			break;
		}
	}
	ImGui::End();
}
