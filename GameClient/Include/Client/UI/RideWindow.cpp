#include "RideWindow.h"
#include "UI/Tab.h"
#include "UI/Panel.h"
#include "Component/SpriteComponent.h"
#include "UI/UIButton.h"
#include "UI/TitleBar.h"
#include "UI/ChkBx.h"
#include "../GameMode/MainGameMode.h"

CRideWindow::CRideWindow()	:
	m_pTab(nullptr)
{
	m_iObjType = (int)OBJECT_CLASS_TYPE::RIDE_WINDOW;
}

CRideWindow::CRideWindow(const CRideWindow& ui)	:
	CWindow(ui)
{
	m_pTab = std::shared_ptr<CComponent>(FindComponentByType<CTab>());
}

CRideWindow::~CRideWindow()
{
}

bool CRideWindow::Init()
{
	if (!CWindow::Init())
		return false;

	m_pTab = CreateComponent<CTab>("Tab");
	((CTab*)m_pTab.get())->SetZOrder(4);
	((CTab*)m_pTab.get())->SetWorldScale(570.f, 400.f, 0.f);
	((CTab*)m_pTab.get())->SetPivot(0.5f, 0.5f, 0.f);
	((CTab*)m_pTab.get())->AddTab();
	((CTab*)m_pTab.get())->AddTab();
	((CTab*)m_pTab.get())->AddTab();
	((CTab*)m_pTab.get())->AddTab();
	((CTab*)m_pTab.get())->AddTab();
	((CTab*)m_pTab.get())->AddTab();

	const char* pSeq[] =
	{
		"eatIcon",
		"transport",
		"gentleride",
		"coasterIcon",
		"waterride",
		"AmazeIcon"
	};

	for (int i = 0; i < (int)RIDE_TYPE::END; ++i)
	{
		std::shared_ptr<CComponent> pBtn = ((CTab*)m_pTab.get())->GetButton(i);
		std::shared_ptr<CComponent> pPanel = ((CTab*)m_pTab.get())->GetPanel(i);

		std::shared_ptr<CComponent> pCom = CreateComponent<CSpriteComponent>("ButtonSprite");

		((CSpriteComponent*)pCom.get())->SetPivot(0.5f, 0.5f, 0.f);
		((CSpriteComponent*)pCom.get())->AddAni2DInfo("Icon", pSeq[i]);
		((CSpriteComponent*)pCom.get())->SetSceneComponentType(SCENE_COMPONENT_TYPE::SCT_UI);
		((CSceneComponent*)pBtn.get())->AddChild(pCom);

		((CChkBx*)pBtn.get())->SetBoxCallBack<CRideWindow>(CHECK_STATE::ON, this, &CRideWindow::Play);
		((CChkBx*)pBtn.get())->SetTexture(CHECK_STATE::OFF, "Button");
		((CChkBx*)pBtn.get())->SetTexture(CHECK_STATE::ON, "ButtonPush");
		((CChkBx*)pBtn.get())->SetWorldScale(31.f, 27.f, 0.f);

		((CPanel*)pPanel.get())->SetTexture("Panel");
	}

	((CTab*)m_pTab.get())->SetButtonSize(31.f, 27.f);

	((CSceneComponent*)m_pMainPanel.get())->AddChild(m_pTab);
	((CSceneComponent*)m_pMainPanel.get())->SetWorldScale(590.f, 400.f, 0.f);
	
	((CTitleBar*)m_pTitleBar.get())->SetTitle(TEXT("Ride"));

	CUIButton* pBtn = AddButton((int)RIDE_TYPE::STALL);

	pBtn->SetTexture(BUTTON_STATE::OFF, "BURGBMain");
	pBtn->SetTexture(BUTTON_STATE::ON, "BURGBMain");
	pBtn->SetTexture(BUTTON_STATE::PRESS, "BURGBMain");

	pBtn = AddButton((int)RIDE_TYPE::STALL);

	pBtn->SetTexture(BUTTON_STATE::OFF, "DRNKSMain");
	pBtn->SetTexture(BUTTON_STATE::ON, "DRNKSMain");
	pBtn->SetTexture(BUTTON_STATE::PRESS, "DRNKSMain");
	pBtn->AddRelativePos(112.f, 0.f, 0.f);

	pBtn = AddButton((int)RIDE_TYPE::STALL);

	pBtn->SetTexture(BUTTON_STATE::OFF, "INFOKMain");
	pBtn->SetTexture(BUTTON_STATE::ON, "INFOKMain");
	pBtn->SetTexture(BUTTON_STATE::PRESS, "INFOKMain");
	pBtn->AddRelativePos(112.f * 2, 0.f, 0.f);

	pBtn = AddButton((int)RIDE_TYPE::STALL);

	pBtn->SetTexture(BUTTON_STATE::OFF, "TLT1Main");
	pBtn->SetTexture(BUTTON_STATE::ON, "TLT1Main");
	pBtn->SetTexture(BUTTON_STATE::PRESS, "TLT1Main");
	pBtn->AddRelativePos(112.f * 3, 0.f, 0.f);

	pBtn = AddButton((int)RIDE_TYPE::WATERRIDE);

	pBtn->SetTexture(BUTTON_STATE::OFF, "RBOATMain");
	pBtn->SetTexture(BUTTON_STATE::ON, "RBOATMain");
	pBtn->SetTexture(BUTTON_STATE::PRESS, "RBOATMain");

	pBtn->SetCallBack(BUTTON_STATE::PRESS, CMainGameMode::ShowConstructWindow);
	pBtn->SetCallBack(BUTTON_STATE::PRESS, CMainGameMode::CloseRideWindow);

	return true;
}

void CRideWindow::Start()
{
	CWindow::Start();

	((CTab*)m_pTab.get())->SetTab();
}

void CRideWindow::Update(float fTime)
{
	CWindow::Update(fTime);
}

void CRideWindow::PostUpdate(float fTime)
{
	CWindow::PostUpdate(fTime);
}

void CRideWindow::Collision(float fTime)
{
	CWindow::Collision(fTime);
}

void CRideWindow::PreRender(float fTime)
{
	CWindow::PreRender(fTime);
}

void CRideWindow::Render(float fTime)
{
	CWindow::Render(fTime);
}

void CRideWindow::PostRender(float fTime)
{
	CWindow::PostRender(fTime);
}

std::shared_ptr<CGameObject> CRideWindow::Clone()
{
	return std::shared_ptr<CGameObject>(new CRideWindow(*this));
}

void CRideWindow::Save(FILE * pFile)
{
	CWindow::Save(pFile);
}

void CRideWindow::Load(FILE * pFile)
{
	CWindow::Load(pFile);

	m_pTab = FindComponentByType<CTab>();

	for (int i = 0; i < 6; ++i)
	{
		std::shared_ptr<CComponent> pBtn = ((CTab*)m_pTab.get())->GetButton(i);

		((CChkBx*)pBtn.get())->SetBoxCallBack<CRideWindow>(CHECK_STATE::ON, this, &CRideWindow::Play);
	}
}

void CRideWindow::Play(CChkBx* pBtn)
{
	for (int i = 0; i < 6; ++i)
	{
		std::shared_ptr<CComponent> pCom = ((CTab*)m_pTab.get())->GetButton(i);

		CSpriteComponent* pSprite = (CSpriteComponent*)((CSceneComponent*)pCom.get())->FindComponentByType<CSpriteComponent>().get();

		pSprite->Stop();
	}

	CSpriteComponent* pCom = (CSpriteComponent*)pBtn->FindComponentByType<CSpriteComponent>().get();

	pCom->Play();
}

CUIButton* CRideWindow::AddButton(int idx)
{
	std::shared_ptr<CComponent> pPanel = ((CTab*)m_pTab.get())->GetPanel(idx);

	std::vector<CSceneComponent*> vecCom;

	((CSceneComponent*)m_pTab.get())->GetAllComponent(vecCom);

	char strNum[5] = {};

	_itoa_s((int)vecCom.size(), strNum, sizeof(strNum)/sizeof(strNum[0]));

	char strTag[32] = "UIButton";

	strcat_s(strTag, strNum);

	std::shared_ptr<CComponent> pBtn = CreateComponent<CUIButton>(strTag);

	Vector3 vScale = ((CSceneComponent*)pPanel.get())->GetWorldScale();
	Vector3 vPivot = ((CSceneComponent*)pPanel.get())->GetPivot();

	((CSceneComponent*)pBtn.get())->SetWorldScale(112.f, 112.f, 0.f);

	((CSceneComponent*)pBtn.get())->SetPivot(0.5f, 0.5f, 0.f);

	((CSceneComponent*)pBtn.get())->SetRelativePos(-vScale.x *vPivot.x + 56.f, vScale.y * vPivot.y - 56.f, 0.f);

	((CSceneComponent*)pBtn.get())->SetInheritScale(false);
	((CUIButton*)pBtn.get())->SetColor(BUTTON_STATE::OFF, Vector4(0.5f, 0.5f, 0.5f, 1.f));
	((CUIButton*)pBtn.get())->SetColor(BUTTON_STATE::ON, Vector4(1.f, 1.f, 1.f, 1.f));
	((CSceneComponent*)pBtn.get())->SetSceneComponentType(SCENE_COMPONENT_TYPE::SCT_UI);

	((CSceneComponent*)pPanel.get())->AddChild(pBtn);

	return (CUIButton*)pBtn.get();
}
