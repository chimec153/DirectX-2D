#include "GoblinRock.h"
#include "Component/Renderer.h"
#include "Component/ColliderSphere.h"
#include "Player3D.h"
#include "Scene/Scene.h"

CGoblinRock::CGoblinRock()	:
	m_iAttackMax(0)
	 , m_iAttackMin	(0)
	, m_fSpeed(450.f)
	, m_fDist(0.f)
	, m_fLimitDist(500.f)
{
}

CGoblinRock::CGoblinRock(const CGoblinRock& rock)	:
	CGameObject(rock)
	, m_iAttackMax(rock.m_iAttackMax)
	, m_iAttackMin(rock.m_iAttackMin)
	, m_fSpeed(rock.m_fSpeed)
	, m_fDist(rock.m_fDist)
	, m_fLimitDist(rock.m_fLimitDist)
{
	std::shared_ptr<CComponent> pCollider = FindComponentByType<CColliderSphere>();

	static_cast<CColliderSphere*>(pCollider.get())->SetCallBack<CGoblinRock>(
		COLLISION_STATE::CS_INIT, this, &CGoblinRock::CollisionInit);
	static_cast<CColliderSphere*>(pCollider.get())->SetCallBack<CGoblinRock>(
		COLLISION_STATE::CS_STAY, this, &CGoblinRock::CollisionStay);
	static_cast<CColliderSphere*>(pCollider.get())->SetCallBack<CGoblinRock>(
		COLLISION_STATE::CS_LAST, this, &CGoblinRock::CollisionEnd);
}

CGoblinRock::~CGoblinRock()
{
}

bool CGoblinRock::Init()
{
	if(!CGameObject::Init())
		return false;

	std::shared_ptr<CComponent> pCom = CreateComponent<CSceneComponent>("Renderer");

	SetRootComponent(pCom);

	static_cast<CSceneComponent*>(pCom.get())->SetMesh("GoblinRock");

	std::shared_ptr<CComponent> pRock = CreateComponent<CColliderSphere>("GoblinRockBody");

	static_cast<CSceneComponent*>(pCom.get())->AddChild(pRock);

	static_cast<CColliderSphere*>(pRock.get())->SetRadius(1.5f);

	return true;
}

void CGoblinRock::Start()
{
	CGameObject::Start();
}

void CGoblinRock::Update(float fTime)
{
	CGameObject::Update(fTime);

	Vector3 vPos = GetWorldPos();

	if (vPos.y < 0.f)
	{
		DestroyRock();
	}

	float fDist = fTime * m_fSpeed;

	AddWorldPos(GetWorldAxis(AXIS_X) * fDist);

	m_fDist += fDist;

	if (m_fDist >= m_fLimitDist)
	{
		DestroyRock();
	}
}

void CGoblinRock::PostUpdate(float fTime)
{
	CGameObject::PostUpdate(fTime);
}

void CGoblinRock::Collision(float fTime)
{
	CGameObject::Collision(fTime);
}

void CGoblinRock::PreRender(float fTime)
{
	CGameObject::PreRender(fTime);
}

void CGoblinRock::Render(float fTime)
{
	CGameObject::Render(fTime);
}

void CGoblinRock::PostRender(float fTime)
{
	CGameObject::PostRender(fTime);
}

std::shared_ptr<CGameObject> CGoblinRock::Clone()
{
	return std::shared_ptr<CGameObject>(new CGoblinRock(*this));
}

void CGoblinRock::Save(FILE * pFile)
{
	CGameObject::Save(pFile);
}

void CGoblinRock::Load(FILE * pFile)
{
	CGameObject::Load(pFile);
}

void CGoblinRock::CollisionInit(CCollider* pSrc, CCollider* pDest, float fTime)
{
	std::string strDest = pDest->GetName();

	if (strDest == "PlayerBody")
	{
		DestroyRock();
	}
}

void CGoblinRock::CollisionStay(CCollider* pSrc, CCollider* pDest, float fTime)
{
}

void CGoblinRock::CollisionEnd(CCollider* pSrc, CCollider* pDest, float fTime)
{
}

void CGoblinRock::DestroyRock()
{
	Destroy();

	std::shared_ptr<CGameObject> pParticle = m_pScene->CreateCloneObj("DustParticle", "DustParticle",
		m_pScene->FindLayer(PARTICLE_LAYER), m_pScene->GetSceneType());
	if (pParticle)
	{
		pParticle->SetWorldPos(GetWorldPos() + Vector3(0.f, 0.f, 0.f));
	}
}
