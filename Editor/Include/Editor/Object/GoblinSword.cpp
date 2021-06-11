#include "GoblinSword.h"
#include "Component/Renderer.h"
#include "Resource/Animation.h"

CGoblinSword::CGoblinSword()
{
}

CGoblinSword::CGoblinSword(const CGoblinSword& goblin)	:
	CGameObject(goblin)
{
}

CGoblinSword::~CGoblinSword()
{
}

bool CGoblinSword::Init()
{
	if(!CGameObject::Init())
		return false;

	std::shared_ptr<CComponent> pCom = CreateComponent<CSceneComponent>("Renderer");

	static_cast<CSceneComponent*>(pCom.get())->SetMesh("GoblinSword");

	SetRootComponent(pCom);

	CAnimation* pAni = CAnimation::CreateAnimation<CAnimation>("SwordAni", "GoblinSwordBone", "Goblin\\goblin_sword.bne");

	pAni->AddAnimationSequence("attack", "goblin_sword");

	static_cast<CSceneComponent*>(pCom.get())->SetAnimation(pAni);

	SAFE_RELEASE(pAni);

	return true;
}

void CGoblinSword::Start()
{
	CGameObject::Start();
}

void CGoblinSword::Update(float fTime)
{
	CGameObject::Update(fTime);
}

void CGoblinSword::PostUpdate(float fTime)
{
	CGameObject::PostUpdate(fTime);
}

void CGoblinSword::Collision(float fTime)
{
	CGameObject::Collision(fTime);
}

void CGoblinSword::PreRender(float fTime)
{
	CGameObject::PreRender(fTime);
}

void CGoblinSword::Render(float fTime)
{
	CGameObject::Render(fTime);
}

void CGoblinSword::PostRender(float fTime)
{
	CGameObject::PostRender(fTime);
}

std::shared_ptr<CGameObject> CGoblinSword::Clone()
{
	return std::shared_ptr<CGameObject>(new CGoblinSword(*this));
}

void CGoblinSword::Save(FILE* pFile)
{
	CGameObject::Save(pFile);
}

void CGoblinSword::Load(FILE* pFile)
{
	CGameObject::Load(pFile);
}
