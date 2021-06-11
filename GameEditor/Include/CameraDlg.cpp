// CameraDlg.cpp: 구현 파일
//

#include "pch.h"
#include "GameEditor.h"
#include "CameraDlg.h"
#include "afxdialogex.h"


// CCameraDlg 대화 상자

IMPLEMENT_DYNAMIC(CCameraDlg, CDialogEx)

CCameraDlg::CCameraDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_CAMERA, pParent)
	, m_fAngle(0)
	, m_fDist(0)
	, m_iType(0)
{

}

CCameraDlg::~CCameraDlg()
{
}

void CCameraDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_ANGLE, m_fAngle);
	DDX_Text(pDX, IDC_EDIT_DIST, m_fDist);
	DDX_Text(pDX, IDC_EDIT_TYPE, m_iType);
}


BEGIN_MESSAGE_MAP(CCameraDlg, CDialogEx)
	ON_EN_CHANGE(IDC_EDIT_ANGLE, &CCameraDlg::OnEnChangeEditAngle)
	ON_EN_CHANGE(IDC_EDIT_DIST, &CCameraDlg::OnEnChangeEditDist)
	ON_EN_CHANGE(IDC_EDIT_TYPE, &CCameraDlg::OnEnChangeEditType)
END_MESSAGE_MAP()


// CCameraDlg 메시지 처리기


void CCameraDlg::OnEnChangeEditAngle()
{
	UpdateData(TRUE);
}


void CCameraDlg::OnEnChangeEditDist()
{
	UpdateData(TRUE);
}


void CCameraDlg::OnEnChangeEditType()
{
	UpdateData(TRUE);
}

int CCameraDlg::GetWidth() const
{
	return m_iWidth;
}

int CCameraDlg::GetHeight() const
{
	return m_iHeight;
}

void CCameraDlg::SetComponent(std::shared_ptr<class CComponent> pCom)
{
	m_pCom = pCom;
}


BOOL CCameraDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	RECT tRC = {};

	GetClientRect(&tRC);

	m_iWidth = tRC.right - tRC.left;
	m_iHeight = tRC.bottom - tRC.top;

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}
