#include "Goblin.h"
#include "Scene/Scene.h"
#include "Component/Renderer.h"
#include "Resource/Animation.h"
#include "Component/Terrain.h"
#include "Component/ColliderSphere.h"
#include "Component/Transform.h"
#include "../Idle.h"
#include "Component/PaperBurn.h"
#include "GoblinRock.h"
#include "Sound/SoundManager.h"
#include "CameraManager.h"
#include "Component/Camera.h"

CGoblin::CGoblin()	:
	m_fIdleTime(0.f)
	, m_fIdleTimeLimit(1.5f)
	, m_fWalkTime(0.f)
	, m_fWalkTimeLimit(4.5f)
{
	m_pState = new CIdle;
}

CGoblin::CGoblin(const CGoblin& obj)	:
	CMonster3D(obj)
	, m_fIdleTime(obj.m_fIdleTime)
	, m_fIdleTimeLimit(obj.m_fIdleTimeLimit)
	, m_fWalkTime(obj.m_fWalkTime)
	, m_fWalkTimeLimit(obj.m_fWalkTimeLimit)
{
	m_iGold = rand() % 26 + 25;
	std::shared_ptr<CComponent> pRenderer = GetRootComponent();

	m_pAni = static_cast<CSceneComponent*>(pRenderer.get())->GetAnimation();

	m_pAni->AddTransition("WalkToWait", "MonsterWalk", this, &CGoblin::Transition, "MonsterWait");
	m_pAni->AddTransition("WalkToAttack", "MonsterWalk", this, &CGoblin::Transition, "MonsterAttack");
	m_pAni->AddTransition("WalkToDMG", "MonsterWalk", this, &CGoblin::Transition, "flinch");
	m_pAni->AddTransition("WalkToDeath", "MonsterWalk", this, &CGoblin::Transition, "MonsterDeath");
	m_pAni->AddTransition("WalkToRun", "MonsterWalk", this, &CGoblin::Transition, "MonsterRun");
	m_pAni->AddTransition("WaitToWalk", "MonsterWait", this, &CGoblin::Transition, "MonsterWalk");
	m_pAni->AddTransition("WaitToAttack", "MonsterWait", this, &CGoblin::Transition, "MonsterAttack");
	m_pAni->AddTransition("WaitToDMG", "MonsterWait", this, &CGoblin::Transition, "flinch");
	m_pAni->AddTransition("WaitToDeath", "MonsterWait", this, &CGoblin::Transition, "MonsterDeath");
	m_pAni->AddTransition("WaitToRun", "MonsterWait", this, &CGoblin::Transition, "MonsterRun");
	m_pAni->AutoTransition("MonsterAttack", "AttackToWait", "MonsterWait");
	m_pAni->AddTransition("AttackToDeath", "MonsterAttack", this, &CGoblin::Transition, "MonsterDeath");
	m_pAni->AddTransition("DMGToWait", "flinch", this, &CGoblin::Transition, "MonsterWait");
	m_pAni->AddTransition("DMGToWalk", "flinch", this, &CGoblin::Transition, "MonsterWalk");
	m_pAni->AddTransition("DMGToAttack", "flinch", this, &CGoblin::Transition, "MonsterAttack");
	m_pAni->AddTransition("DMGToDeath", "flinch", this, &CGoblin::DMGToDeath, "MonsterDeath");
	m_pAni->AddTransition("IdleToDeath", "MonsterWait", this, &CGoblin::DMGToDeath, "MonsterDeath");
	m_pAni->AutoTransition("MonsterDeath", "DeathToDeadBody", "DeadBody");
	m_pAni->AddNotifyFunction("Attack", this, &CGoblin::ThrowRock);
	m_pAni->AddNotifyFunction("Death", this, &CGoblin::Death);
	m_pAni->AddNotifyFunction("Walk1", this, &CGoblin::WalkSound);
	m_pAni->AddNotifyFunction("Walk2", this, &CGoblin::WalkSound);

	std::shared_ptr<CComponent> pCollider = FindComponentByType<CColliderSphere>();

	static_cast<CColliderSphere*>(pCollider.get())->SetCallBack<CMonster3D>(COLLISION_STATE::CS_INIT,
		this, &CMonster3D::CollisionInit);
	static_cast<CColliderSphere*>(pCollider.get())->SetCallBack<CMonster3D>(COLLISION_STATE::CS_STAY,
		this, &CMonster3D::CollisionStay);
	static_cast<CColliderSphere*>(pCollider.get())->SetCallBack<CMonster3D>(COLLISION_STATE::CS_LAST,
		this, &CMonster3D::CollisionLast);

	m_pPaper = FindComponentByType<CPaperBurn>();
	static_cast<CSceneComponent*>(pRenderer.get())->AddObjComponent(m_pPaper);
	static_cast<CPaperBurn*>(m_pPaper.get())->AddEndCallBack(this, &CGoblin::Resolve);
}

CGoblin::~CGoblin()
{
	SAFE_RELEASE(m_pAni);
}

bool CGoblin::Init()
{
	GET_SINGLE(CSoundManager)->LoadSound("Effect", "Goral_Atk01", false,
		"Goblin\\Goral_Atk01.ogg");
	GET_SINGLE(CSoundManager)->LoadSound("Effect", "Goral_Death00", false,
		"Goblin\\Goral_Death00.ogg");
	GET_SINGLE(CSoundManager)->LoadSound("Effect", "Goral_Death01", false,
		"Goblin\\Goral_Death01.ogg");
	GET_SINGLE(CSoundManager)->LoadSound("Effect", "Goral_Run00", false,
		"Goblin\\Goral_Run00.ogg");
	GET_SINGLE(CSoundManager)->LoadSound("Effect", "Goral_Run01", false,
		"Goblin\\Goral_Run01.ogg");

	std::shared_ptr<CComponent> pCom = CreateComponent<CSceneComponent>("Goblin");

	SetRootComponent(pCom);

	m_iHPMax = m_iHP = 150;
	m_iAttackMax = 25;
	m_iAttackMin = 16;
	m_iArmorMax = 10;
	m_iArmorMin = 5;
	m_fSpeed = 18.f;

	AddLootItem("Æ÷¼Ç");
	AddLootItem("»§");

	static_cast<CSceneComponent*>(pCom.get())->SetInstance(true);
	static_cast<CSceneComponent*>(pCom.get())->SetMesh("Goblin");
	static_cast<CSceneComponent*>(pCom.get())->AddRenderState("OutLineObject");

	if (!CMonster3D::Init())
		return false;

	m_pAni = CAnimation::CreateAnimation<CAnimation>("Gobline Animation",
		"GoblinBone", "Goblin\\AN_NPC_MOB_TL_Goblin_Common_AS_HSN.bne");
	m_pAni->SetSkeleton("GoblinBone");
	m_pAni->AddAnimationSequence("MonsterWalk", "Goblin_Walk_F");
	m_pAni->AddAnimationSequence("MonsterWait", "Goblin_Idle_C");
	m_pAni->AddAnimationSequence("MonsterRun", "Goblin_Run_F");
	m_pAni->AddAnimationSequence("MonsterAttack", "Goblin_ATK_01");
	m_pAni->AddAnimationSequence("MonsterDeath", "Goblin_Die");
	m_pAni->AddAnimationSequence("flinch", "Goblin_DMG_F");
	m_pAni->AddAnimationSequence("DeadBody", "Goblin_DeadBody");
	m_pAni->CreateNotify("MonsterAttack", "Attack", 28.6f);
	m_pAni->CreateNotify("MonsterDeath", "Death", 0.f);
	m_pAni->CreateNotify("MonsterWalk", "Walk1", 23.3f);
	m_pAni->CreateNotify("MonsterWalk", "Walk2", 24.4f);
	m_pAni->SetTimeScale(2.f);

	m_pAni->SetSkeleton("GoblinBone");

	static_cast<CSceneComponent*>(pCom.get())->SetAnimation(m_pAni);
	static_cast<CSceneComponent*>(pCom.get())->SetFrustumCulling(true);

	std::shared_ptr<CComponent> pCollider = CreateComponent<CColliderSphere>("GoblinBody");

	static_cast<CColliderSphere*>(pCollider.get())->SetRadius(12.f);
	static_cast<CColliderSphere*>(pCollider.get())->SetInstance(true);

	static_cast<CSceneComponent*>(pCom.get())->AddChild(pCollider);
	static_cast<CColliderSphere*>(pCollider.get())->AddRelativePosY(4.5f);

	m_pPaper = CreateComponent<CPaperBurn>("Paper");

	static_cast<CSceneComponent*>(pCom.get())->AddObjComponent(m_pPaper);
	// ÁÖ¸»±îÁö ÀüÅõ´Ù »Ì¾Æ³ö

	return true;
}

void CGoblin::Start()
{
	CMonster3D::Start();
}

void CGoblin::Update(float fTime)
{
	CMonster3D::Update(fTime);


	if (m_bDie)
	{
		std::string strName = m_pAni->GetSequence()->GetName();

		if (strName != "Goblin_DeadBody" &&
			strName != "Goblin_Die")
		{
			m_pState->Attacked(this, fTime);
		}
	}
	else
	{
		if (m_pPlayer)
		{
			Vector3 vTargetPos = m_pPlayer->GetBoneWorldPos();

			Vector3 vPos = GetWorldPos();

			Vector3 vDir = vTargetPos - vPos;

			float fDist = vDir.Length();

			if (fDist < 150.f)
			{
				std::string strName = m_pAni->GetSequence()->GetName();
				if (m_pAni->IsEnd())
				{
					Attack(fTime);
				}
				else if (strName == "Goblin_Walk_F" ||
					strName == "Goblin_Idle_C")
				{
					Attack(fTime);
				}
			}
			else
			{
				std::string strName = m_pAni->GetSequence()->GetName();
				if (m_pAni->IsEnd())
				{
					Move(fTime);
				}
				else if (strName == "Goblin_Walk_F" ||
					strName == "Goblin_Idle_C")
				{
					Move(fTime);
				}
			}

			vDir /= fDist;

			Vector3 vAxisX = Vector3::Axis[AXIS_X];

			float fAngle = RadToDeg(acosf(vAxisX.Dot(vDir)));

			if (vDir.z < 0.f)
				fAngle *= -1.f;

			SetWorldRotY(-fAngle);
		}
		else
		{
			CLayer* pLayer = m_pScene->FindLayer(ALBEDO_LAYER);

			std::shared_ptr<CGameObject> pPlayer = pLayer->FindObj("Player");

			if (pPlayer)
			{
				Vector3 vTargetPos = pPlayer->GetBoneWorldPos();

				Vector3 vPos = GetWorldPos();

				float fDist = (vTargetPos - vPos).Length();

				if (fDist <= 1000.f)
				{
					m_pPlayer = pPlayer;

					m_bFight = true;
				}
			}
			if (m_fIdleTime > 0.f)
			{
				m_fIdleTime -= fTime;
				Idle(fTime);
			}
			else
			{
				m_fWalkTime += fTime;

				if (m_fWalkTime >= m_fWalkTimeLimit)
				{
					m_fWalkTime -= m_fWalkTimeLimit;
					m_fIdleTime += (rand() % 300) / 100.f + 3.f;
				}

				if (m_pAni->IsEnd())
				{
					Move(fTime);
				}
				else if (m_pAni->GetSequence()->GetName() == "Goblin_Walk_F")
				{
					Move(fTime);
				}
			}
		}
	}
}

void CGoblin::PostUpdate(float fTime)
{
	CMonster3D::PostUpdate(fTime);
}

void CGoblin::Collision(float fTime)
{
	CMonster3D::Collision(fTime);
}

void CGoblin::PreRender(float fTime)
{
	CMonster3D::PreRender(fTime);
}

void CGoblin::Render(float fTime)
{
	CMonster3D::Render(fTime);
}

void CGoblin::PostRender(float fTime)
{
	CMonster3D::PostRender(fTime);
}

std::shared_ptr<CGameObject> CGoblin::Clone()
{
	return std::shared_ptr<CGameObject>(new CGoblin(*this));
}

void CGoblin::Save(FILE* pFile)
{
	CGameObject::Save(pFile);
}

void CGoblin::Load(FILE* pFile)
{
	CGameObject::Load(pFile);
}

void CGoblin::CollisionInit(CCollider* pSrc, CCollider* pDest, float fTime)
{
}

void CGoblin::CollisionStay(CCollider* pSrc, CCollider* pDest, float fTime)
{
}

void CGoblin::CollisionEnd(CCollider* pSrc, CCollider* pDest, float fTime)
{
}

void CGoblin::Move(float fTime)
{
	CMonster3D::Move(fTime);
}

void CGoblin::Attack(float fTime)
{
	CMonster3D::Attack(fTime);
}

void CGoblin::AttackAround(float fTime)
{
	CMonster3D::AttackAround(fTime);
}

void CGoblin::HeavyAttack(float fTime)
{
	CMonster3D::HeavyAttack(fTime);
}

void CGoblin::LongAttack(float fTime)
{
	CMonster3D::LongAttack(fTime);
}

void CGoblin::Idle(float fTime)
{
	CMonster3D::Idle(fTime);
}

bool CGoblin::Transition()
{
	return false;
}

bool CGoblin::DMGToDeath()
{
	return m_iHP <= 0;
}

void CGoblin::ThrowRock(float fTime)
{
	GET_SINGLE(CSoundManager)->Play("Goral_Atk01");

	std::shared_ptr<CGameObject> pRock = m_pScene->CreateCloneObj("GoblinRock", "GoblinRock", 
		m_pScene->FindLayer(ALBEDO_LAYER), m_pScene->GetSceneType());

	Vector3 vPos = GetWorldPos();

	pRock->SetWorldPos(vPos + Vector3(0.f, 7.f, 0.f) - GetWorldAxis(AXIS_Z) * 5.f);
	static_cast<CGoblinRock*>(pRock.get())->SetAttackMax(m_iAttackMax);
	static_cast<CGoblinRock*>(pRock.get())->SetAttackMin(m_iAttackMin);
	static_cast<CGoblinRock*>(pRock.get())->SetOwner(this);

	if (m_pPlayer)
	{
		Vector3 vTargetPos = m_pPlayer->GetWorldPos() + Vector3(0.f, 20.f, 0.f);

		Vector3 vDir = vTargetPos - vPos;

		vDir.Normalize();

		Vector3 vAxis = pRock->GetWorldAxis(AXIS_X);

		Vector3 vAxisRot = vAxis.Cross(vDir);

		vAxisRot.Normalize();

		float fAngle = RadToDeg(acosf(vAxis.Dot(vDir)));

		std::shared_ptr<CComponent> pCom = pRock->GetRootComponent();

		std::shared_ptr<CTransform> pTransform = static_cast<CSceneComponent*>(pCom.get())->GetTransform();

		pTransform->SetQuaternionRotAxis(vAxisRot, fAngle);
	}
}

void CGoblin::Death(float fTime)
{
	if (rand() % 2 == 0)
	{
		GET_SINGLE(CSoundManager)->Play("Goral_Death00");
	}
	else
	{
		GET_SINGLE(CSoundManager)->Play("Goral_Death01");
	}
}

void CGoblin::Resolve(float fTime)
{
	Destroy();
}

void CGoblin::WalkSound(float fTime)
{
	CCamera* pCam = GET_SINGLE(CCameraManager)->GetMainCam();

	float fDist = (pCam->GetWorldPos() - GetWorldPos()).Length();

	float fPercent = 1.f - (fDist / 1000.f) * (fDist / 1000.f);

	if (rand() % 2 == 0)
	{
		GET_SINGLE(CSoundManager)->Vol("Goral_Run00", fPercent);
		GET_SINGLE(CSoundManager)->Play("Goral_Run00");
	}
	else
	{
		GET_SINGLE(CSoundManager)->Vol("Goral_Run01", fPercent);
		GET_SINGLE(CSoundManager)->Play("Goral_Run01");
	}
}

void CGoblin::ChangeSequenceAll(const char* pSequence)
{
	m_pAni->ChangeSequence(pSequence);
}