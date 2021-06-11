#include "OrderInhibitor.h"
#include "Resource/ResourceManager.h"
#include "Component/Renderer.h"

COrderInhibitor::COrderInhibitor()
{
}

COrderInhibitor::COrderInhibitor(const COrderInhibitor& obj)	:
	CGameObject(obj)
{
}

COrderInhibitor::~COrderInhibitor()
{
}

bool COrderInhibitor::Init()
{
	if (!CGameObject::Init())
		return false;

	if (!CResourceManager::GetInst()->LoadMesh(MESH_TYPE::MT_ANIMATION_MESH, "sruap_orderinhibitor", L"sruap_orderinhibitor.FBX"))
		return false;

	std::shared_ptr<CComponent> pRenderer = CreateComponent<CRenderer>("OrderInhibitor");

	CRenderer* _pRenderer = static_cast<CRenderer*>(pRenderer.get());

	_pRenderer->SetMesh("sruap_orderinhibitor");
	_pRenderer->AddWorldRotX(-90.f);
	_pRenderer->SetWorldScale(0.1f, 0.1f, 0.1f);
	_pRenderer->SetWorldPos(-200.f, -150.f, -350.f);
	_pRenderer->SetSceneComponentType(SCENE_COMPONENT_TYPE::SCT_3D);

	SetRootComponent(pRenderer);

	return true;
}

void COrderInhibitor::Start()
{
	CGameObject::Start();
}

void COrderInhibitor::Update(float fTime)
{
	CGameObject::Update(fTime);
}

void COrderInhibitor::PostUpdate(float fTime)
{
	CGameObject::PostUpdate(fTime);
}

void COrderInhibitor::Collision(float fTime)
{
	CGameObject::Collision(fTime);
}

void COrderInhibitor::PreRender(float fTime)
{
	CGameObject::PreRender(fTime);
}

void COrderInhibitor::Render(float fTime)
{
	CGameObject::Render(fTime);
}

void COrderInhibitor::PostRender(float fTime)
{
	CGameObject::PostRender(fTime);
}

std::shared_ptr<CGameObject> COrderInhibitor::Clone()
{
	return std::shared_ptr<CGameObject>();
}

void COrderInhibitor::Save(FILE* pFile)
{
	CGameObject::Save(pFile);
}

void COrderInhibitor::Load(FILE* pFile)
{
	CGameObject::Load(pFile);
}
