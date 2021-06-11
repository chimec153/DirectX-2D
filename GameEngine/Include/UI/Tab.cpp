#include "Tab.h"
#include "Panel.h"
#include "ChkBx.h"
#include "../Object/GameObject.h"

CTab::CTab()
{
	m_iSceneComponentClassType = (int)SCENECOMPONENT_CLASS_TYPE::UI_TAB;
	m_eSceneComponentType = SCENE_COMPONENT_TYPE::SCT_UI;
}

CTab::CTab(const CTab& ui)	:
	CUIControl(ui)
{
	size_t iPnlSz = ui.m_vecPanel.size();

	for (size_t i = 0; i < iPnlSz; ++i)
	{
		std::string strPanel = ui.m_vecPanel[i]->GetName();
		std::string strBtn = ui.m_vecButton[i]->GetName();

		m_vecPanel.push_back(FindComponent(strPanel));
		m_vecButton.push_back(FindComponent(strBtn));
	}
}

CTab::~CTab()
{
}

std::shared_ptr<CComponent> CTab::GetPanel(int idx)
{
	size_t iSz = m_vecPanel.size();

	if (idx >= iSz)
	{
		char strNum[32] = {};
		sprintf_s(strNum, "Panel%d", idx);

		return FindComponentByType<CPanel>(strNum);
	}

	return m_vecPanel[idx];
}

std::shared_ptr<CComponent> CTab::GetButton(int idx)
{
	size_t iSz = m_vecButton.size();

	if (idx >= iSz)
	{
		char strTab[32] = {};
		sprintf_s(strTab, "Tab%d", idx);

		return FindComponentByType<CChkBx>(strTab);
	}

	return m_vecButton[idx];
}

void CTab::AddTab()
{
	Vector3 vScale = GetWorldScale();
	Vector3 vPivot = GetPivot();

	int iSz = (int)m_vecButton.size();

	char strPanel[32] = {};
	char strTab[32] = {};

	sprintf_s(strPanel, "Panel%d", iSz);
	sprintf_s(strTab, "Tab%d", iSz);

	std::shared_ptr<CComponent> pPanel = m_pObj->CreateComponent<CPanel>(strPanel);
	std::shared_ptr<CComponent> pButton = m_pObj->CreateComponent<CChkBx>(strTab);

	((CChkBx*)pButton.get())->SetBoxCallBack<CTab>(CHECK_STATE::ON, this, &CTab::Tab);

	((CPanel*)pPanel.get())->SetWorldScale(vScale.x, vScale.y - 40.f, vScale.z);
	((CChkBx*)pButton.get())->SetWorldScale(40.f, 40.f, 0.f);

	((CPanel*)pPanel.get())->SetPivot(0.5f, 0.5f, 0.f);
	((CChkBx*)pButton.get())->SetPivot(0.5f, 0.5f, 0.f);

	((CPanel*)pPanel.get())->SetInheritScale(false);
	((CChkBx*)pButton.get())->SetInheritScale(false);

	AddChild(pPanel);
	AddChild(pButton);

	((CChkBx*)pButton.get())->SetRelativePos(20.f-vScale.x * vPivot.x + iSz * 40.f, vScale.y * vPivot.y - 20.f, 0.f);
	((CPanel*)pPanel.get())->SetRelativePos(0.f, -20.f, 0.f);

	m_vecPanel.push_back(pPanel);
	m_vecButton.push_back(pButton);
}

size_t CTab::GetSize() const
{
	return m_vecButton.size();
}

void CTab::SetButtonSize(float x, float y)
{
	Vector3 vScale = GetWorldScale();
	Vector3 vPivot = GetPivot();

	size_t iSz = m_vecButton.size();

	for (size_t i = 0; i < iSz; ++i)
	{
		Vector3 vButtonPivot = ((CChkBx*)m_vecButton[i].get())->GetPivot();

		((CChkBx*)m_vecButton[i].get())->SetRelativePos(x * vPivot.x - vScale.x * vPivot.x + i * x, vScale.y * vPivot.y - y * vButtonPivot.y, 0.f);
		((CPanel*)m_vecPanel[i].get())->SetRelativePos(0.f, -vButtonPivot.y * y, 0.f);
	}
}

void CTab::SetTab()
{
	while (true)
	{
		int iSz = (int)m_vecButton.size();

		char strPanel[32] = {};
		char strTab[32] = {};

		sprintf_s(strPanel, "Panel%d", iSz);
		sprintf_s(strTab, "Tab%d", iSz);

		std::shared_ptr<CComponent> pChkBx = FindComponentByType<CChkBx>(strTab);
		std::shared_ptr<CComponent> pPanel = FindComponentByType<CPanel>(strPanel);

		if (!pChkBx)
			return;

		((CChkBx*)pChkBx.get())->SetBoxCallBack<CTab>(CHECK_STATE::ON, this, &CTab::Tab);

		m_vecButton.push_back(pChkBx);
		m_vecPanel.push_back(pPanel);
	}
}

bool CTab::Init()
{
	if (!CUIControl::Init())
		return false;

	return true;
}

void CTab::Start()
{
	CUIControl::Start();
}

void CTab::Update(float fTime)
{
	CUIControl::Update(fTime);

	size_t iSz = m_vecButton.size();

	for (size_t i = 0; i < iSz; ++i)
	{
		if (((CPanel*)m_vecPanel[i].get())->IsEnable())
		{
			if (!((CChkBx*)m_vecButton[i].get())->IsOn())
				((CChkBx*)m_vecButton[i].get())->Check(true);
		}
	}
}

void CTab::PostUpdate(float fTime)
{
	CUIControl::PostUpdate(fTime);
}

void CTab::Collision(float fTime)
{
	CUIControl::Collision(fTime);
}

void CTab::PreRender(float fTime)
{
	CUIControl::PreRender(fTime);
}

void CTab::Render(float fTime)
{
	CUIControl::Render(fTime);
}

void CTab::PostRender(float fTime)
{
	CUIControl::PostRender(fTime);
}

std::shared_ptr<CComponent> CTab::Clone()
{
	return std::shared_ptr<CComponent>(new CTab(*this));
}

void CTab::Save(FILE* pFile)
{
	CUIControl::Save(pFile);
}

void CTab::Load(FILE * pFile)
{
	CUIControl::Load(pFile);

}

void CTab::Tab(CChkBx* pBtn)
{
	int iZ = GetZOrder();

	size_t iSz = m_vecButton.size();

	for (size_t i = 0; i < iSz; ++i)
	{
		if (m_vecButton[i].get() != pBtn)
		{
			((CChkBx*)m_vecButton[i].get())->SetZOrder(iZ - 1);
			((CChkBx*)m_vecButton[i].get())->Check(false);
			((CPanel*)m_vecPanel[i].get())->SetEnable(false);
		}

		else
		{
			((CChkBx*)m_vecButton[i].get())->SetZOrder(iZ - 2);
			((CPanel*)m_vecPanel[i].get())->SetEnable(true);
		}
	}
}
