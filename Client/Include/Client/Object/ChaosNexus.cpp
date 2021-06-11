#include "ChaosNexus.h"
#include "Resource/ResourceManager.h"
#include "Component/Renderer.h"

CChaosNexus::CChaosNexus()
{
}

CChaosNexus::CChaosNexus(const CChaosNexus& obj)	:
	CGameObject(obj)
{
}

CChaosNexus::~CChaosNexus()
{
}

bool CChaosNexus::Init()
{
	if (!CGameObject::Init())
		return false;

	if (!CResourceManager::GetInst()->LoadMesh(MESH_TYPE::MT_ANIMATION_MESH, "sruap_chaosnexus", L"sruap_chaosnexus.FBX"))
		return false;

	std::shared_ptr<CComponent> pRenderer = CreateComponent<CRenderer>("ChaosNexus");

	CRenderer* _pRenderer = static_cast<CRenderer*>(pRenderer.get());

	_pRenderer->SetMesh("sruap_chaosnexus");
	_pRenderer->AddWorldRotX(-90.f);
	_pRenderer->SetWorldScale(0.1f, 0.1f, 0.1f);
	_pRenderer->SetWorldPos(-200.f, -150.f, -400.f);
	_pRenderer->SetSceneComponentType(SCENE_COMPONENT_TYPE::SCT_3D);

	SetRootComponent(pRenderer);

	return true;
}

void CChaosNexus::Start()
{
	CGameObject::Start();
}

void CChaosNexus::Update(float fTime)
{
	CGameObject::Update(fTime);
}

void CChaosNexus::PostUpdate(float fTime)
{
	CGameObject::PostUpdate(fTime);
}

void CChaosNexus::Collision(float fTime)
{
	CGameObject::Collision(fTime);
}

void CChaosNexus::PreRender(float fTime)
{
	CGameObject::PreRender(fTime);
}

void CChaosNexus::Render(float fTime)
{
	CGameObject::Render(fTime);
}

void CChaosNexus::PostRender(float fTime)
{
	CGameObject::PostRender(fTime);
}

std::shared_ptr<CGameObject> CChaosNexus::Clone()
{
	return std::shared_ptr<CGameObject>();
}

void CChaosNexus::Save(FILE* pFile)
{
	CGameObject::Save(pFile);
}

void CChaosNexus::Load(FILE* pFile)
{
	CGameObject::Load(pFile);
}
