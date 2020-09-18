#include "Player.h"
#include "Resource/ResourceManager.h"
#include "Component/Mesh2DComponent.h"
#include "Resource/Material.h"
#include "Bullet.h"
#include "Scene/Scene.h"

CPlayer::CPlayer()	:
	m_pMesh(nullptr),
	m_pChildMesh(nullptr),
	m_fSpeed(200.f)
{
}

CPlayer::CPlayer(const CPlayer& player)	:
	CGameObject(player)
{
}

CPlayer::~CPlayer()
{
	SAFE_RELEASE(m_pMesh);
	SAFE_RELEASE(m_pChildMesh);
}

bool CPlayer::Init()
{
	m_pMesh = CreateComponent<CMesh2DComponent>("Mesh");
	m_pChildMesh = CreateComponent<CMesh2DComponent>("Mesh");

	SetRootComponent(m_pMesh);

	m_pMesh->SetRelativePos(0.f, 0.f, 0.f);
	m_pMesh->SetRelativeScale(200.f, 100.f, 1.f);
	m_pMesh->SetPivot(0.5f, 0.5f, 0.f);

	CMaterial* pMaterial = m_pMesh->GetMaterial();

	pMaterial->SetDiffuseColor(0.7f, 0.7f, 0.7f, 1.f);

	SAFE_RELEASE(pMaterial);
	
	m_pChildMesh->SetRelativePos(-200.f, 0.f, 0.f);
	m_pChildMesh->SetInheritScale(false);
	m_pChildMesh->SetRelativeScale(100.f, 50.f, 1.f);
	m_pChildMesh->SetPivot(0.5f, 0.5f, 0.f);

	m_pMesh->AddChild(m_pChildMesh);

	return true;
}

void CPlayer::Start()
{
	CGameObject::Start();
}

void CPlayer::Input(float fTime)
{
	CGameObject::Input(fTime);

	if (GetAsyncKeyState('W') & 0x8000)
		m_pMesh->AddRelativePos(m_pMesh->GetRelativeAxis(AXIS_Y) * m_fSpeed * fTime);

	if (GetAsyncKeyState('S') & 0x8000)
		m_pMesh->AddRelativePos(m_pMesh->GetRelativeAxis(AXIS_Y) * -m_fSpeed * fTime);

	if (GetAsyncKeyState('A') & 0x8000)
		m_pMesh->AddRelativePos(m_pMesh->GetRelativeAxis(AXIS_X) * -m_fSpeed * fTime);

	if (GetAsyncKeyState('D') & 0x8000)
		m_pMesh->AddRelativePos(m_pMesh->GetRelativeAxis(AXIS_X) * m_fSpeed * fTime);

	if (GetAsyncKeyState('Q') & 0x8000)
		m_pMesh->AddRelativeRotZ(180.f * fTime);

	if (GetAsyncKeyState('E') & 0x8000)
		m_pMesh->AddRelativeRotZ(-180.f * fTime);

	if (GetAsyncKeyState(VK_SPACE) & 0x8000)
	{
		CBullet* pBullet = m_pScene->CreateObject<CBullet>("Bullet");

		pBullet->SetRelativeRot(GetWorldRot());
		pBullet->SetRelativePos(GetWorldPos());
		pBullet->AddRelativePos(pBullet->GetWorldAxis(AXIS_Y) * 150.f);

		SAFE_RELEASE(pBullet);
	}

	if (GetAsyncKeyState(VK_F1) & 0x8000)
	{
		for (int i = 0; i < 3; ++i)
		{
			CBullet* pBullet = m_pScene->CreateObject<CBullet>("Bullet");

			Vector3 vRot = GetWorldRot();

			vRot.z += 30.f * (i - 1);

			pBullet->SetRelativeRot(vRot);
			pBullet->SetRelativePos(GetWorldPos());
			pBullet->AddRelativePos(pBullet->GetWorldAxis(AXIS_Y) * 150.f);

			SAFE_RELEASE(pBullet);
		}
	}

	if (GetAsyncKeyState(VK_F2) & 0x8000)
	{
		for (int i = 0; i < 16; ++i)
		{
			CBullet* pBullet = m_pScene->CreateObject<CBullet>("Bullet");

			Vector3 vRot = GetWorldRot();

			vRot.z += 360.f / 16.f * (i - 1);

			pBullet->SetRelativeRot(vRot);
			pBullet->SetRelativePos(GetWorldPos());
			pBullet->AddRelativePos(pBullet->GetWorldAxis(AXIS_Y) * 150.f);

			SAFE_RELEASE(pBullet);
		}
	}
}

void CPlayer::Update(float fTime)
{
	CGameObject::Update(fTime);
}

void CPlayer::PostUpdate(float fTime)
{
	CGameObject::PostUpdate(fTime);
}

void CPlayer::Collision(float fTime)
{
	CGameObject::Collision(fTime);
}

void CPlayer::PreRender(float fTime)
{
	CGameObject::PreRender(fTime);
}

void CPlayer::Render(float fTime)
{
	CGameObject::Render(fTime);
}

void CPlayer::PostRender(float fTime)
{
	CGameObject::PostRender(fTime);
}
