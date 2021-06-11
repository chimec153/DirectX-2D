#include "ConstructionWindow.h"
#include "UI/RadioButton.h"
#include "UI/UIButton.h"
#include "UI/ChkBx.h"

CConstructionWindow::CConstructionWindow()
{
	m_iObjType = (int)OBJECT_CLASS_TYPE::CONSTRUCTION_WINDOW;
}

CConstructionWindow::CConstructionWindow(const CConstructionWindow& ui)	:
	CWindow(ui)
{
}

CConstructionWindow::~CConstructionWindow()
{
}

bool CConstructionWindow::Init()
{
	if (!CWindow::Init())
		return false;

	((CSceneComponent*)m_pMainPanel.get())->SetWorldScale(200.f, 400.f, 1.f);

	Vector3 vPanelScale = ((CSceneComponent*)m_pMainPanel.get())->GetWorldScale();
	Vector3 vPanelPivot = ((CSceneComponent*)m_pMainPanel.get())->GetPivot();

	Vector3 vBtnScale = Vector3(20.f, 20.f, 1.f);
	Vector3 vBtnPivot = Vector3(0.5f, 0.5f, 0.f);

	std::shared_ptr<CComponent> pSpeacial = CreateComponent<CUIButton>("Special");

	CUIButton* pSpeacialBtn = (CUIButton*)pSpeacial.get();

	pSpeacialBtn->SetTexture(BUTTON_STATE::OFF, "Panel");
	pSpeacialBtn->SetTexture(BUTTON_STATE::ON, "Panel");
	pSpeacialBtn->SetTexture(BUTTON_STATE::PRESS, "Panel");
	pSpeacialBtn->SetPivot(0.5f, 0.5f, 0.f);
	pSpeacialBtn->SetWorldScale(vPanelScale.x - 30.f, 10.f, 1.f);
	pSpeacialBtn->SetInheritScale(false);

	pSpeacialBtn->SetRelativePos(0.f, vBtnScale.y * 4.f, 0.f);

	((CSceneComponent*)m_pMainPanel.get())->AddChild(pSpeacial);

	std::shared_ptr<CComponent> pDirSmL = CreateComponent<CRadioButton>("DirSmallLeft");

	CRadioButton* pDirSmlBtn = (CRadioButton*)pDirSmL.get();

	pDirSmlBtn->SetTexture(RADIO_STATE::OFF, "RailSmLeft");
	pDirSmlBtn->SetTexture(RADIO_STATE::ON, "RailSmLeft");
	pDirSmlBtn->SetPivot(0.5f, 0.5f, 0.f);
	pDirSmlBtn->SetWorldScale(vBtnScale);
	pDirSmlBtn->SetInheritScale(false);

	Vector3 vDirSmlScale = pDirSmlBtn->GetWorldScale();
	pDirSmlBtn->SetRelativePos(Vector3(0.f, vPanelScale.y * vPanelPivot.y, 0.f) + vDirSmlScale * pDirSmlBtn->GetPivot() + Vector3(-vBtnScale.x*3.f, -vDirSmlScale.y, 0.f));

	((CSceneComponent*)m_pMainPanel.get())->AddChild(pDirSmL);

	std::shared_ptr<CComponent> pDirL = CreateComponent<CRadioButton>("DirLeft");

	CRadioButton* pDirLeft = (CRadioButton*)pDirL.get();

	pDirLeft->SetTexture(RADIO_STATE::OFF, "RailLeft");
	pDirLeft->SetTexture(RADIO_STATE::ON, "RailLeft");
	pDirLeft->SetPivot(0.5f, 0.5f, 0.f);
	pDirLeft->SetWorldScale(vBtnScale);
	pDirLeft->SetInheritScale(false);

	Vector3 vDirLeftScale = pDirLeft->GetWorldScale();
	pDirLeft->SetRelativePos(Vector3(0.f, vPanelScale.y * vPanelPivot.y, 0.f) 
		+ vDirLeftScale * pDirLeft->GetPivot() + Vector3(-vBtnScale.x * 2.f, -vDirLeftScale.y, 0.f));

	((CSceneComponent*)m_pMainPanel.get())->AddChild(pDirL);
	pDirSmlBtn->AddButton(pDirLeft);

	std::shared_ptr<CComponent> pDirLL = CreateComponent<CRadioButton>("DirLongLeft");

	CRadioButton* pDirLLeft = (CRadioButton*)pDirLL.get();

	pDirLLeft->SetTexture(RADIO_STATE::OFF, "RailLongLeft");
	pDirLLeft->SetTexture(RADIO_STATE::ON, "RailLongLeft");
	pDirLLeft->SetPivot(0.5f, 0.5f, 0.f);
	pDirLLeft->SetWorldScale(vBtnScale);
	pDirLLeft->SetInheritScale(false);

	Vector3 vDirLLeftScale = pDirLLeft->GetWorldScale();
	pDirLLeft->SetRelativePos(Vector3(0.f, vPanelScale.y * vPanelPivot.y, 0.f) 
		+ vDirLLeftScale * pDirLLeft->GetPivot() + Vector3(-vBtnScale.x , -vDirLLeftScale.y, 0.f));

	((CSceneComponent*)m_pMainPanel.get())->AddChild(pDirLL);
	pDirSmlBtn->AddButton(pDirLLeft);

	std::shared_ptr<CComponent> pDirUp = CreateComponent<CRadioButton>("DirUp");

	CRadioButton* pDirUpBtn = (CRadioButton*)pDirUp.get();

	pDirUpBtn->SetTexture(RADIO_STATE::OFF, "RailGo");
	pDirUpBtn->SetTexture(RADIO_STATE::ON, "RailGo");
	pDirUpBtn->SetPivot(0.5f, 0.5f, 0.f);
	pDirUpBtn->SetWorldScale(vBtnScale);
	pDirUpBtn->SetInheritScale(false);

	Vector3 vDirUpScale = pDirUpBtn->GetWorldScale();
	pDirUpBtn->SetRelativePos(Vector3(0.f, vPanelScale.y * vPanelPivot.y, 0.f) 
		+ vDirUpScale * pDirUpBtn->GetPivot() + Vector3(0.f, -vDirUpScale.y, 0.f));

	((CSceneComponent*)m_pMainPanel.get())->AddChild(pDirUp);
	pDirSmlBtn->AddButton(pDirUpBtn);

	std::shared_ptr<CComponent> pDirLR = CreateComponent<CRadioButton>("DirLongRight");

	CRadioButton* pDirLRight = (CRadioButton*)pDirLR.get();

	pDirLRight->SetTexture(RADIO_STATE::OFF, "RailLongRight");
	pDirLRight->SetTexture(RADIO_STATE::ON, "RailLongRight");
	pDirLRight->SetPivot(0.5f, 0.5f, 0.f);
	pDirLRight->SetWorldScale(vBtnScale);
	pDirLRight->SetInheritScale(false);

	Vector3 vDirLRightScale = pDirLRight->GetWorldScale();
	pDirLRight->SetRelativePos(Vector3(0.f, vPanelScale.y* vPanelPivot.y, 0.f) 
		+ vDirLRightScale * pDirLRight->GetPivot() + Vector3(vDirLRightScale.x , -vDirLRightScale.y, 0.f));

	((CSceneComponent*)m_pMainPanel.get())->AddChild(pDirLR);
	pDirSmlBtn->AddButton(pDirLRight);

	std::shared_ptr<CComponent> pDirR = CreateComponent<CRadioButton>("DirRight");

	CRadioButton* pDirRight = (CRadioButton*)pDirR.get();

	pDirRight->SetTexture(RADIO_STATE::OFF, "RailRight");
	pDirRight->SetTexture(RADIO_STATE::ON, "RailRight");
	pDirRight->SetPivot(0.5f, 0.5f, 0.f);
	pDirRight->SetWorldScale(vBtnScale);
	pDirRight->SetInheritScale(false);

	Vector3 vDirRightScale = pDirLeft->GetWorldScale();
	pDirRight->SetRelativePos(Vector3(0.f, vPanelScale.y* vPanelPivot.y, 0.f) 
		+ vDirRightScale * pDirRight->GetPivot() + Vector3(vDirRightScale.x * 2.f, -vDirRightScale.y, 0.f));

	((CSceneComponent*)m_pMainPanel.get())->AddChild(pDirR);
	pDirSmlBtn->AddButton(pDirRight);

	std::shared_ptr<CComponent> pDirSmR = CreateComponent<CRadioButton>("DirSmallRight");

	CRadioButton* pDirSmlRBtn = (CRadioButton*)pDirSmR.get();

	pDirSmlRBtn->SetTexture(RADIO_STATE::OFF, "RailSmRight");
	pDirSmlRBtn->SetTexture(RADIO_STATE::ON, "RailSmRight");
	pDirSmlRBtn->SetPivot(0.5f, 0.5f, 0.f);
	pDirSmlRBtn->SetWorldScale(vBtnScale);
	pDirSmlRBtn->SetInheritScale(false);

	Vector3 vDirSmlRScale = pDirSmlRBtn->GetWorldScale();
	pDirSmlRBtn->SetRelativePos(Vector3(0.f, vPanelScale.y* vPanelPivot.y, 0.f) 
		+ vDirSmlRScale * pDirSmlRBtn->GetPivot() + Vector3(vDirSmlRScale.x * 3.f, -vDirSmlRScale.y, 0.f));

	((CSceneComponent*)m_pMainPanel.get())->AddChild(pDirSmR);
	pDirSmlBtn->AddButton(pDirSmlRBtn);

	std::shared_ptr<CComponent> pSlpDD = CreateComponent<CRadioButton>("SlopeDownDown");

	CRadioButton* pSlpDDBtn = (CRadioButton*)pSlpDD.get();

	pSlpDDBtn->SetTexture(RADIO_STATE::OFF, "RailDownDown");
	pSlpDDBtn->SetTexture(RADIO_STATE::ON, "RailDownDown");
	pSlpDDBtn->SetPivot(0.5f, 0.5f, 0.f);
	pSlpDDBtn->SetWorldScale(vBtnScale);
	pSlpDDBtn->SetInheritScale(false);

	pSlpDDBtn->SetRelativePos(Vector3(0.f, vPanelScale.y* vPanelPivot.y, 0.f) + vBtnScale * vBtnPivot + Vector3(-vBtnScale.x *2.f-vBtnScale.x/2.f, -vBtnScale.y * 3.f, 0.f));

	((CSceneComponent*)m_pMainPanel.get())->AddChild(pSlpDD);

	std::shared_ptr<CComponent> pSlpD = CreateComponent<CRadioButton>("SlopeDown");

	CRadioButton* pSlpDBtn = (CRadioButton*)pSlpD.get();

	pSlpDBtn->SetTexture(RADIO_STATE::OFF, "RailDown");
	pSlpDBtn->SetTexture(RADIO_STATE::ON, "RailDown");
	pSlpDBtn->SetPivot(0.5f, 0.5f, 0.f);
	pSlpDBtn->SetWorldScale(vBtnScale);
	pSlpDBtn->SetInheritScale(false);

	pSlpDBtn->SetRelativePos(Vector3(0.f, vPanelScale.y* vPanelPivot.y, 0.f) + vBtnScale * vBtnPivot + Vector3(-vBtnScale.x - vBtnScale.x / 2.f, -vBtnScale.y * 3.f, 0.f));

	((CSceneComponent*)m_pMainPanel.get())->AddChild(pSlpD);
	pSlpDDBtn->AddButton(pSlpDBtn);

	std::shared_ptr<CComponent> pSlpN = CreateComponent<CRadioButton>("SlopeNone");

	CRadioButton* pSlpNBtn = (CRadioButton*)pSlpN.get();

	pSlpNBtn->SetTexture(RADIO_STATE::OFF, "RailNone");
	pSlpNBtn->SetTexture(RADIO_STATE::ON, "RailNone");
	pSlpNBtn->SetPivot(0.5f, 0.5f, 0.f);
	pSlpNBtn->SetWorldScale(vBtnScale);
	pSlpNBtn->SetInheritScale(false);

	pSlpNBtn->SetRelativePos(Vector3(0.f, vPanelScale.y* vPanelPivot.y, 0.f) + vBtnScale * vBtnPivot + Vector3(-vBtnScale.x / 2.f, -vBtnScale.y * 3.f, 0.f));

	((CSceneComponent*)m_pMainPanel.get())->AddChild(pSlpN);
	pSlpDDBtn->AddButton(pSlpNBtn);

	std::shared_ptr<CComponent> pSlpU = CreateComponent<CRadioButton>("SlopeUp");

	CRadioButton* pSlpUBtn = (CRadioButton*)pSlpU.get();

	pSlpUBtn->SetTexture(RADIO_STATE::OFF, "RailUp");
	pSlpUBtn->SetTexture(RADIO_STATE::ON, "RailUp");
	pSlpUBtn->SetPivot(0.5f, 0.5f, 0.f);
	pSlpUBtn->SetWorldScale(vBtnScale);
	pSlpUBtn->SetInheritScale(false);

	pSlpUBtn->SetRelativePos(Vector3(0.f, vPanelScale.y* vPanelPivot.y, 0.f) + vBtnScale * vBtnPivot + Vector3(vBtnScale.x / 2.f, -vBtnScale.y * 3.f, 0.f));

	((CSceneComponent*)m_pMainPanel.get())->AddChild(pSlpU);
	pSlpDDBtn->AddButton(pSlpUBtn);

	std::shared_ptr<CComponent> pSlpUU = CreateComponent<CRadioButton>("SlopeUpUp");

	CRadioButton* pSlpUUBtn = (CRadioButton*)pSlpUU.get();

	pSlpUUBtn->SetTexture(RADIO_STATE::OFF, "RailUpUp");
	pSlpUUBtn->SetTexture(RADIO_STATE::ON, "RailUpUp");
	pSlpUUBtn->SetPivot(0.5f, 0.5f, 0.f);
	pSlpUUBtn->SetWorldScale(vBtnScale);
	pSlpUUBtn->SetInheritScale(false);

	pSlpUUBtn->SetRelativePos(Vector3(0.f, vPanelScale.y* vPanelPivot.y, 0.f) + vBtnScale * vBtnPivot + Vector3(vBtnScale.x / 2.f + vBtnScale.x, -vBtnScale.y * 3.f, 0.f));

	((CSceneComponent*)m_pMainPanel.get())->AddChild(pSlpUU);
	pSlpDDBtn->AddButton(pSlpUUBtn);

	std::shared_ptr<CComponent> pSlpChain = CreateComponent<CChkBx>("SlopeChain");

	CChkBx* pSlpShainBtn = (CChkBx*)pSlpChain.get();

	pSlpShainBtn->SetTexture(CHECK_STATE::OFF, "RailChain");
	pSlpShainBtn->SetTexture(CHECK_STATE::ON, "RailChain");
	pSlpShainBtn->SetPivot(0.5f, 0.5f, 0.f);
	pSlpShainBtn->SetWorldScale(vBtnScale);
	pSlpShainBtn->SetInheritScale(false);

	pSlpShainBtn->SetRelativePos(Vector3(0.f, vPanelScale.y* vPanelPivot.y, 0.f) + vBtnScale * vBtnPivot + Vector3(vBtnScale.x / 2.f + vBtnScale.x * 2.f, -vBtnScale.y * 3.f, 0.f));

	((CSceneComponent*)m_pMainPanel.get())->AddChild(pSlpChain);

	std::shared_ptr<CComponent> pRBL = CreateComponent<CRadioButton>("R&BLeft");

	CRadioButton* pRBLBtn = (CRadioButton*)pRBL.get();

	pRBLBtn->SetTexture(RADIO_STATE::OFF, "RollLeft");
	pRBLBtn->SetTexture(RADIO_STATE::ON, "RollLeft");
	pRBLBtn->SetPivot(0.5f, 0.5f, 0.f);
	pRBLBtn->SetWorldScale(vBtnScale);
	pRBLBtn->SetInheritScale(false);

	pRBLBtn->SetRelativePos(Vector3(0.f, vPanelScale.y* vPanelPivot.y, 0.f) + Vector3(-vBtnScale.x, -vBtnScale.y * 5.f, 0.f));

	((CSceneComponent*)m_pMainPanel.get())->AddChild(pRBL);

	std::shared_ptr<CComponent> pRB = CreateComponent<CRadioButton>("R&B");

	CRadioButton* pRBBtn = (CRadioButton*)pRB.get();

	pRBBtn->SetTexture(RADIO_STATE::OFF, "RollNone");
	pRBBtn->SetTexture(RADIO_STATE::ON, "RollNone");
	pRBBtn->SetPivot(0.5f, 0.5f, 0.f);
	pRBBtn->SetWorldScale(vBtnScale);
	pRBBtn->SetInheritScale(false);

	pRBBtn->SetRelativePos(Vector3(0.f, vPanelScale.y* vPanelPivot.y, 0.f) + Vector3(0.f, -vBtnScale.y * 5.f, 0.f));

	((CSceneComponent*)m_pMainPanel.get())->AddChild(pRB);

	std::shared_ptr<CComponent> pRBR = CreateComponent<CRadioButton>("R&BRight");

	CRadioButton* pRBRBtn = (CRadioButton*)pRBR.get();

	pRBRBtn->SetTexture(RADIO_STATE::OFF, "RollRight");
	pRBRBtn->SetTexture(RADIO_STATE::ON, "RollRight");
	pRBRBtn->SetPivot(0.5f, 0.5f, 0.f);
	pRBRBtn->SetWorldScale(vBtnScale);
	pRBRBtn->SetInheritScale(false);

	pRBRBtn->SetRelativePos(Vector3(0.f, vPanelScale.y* vPanelPivot.y, 0.f) + Vector3(vBtnScale.x, -vBtnScale.y * 5.f, 0.f));

	((CSceneComponent*)m_pMainPanel.get())->AddChild(pRBR);

	std::shared_ptr<CComponent> pContruct = CreateComponent<CUIButton>("Construct");

	CUIButton* pConBtn = (CUIButton*)pContruct.get();

	pConBtn->SetTexture(BUTTON_STATE::OFF, "Panel");
	pConBtn->SetTexture(BUTTON_STATE::ON, "Panel");
	pConBtn->SetTexture(BUTTON_STATE::PRESS, "Panel");
	pConBtn->SetTexture(BUTTON_STATE::DISABLE, "Panel");
	pConBtn->SetPivot(0.5f, 0.5f, 0.f);
	pConBtn->SetWorldScale(vPanelScale.x - 5.f, vPanelScale.x + 10.f, 1.f);
	pConBtn->SetInheritScale(false);

	pConBtn->SetRelativePos(0.f, -vBtnScale.y * 3.f, 0.f);

	((CSceneComponent*)m_pMainPanel.get())->AddChild(pContruct);

	std::shared_ptr<CComponent> pBack = CreateComponent<CUIButton>("Back");

	CUIButton* pBackBtn = (CUIButton*)pBack.get();

	pBackBtn->SetTexture(BUTTON_STATE::OFF, "RailPrev");
	pBackBtn->SetTexture(BUTTON_STATE::ON, "RailPrev");
	pBackBtn->SetTexture(BUTTON_STATE::PRESS, "RailPrev");
	pBackBtn->SetTexture(BUTTON_STATE::DISABLE, "RailPrev");
	pBackBtn->SetPivot(0.5f, 0.5f, 0.f);
	pBackBtn->SetWorldScale(vBtnScale);
	pBackBtn->SetInheritScale(false);

	pBackBtn->SetRelativePos(-vBtnScale.x * 3.f, -vBtnScale.y * 8.f, 0.f);

	((CSceneComponent*)m_pMainPanel.get())->AddChild(pBack);

	std::shared_ptr<CComponent> pDest = CreateComponent<CUIButton>("Destruct");

	CUIButton* pDestBtn = (CUIButton*)pDest.get();

	pDestBtn->SetTexture(BUTTON_STATE::OFF, "RailDestruct");
	pDestBtn->SetTexture(BUTTON_STATE::ON, "RailDestruct");
	pDestBtn->SetTexture(BUTTON_STATE::PRESS, "RailDestruct");
	pDestBtn->SetTexture(BUTTON_STATE::DISABLE, "RailDestruct");
	pDestBtn->SetPivot(0.5f, 0.5f, 0.f);
	pDestBtn->SetWorldScale(vBtnScale.x * 2.f, vBtnScale.y, 1.f);
	pDestBtn->SetInheritScale(false);

	pDestBtn->SetRelativePos(0.f, -vBtnScale.y * 8.f, 0.f);

	((CSceneComponent*)m_pMainPanel.get())->AddChild(pDest);

	std::shared_ptr<CComponent> pNext = CreateComponent<CUIButton>("Next");

	CUIButton* pNextBtn = (CUIButton*)pNext.get();

	pNextBtn->SetTexture(BUTTON_STATE::OFF, "RailNext");
	pNextBtn->SetTexture(BUTTON_STATE::ON, "RailNext");
	pNextBtn->SetTexture(BUTTON_STATE::PRESS, "RailNext");
	pNextBtn->SetTexture(BUTTON_STATE::DISABLE, "RailNext");
	pNextBtn->SetPivot(0.5f, 0.5f, 0.f);
	pNextBtn->SetWorldScale(vBtnScale);
	pNextBtn->SetInheritScale(false);

	pNextBtn->SetRelativePos(vBtnScale.x * 3.f, -vBtnScale.y * 8.f, 0.f);

	((CSceneComponent*)m_pMainPanel.get())->AddChild(pNext);

	std::shared_ptr<CComponent> pEntrance = CreateComponent<CUIButton>("Entrance");

	CUIButton* pEntranceBtn = (CUIButton*)pEntrance.get();

	pEntranceBtn->SetTexture(BUTTON_STATE::OFF, "Panel");
	pEntranceBtn->SetTexture(BUTTON_STATE::ON, "Panel");
	pEntranceBtn->SetTexture(BUTTON_STATE::PRESS, "Panel");
	pEntranceBtn->SetTexture(BUTTON_STATE::DISABLE, "Panel");
	pEntranceBtn->SetPivot(0.5f, 0.5f, 0.f);
	pEntranceBtn->SetWorldScale(vPanelScale.x / 2.f, 10.f, 1.f);
	pEntranceBtn->SetInheritScale(false);

	pEntranceBtn->SetRelativePos(-vBtnScale.x * 2.f, -vBtnScale.y * 9.f, 0.f);

	((CSceneComponent*)m_pMainPanel.get())->AddChild(pEntrance);

	std::shared_ptr<CComponent> pExit = CreateComponent<CUIButton>("Exit");

	CUIButton* pExitBtn = (CUIButton*)pExit.get();

	pExitBtn->SetTexture(BUTTON_STATE::OFF, "Panel");
	pExitBtn->SetTexture(BUTTON_STATE::ON, "Panel");
	pExitBtn->SetTexture(BUTTON_STATE::PRESS, "Panel");
	pExitBtn->SetTexture(BUTTON_STATE::DISABLE, "Panel");
	pExitBtn->SetPivot(0.5f, 0.5f, 0.f);
	pExitBtn->SetWorldScale(vPanelScale.x / 2.f, 10.f, 1.f);
	pExitBtn->SetInheritScale(false);
	
	pExitBtn->SetRelativePos(vBtnScale.x * 2.f, -vBtnScale.y * 9.f, 0.f);

	((CSceneComponent*)m_pMainPanel.get())->AddChild(pExit);

	return true;
}

void CConstructionWindow::Start()
{
	CWindow::Start();
}

void CConstructionWindow::Update(float fTime)
{
	CWindow::Update(fTime);
}

void CConstructionWindow::PostUpdate(float fTime)
{
	CWindow::PostUpdate(fTime);
}

void CConstructionWindow::Collision(float fTime)
{
	CWindow::Collision(fTime);
}

void CConstructionWindow::PreRender(float fTime)
{
	CWindow::PreRender(fTime);
}

void CConstructionWindow::Render(float fTime)
{
	CWindow::Render(fTime);
}

void CConstructionWindow::PostRender(float fTime)
{
	CWindow::PostRender(fTime);
}

std::shared_ptr<CGameObject> CConstructionWindow::Clone()
{
	return std::shared_ptr<CGameObject>(new CConstructionWindow(*this));
}

void CConstructionWindow::Save(FILE* pFile)
{
	CWindow::Save(pFile);
}

void CConstructionWindow::Load(FILE* pFile)
{
	CWindow::Load(pFile);
}
