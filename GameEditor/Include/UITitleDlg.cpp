// UITitleDlg.cpp: 구현 파일
//

#include "pch.h"
#include "GameEditor.h"
#include "UITitleDlg.h"
#include "afxdialogex.h"


// CUITitleDlg 대화 상자

IMPLEMENT_DYNAMIC(CUITitleDlg, CDialogEx)

CUITitleDlg::CUITitleDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_UI_TITLE, pParent)
{

}

CUITitleDlg::~CUITitleDlg()
{
}

void CUITitleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

int CUITitleDlg::GetWidth() const
{
	return m_iWidth;
}

int CUITitleDlg::GetHeight() const
{
	return m_iHeight;
}

void CUITitleDlg::SetComponent(std::shared_ptr<class CComponent> pCom)
{
	m_pCom = pCom;
}


BEGIN_MESSAGE_MAP(CUITitleDlg, CDialogEx)
END_MESSAGE_MAP()


// CUITitleDlg 메시지 처리기


BOOL CUITitleDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	RECT tRC = {};

	GetClientRect(&tRC);

	m_iWidth = tRC.right - tRC.left;
	m_iHeight = tRC.bottom - tRC.top;

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}
