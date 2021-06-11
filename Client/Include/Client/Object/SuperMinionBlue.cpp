#include "SuperMinionBlue.h"
#include "Resource/ResourceManager.h"
#include "Component/Renderer.h"

CSuperMinionBlue::CSuperMinionBlue()
{
}

CSuperMinionBlue::CSuperMinionBlue(const CSuperMinionBlue& obj)	:
	CGameObject(obj)
{
}

CSuperMinionBlue::~CSuperMinionBlue()
{
}

bool CSuperMinionBlue::Init()
{
	if (!CGameObject::Init())
		return false;

	if (!CResourceManager::GetInst()->LoadMesh(MESH_TYPE::MT_ANIMATION_MESH, "blue_minion_superminion", L"blue_minion_superminion.FBX"))
		return false;

	std::shared_ptr<CComponent> pRenderer = CreateComponent<CRenderer>("MinionRenderer");

	CRenderer* _pRenderer = static_cast<CRenderer*>(pRenderer.get());

	_pRenderer->SetMesh("blue_minion_superminion");
	_pRenderer->SetWorldScale(1.f, 1.f, 1.f);
	_pRenderer->SetSceneComponentType(SCENE_COMPONENT_TYPE::SCT_3D);
	_pRenderer->AddWorldRotX(-90.f);

	SetRootComponent(pRenderer);

	SetWorldPos(-350.f, 150.f, -150.f);

	return true;
}

void CSuperMinionBlue::Start()
{
	CGameObject::Start();
}

void CSuperMinionBlue::Update(float fTime)
{
	CGameObject::Update(fTime);
}

void CSuperMinionBlue::PostUpdate(float fTime)
{
	CGameObject::PostUpdate(fTime);
}

void CSuperMinionBlue::Collision(float fTime)
{
	CGameObject::Collision(fTime);
}

void CSuperMinionBlue::PreRender(float fTime)
{
	CGameObject::PreRender(fTime);
}

void CSuperMinionBlue::Render(float fTime)
{
	CGameObject::Render(fTime);
}

void CSuperMinionBlue::PostRender(float fTime)
{
	CGameObject::PostRender(fTime);
}

std::shared_ptr<CGameObject> CSuperMinionBlue::Clone()
{
	return std::shared_ptr<CGameObject>(new CSuperMinionBlue(*this));
}

void CSuperMinionBlue::Save(FILE * pFile)
{
	CGameObject::Save(pFile);
}

void CSuperMinionBlue::Load(FILE * pFile)
{
	CGameObject::Load(pFile);
}
