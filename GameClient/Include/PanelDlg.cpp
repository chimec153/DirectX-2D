// PanelDlg.cpp: 구현 파일
//

#include "pch.h"
#include "GameEditor.h"
#include "PanelDlg.h"
#include "afxdialogex.h"


// CPanelDlg 대화 상자

IMPLEMENT_DYNAMIC(CPanelDlg, CDialogEx)

CPanelDlg::CPanelDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_PANEL, pParent)
{

}

CPanelDlg::~CPanelDlg()
{
}

void CPanelDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

int CPanelDlg::GetWidth() const
{
	return m_iWidth;
}

int CPanelDlg::GetHeight() const
{
	return m_iHeight;
}

void CPanelDlg::SetComponent(std::shared_ptr<class CComponent> pCom)
{
	m_pCom = pCom;
}


BEGIN_MESSAGE_MAP(CPanelDlg, CDialogEx)
END_MESSAGE_MAP()


// CPanelDlg 메시지 처리기


BOOL CPanelDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	RECT tRC = {};

	GetClientRect(&tRC);

	m_iWidth = tRC.right - tRC.left;
	m_iHeight = tRC.bottom - tRC.top;

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}
