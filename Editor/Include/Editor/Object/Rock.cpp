#include "Rock.h"
#include "Component/Renderer.h"
#include "../Editor3D.h"

CRock::CRock()
{
	m_iObjType = static_cast<int>(EDITOR_OBJECT_TYPE::ROCK);
}

CRock::CRock(const CRock& rock)	:
	CGameObject(rock)
{
}

CRock::~CRock()
{
}

bool CRock::Init()
{
	if (!CGameObject::Init())
		return false;

	std::shared_ptr<CComponent> pCom = CreateComponent<CSceneComponent>("Rock");

	SetRootComponent(pCom);

	CSceneComponent* _pCom = static_cast<CSceneComponent*>(pCom.get());

	_pCom->SetMesh("Rock");

	return true;
}

void CRock::Start()
{
	CGameObject::Start();
}

void CRock::Update(float fTime)
{
	CGameObject::Update(fTime);
}

void CRock::PostUpdate(float fTime)
{
	CGameObject::PostUpdate(fTime);
}

void CRock::Collision(float fTime)
{
	CGameObject::Collision(fTime);
}

void CRock::PreRender(float fTime)
{
	CGameObject::PreRender(fTime);
}

void CRock::Render(float fTime)
{
	CGameObject::Render(fTime);
}

void CRock::PostRender(float fTime)
{
	CGameObject::PostRender(fTime);
}

std::shared_ptr<CGameObject> CRock::Clone()
{
	return std::shared_ptr<CGameObject>(new CRock(*this));
}

void CRock::Save(FILE* pFile)
{
	CGameObject::Save(pFile);
}

void CRock::Load(FILE* pFile)
{
	CGameObject::Load(pFile);
}
