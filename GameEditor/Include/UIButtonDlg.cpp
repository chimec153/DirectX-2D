// UIButtonDlg.cpp: 구현 파일
//

#include "pch.h"
#include "GameEditor.h"
#include "UIButtonDlg.h"
#include "afxdialogex.h"


// CUIButtonDlg 대화 상자

IMPLEMENT_DYNAMIC(CUIButtonDlg, CDialogEx)

CUIButtonDlg::CUIButtonDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_UI_BUTTON, pParent)
{

}

CUIButtonDlg::~CUIButtonDlg()
{
}

void CUIButtonDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

int CUIButtonDlg::GetWidth() const
{
	return m_iWidth;
}

int CUIButtonDlg::GetHeight() const
{
	return m_iHeight;
}

void CUIButtonDlg::SetComponent(std::shared_ptr<class CComponent> pCom)
{
	m_pCom = pCom;
}


BEGIN_MESSAGE_MAP(CUIButtonDlg, CDialogEx)
END_MESSAGE_MAP()


// CUIButtonDlg 메시지 처리기


BOOL CUIButtonDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	RECT tRC = {};

	GetClientRect(&tRC);

	m_iWidth = tRC.right - tRC.left;
	m_iHeight = tRC.bottom - tRC.top;

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}
