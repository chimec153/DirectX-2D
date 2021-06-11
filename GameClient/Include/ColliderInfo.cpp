#include "pch.h"
#include "GameEditor.h"
#include "ColliderInfo.h"
#include "Component/ColliderRect.h"

#undef new
IMPLEMENT_DYNCREATE(CColliderInfo, CDialogEx)

CColliderInfo::CColliderInfo()	:
	CDialogEx(IDD_COLLIDER_RECT)
	, m_tInfo()
	, m_tExtent()
{

}

CColliderInfo::~CColliderInfo()
{
}


BEGIN_MESSAGE_MAP(CColliderInfo, CDialogEx)
	ON_EN_CHANGE(IDC_EDIT_LEFT, &CColliderInfo::OnEnChangeEditLeft)
	ON_EN_CHANGE(IDC_EDIT_RIGHT, &CColliderInfo::OnEnChangeEditRight)
	ON_EN_CHANGE(IDC_EDIT_BOT, &CColliderInfo::OnEnChangeEditBot)
	ON_EN_CHANGE(IDC_EDIT_EXX, &CColliderInfo::OnEnChangeEditExx)
	ON_EN_CHANGE(IDC_EDIT_EXY, &CColliderInfo::OnEnChangeEditExy)
END_MESSAGE_MAP()

void CColliderInfo::OnDraw(CDC* pDC)
{
}

#ifdef _DEBUG
void CColliderInfo::AssertValid() const
{
	CDialogEx::AssertValid();
}

#ifndef _WIN32_WCE
void CColliderInfo::Dump(CDumpContext& dc) const
{
	CDialogEx::Dump(dc);
}
#endif
#endif

void CColliderInfo::OnEnChangeEditTop()
{
	UpdateData(TRUE);
}


void CColliderInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_TOP, m_tInfo.fT);
	DDX_Text(pDX, IDC_EDIT_LEFT, m_tInfo.fL);
	DDX_Text(pDX, IDC_EDIT_RIGHT, m_tInfo.fR);
	DDX_Text(pDX, IDC_EDIT_BOT, m_tInfo.fB);
	DDX_Text(pDX, IDC_EDIT_EXX, m_tExtent.x);
	DDX_Text(pDX, IDC_EDIT_EXY, m_tExtent.y);
}


void CColliderInfo::OnEnChangeEditLeft()
{
	UpdateData(TRUE);

	
}


void CColliderInfo::OnEnChangeEditRight()
{
	UpdateData(TRUE);
}


void CColliderInfo::OnEnChangeEditBot()
{
	UpdateData(TRUE);
}


void CColliderInfo::OnEnChangeEditExx()
{
	UpdateData(TRUE);

	((CColliderRect*)m_pRC.get())->SetExtend(m_tExtent);
}


void CColliderInfo::OnEnChangeEditExy()
{
	UpdateData(TRUE);

	((CColliderRect*)m_pRC.get())->SetExtend(m_tExtent);
}

void CColliderInfo::SetCollider(std::shared_ptr<CComponent> pRC)
{
	m_pRC = pRC;

	m_tExtent = ((CColliderRect*)pRC.get())->GetExtent();
	m_tInfo = ((CColliderRect*)pRC.get())->GetInfo();

	UpdateData(FALSE);
}

int CColliderInfo::GetHeight() const
{
	return m_iHeight;
}

int CColliderInfo::GetWidth() const
{
	return m_iWidth;
}


BOOL CColliderInfo::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	RECT tRC = {};

	GetClientRect(&tRC);

	m_iHeight = tRC.bottom - tRC.top;

	m_iWidth = tRC.right - tRC.left;

	return TRUE;
				
}
