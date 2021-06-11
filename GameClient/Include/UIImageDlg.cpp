// UIImageDlg.cpp: 구현 파일
//

#include "pch.h"
#include "GameEditor.h"
#include "UIImageDlg.h"
#include "afxdialogex.h"


// CUIImageDlg 대화 상자

IMPLEMENT_DYNAMIC(CUIImageDlg, CDialogEx)

CUIImageDlg::CUIImageDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_UI_IMAGE, pParent)
{

}

CUIImageDlg::~CUIImageDlg()
{
}

void CUIImageDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

int CUIImageDlg::GetWidth() const
{
	return m_iWidth;
}

int CUIImageDlg::GetHeight() const
{
	return m_iHeight;
}

void CUIImageDlg::SetComponent(std::shared_ptr<class CComponent> pCom)
{
	m_pCom = pCom;
}


BEGIN_MESSAGE_MAP(CUIImageDlg, CDialogEx)
END_MESSAGE_MAP()


// CUIImageDlg 메시지 처리기


BOOL CUIImageDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	RECT tRC = {};

	GetClientRect(&tRC);

	m_iWidth = tRC.right - tRC.left;
	m_iHeight = tRC.bottom - tRC.top;

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}
