#include "Mesh2DComponent.h"
#include "../Resource/Mesh.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"
#include "../Resource/ResourceManager.h"

CMesh2DComponent::CMesh2DComponent() :
	m_pMesh(nullptr)
{
}

CMesh2DComponent::CMesh2DComponent(const CMesh2DComponent& com)	:
	CMeshComponent(com)
{
	m_pMesh = com.m_pMesh;

	if (m_pMesh)
		m_pMesh->AddRef();
}

CMesh2DComponent::~CMesh2DComponent()
{
	SAFE_RELEASE(m_pMesh);
}

CMesh* CMesh2DComponent::GetMesh() const
{
	if (m_pMesh)
		m_pMesh->AddRef();

	return m_pMesh;
}

void CMesh2DComponent::SetMesh(const std::string& strName)
{
	SAFE_RELEASE(m_pMesh);

	m_pMesh = m_pScene->GetResource()->FindMesh(strName);
}

void CMesh2DComponent::SetMesh(class CMesh* pMesh)
{
	SAFE_RELEASE(m_pMesh);

	m_pMesh = pMesh;

	if (m_pMesh)
		m_pMesh->AddRef();
}

bool CMesh2DComponent::Init()
{
	if (!CMeshComponent::Init())
		return false;

	CMesh* pMesh = (CMesh*)GET_SINGLE(CResourceManager)->GetDefaultMesh();

	SetMesh(pMesh);

	SAFE_RELEASE(pMesh);

	return true;
}

void CMesh2DComponent::Start()
{
	CMeshComponent::Start();
}

void CMesh2DComponent::Update(float fTime)
{
	CMeshComponent::Update(fTime);
}

void CMesh2DComponent::PostUpdate(float fTime)
{
	CMeshComponent::PostUpdate(fTime);
}

void CMesh2DComponent::Collision(float fTime)
{
	CMeshComponent::Collision(fTime);
}

void CMesh2DComponent::PreRender(float fTime)
{
	CMeshComponent::PreRender(fTime);
}

void CMesh2DComponent::Render(float fTime)
{
	CMeshComponent::Render(fTime);
}

void CMesh2DComponent::PostRender(float fTime)
{
	CMeshComponent::PostRender(fTime);
}
