// CheckBoxDlg.cpp: 구현 파일
//

#include "pch.h"
#include "GameEditor.h"
#include "CheckBoxDlg.h"
#include "afxdialogex.h"


// CCheckBoxDlg 대화 상자

IMPLEMENT_DYNAMIC(CCheckBoxDlg, CDialogEx)

CCheckBoxDlg::CCheckBoxDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_UI_CHECK, pParent)
{

}

CCheckBoxDlg::~CCheckBoxDlg()
{
}

void CCheckBoxDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

int CCheckBoxDlg::GetWidth() const
{
	return m_iWidth;
}

int CCheckBoxDlg::GetHeight() const
{
	return m_iHeight;
}

void CCheckBoxDlg::SetComponent(std::shared_ptr<class CComponent> pCom)
{
	m_pCom = pCom;
}


BEGIN_MESSAGE_MAP(CCheckBoxDlg, CDialogEx)
END_MESSAGE_MAP()


// CCheckBoxDlg 메시지 처리기


BOOL CCheckBoxDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	RECT tRC = {};

	GetClientRect(&tRC);

	m_iWidth = tRC.right - tRC.left;
	m_iHeight = tRC.bottom - tRC.top;

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}
