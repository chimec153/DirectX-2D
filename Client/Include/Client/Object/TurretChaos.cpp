#include "TurretChaos.h"
#include "Resource/ResourceManager.h"
#include "Component/Renderer.h"

CTurretChaos::CTurretChaos()
{
}

CTurretChaos::CTurretChaos(const CTurretChaos& obj)	:
	CGameObject(obj)
{
}

CTurretChaos::~CTurretChaos()
{
}

bool CTurretChaos::Init()
{
	if (!CGameObject::Init())
		return false;

	if (!CResourceManager::GetInst()->LoadMesh(MESH_TYPE::MT_ANIMATION_MESH, "sruap_turret_chaos1", L"sruap_turret_chaos1.FBX"))
		return false;

	std::shared_ptr<CComponent> pRenderer = CreateComponent<CRenderer>("TurretChaosRenderer");

	CRenderer* _pRenderer = static_cast<CRenderer*>(pRenderer.get());

	_pRenderer->SetMesh("sruap_turret_chaos1");
	_pRenderer->AddWorldRotX(-90.f);
	_pRenderer->SetWorldScale(0.1f, 0.1f, 0.1f);
	_pRenderer->SetWorldPos(-200.f, -150.f, -200.f);
	_pRenderer->SetSceneComponentType(SCENE_COMPONENT_TYPE::SCT_3D);

	SetRootComponent(pRenderer);

	return true;
}

void CTurretChaos::Start()
{
	CGameObject::Start();
}

void CTurretChaos::Update(float fTime)
{
	CGameObject::Update(fTime);
}

void CTurretChaos::PostUpdate(float fTime)
{
	CGameObject::PostUpdate(fTime);
}

void CTurretChaos::Collision(float fTime)
{
	CGameObject::Collision(fTime);
}

void CTurretChaos::PreRender(float fTime)
{
	CGameObject::PreRender(fTime);
}

void CTurretChaos::Render(float fTime)
{
	CGameObject::Render(fTime);
}

void CTurretChaos::PostRender(float fTime)
{
	CGameObject::PostRender(fTime);
}

std::shared_ptr<CGameObject> CTurretChaos::Clone()
{
	return std::shared_ptr<CGameObject>(new CTurretChaos(*this));
}

void CTurretChaos::Save(FILE* pFile)
{
	CGameObject::Save(pFile);
}

void CTurretChaos::Load(FILE* pFile)
{
	CGameObject::Load(pFile);
}
