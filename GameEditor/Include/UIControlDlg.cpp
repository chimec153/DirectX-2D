// UIControlDlg.cpp: 구현 파일
//

#include "pch.h"
#include "GameEditor.h"
#include "UIControlDlg.h"
#include "afxdialogex.h"


// CUIControlDlg 대화 상자

IMPLEMENT_DYNAMIC(CUIControlDlg, CDialogEx)

CUIControlDlg::CUIControlDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_UI_CONTROL, pParent)
{

}

CUIControlDlg::~CUIControlDlg()
{
}

void CUIControlDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

int CUIControlDlg::GetWidth() const
{
	return m_iWidth;
}

int CUIControlDlg::GetHeight() const
{
	return m_iHeight;
}

void CUIControlDlg::SetComponent(std::shared_ptr<class CComponent> pCom)
{
	m_pCom = pCom;
}


BEGIN_MESSAGE_MAP(CUIControlDlg, CDialogEx)
END_MESSAGE_MAP()


// CUIControlDlg 메시지 처리기


BOOL CUIControlDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	RECT tRC = {};

	GetClientRect(&tRC);

	m_iWidth = tRC.right - tRC.left;
	m_iHeight = tRC.bottom - tRC.top;

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}
