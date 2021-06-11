#include "MinionSiege.h"
#include "Resource/ResourceManager.h"
#include "Component/Renderer.h"

CMinionSiege::CMinionSiege()
{
}

CMinionSiege::CMinionSiege(const CMinionSiege& obj)	:
	CGameObject(obj)
{
}

CMinionSiege::~CMinionSiege()
{
}

bool CMinionSiege::Init()
{
	if (!CGameObject::Init())
		return false;

	if (!CResourceManager::GetInst()->LoadMesh(MESH_TYPE::MT_ANIMATION_MESH, "order_minion_siege_tx_cm", L"order_minion_siege_tx_cm.FBX"))
		return false;

	std::shared_ptr<CComponent> pRenderer = CreateComponent<CRenderer>("MinionRenderer");

	CRenderer* _pRenderer = static_cast<CRenderer*>(pRenderer.get());

	_pRenderer->SetMesh("order_minion_siege_tx_cm");
	_pRenderer->SetWorldScale(1.f, 1.f, 1.f);
	_pRenderer->SetSceneComponentType(SCENE_COMPONENT_TYPE::SCT_3D);
	_pRenderer->AddWorldRotX(-90.f);

	SetRootComponent(pRenderer);

	SetWorldPos(-300.f, 150.f, -150.f);

	return true;
}

void CMinionSiege::Start()
{
	CGameObject::Start();
}

void CMinionSiege::Update(float fTime)
{
	CGameObject::Update(fTime);
}

void CMinionSiege::PostUpdate(float fTime)
{
	CGameObject::PostUpdate(fTime);
}

void CMinionSiege::Collision(float fTime)
{
	CGameObject::Collision(fTime);
}

void CMinionSiege::PreRender(float fTime)
{
	CGameObject::PreRender(fTime);
}

void CMinionSiege::Render(float fTime)
{
	CGameObject::Render(fTime);
}

void CMinionSiege::PostRender(float fTime)
{
	CGameObject::PostRender(fTime);
}

std::shared_ptr<CGameObject> CMinionSiege::Clone()
{
	return std::shared_ptr<CGameObject>(new CMinionSiege(*this));
}

void CMinionSiege::Save(FILE* pFile)
{
	CGameObject::Save(pFile);
}

void CMinionSiege::Load(FILE* pFile)
{
	CGameObject::Load(pFile);
}
