#include "Minion.h"
#include "Component/Mesh2DComponent.h"
#include "Resource/Material.h"
#include "Component/SpriteComponent.h"
#include "Scene/Scene.h"
#include "Component/ColliderRect.h"
#include "Component/ColliderCircle.h"
#include "Component/ColliderOBB2D.h"
#include "Component/Transform.h"
#include "Component/ColliderLine.h"

CMinion::CMinion()	:
	m_pMesh(nullptr)
{
	m_iObjType = (int)OBJECT_CLASS_TYPE::OCT_MINION;
}

CMinion::CMinion(const CMinion& minion)	:
	CGameObject(minion)
{
	if(minion.m_pMesh)
		m_pMesh = minion.m_pMesh->Clone();
}

CMinion::~CMinion()
{
}

bool CMinion::Init()
{
	if (!CGameObject::Init())
		return false;

	m_pMesh = CreateComponent<CSpriteComponent>("Mesh");

	SetRootComponent(m_pMesh);
	((CSpriteComponent*)m_pMesh.get())->SetTexture(LINK_TYPE::LT_DIF, "minion", TEXT("cuphead_hit_0003.png"),TEXTURE_PATH);
	((CSpriteComponent*)m_pMesh.get())->SetWorldPos(300.f, 300.f, 0.f);
	((CSpriteComponent*)m_pMesh.get())->SetWorldScale(12.f, 12.f, 1.f);
	((CSpriteComponent*)m_pMesh.get())->SetPivot(0.5f, 0.5f, 0.f);

	//CColliderRect* pRC = CreateComponent<CColliderRect>("MonsterBody");

	//pRC->SetInheritScale(false);

	////pRC->SetRadius(50.f);
	////pRC->SetExtend(50.f, 50.f, 0.f);
	//pRC->SetExtend(50.f, 50.f);
	//pRC->SetProFile("Monster");
	//m_pMesh->AddChild(pRC);

	//SAFE_RELEASE(pRC);

	/*CColliderOBB2D* pOC = CreateComponent<CColliderOBB2D>("MonBody");

	pOC->SetExtend(100.f, 40.f, 1.f);

	m_pMesh->AddChild(pOC);

	SAFE_RELEASE(pOC);*/

	std::shared_ptr<CComponent> pCC = CreateComponent<CColliderCircle>("MonBody");

	((CColliderCircle*)pCC.get())->SetRadius(50.f);

	((CSpriteComponent*)m_pMesh.get())->AddChild(pCC);
/*
	CColliderLine* pLC = CreateComponent<CColliderLine>("MonBody");

	pLC->SetWorldScale(40.f, 40.f, 1.f);

	m_pMesh->AddChild(pLC);

	SAFE_RELEASE(pLC);*/

	((CSpriteComponent*)m_pMesh.get())->AddAni2DInfo("Idle", "gentleride");
	((CSpriteComponent*)m_pMesh.get())->AddAni2DInfo("Run", "AnaRun");
	((CSpriteComponent*)m_pMesh.get())->AddAni2DInfo("Attack", "AnaAttack");

	return true;
}

void CMinion::Start()
{
	CGameObject::Start();
}

void CMinion::Input(float fTime)
{
	CGameObject::Input(fTime);
}

void CMinion::Update(float fTime)
{
	CGameObject::Update(fTime);
}

void CMinion::PostUpdate(float fTime)
{
	CGameObject::PostUpdate(fTime);

	/*CGameObject* pObj = m_pScene->GetGameMode()->GetPlayer();

	m_pRootComponent->GetTransform()->LookAt(pObj->GetWorldPos());

	SAFE_RELEASE(pObj);*/
}

void CMinion::Collision(float fTime)
{
	CGameObject::Collision(fTime);
}

void CMinion::PreRender(float fTime)
{
	CGameObject::PreRender(fTime);
}

void CMinion::Render(float fTime)
{
	CGameObject::Render(fTime);
}

void CMinion::PostRender(float fTime)
{
	CGameObject::PostRender(fTime);
}

std::shared_ptr<CGameObject> CMinion::Clone()
{
	return std::shared_ptr<CGameObject>(new CMinion(*this));
}

void CMinion::Fire()
{
	std::shared_ptr<CGameObject> pObj = m_pScene->CreateCloneObj("MinionBullet", "MinionBullet", m_pLayer, m_pScene->GetSceneType());

	pObj->SetWorldPos(GetWorldPos());
	pObj->AddWorldRotZ(90.f);
	pObj->AddWorldPos(GetWorldAxis(AXIS_X) * -(GetWorldScale().x * GetPivot().x + pObj->GetWorldScale().x * pObj->GetPivot().x));
}

void CMinion::CallNotify(const std::string& strTag)
{
	if (strTag == "Fire")
		Fire();
}

void CMinion::Save(FILE* pFile)
{
	CGameObject::Save(pFile);

	std::string strName = m_pMesh->GetName();

	int iLength = (int)strName.length();

	fwrite(&iLength, sizeof(int), 1, pFile);

	fwrite(strName.c_str(), sizeof(char), iLength, pFile);
}

void CMinion::Load(FILE* pFile)
{
	CGameObject::Load(pFile);

	int iLength = 0;

	char strName[256] = {};

	fread(&iLength, sizeof(int), 1, pFile);

	fread(strName, sizeof(char), iLength, pFile);

	std::string strTag = strName;

	m_pMesh = std::shared_ptr<CComponent>(FindComponent(strTag));

}
