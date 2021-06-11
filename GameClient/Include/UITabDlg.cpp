// UITabDlg.cpp: 구현 파일
//

#include "pch.h"
#include "GameEditor.h"
#include "UITabDlg.h"
#include "afxdialogex.h"


// CUITabDlg 대화 상자

IMPLEMENT_DYNAMIC(CUITabDlg, CDialogEx)

CUITabDlg::CUITabDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_TAB, pParent)
{

}

CUITabDlg::~CUITabDlg()
{
}

void CUITabDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

int CUITabDlg::GetWidth() const
{
	return m_iWidth;
}

int CUITabDlg::GetHeight() const
{
	return m_iHeight;
}

void CUITabDlg::SetComponent(std::shared_ptr<class CComponent> pCom)
{
	m_pCom = pCom;
}


BEGIN_MESSAGE_MAP(CUITabDlg, CDialogEx)
END_MESSAGE_MAP()


// CUITabDlg 메시지 처리기


BOOL CUITabDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	RECT tRC = {};

	GetClientRect(&tRC);

	m_iWidth = tRC.right - tRC.left;
	m_iHeight = tRC.bottom - tRC.top;

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}
