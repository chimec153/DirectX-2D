#include "MinionCaster.h"
#include "Resource/ResourceManager.h"
#include "Component/Renderer.h"

CMinionCaster::CMinionCaster()
{
}

CMinionCaster::CMinionCaster(const CMinionCaster& obj)	:
	CGameObject(obj)
{
}

CMinionCaster::~CMinionCaster()
{
}

bool CMinionCaster::Init()
{
	if (!CGameObject::Init())
		return false;

	if (!CResourceManager::GetInst()->LoadMesh(MESH_TYPE::MT_ANIMATION_MESH, "orderminion_caster_tx_cm", L"orderminion_caster_tx_cm.FBX"))
		return false;

	std::shared_ptr<CComponent> pRenderer = CreateComponent<CRenderer>("MinionRenderer");

	CRenderer* _pRenderer = static_cast<CRenderer*>(pRenderer.get());

	_pRenderer->SetMesh("orderminion_caster_tx_cm");
	_pRenderer->SetWorldScale(1.f, 1.f, 1.f);
	_pRenderer->SetSceneComponentType(SCENE_COMPONENT_TYPE::SCT_3D);
	_pRenderer->AddWorldRotX(-90.f);

	SetRootComponent(pRenderer);

	SetWorldPos(-250.f, 150.f, -150.f);

	return true;
}

void CMinionCaster::Start()
{
	CGameObject::Start();
}

void CMinionCaster::Update(float fTime)
{
	CGameObject::Update(fTime);
}

void CMinionCaster::PostUpdate(float fTime)
{
	CGameObject::PostUpdate(fTime);
}

void CMinionCaster::Collision(float fTime)
{
	CGameObject::Collision(fTime);
}

void CMinionCaster::PreRender(float fTime)
{
	CGameObject::PreRender(fTime);
}

void CMinionCaster::Render(float fTime)
{
	CGameObject::Render(fTime);
}

void CMinionCaster::PostRender(float fTime)
{
	CGameObject::PostRender(fTime);
}

std::shared_ptr<CGameObject> CMinionCaster::Clone()
{
	return std::shared_ptr<CGameObject>(new CMinionCaster(*this));
}

void CMinionCaster::Save(FILE* pFile)
{
	CGameObject::Save(pFile);
}

void CMinionCaster::Load(FILE* pFile)
{
	CGameObject::Load(pFile);
}
