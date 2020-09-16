#include "MeshComponent.h"
#include "../Resource/ResourceManager.h"

CMeshComponent::CMeshComponent()
{
}

CMeshComponent::CMeshComponent(const CMeshComponent& com)	:
	CSceneComponent(com)
{
}

CMeshComponent::~CMeshComponent()
{
}

bool CMeshComponent::Init()
{
	if (!CSceneComponent::Init())
		return false;

	return true;
}

void CMeshComponent::Start()
{
	CSceneComponent::Start();
}

void CMeshComponent::Update(float fTime)
{
	CSceneComponent::Update(fTime);
}

void CMeshComponent::PostUpdate(float fTime)
{
	CSceneComponent::PostUpdate(fTime);
}

void CMeshComponent::Collision(float fTime)
{
	CSceneComponent::Collision(fTime);
}

void CMeshComponent::PreRender(float fTime)
{
	CSceneComponent::PreRender(fTime);
}

void CMeshComponent::Render(float fTime)
{
	CSceneComponent::Render(fTime);
}

void CMeshComponent::PostRender(float fTime)
{
	CSceneComponent::PostRender(fTime);
}
