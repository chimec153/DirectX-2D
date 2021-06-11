#include "Mesh2DComponent.h"
#include "../Resource/Mesh.h"
#include "../Scene/Scene.h"
#include "../Resource/ResourceManager.h"
#include "../Resource/Material.h"
#include "../LayerInstance.h"

CMesh2DComponent::CMesh2DComponent()
{
	m_eSceneComponentType = SCENE_COMPONENT_TYPE::SCT_2D;
	m_iSceneComponentClassType = (int)SCENECOMPONENT_CLASS_TYPE::SCT_MESH2D;
}

CMesh2DComponent::CMesh2DComponent(const CMesh2DComponent& com)	:
	CSceneComponent(com)
{
}

CMesh2DComponent::~CMesh2DComponent()
{
}


bool CMesh2DComponent::Init()
{
	if (!CSceneComponent::Init())
		return false;

	class CMesh* pMesh = GET_SINGLE(CResourceManager)->GetDefaultMesh();

	SetMesh(pMesh);

	SAFE_RELEASE(pMesh);

	return true;
}

void CMesh2DComponent::Start()
{
	CSceneComponent::Start();
}

void CMesh2DComponent::Update(float fTime)
{
	CSceneComponent::Update(fTime);
}

void CMesh2DComponent::PostUpdate(float fTime)
{
	CSceneComponent::PostUpdate(fTime);
}

void CMesh2DComponent::Collision(float fTime)
{
	CSceneComponent::Collision(fTime);
}

void CMesh2DComponent::PreRender(float fTime)
{
	CSceneComponent::PreRender(fTime);
}

void CMesh2DComponent::Render(float fTime)
{
	CSceneComponent::Render(fTime);
}

void CMesh2DComponent::PostRender(float fTime)
{
	CSceneComponent::PostRender(fTime);
}

std::shared_ptr<CComponent> CMesh2DComponent::Clone()
{
	return std::shared_ptr<CComponent>(new CMesh2DComponent(*this));
}

void CMesh2DComponent::Save(FILE* pFile)
{
	CSceneComponent::Save(pFile);
}

void CMesh2DComponent::Load(FILE* pFile)
{
	CSceneComponent::Load(pFile);
}
