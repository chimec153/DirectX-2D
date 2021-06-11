// UISliderDlg.cpp: 구현 파일
//

#include "pch.h"
#include "GameEditor.h"
#include "UISliderDlg.h"
#include "afxdialogex.h"


// CUISliderDlg 대화 상자

IMPLEMENT_DYNAMIC(CUISliderDlg, CDialogEx)

CUISliderDlg::CUISliderDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_SLIDER, pParent)
{

}

CUISliderDlg::~CUISliderDlg()
{
}

void CUISliderDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

int CUISliderDlg::GetWidth() const
{
	return m_iWidth;
}

int CUISliderDlg::GetHeight() const
{
	return m_iHeight;
}

void CUISliderDlg::SetComponent(std::shared_ptr<class CComponent> pCom)
{
	m_pCom = pCom;
}


BEGIN_MESSAGE_MAP(CUISliderDlg, CDialogEx)
END_MESSAGE_MAP()


// CUISliderDlg 메시지 처리기


BOOL CUISliderDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	RECT tRC = {};

	GetClientRect(&tRC);

	m_iWidth = tRC.right - tRC.left;
	m_iHeight = tRC.bottom - tRC.top;

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}
