// ColliderCircleDlg.cpp: 구현 파일
//

#include "pch.h"
#include "GameEditor.h"
#include "ColliderCircleDlg.h"
#include "afxdialogex.h"
#include "Component/ColliderCircle.h"

// CColliderCircleDlg 대화 상자

IMPLEMENT_DYNAMIC(CColliderCircleDlg, CDialogEx)

CColliderCircleDlg::CColliderCircleDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_COLLIDERCIRCLE, pParent)
	, m_fR(0)
	, m_vPos()
	, m_pCC(nullptr)
	, m_iHeight(0)
	, m_iWidth(0)
{

}

CColliderCircleDlg::~CColliderCircleDlg()
{
}

void CColliderCircleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_RADIUS, m_fR);
	DDX_Text(pDX, IDC_EDIT_CENTER_X, m_vPos.x);
	DDX_Text(pDX, IDC_EDIT_CENTER_Y, m_vPos.y);
	DDX_Text(pDX, IDC_EDIT_CENTER_Z, m_vPos.z);
}


BEGIN_MESSAGE_MAP(CColliderCircleDlg, CDialogEx)
	ON_EN_CHANGE(IDC_EDIT_RADIUS, &CColliderCircleDlg::OnEnChangeEditRadius)
	ON_EN_CHANGE(IDC_EDIT_CENTER_X, &CColliderCircleDlg::OnEnChangeEditCenterX)
	ON_EN_CHANGE(IDC_EDIT_CENTER_Y, &CColliderCircleDlg::OnEnChangeEditCenterY)
	ON_EN_CHANGE(IDC_EDIT_CENTER_Z, &CColliderCircleDlg::OnEnChangeEditCenterZ)
	ON_EN_CHANGE(IDC_EDIT_CENTER_Y, &CColliderCircleDlg::OnEnChangeEditCenterY)
	ON_EN_CHANGE(IDC_EDIT_CENTER_Z, &CColliderCircleDlg::OnEnChangeEditCenterZ)
END_MESSAGE_MAP()


// CColliderCircleDlg 메시지 처리기


void CColliderCircleDlg::OnEnChangeEditRadius()
{
	UpdateData(TRUE);

	((CColliderCircle*)m_pCC.get())->SetRadius(m_fR);
}


void CColliderCircleDlg::OnEnChangeEditCenterX()
{
	UpdateData(TRUE);

	((CSceneComponent*)m_pCC.get())->SetWorldPos(m_vPos);
}


void CColliderCircleDlg::OnEnChangeEditCenterY()
{
	UpdateData(TRUE);

	((CSceneComponent*)m_pCC.get())->SetWorldPos(m_vPos);
}


void CColliderCircleDlg::OnEnChangeEditCenterZ()
{
	UpdateData(TRUE);

	((CSceneComponent*)m_pCC.get())->SetWorldPos(m_vPos);
}

void CColliderCircleDlg::SetCollider(std::shared_ptr<class CComponent> pCC)
{
	m_pCC = pCC;

	SphereInfo tInfo = ((CColliderCircle*)m_pCC.get())->GetInfo();

	m_vPos = tInfo.vCenter;
	m_fR = tInfo.fRadius;

	UpdateData(FALSE);
}

int CColliderCircleDlg::GetHeight() const
{
	return m_iHeight;
}

int CColliderCircleDlg::GetWidth() const
{
	return m_iWidth;
}


BOOL CColliderCircleDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	RECT tRC = {};

	GetClientRect(&tRC);

	m_iWidth = tRC.right - tRC.left;
	m_iHeight = tRC.bottom - tRC.top;

	return TRUE;  
				  
}
