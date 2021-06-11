#include "ChaosInhibitor.h"
#include "Resource/ResourceManager.h"
#include "Component/Renderer.h"

CChaosInhibitor::CChaosInhibitor()
{
}

CChaosInhibitor::CChaosInhibitor(const CChaosInhibitor& obj)	:
	CGameObject(obj)
{
}

CChaosInhibitor::~CChaosInhibitor()
{
}

bool CChaosInhibitor::Init()
{
	if (!CGameObject::Init())
		return false;

	if (!CResourceManager::GetInst()->LoadMesh(MESH_TYPE::MT_ANIMATION_MESH, "sruap_chaosinhibitor", L"sruap_chaosinhibitor.FBX"))
		return false;

	std::shared_ptr<CComponent> pRenderer = CreateComponent<CRenderer>("ChaosInhibitor");

	CRenderer* _pRenderer = static_cast<CRenderer*>(pRenderer.get());

	_pRenderer->SetMesh("sruap_chaosinhibitor");
	_pRenderer->AddWorldRotX(-90.f);
	_pRenderer->SetWorldScale(0.1f, 0.1f, 0.1f);
	_pRenderer->SetWorldPos(-200.f, -150.f, -300.f);
	_pRenderer->SetSceneComponentType(SCENE_COMPONENT_TYPE::SCT_3D);

	SetRootComponent(pRenderer);

	return true;
}

void CChaosInhibitor::Start()
{
	CGameObject::Start();
}

void CChaosInhibitor::Update(float fTime)
{
	CGameObject::Update(fTime);
}

void CChaosInhibitor::PostUpdate(float fTime)
{
	CGameObject::PostUpdate(fTime);
}

void CChaosInhibitor::Collision(float fTime)
{
	CGameObject::Collision(fTime);
}

void CChaosInhibitor::PreRender(float fTime)
{
	CGameObject::PreRender(fTime);
}

void CChaosInhibitor::Render(float fTime)
{
	CGameObject::Render(fTime);
}

void CChaosInhibitor::PostRender(float fTime)
{
	CGameObject::PostRender(fTime);
}

std::shared_ptr<CGameObject> CChaosInhibitor::Clone()
{
	return std::shared_ptr<CGameObject>();
}

void CChaosInhibitor::Save(FILE* pFile)
{
	CGameObject::Save(pFile);
}

void CChaosInhibitor::Load(FILE* pFile)
{
	CGameObject::Load(pFile);
}
