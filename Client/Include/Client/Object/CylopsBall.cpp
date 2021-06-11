#include "CylopsBall.h"
#include "Component/Renderer.h"
#include "Resource/Animation.h"
#include "Resource/ResourceManager.h"
#include "Component/ColliderSphere.h"
#include "Scene/Scene.h"
#include "Component/Particle.h"
#include "Particle3D.h"
#include "Component/Decal.h"
#include "Component/Transform.h"
#include "Sound/SoundManager.h"
#include "CameraManager.h"
#include "Component/Camera.h"

CCylopsBall::CCylopsBall()	:
	m_pBallAni(nullptr)
	, m_bThrow(false)
	, m_fSpeed(800.f)
{
}

CCylopsBall::CCylopsBall(const CCylopsBall& mon) :
	CGameObject(mon)
	, m_pBallAni(mon.m_pBallAni)
	, m_bThrow(mon.m_bThrow)
	, m_fSpeed(mon.m_fSpeed)
{
	std::shared_ptr<CComponent> pRenderer= GetRootComponent();

	m_pBallAni = static_cast<CSceneComponent*>(pRenderer.get())->GetAnimation();

	m_pBallAni->AddTransition("WalkToLongAtk", "MonsterWalk", this, &CCylopsBall::Transition, "longAtk01_Start");
	m_pBallAni->AddTransition("LongAtkToWalk", "LongAtk01_End_A", this, &CCylopsBall::Transition, "MonsterWalk");
	m_pBallAni->AddTransition("LongAtkToEnd", "LongAtk01_Loop", this, &CCylopsBall::Transition, "LongAtk01_End_A");
	m_pBallAni->AddTransition("WalkToLongAtkB", "MonsterWalk", this, &CCylopsBall::Transition, "LongAtk02_End_B");
	m_pBallAni->AutoTransition("LongAtk02_End_B", "LongAtkBToWalk", "MonsterWalk");
	m_pBallAni->AddTransitionNotify("LongAtkBToWalk", "LongAtk02_End_B", this, &CCylopsBall::LongAtkBEnd);

	std::shared_ptr<CComponent> pCollider = FindComponentByType<CColliderSphere>();

	static_cast<CColliderSphere*>(pCollider.get())->SetCallBack<CCylopsBall>(COLLISION_STATE::CS_INIT,
		this, &CCylopsBall::CollisionInit);
	static_cast<CColliderSphere*>(pCollider.get())->SetCallBack<CCylopsBall>(COLLISION_STATE::CS_STAY,
		this, &CCylopsBall::CollisionStay);
	static_cast<CColliderSphere*>(pCollider.get())->SetCallBack<CCylopsBall>(COLLISION_STATE::CS_LAST,
		this, &CCylopsBall::CollisionLast);
}

CCylopsBall::~CCylopsBall()
{
	SAFE_RELEASE(m_pBallAni);
}

void CCylopsBall::Throw()
{
	m_bThrow = true;
}

void CCylopsBall::Punched()
{
	m_pBallAni->ChangeSequence("LongAtk02_End_B");
}

bool CCylopsBall::Init()
{
	GET_SINGLE(CResourceManager)->LoadMesh(MESH_TYPE::MT_ANIMATION_MESH, "MonsterBall", TEXT("Cyclops_Anim\\CyclopsClown_Ball.msh"));

	GET_SINGLE(CSoundManager)->LoadSound("Effect", "Bomb01_Exp", false,
		"Cyclops\\Bomb01_Exp.ogg");

	std::shared_ptr<CComponent> pBall = CreateComponent<CSceneComponent>("Ball");

	SetRootComponent(pBall);

	static_cast<CSceneComponent*>(pBall.get())->SetMesh("MonsterBall");

	m_pBallAni = CAnimation::CreateAnimation<CAnimation>("BallAni", "BallBone", "Cyclops_Anim\\activeMove_End.bne");
	m_pBallAni->SetSkeleton("BallBone");
	static_cast<CSceneComponent*>(pBall.get())->SetAnimation(m_pBallAni);

	m_pBallAni->AddAnimationSequence("MonsterWalk", "Walk_Ball");
	m_pBallAni->AddAnimationSequence("longAtk01_Start", "longAtk01_Start_Ball");
	m_pBallAni->AddAnimationSequence("LongAtk01_Loop", "LongAtk01_Loop_Ball");
	m_pBallAni->AddAnimationSequence("LongAtk01_End_A", "LongAtk01_End_A_Ball");
	m_pBallAni->AddAnimationSequence("LongAtk02_End_B", "Cyclops_LongAtk02_End_B_Ball");
	m_pBallAni->AutoTransition("LongAtkToLoop", "longAtk01_Start", "LongAtk01_Loop");

	std::shared_ptr<CComponent> pBallCollider = CreateComponent<CColliderSphere>("BallBody", m_pScene->FindLayer(COLLIDER_LAYER));

	static_cast<CSceneComponent*>(pBall.get())->AddChild(pBallCollider);

	static_cast<CColliderSphere*>(pBallCollider.get())->SetRadius(25.f);

	return true;
}

void CCylopsBall::Start()
{
	CGameObject::Start();
}

void CCylopsBall::Update(float fTime)
{
	CGameObject::Update(fTime);

	if (m_bThrow)
	{
		Vector3 vAxis = GetWorldAxis(AXIS_Z);

		AddWorldPos(vAxis * fTime * m_fSpeed);
	}

	Vector3 vPos = GetWorldPos();

	if (m_bThrow && vPos.y <= -12.5f)
	{
		GET_SINGLE(CSoundManager)->Play("Bomb01_Exp");

		CCamera* pCam = GET_SINGLE(CCameraManager)->GetMainCam();

		if (pCam)
		{
			pCam->SetShake(true);
			pCam->SetShakeMass(50.f);
			pCam->SetShakePower(5.f);
			pCam->SetShakeMass(50.f);
		}

		Destroy();

		std::shared_ptr<CGameObject> pParticle = m_pScene->CreateCloneObj("RockParticle", "RockParticle", 
			m_pScene->FindLayer(PARTICLE_LAYER), m_pScene->GetSceneType());

		static_cast<CParticle3D*>(pParticle.get())->SetWorldPos(vPos);

		std::shared_ptr<CGameObject> pDecal = m_pScene->CreateCloneObj("Crack","Crack", 
			m_pScene->FindLayer(DECAL_LAYER), m_pScene->GetSceneType());

		pDecal->SetWorldPos(GetWorldPos());

		pParticle = m_pScene->CreateCloneObj("DarkDustParticle", "DarkDustParticle",
			m_pScene->FindLayer(PARTICLE_LAYER), m_pScene->GetSceneType());

		pParticle->SetWorldPos(GetWorldPos() + Vector3(0.f, 2.5f, 0.f));
	}
}

void CCylopsBall::PostUpdate(float fTime)
{
	CGameObject::PostUpdate(fTime);
}

void CCylopsBall::Collision(float fTime)
{
	CGameObject::Collision(fTime);
}

void CCylopsBall::PreRender(float fTime)
{
	CGameObject::PreRender(fTime);
}

void CCylopsBall::Render(float fTime)
{
	CGameObject::Render(fTime);
}

void CCylopsBall::PostRender(float fTime)
{
	CGameObject::PostRender(fTime);
}

std::shared_ptr<CGameObject> CCylopsBall::Clone()
{
	return std::shared_ptr<CGameObject>(new CCylopsBall(*this));
}

void CCylopsBall::Save(FILE* pFile)
{
	CGameObject::Save(pFile);
}

void CCylopsBall::Load(FILE* pFile)
{
	CGameObject::Load(pFile);
}

bool CCylopsBall::Transition()
{
	return false;
}

void CCylopsBall::CollisionInit(CCollider* pSrc, CCollider* pDest, float fTime)
{
	std::string strSrc = pSrc->GetName();
	std::string strDest = pDest->GetName();

	if (strDest == "PlayerBody" && strSrc == "BallBody")
	{
		GET_SINGLE(CSoundManager)->Play("Bomb01_Exp");
		Destroy();


		std::shared_ptr<CGameObject> pParticle = m_pScene->CreateCloneObj("RockParticle","RockParticle", m_pScene->FindLayer(PARTICLE_LAYER), m_pScene->GetSceneType());

		static_cast<CParticle3D*>(pParticle.get())->SetWorldPos(pSrc->GetCrs());
		pParticle = m_pScene->CreateCloneObj("DarkDustParticle", "DarkDustParticle",
			m_pScene->FindLayer(PARTICLE_LAYER), m_pScene->GetSceneType());

		pParticle->SetWorldPos(GetWorldPos() + Vector3(0.f, 2.5f, 0.f));
	}
	else if (strDest == "BladeBody")
	{
		GET_SINGLE(CSoundManager)->Play("Bomb01_Exp");
		Destroy();

		std::shared_ptr<CGameObject> pParticle = m_pScene->CreateCloneObj("RockParticle", "RockParticle", m_pScene->FindLayer(PARTICLE_LAYER), m_pScene->GetSceneType());

		static_cast<CParticle3D*>(pParticle.get())->SetWorldPos(pSrc->GetCrs());

		pParticle = m_pScene->CreateCloneObj("DarkDustParticle", "DarkDustParticle",
			m_pScene->FindLayer(PARTICLE_LAYER), m_pScene->GetSceneType());

		pParticle->SetWorldPos(GetWorldPos() + Vector3(0.f, 2.5f, 0.f));
		/*
		Vector3 vAxis = GetWorldAxis(AXIS_Z);

		Vector3 vCross = pSrc->GetCrs();

		Vector3 vNormal = GetWorldPos() - vCross;

		vNormal.Normalize();

		Vector3 vReflect = vAxis - vNormal.Dot(vAxis) * 2.f * vNormal;

		Vector3 vAngle = vReflect.Cross(vAxis);

		float fAngle = vReflect.Dot(vAxis);

		std::shared_ptr<CComponent> pCom = GetRootComponent();

		std::shared_ptr<CTransform> pTransform = static_cast<CSceneComponent*>(pCom.get())->GetTransform();

		pTransform->AddQuaternionRotAxis(vAngle, RadToDeg(fAngle));*/
	}
}

void CCylopsBall::CollisionStay(CCollider* pSrc, CCollider* pDest, float fTime)
{
}

void CCylopsBall::CollisionLast(CCollider* pSrc, CCollider* pDest, float fTime)
{
}

void CCylopsBall::LongAtkBEnd()
{
	Destroy();
}
