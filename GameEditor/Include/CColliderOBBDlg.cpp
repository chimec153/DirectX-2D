#include "pch.h"
#include "GameEditor.h"
#include "CColliderOBBDlg.h"
#include "afxdialogex.h"
#include "Component/ColliderOBB2D.h"

IMPLEMENT_DYNAMIC(CColliderOBBDlg, CDialogEx)

CColliderOBBDlg::CColliderOBBDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_COLLIDER_OBB2D, pParent)
	, m_vPos()
	, m_pOC(nullptr)
	, m_iHeight(0)
	, m_iWidth(0)
{
	memset(m_vAxis, 0, sizeof(Vector3) * AXIS_END);
	memset(m_fLength, 0, 4 * AXIS_END);
}

CColliderOBBDlg::~CColliderOBBDlg()
{
}

void CColliderOBBDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_POS_X, m_vPos.x);
	DDX_Text(pDX, IDC_EDIT_POS_Y, m_vPos.y);
	DDX_Text(pDX, IDC_EDIT_POS_Z, m_vPos.z);
	DDX_Text(pDX, IDC_EDIT_AXIS_X_X, m_vAxis[AXIS_X].x);
	DDX_Text(pDX, IDC_EDIT_AXIS_X_Y, m_vAxis[AXIS_X].y);
	DDX_Text(pDX, IDC_EDIT_AXIS_X_Z, m_vAxis[AXIS_X].z);
	DDX_Text(pDX, IDC_EDIT_AXIS_Y_X, m_vAxis[AXIS_Y].x);
	DDX_Text(pDX, IDC_EDIT_AXIS_Y_Y, m_vAxis[AXIS_Y].y);
	DDX_Text(pDX, IDC_EDIT_AXIS_Y_Z, m_vAxis[AXIS_Y].z);
	DDX_Text(pDX, IDC_EDIT_AXIS_Z_X, m_vAxis[AXIS_Z].x);
	DDX_Text(pDX, IDC_EDIT_AXIS_Z_Y, m_vAxis[AXIS_Z].y);
	DDX_Text(pDX, IDC_EDIT_AXIS_Z_Z, m_vAxis[AXIS_Z].z);
	DDX_Text(pDX, IDC_EDIT_LENGTH_X, m_fLength[AXIS_X]);
	DDX_Text(pDX, IDC_EDIT_LENGTH_Y, m_fLength[AXIS_Y]);
	DDX_Text(pDX, IDC_EDIT_LENGTH_Z, m_fLength[AXIS_Z]);
}

BEGIN_MESSAGE_MAP(CColliderOBBDlg, CDialogEx)
	ON_EN_CHANGE(IDC_EDIT_POS_X, &CColliderOBBDlg::OnEnChangeEditPosX)
	ON_EN_CHANGE(IDC_EDIT_POS_Y, &CColliderOBBDlg::OnEnChangeEditPosY)
	ON_EN_CHANGE(IDC_EDIT_POS_Z, &CColliderOBBDlg::OnEnChangeEditPosZ)
	ON_EN_CHANGE(IDC_EDIT_AXIS_X_X, &CColliderOBBDlg::OnEnChangeEditAxisXX)
	ON_EN_CHANGE(IDC_EDIT_AXIS_X_Y, &CColliderOBBDlg::OnEnChangeEditAxisXY)
	ON_EN_CHANGE(IDC_EDIT_AXIS_X_Z, &CColliderOBBDlg::OnEnChangeEditAxisXZ)
	ON_EN_CHANGE(IDC_EDIT_AXIS_Y_X, &CColliderOBBDlg::OnEnChangeEditAxisYX)
	ON_EN_CHANGE(IDC_EDIT_AXIS_Y_Y, &CColliderOBBDlg::OnEnChangeEditAxisYY)
	ON_EN_CHANGE(IDC_EDIT_AXIS_Y_Z, &CColliderOBBDlg::OnEnChangeEditAxisYZ)
	ON_EN_CHANGE(IDC_EDIT_AXIS_Z_X, &CColliderOBBDlg::OnEnChangeEditAxisZX)
	ON_EN_CHANGE(IDC_EDIT_AXIS_Z_Y, &CColliderOBBDlg::OnEnChangeEditAxisZY)
	ON_EN_CHANGE(IDC_EDIT_AXIS_Z_Z, &CColliderOBBDlg::OnEnChangeEditAxisZZ)
	ON_EN_CHANGE(IDC_EDIT_LENGTH_X, &CColliderOBBDlg::OnEnChangeEditLengthX)
	ON_EN_CHANGE(IDC_EDIT_LENGTH_Y, &CColliderOBBDlg::OnEnChangeEditLengthY)
	ON_EN_CHANGE(IDC_EDIT_LENGTH_Z, &CColliderOBBDlg::OnEnChangeEditLengthZ)
END_MESSAGE_MAP()

void CColliderOBBDlg::OnEnChangeEditPosX()
{
	UpdateData(TRUE);


}

void CColliderOBBDlg::OnEnChangeEditPosY()
{
	UpdateData(TRUE);
}

void CColliderOBBDlg::OnEnChangeEditPosZ()
{
	UpdateData(TRUE);
}

void CColliderOBBDlg::OnEnChangeEditAxisXX()
{
	UpdateData(TRUE);
}

void CColliderOBBDlg::OnEnChangeEditAxisXY()
{
	UpdateData(TRUE);
}

void CColliderOBBDlg::OnEnChangeEditAxisXZ()
{
	UpdateData(TRUE);
}

void CColliderOBBDlg::OnEnChangeEditAxisYX()
{
	UpdateData(TRUE);
}

void CColliderOBBDlg::OnEnChangeEditAxisYY()
{
	UpdateData(TRUE);
}

void CColliderOBBDlg::OnEnChangeEditAxisYZ()
{
	UpdateData(TRUE);
}

void CColliderOBBDlg::OnEnChangeEditAxisZX()
{
	UpdateData(TRUE);
}

void CColliderOBBDlg::OnEnChangeEditAxisZY()
{
	UpdateData(TRUE);
}

void CColliderOBBDlg::OnEnChangeEditAxisZZ()
{
	UpdateData(TRUE);
}

void CColliderOBBDlg::OnEnChangeEditLengthX()
{
	UpdateData(TRUE);
}

void CColliderOBBDlg::OnEnChangeEditLengthY()
{
	UpdateData(TRUE);
}

void CColliderOBBDlg::OnEnChangeEditLengthZ()
{
	UpdateData(TRUE);
}

int CColliderOBBDlg::GetHeight() const
{
	return m_iHeight;
}

int CColliderOBBDlg::GetWidth() const
{
	return m_iWidth;
}

void CColliderOBBDlg::SetCollider(std::shared_ptr<CComponent> pOC)
{
	m_pOC = pOC;

	OBBInfo tInfo = ((CColliderOBB2D*)m_pOC.get())->GetInfo();

	m_vPos = tInfo.vPos;
	memcpy(m_vAxis, tInfo.vAxis, sizeof(Vector3) * AXIS_END);
	memcpy(m_fLength, tInfo.fLength, 4 * AXIS_END);
}


BOOL CColliderOBBDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	RECT tRC = {};

	GetClientRect(&tRC);

	m_iWidth = tRC.right - tRC.left;
	m_iHeight = tRC.bottom - tRC.top;

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}
