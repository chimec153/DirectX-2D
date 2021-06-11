#include "Marry.h"
#include "Component/SpriteComponent.h"
#include "Component/Sound.h"
#include "Component/TileMap.h"
#include "Resource/Material.h"
#include "Render/Distortion.h"
#include "Device.h"

CMarry::CMarry()	:
	m_pAni(nullptr),
	m_pSnd(nullptr)
{
	m_iObjType = (int)OBJECT_CLASS_TYPE::MARRY;
}

CMarry::CMarry(const CMarry& obj)	:
	CGameObject(obj)
{
	if(obj.m_pAni)
		m_pAni = obj.m_pAni->Clone();

	if (obj.m_pSnd)
		m_pSnd = obj.m_pSnd->Clone();
}

CMarry::~CMarry()
{
}

bool CMarry::Init()
{
	if (!CGameObject::Init())
		return false;

	m_pAni = CreateComponent<CSpriteComponent>("Marry");
	((CSpriteComponent*)m_pAni.get())->SetWorldPos(800.f, 100.f, 0.f);
	((CSpriteComponent*)m_pAni.get())->AddAni2DInfo("Run", "MarryGoRound");
	static_cast<CSpriteComponent*>(m_pAni.get())->SetMesh("Basic");
	static_cast<CSpriteComponent*>(m_pAni.get())->GetMaterial()->SetShader("Distortion2D");
	Resolution tRS = RESOLUTION;
	CDistortion* pDist = static_cast<CSpriteComponent*>(m_pAni.get())->AddPostProcess<CDistortion>("Distort");
	pDist->SetSpeed(10.f);
	pDist->SetWeight(5.f/(float)tRS.iWidth);
	pDist->SetFrequency(30.f);
	SAFE_RELEASE(pDist);

	SetRootComponent(m_pAni);

	m_pSnd = CreateComponent<CSound>("Sound");
	((CSound*)m_pSnd.get())->SetSound(SOUND_TYPE::BGM, "MarryGoRound");

	((CSpriteComponent*)m_pAni.get())->AddChild(m_pSnd);

	/*CTileMap* pMap = CreateComponent<CTileMap>("TileMap");

	pMap->CreateTile(TILE_TYPE::ISOMETRIC, 10, 10, Vector3::Zero, Vector2(17, 10));

	m_pAni->AddChild(pMap);

	SAFE_RELEASE(pMap);*/

	return true;
}

void CMarry::Start()
{
	CGameObject::Start();

	((CSound*)m_pSnd.get())->Play();
}

void CMarry::Update(float fTime)
{
	CGameObject::Update(fTime);
}

void CMarry::PostUpdate(float fTime)
{
	CGameObject::PostUpdate(fTime);
}

void CMarry::Collision(float fTime)
{
	CGameObject::Collision(fTime);
}

void CMarry::PreRender(float fTime)
{
	CGameObject::PreRender(fTime);
}

void CMarry::Render(float fTime)
{
	CGameObject::Render(fTime);
}

void CMarry::PostRender(float fTime)
{
	CGameObject::PostRender(fTime);
}

std::shared_ptr<CGameObject> CMarry::Clone()
{
	return std::shared_ptr<CGameObject>(new CMarry(*this));
}

void CMarry::Save(FILE* pFile)
{
	CGameObject::Save(pFile);
}

void CMarry::Load(FILE* pFile)
{
	CGameObject::Load(pFile);

	m_pSnd = FindComponentByType<CSound>();

	m_pAni = FindComponentByType<CSpriteComponent>();
}
