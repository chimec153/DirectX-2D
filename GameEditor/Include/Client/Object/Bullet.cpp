#include "Bullet.h"
#include "Resource/Mesh2D.h"
#include "Component/Mesh2DComponent.h"
#include "Resource/Material.h"

CBullet::CBullet()	:
	m_fSpeed(800.f),
	m_fDist(0.f),
	m_fLimitDist(600.f),
	m_pMesh(nullptr)
{
}

CBullet::CBullet(const CBullet& bullet)	:
	CGameObject(bullet)
{
	m_fSpeed = bullet.m_fSpeed;
	m_fDist = bullet.m_fDist;
	m_fLimitDist = bullet.m_fLimitDist;

	//m_pMesh = bullet.m_pMesh->Clone();
}

CBullet::~CBullet()
{
	SAFE_RELEASE(m_pMesh);
}

bool CBullet::Init()
{
	m_pMesh = CreateComponent<CMesh2DComponent>("Mesh");

	SetRootComponent(m_pMesh);

	m_pMesh->SetRelativeScale(50.f, 50.f, 1.f);
	m_pMesh->SetPivot(0.5f, 0.5f, 0.f);

	CMaterial* pMaterial = m_pMesh->GetMaterial();

	pMaterial->SetDiffuseColor(0.1f, 1.f, 1.f, 1.f);

	SAFE_RELEASE(pMaterial);

	return true;
}

void CBullet::Start()
{
}

void CBullet::Input(float fTime)
{
}

void CBullet::Update(float fTime)
{
	CGameObject::Update(fTime);

	m_pMesh->AddRelativePos(GetRelativeAxis(AXIS_Y) * m_fSpeed * fTime);

	m_fDist += m_fSpeed * fTime;

	if (m_fDist >= m_fLimitDist)
	{
		Destroy();
	}
}

void CBullet::PostUpdate(float fTime)
{
	CGameObject::PostUpdate(fTime);
}

void CBullet::Collision(float fTime)
{
	CGameObject::Collision(fTime);
}

void CBullet::PreRender(float fTime)
{
	CGameObject::PreRender(fTime);
}

void CBullet::Render(float fTime)
{
	CGameObject::Render(fTime);
}

void CBullet::PostRender(float fTime)
{
	CGameObject::PostRender(fTime);
}
