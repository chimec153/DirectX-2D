#include "TargetUI.h"
#include "UI/Bar.h"
#include "Scene/Scene.h"
#include "Monster3D.h"
#include "UI/UIImage.h"
#include "../Editor3D.h"

CTargetUI::CTargetUI()
{
	m_iObjType = static_cast<int>(EDITOR_OBJECT_TYPE::TARGETUI);
}

CTargetUI::CTargetUI(const CTargetUI& ui) :
	CGameObject(ui)
{

}
CTargetUI::~CTargetUI()
{

}

bool CTargetUI::Init()
{
	if (!CGameObject::Init())
		return false;


	std::shared_ptr<CComponent> pBarImage = CreateComponent<CUIImage>("BarImage");

	SetRootComponent(pBarImage);

	static_cast<CUIImage*>(pBarImage.get())->SetTexture("BLUITarget_I11");
	static_cast<CUIImage*>(pBarImage.get())->SetUV(Vector2(199.f, 196.f), Vector2(839.f, 312.f));
	static_cast<CUIImage*>(pBarImage.get())->SetWorldScale(640.f, 116.f, 0.f);
	static_cast<CUIImage*>(pBarImage.get())->SetWorldPos(-320.f, 400.f, 0.f);
	static_cast<CUIImage*>(pBarImage.get())->SetZOrder(5);

	std::shared_ptr<CComponent> pBarBack = CreateComponent<CUIImage>("BarBack");

	static_cast<CUIImage*>(pBarImage.get())->AddChild(pBarBack);

	static_cast<CUIImage*>(pBarBack.get())->SetInheritScale(false);
	static_cast<CUIImage*>(pBarBack.get())->SetTexture("BLUITarget_I11");
	static_cast<CUIImage*>(pBarBack.get())->SetUV(Vector2(287.f, 672.f), Vector2(851, 691.f));
	static_cast<CUIImage*>(pBarBack.get())->SetWorldScale(564.f, 19.f, 0.f);
	static_cast<CUIImage*>(pBarBack.get())->SetRelativePos(75.f, 51.f, 0.f);
	static_cast<CUIImage*>(pBarBack.get())->SetZOrder(4);

	m_pBar = CreateComponent<CBar>("HPBar");

	static_cast<CUIImage*>(pBarBack.get())->AddChild(m_pBar);

	static_cast<CBar*>(m_pBar.get())->SetInheritScale(false);
	static_cast<CBar*>(m_pBar.get())->SetTexture("BLUITarget_I11");
	static_cast<CBar*>(m_pBar.get())->SetUV(Vector2(0.f, 772.f), Vector2(497.f, 784.f));
	static_cast<CBar*>(m_pBar.get())->SetWorldScale(497.f, 12.f, 0.f);
	static_cast<CBar*>(m_pBar.get())->SetRelativePos(35.f, 4.f, 0.f);
	static_cast<CBar*>(m_pBar.get())->SetZOrder(3);

	std::shared_ptr<CComponent> pLevelImage = CreateComponent<CUIImage>("LevelImage");

	static_cast<CUIImage*>(pBarImage.get())->AddChild(pLevelImage);

	static_cast<CUIImage*>(pLevelImage.get())->SetInheritScale(false);
	static_cast<CUIImage*>(pLevelImage.get())->SetTexture("BLUITarget_I11");
	static_cast<CUIImage*>(pLevelImage.get())->SetUV(Vector2(665.f, 325.f), Vector2(754, 411.f));
	static_cast<CUIImage*>(pLevelImage.get())->SetWorldScale(89.f, 86.f, 0.f);
	static_cast<CUIImage*>(pLevelImage.get())->SetRelativePos(32.f, 20.f, 0.f);
	static_cast<CUIImage*>(pLevelImage.get())->SetZOrder(2);


	return true;
}

void CTargetUI::Start()
{
	CGameObject::Start();
}

void CTargetUI::Update(float fTime)
{
	CGameObject::Update(fTime);

	if (m_pTarget)
	{
		if (static_cast<CMonster3D*>(m_pTarget)->IsDie())
		{
			SetEnable(false);
		}
		else
		{
			int iHP = static_cast<CMonster3D*>(m_pTarget)->GetHP();
			int iHPMax = static_cast<CMonster3D*>(m_pTarget)->GetHPMax();

			static_cast<CBar*>(m_pBar.get())->SetPrg(iHP / static_cast<float>(iHPMax));
		}
	}
}

void CTargetUI::PostUpdate(float fTime)
{
	CGameObject::PostUpdate(fTime);
}

void CTargetUI::Collision(float fTime)
{
	CGameObject::Collision(fTime);
}

void CTargetUI::PreRender(float fTime)
{
	CGameObject::PreRender(fTime);
}

void CTargetUI::Render(float fTime)
{
	CGameObject::Render(fTime);
}

void CTargetUI::PostRender(float fTime)
{
	CGameObject::PostRender(fTime);
}

std::shared_ptr<CGameObject> CTargetUI::Clone()
{
	return std::shared_ptr<CGameObject>(new CTargetUI(*this));
}

void CTargetUI::Save(FILE* pFile)
{
	CGameObject::Save(pFile);
}

void CTargetUI::Load(FILE* pFile)
{
	CGameObject::Load(pFile);
}