#include "Component.h"
#include "../Object/GameObject.h"
#include "../Scene/Scene.h"

CComponent::CComponent()	:
	m_pObj(nullptr),
	m_pScene(nullptr),
	m_pLayer(nullptr),
	m_eType(COMPONENT_TYPE::CT_SCENE),
	m_bStart(false)
{
}

CComponent::CComponent(const CComponent& com)	:
	CRef(com)
{
	m_bStart = false;

	m_pObj = com.m_pObj;
	m_pScene = com.m_pScene;
	m_pLayer = com.m_pLayer;
	m_eType = com.m_eType;
}

CComponent::~CComponent()
{
}

CGameObject* CComponent::GetObj()	const
{
	return m_pObj;
}
COMPONENT_TYPE CComponent::GetType() const
{
	return m_eType;
}
void CComponent::SetScene(CScene* pScene)
{
	m_pScene = pScene;
}
CLayer* CComponent::GetLayer() const
{
	return m_pLayer;
}
CScene* CComponent::GetScene() const
{
	return m_pScene;
}
bool CComponent::IsStart() const
{
	return m_bStart;
}
bool CComponent::Init()
{
	return true;
}

void CComponent::Start()
{
}

void CComponent::Update(float fTime)
{
}

void CComponent::PostUpdate(float fTime)
{
}

void CComponent::Collision(float fTime)
{
}

void CComponent::PreRender(float fTime)
{
}

void CComponent::Render(float fTime)
{
}

void CComponent::PostRender(float fTime)
{
}

std::shared_ptr<CComponent> CComponent::Clone()
{
	return std::shared_ptr<CComponent>();
}

void CComponent::Save(FILE* pFile)
{
	CRef::Save(pFile);

	fwrite(&m_eType, sizeof(enum::COMPONENT_TYPE), 1, pFile);
}

void CComponent::Load(FILE* pFile)
{
	CRef::Load(pFile);

	fread(&m_eType, sizeof(enum::COMPONENT_TYPE), 1, pFile);
}

void CComponent::ShowWindow()
{
	CRef::ShowWindow();
}
