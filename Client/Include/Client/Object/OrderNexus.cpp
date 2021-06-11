#include "OrderNexus.h"
#include "Resource/ResourceManager.h"
#include "Component/Renderer.h"

COrderNexus::COrderNexus()
{
}

COrderNexus::COrderNexus(const COrderNexus& obj)	:
	CGameObject(obj)
{
}

COrderNexus::~COrderNexus()
{
}

bool COrderNexus::Init()
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
	_pRenderer->SetWorldPos(-200.f, -150.f, -450.f);
	_pRenderer->SetSceneComponentType(SCENE_COMPONENT_TYPE::SCT_3D);

	SetRootComponent(pRenderer);

	return true;
}

void COrderNexus::Start()
{
	CGameObject::Start();
}

void COrderNexus::Update(float fTime)
{
	CGameObject::Update(fTime);
}

void COrderNexus::PostUpdate(float fTime)
{
	CGameObject::PostUpdate(fTime);
}

void COrderNexus::Collision(float fTime)
{
	CGameObject::Collision(fTime);
}

void COrderNexus::PreRender(float fTime)
{
	CGameObject::PreRender(fTime);
}

void COrderNexus::Render(float fTime)
{
	CGameObject::Render(fTime);
}

void COrderNexus::PostRender(float fTime)
{
	CGameObject::PostRender(fTime);
}

std::shared_ptr<CGameObject> COrderNexus::Clone()
{
	return std::shared_ptr<CGameObject>(new COrderNexus(*this));
}

void COrderNexus::Save(FILE* pFile)
{
	CGameObject::Save(pFile);
}

void COrderNexus::Load(FILE* pFile)
{
	CGameObject::Load(pFile);
}
