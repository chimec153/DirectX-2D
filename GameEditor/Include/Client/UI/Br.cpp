#include "Br.h"
#include "UI/UIImage.h"
#include "UI/Bar.h"

CBr::CBr()	:
	m_pImg(nullptr),
	m_pBr(nullptr),
	m_iProgress(0)
{
	m_iObjType = (int)OBJECT_CLASS_TYPE::BAR;
}

CBr::CBr(const CBr& obj)	:
	CGameObject(obj)
{
	m_pImg = std::shared_ptr<CComponent>(FindComponent("Img"));
	m_pBr = std::shared_ptr<CComponent>(FindComponent("Bar"));
	m_iProgress = obj.m_iProgress;
}

CBr::~CBr()
{
}

void CBr::SetProgress(int iProgress)
{
	((CBar*)m_pBr.get())->SetPrg(iProgress /100.f);
	m_iProgress = iProgress;
}

void CBr::AddProgress()
{
	m_iProgress++;
	if (m_iProgress > 100)
	{
		m_iProgress = 100;
	}
	((CBar*)m_pBr.get())->SetPrg(m_iProgress / 100.f);
}

bool CBr::Init()
{
	if (!CGameObject::Init())
		return false;

	m_pImg = CreateComponent<CUIImage>("Img");
	((CUIImage*)m_pImg.get())->SetTexture("Back");
	((CUIImage*)m_pImg.get())->SetWorldPos(10.f, 200.f, 0.f);
	((CUIImage*)m_pImg.get())->SetWorldScale(200.f, 40.f, 1.f);
	((CUIImage*)m_pImg.get())->SetZOrder(1);
	
	SetRootComponent(m_pImg);

	m_pBr = CreateComponent<CBar>("Bar");
	((CBar*)m_pBr.get())->SetInheritScale(false);
	((CBar*)m_pBr.get())->SetWorldScale(190.f, 30.f, 1.f);
	((CBar*)m_pBr.get())->SetRelativePos(5.f, 5.f, 0.f);
	((CBar*)m_pBr.get())->SetTexture("Bar");
	((CBar*)m_pBr.get())->SetPrg(0.f);

	((CSceneComponent*)m_pImg.get())->AddChild(m_pBr);

	return true;
}

void CBr::Start()
{
	CGameObject::Start();
}

void CBr::Update(float fTime)
{
	CGameObject::Update(fTime);
}

void CBr::PostUpdate(float fTime)
{
	CGameObject::PostUpdate(fTime);
}

void CBr::Collision(float fTime)
{
	CGameObject::Collision(fTime);
}

void CBr::PreRender(float fTime)
{
	CGameObject::PreRender(fTime);
}

void CBr::Render(float fTime)
{
	CGameObject::Render(fTime);
}

void CBr::PostRender(float fTime)
{
	CGameObject::PostRender(fTime);
}

std::shared_ptr<CGameObject> CBr::Clone()
{
	return std::shared_ptr<CGameObject>(new CBr(*this));
}

void CBr::Save(FILE* pFile)
{
	CGameObject::Save(pFile);
}

void CBr::Load(FILE* pFile)
{
	CGameObject::Load(pFile);

	m_pImg = FindComponentByType<CUIImage>();

	m_pBr = FindComponentByType<CBar>();
}
