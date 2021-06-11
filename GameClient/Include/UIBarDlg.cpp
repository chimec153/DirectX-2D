// UIBarDlg.cpp: 구현 파일
//

#include "pch.h"
#include "GameEditor.h"
#include "UIBarDlg.h"
#include "afxdialogex.h"


// CUIBarDlg 대화 상자

IMPLEMENT_DYNAMIC(CUIBarDlg, CDialogEx)

CUIBarDlg::CUIBarDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_UI_BAR, pParent)
{

}

CUIBarDlg::~CUIBarDlg()
{
}

void CUIBarDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

int CUIBarDlg::GetWidth() const
{
	return m_iWidth;
}

int CUIBarDlg::GetHeight() const
{
	return m_iHeight;
}

void CUIBarDlg::SetComponent(std::shared_ptr<class CComponent> pCom)
{
	m_pCom = pCom;
}


BEGIN_MESSAGE_MAP(CUIBarDlg, CDialogEx)
END_MESSAGE_MAP()


// CUIBarDlg 메시지 처리기


BOOL CUIBarDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	RECT tRC = {};

	GetClientRect(&tRC);

	m_iWidth = tRC.right - tRC.left;
	m_iHeight = tRC.bottom - tRC.top;

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}
