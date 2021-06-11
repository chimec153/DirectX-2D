#include "Minin.h"
#include "Resource/ResourceManager.h"
#include "Component/Renderer.h"

CMinin::CMinin()
{
}

CMinin::CMinin(const CMinin& obj)	:
	CGameObject(obj)
{
}

CMinin::~CMinin()
{
}

bool CMinin::Init()
{
	if (!CGameObject::Init())
		return false;

	if (!CResourceManager::GetInst()->LoadMesh(MESH_TYPE::MT_ANIMATION_MESH, "armadillo", L"armadillo.FBX"))
		return false;

	std::shared_ptr<CComponent> pRenderer = CreateComponent<CRenderer>("armadillo");

	CRenderer* _pRenderer = static_cast<CRenderer*>(pRenderer.get());

	_pRenderer->SetMesh("armadillo");
	_pRenderer->SetWorldScale(1.f, 1.f, 1.f);
	_pRenderer->SetSceneComponentType(SCENE_COMPONENT_TYPE::SCT_3D);
	_pRenderer->AddWorldRotX(-90.f);

	SetRootComponent(pRenderer);

	SetWorldPos(-200.f, 150.f, -150.f);

	return true;
}

void CMinin::Start()
{
	CGameObject::Start();
}

void CMinin::Update(float fTime)
{
	CGameObject::Update(fTime);
}

void CMinin::PostUpdate(float fTime)
{
	CGameObject::PostUpdate(fTime);
}

void CMinin::Collision(float fTime)
{
	CGameObject::Collision(fTime);
}

void CMinin::PreRender(float fTime)
{
	CGameObject::PreRender(fTime);
}

void CMinin::Render(float fTime)
{
	CGameObject::Render(fTime);
}

void CMinin::PostRender(float fTime)
{
	CGameObject::PostRender(fTime);
}

std::shared_ptr<CGameObject> CMinin::Clone()
{
	return std::shared_ptr<CGameObject>(new CMinin(*this));
}

void CMinin::Save(FILE* pFile)
{
	CGameObject::Save(pFile);
}

void CMinin::Load(FILE* pFile)
{
	CGameObject::Load(pFile);
}
