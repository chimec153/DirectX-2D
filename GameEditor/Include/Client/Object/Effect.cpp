#include "Effect.h"
#include "Component/SpriteComponent.h"

CEffect::CEffect()
{
	m_iObjType = (int)OBJECT_CLASS_TYPE::EFFECT;
}

CEffect::CEffect(const CEffect& eft)	:
	CGameObject(eft)
{
}

CEffect::~CEffect()
{
}

bool CEffect::Init()
{
	if (!CGameObject::Init())
		return false;

	std::shared_ptr<CComponent> pEft = CreateComponent<CSpriteComponent>("Effect");

	((CSpriteComponent*)pEft.get())->AddAni2DInfo("Expld", "Explode", 1.f, true);
	((CSpriteComponent*)pEft.get())->AddNotify("Expld", "End", 17);
	((CSpriteComponent*)pEft.get())->DeleteSprite("Expld");
	((CSpriteComponent*)pEft.get())->SetPivot(0.5f, 0.5f, 0.f);
	//pEft->SetInstance();

	SetRootComponent(pEft);

	return true;
}

void CEffect::Start()
{
	CGameObject::Start();
}

void CEffect::Update(float fTime)
{
	CGameObject::Update(fTime);
}

void CEffect::PostUpdate(float fTime)
{
	CGameObject::PostUpdate(fTime);
}

void CEffect::Collision(float fTime)
{
	CGameObject::Collision(fTime);
}

void CEffect::PreRender(float fTime)
{
	CGameObject::PreRender(fTime);
}

void CEffect::Render(float fTime)
{
	CGameObject::Render(fTime);
}

void CEffect::PostRender(float fTime)
{
	CGameObject::PostRender(fTime);
}

std::shared_ptr<CGameObject> CEffect::Clone()
{
	return std::shared_ptr<CGameObject>(new CEffect(*this));
}

void CEffect::CallNotify(const std::string& strTag)
{
	if (strTag == "End")
	{
		Destroy();
	}
}

void CEffect::Save(FILE* pFile)
{
	CGameObject::Save(pFile);
}

void CEffect::Load(FILE* pFile)
{
	CGameObject::Load(pFile);
}
