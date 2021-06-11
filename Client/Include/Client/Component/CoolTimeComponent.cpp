#include "CoolTimeComponent.h"
#include "Resource/ShaderManager.h"
#include "Resource/Material.h"
#include "Resource/Mesh.h"

CCoolTimeComponent::CCoolTimeComponent()	:
	m_tCBuffer()
{
}

CCoolTimeComponent::CCoolTimeComponent(const CCoolTimeComponent& com)	:
	CSceneComponent(com)
	, m_tCBuffer(com.m_tCBuffer)
{
}

CCoolTimeComponent::~CCoolTimeComponent()
{
}

bool CCoolTimeComponent::Init()
{
	if(!CSceneComponent::Init())
		return false;

	SetMesh("DefaultMesh2D");
	GetMaterial()->SetShader("CoolTimeShader");

	return true;
}

void CCoolTimeComponent::Start()
{
	CSceneComponent::Start();
}

void CCoolTimeComponent::Update(float fTime)
{
	CSceneComponent::Update(fTime);
}

void CCoolTimeComponent::PostUpdate(float fTime)
{
	CSceneComponent::PostUpdate(fTime);
}

void CCoolTimeComponent::Collision(float fTime)
{
	CSceneComponent::Collision(fTime);
}

void CCoolTimeComponent::PreRender(float fTime)
{
	CSceneComponent::PreRender(fTime);
}

void CCoolTimeComponent::Render(float fTime)
{
	CSceneComponent::Render(fTime);
	GET_SINGLE(CShaderManager)->UpdateCBuffer("Sprite", &m_tCBuffer);
	GetMesh()->Render(fTime);
}

void CCoolTimeComponent::PostRender(float fTime)
{
	CSceneComponent::PostRender(fTime);
}

CCoolTimeComponent* CCoolTimeComponent::Clone(float fTime)
{
	return new CCoolTimeComponent(*this);
}

void CCoolTimeComponent::Save(FILE * pFile)
{
	CSceneComponent::Save(pFile);
}

void CCoolTimeComponent::Load(FILE * pFile)
{
	CSceneComponent::Load(pFile);
}
