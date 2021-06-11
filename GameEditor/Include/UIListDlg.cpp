// UIListDlg.cpp: 구현 파일
//

#include "pch.h"
#include "GameEditor.h"
#include "UIListDlg.h"
#include "afxdialogex.h"


// CUIListDlg 대화 상자

IMPLEMENT_DYNAMIC(CUIListDlg, CDialogEx)

CUIListDlg::CUIListDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_UI_LIST, pParent)
{

}

CUIListDlg::~CUIListDlg()
{
}

void CUIListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

int CUIListDlg::GetWidth() const
{
	return m_iWidth;
}

int CUIListDlg::GetHeight() const
{
	return m_iHeight;
}

void CUIListDlg::SetComponent(std::shared_ptr<class CComponent> pCom)
{
	m_pCom = pCom;
}


BEGIN_MESSAGE_MAP(CUIListDlg, CDialogEx)
END_MESSAGE_MAP()


// CUIListDlg 메시지 처리기


BOOL CUIListDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	RECT tRC = {};

	GetClientRect(&tRC);

	m_iWidth = tRC.right - tRC.left;
	m_iHeight = tRC.bottom - tRC.top;

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}
