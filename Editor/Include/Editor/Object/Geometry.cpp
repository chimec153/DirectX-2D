#include "Geometry.h"
#include "Resource/ResourceManager.h"
#include "Component/Renderer.h"

CGeometry::CGeometry()
{
}

CGeometry::CGeometry(const CGeometry& obj) :
	CGameObject(obj)
{
}

CGeometry::~CGeometry()
{
}

bool CGeometry::Init()
{
	if (!CGameObject::Init())
		return false;

	if (!CResourceManager::GetInst()->LoadMesh(MESH_TYPE::MT_STATIC_MESH, "basegeo", L"basegeo.FBX"))
		return false;

	std::shared_ptr<CComponent> pRenderer = CreateComponent<CSceneComponent>("basegeo");

	CSceneComponent* _pRenderer = static_cast<CSceneComponent*>(pRenderer.get());

	_pRenderer->SetMesh("basegeo");
	_pRenderer->SetWorldScale(0.1f, 0.1f, 0.1f);
	_pRenderer->SetSceneComponentType(SCENE_COMPONENT_TYPE::SCT_3D);
	_pRenderer->SetWorldPos(0.f, -500.f, 0.f);
	_pRenderer->SetSceneComponentType(SCENE_COMPONENT_TYPE::SCT_3D);

	SetRootComponent(pRenderer);

	return true;
}

void CGeometry::Start()
{
	CGameObject::Start();
}

void CGeometry::Update(float fTime)
{
	CGameObject::Update(fTime);
}

void CGeometry::PostUpdate(float fTime)
{
	CGameObject::PostUpdate(fTime);
}

void CGeometry::Collision(float fTime)
{
	CGameObject::Collision(fTime);
}

void CGeometry::PreRender(float fTime)
{
	CGameObject::PreRender(fTime);
}

void CGeometry::Render(float fTime)
{
	CGameObject::Render(fTime);
}

void CGeometry::PostRender(float fTime)
{
	CGameObject::PostRender(fTime);
}

std::shared_ptr<CGameObject> CGeometry::Clone()
{
	return std::shared_ptr<CGameObject>();
}

void CGeometry::Save(FILE* pFile)
{
	CGameObject::Save(pFile);
}

void CGeometry::Load(FILE* pFile)
{
	CGameObject::Load(pFile);
}
