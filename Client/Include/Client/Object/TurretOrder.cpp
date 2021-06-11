#include "TurretOrder.h"
#include "Resource/ResourceManager.h"
#include "Component/Renderer.h"

CTurretOrder::CTurretOrder()
{
}

CTurretOrder::CTurretOrder(const CTurretOrder& obj)	:
	CGameObject(obj)
{
}

CTurretOrder::~CTurretOrder()
{
}

bool CTurretOrder::Init()
{
	if (!CGameObject::Init())
		return false;

	if (!CResourceManager::GetInst()->LoadMesh(MESH_TYPE::MT_ANIMATION_MESH, "sruap_turret_order1", L"sruap_turret_order1.FBX"))
		return false;

	std::shared_ptr<CComponent> pRenderer = CreateComponent<CRenderer>("TurretOrderRenderer");

	CRenderer* _pRenderer = static_cast<CRenderer*>(pRenderer.get());

	_pRenderer->SetMesh("sruap_turret_order1");
	_pRenderer->AddWorldRotX(-90.f);
	_pRenderer->SetWorldScale(0.1f, 0.1f, 0.1f);
	_pRenderer->SetWorldPos(-200.f, -150.f, -250.f);
	_pRenderer->SetSceneComponentType(SCENE_COMPONENT_TYPE::SCT_3D);

	SetRootComponent(pRenderer);

	return true;
}

void CTurretOrder::Start()
{
	CGameObject::Start();
}

void CTurretOrder::Update(float fTime)
{
	CGameObject::Update(fTime);
}

void CTurretOrder::PostUpdate(float fTime)
{
	CGameObject::PostUpdate(fTime);
}

void CTurretOrder::Collision(float fTime)
{
	CGameObject::Collision(fTime);
}

void CTurretOrder::PreRender(float fTime)
{
	CGameObject::PreRender(fTime);
}

void CTurretOrder::Render(float fTime)
{
	CGameObject::Render(fTime);
}

void CTurretOrder::PostRender(float fTime)
{
	CGameObject::PostRender(fTime);
}

std::shared_ptr<CGameObject> CTurretOrder::Clone()
{
	return std::shared_ptr<CGameObject>(new CTurretOrder(*this));
}

void CTurretOrder::Save(FILE* pFile)
{
	CGameObject::Save(pFile);
}

void CTurretOrder::Load(FILE* pFile)
{
	CGameObject::Load(pFile);
}
