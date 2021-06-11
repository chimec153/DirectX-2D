#include "MinionBullet.h"
#include "Component/SpriteComponent.h"
#include "Scene/Scene.h"
#include "Resource/Material.h"
#include "Component/ColliderCircle.h"

CMinionBullet::CMinionBullet()	:
	m_pSprite(nullptr),
	m_fDist(0.f),
	m_fLimitDist(1000.f)
{
	m_iObjType = (int)OBJECT_CLASS_TYPE::MINION_BULLET;
}

CMinionBullet::CMinionBullet(const CMinionBullet& bullet)	:
	CGameObject(bullet)
{
	m_fDist = bullet.m_fDist;
	m_fLimitDist = bullet.m_fLimitDist;

	m_pSprite = m_pRootComponent;	
}

CMinionBullet::~CMinionBullet()
{
}

bool CMinionBullet::Init()
{
	SetRootComponent(CreateComponent<CSpriteComponent>("MonBullet"));

	m_pSprite = m_pRootComponent;

	((CSpriteComponent*)m_pSprite.get())->AddAni2DInfo("Idle", "ARRTIdle");
	((CSpriteComponent*)m_pSprite.get())->AddAni2DInfo("explode", "Explode");

	((CSpriteComponent*)m_pSprite.get())->SetWorldScale(12.f, 3.f, 1.f);
	((CSpriteComponent*)m_pSprite.get())->SetPivot(0.5f, 0.5f, 0.f);

	std::shared_ptr<CMaterial> pMat = ((CSpriteComponent*)m_pSprite.get())->GetMaterial();

	pMat->SetDiffuseColor(1.f, 0.f, 0.f, 1.f);

	return true;
}

void CMinionBullet::Start()
{
	CGameObject::Start();
}

void CMinionBullet::Input(float fTime)
{
	CGameObject::Input(fTime);
}

void CMinionBullet::Update(float fTime)
{
	CGameObject::Update(fTime);

	Vector3 vPos = GetWorldPos();

	std::shared_ptr<CGameObject> pObj = m_pScene->GetGameMode()->GetPlayer();

	Vector3 vTarget = pObj->GetWorldPos();

	Vector3 vDir = GetWorldAxis(AXIS_Y);

	vTarget -= vPos;

		float fCross = vTarget.Cross(vDir).z;

		if (fCross < 0)
			AddWorldRotZ(90.f * fTime);

		else
			AddWorldRotZ(90.f * -fTime);

	AddWorldPos(GetWorldAxis(AXIS_Y) * fTime * 700.f);

	m_fDist += 700.f * fTime;

	if (m_fDist >= m_fLimitDist)
		Destroy();
}

void CMinionBullet::PostUpdate(float fTime)
{
	CGameObject::PostUpdate(fTime);
}

void CMinionBullet::Collision(float fTime)
{
	CGameObject::Collision(fTime);
}

void CMinionBullet::PreRender(float fTime)
{
	CGameObject::PreRender(fTime);
}

void CMinionBullet::Render(float fTime)
{
	CGameObject::Render(fTime);
}

void CMinionBullet::PostRender(float fTime)
{
	CGameObject::PostRender(fTime);
}

std::shared_ptr<CGameObject> CMinionBullet::Clone()
{
	return std::shared_ptr<CGameObject>(new CMinionBullet(*this));
}

void CMinionBullet::Save(FILE* pFile)
{
	CGameObject::Save(pFile);

	fwrite(&m_fDist, 4, 1, pFile);
	fwrite(&m_fLimitDist, 4, 1, pFile);
}

void CMinionBullet::Load(FILE* pFile)
{
	CGameObject::Load(pFile);

	m_pSprite = FindComponentByType<CSpriteComponent>();

	fread(&m_fDist, 4, 1, pFile);
	fread(&m_fLimitDist, 4, 1, pFile);
}

void CMinionBullet::ColInit(CCollider* pSrc, CCollider* pDest, float fTime)
{
	((CSpriteComponent*)m_pSprite.get())->ChangeClip("explode");
}
