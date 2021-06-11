#include "Ride.h"
#include "Component/Renderer.h"
#include "Resource/Animation.h"
#include "Player3D.h"
#include "Resource/Material.h"
#include "Scene/Scene.h"

CRide::CRide()	:
	m_pPlayer(nullptr)
	, m_pAni(nullptr)
	, m_fHeight(0.f)
{
}

CRide::CRide(const CRide& ride)	:
	CGameObject(ride)
	, m_pPlayer(nullptr)
	, m_pAni(ride.m_pAni)
	, m_fHeight(ride.m_fHeight)
{
}

CRide::~CRide()
{
	SAFE_RELEASE(m_pAni);
}

void CRide::SetPlayer()
{
	m_pAni->SetSocketObject("Saddle", m_pPlayer);
}

void CRide::ExitPlayer()
{
	m_pAni->DeleteSocketObject("Saddle", m_pPlayer);
}

bool CRide::Init()
{
	if(!CGameObject::Init())
		return false;
	SetShadowAll(true);

	std::shared_ptr<CComponent> pRenderer = CreateComponent<CSceneComponent>("Renderer");

	SetRootComponent(pRenderer);

	static_cast<CSceneComponent*>(pRenderer.get())->SetMesh("BoneDragon");
	SetShadowAll(true);
	static_cast<CSceneComponent*>(pRenderer.get())->SetShader("Alpha");
	static_cast<CSceneComponent*>(pRenderer.get())->AddRenderState(CULL_NONE);
	std::shared_ptr<CMaterial> pMtrl = static_cast<CSceneComponent*>(pRenderer.get())->GetMaterial();

	pMtrl->SetEmissiveTex(false);

	m_pAni = CAnimation::CreateAnimation<CAnimation>("RideAni", "RideBone", "Vehicle\\Vehicle_BoneDragon_Anim.bne");

	m_pAni->SetSkeleton("RideBone");

	static_cast<CSceneComponent*>(pRenderer.get())->SetAnimation(m_pAni);

	m_pAni->AddAnimationSequence("Idle01", "BoneDragon_Idle01");
	m_pAni->AddAnimationSequence("FlyBack", "BoneDragon_FlyBack");
	m_pAni->AddAnimationSequence("FlyDescent", "BoneDragon_FlyDescent");
	m_pAni->AddAnimationSequence("FlyFast", "BoneDragon_FlyFast");
	m_pAni->AddAnimationSequence("FlyForward", "BoneDragon_FlyForward");
	m_pAni->AddAnimationSequence("FlyUp", "BoneDragon_FlyUp");
	m_pAni->AddAnimationSequence("Flywait", "BoneDragon_Flywait");
	m_pAni->AddAnimationSequence("Jump01", "BoneDragon_Jump01");
	m_pAni->AddAnimationSequence("JumpEnd", "BoneDragon_JumpEnd");
	m_pAni->AddAnimationSequence("JumpEndLand", "BoneDragon_JumpEndLand");
	m_pAni->AddAnimationSequence("JumpLoop", "BoneDragon_JumpLoop");
	m_pAni->AddAnimationSequence("JumpStart", "BoneDragon_JumpStart");
	m_pAni->AddAnimationSequence("run", "BoneDragon_run");
	m_pAni->AddAnimationSequence("unarmedwait", "BoneDragon_unarmedwait");
	m_pAni->AddTransition("IdleToFlywait", "Idle01", this, &CRide::IdleToFlyWait, "Flywait");
	m_pAni->AddTransition("IdleToFlyUp", "Idle01", this, &CRide::IdleToFlyUp, "FlyUp");
	m_pAni->AddTransition("FlyWaitToUp", "Flywait", this, &CRide::IdleToFlyUp, "FlyUp");
	m_pAni->AddTransition("FlyWaitToForward", "Flywait", this, &CRide::FlyWaitToForward, "FlyForward");
	m_pAni->AddTransition("FlyWaitToDescent", "Flywait", this, &CRide::FlyWaitToDescent, "FlyDescent");
	m_pAni->AddTransition("FlyWaitToBack", "Flywait", this, &CRide::FlyWaitToBack, "FlyBack");
	m_pAni->AddTransition("FlyForwardToWait", "FlyForward", this, &CRide::FlyForwardToWait, "Flywait");
	m_pAni->AddTransition("FlyDescentToWait", "FlyDescent", this, &CRide::FlyDescentToWait, "Flywait");
	m_pAni->AddTransition("FlyBackToWait", "FlyBack", this, &CRide::FlyForwardToWait, "Flywait");
	m_pAni->AddTransition("FlyUpToWait", "FlyUp", this, &CRide::FlyDescentToWait, "Flywait");
	m_pAni->AddTransition("FlywaitToIdle", "Flywait", this, &CRide::FlyWaitToIdle, "Idle01");

	m_pAni->AddSocket("Saddle", "Bip01-Spine");
	m_pAni->SetSocketOffset("Saddle", Vector3(0.f, 0.f, 0.f));
	m_pAni->SetSocketRotation("Saddle", Vector3(-90.f, 0.f, 0.f));

	return true;
}

void CRide::Start()
{
	CGameObject::Start();

	CLayer* pLayer = m_pScene->FindLayer(ALBEDO_LAYER);
	if (pLayer)
	{
		std::shared_ptr<CGameObject> pPlayer = pLayer->FindObj("Player");

		m_pPlayer = static_cast<CPlayer3D*>(pPlayer.get());
	}
}

void CRide::Update(float fTime)
{
	CGameObject::Update(fTime);
	if (m_pPlayer)
	{
		m_pPlayer->SetWorldPos(0.f, 0.f, 0.f);
		m_pPlayer->SetWorldRotY(0.f);
		m_fHeight = m_pPlayer->GetHeight();

		if (m_fHeight < 0.f)
		{
			AddWorldPosY(-m_fHeight);
		}
	}
}

void CRide::PostUpdate(float fTime)
{
	CGameObject::PostUpdate(fTime);
}

void CRide::Collision(float fTime)
{
	CGameObject::Collision(fTime);
}

void CRide::PreRender(float fTime)
{
	CGameObject::PreRender(fTime);
}

void CRide::Render(float fTime)
{
	CGameObject::Render(fTime);
}

void CRide::PostRender(float fTime)
{
	CGameObject::PostRender(fTime);
}

std::shared_ptr<CGameObject> CRide::Clone()
{
	return std::shared_ptr<CGameObject>(new CRide(*this));
}

void CRide::Save(FILE* pFile)
{
	CGameObject::Save(pFile);
}

void CRide::Load(FILE* pFile)
{
	CGameObject::Load(pFile);
}

bool CRide::WalkToIdle()
{
	return false;
}

bool CRide::IdleToFlyWait()
{

	return  m_fHeight > 0.f;
}

bool CRide::IdleToFlyUp()
{
	if (m_pPlayer)
	{
		return m_pPlayer->GetY() > 0.f;
	}
	return false;
}

bool CRide::FlyWaitToForward()
{
	if (m_pPlayer)
	{
		return m_pPlayer->GetX() > 0.f;
	}
	return false;
}

bool CRide::FlyWaitToDescent()
{
	if (m_pPlayer)
	{
		return m_pPlayer->GetY() < 0.f;
	}
	return false;
}

bool CRide::FlyWaitToBack()
{
	return m_pPlayer->GetX() < 0.f;
}

bool CRide::FlyForwardToWait()
{
	if (m_pPlayer)
	{
		return m_pPlayer->GetX() == 0.f;
	}
	return false;
}

bool CRide::FlyDescentToWait()
{
	if (m_pPlayer)
	{
		return m_pPlayer->GetY() == 0.f;
	}
	return false;
}

bool CRide::FlyWaitToIdle()
{
	return m_fHeight == 0.f;
}
