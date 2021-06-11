// FontDlg.cpp: 구현 파일
//

#include "pch.h"
#include "GameEditor.h"
#include "FontDlg.h"
#include "afxdialogex.h"


// CFontDlg 대화 상자

IMPLEMENT_DYNAMIC(CFontDlg, CDialogEx)

CFontDlg::CFontDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_FONT, pParent)
{

}

CFontDlg::~CFontDlg()
{
}

void CFontDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

int CFontDlg::GetWidth() const
{
	return m_iWidth;
}

int CFontDlg::GetHeight() const
{
	return m_iHeight;
}

void CFontDlg::SetComponent(std::shared_ptr<class CComponent> pCom)
{
	m_pCom = pCom;
}


BEGIN_MESSAGE_MAP(CFontDlg, CDialogEx)
END_MESSAGE_MAP()


// CFontDlg 메시지 처리기


BOOL CFontDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	RECT tRC = {};

	GetClientRect(&tRC);

	m_iWidth = tRC.right - tRC.left;
	m_iHeight = tRC.bottom - tRC.top;

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}
