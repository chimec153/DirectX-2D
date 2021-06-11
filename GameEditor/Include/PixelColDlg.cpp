// PixelColDlg.cpp: 구현 파일
//

#include "pch.h"
#include "GameEditor.h"
#include "PixelColDlg.h"
#include "afxdialogex.h"


// CPixelColDlg 대화 상자

IMPLEMENT_DYNAMIC(CPixelColDlg, CDialogEx)

CPixelColDlg::CPixelColDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_COLLIDER_PIXEL, pParent)
{

}

CPixelColDlg::~CPixelColDlg()
{
}

void CPixelColDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CPixelColDlg, CDialogEx)
	ON_EN_CHANGE(IDC_PIXEL_RECT_TOP, &CPixelColDlg::OnEnChangePixelRectTop)
END_MESSAGE_MAP()


// CPixelColDlg 메시지 처리기


void CPixelColDlg::OnEnChangePixelRectTop()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

int CPixelColDlg::GetWidth() const
{
	return m_iWidth;
}

int CPixelColDlg::GetHeight() const
{
	return m_iHeight;
}

void CPixelColDlg::SetComponent(std::shared_ptr<class CComponent> pCom)
{
	m_pCom = pCom;
}


BOOL CPixelColDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	RECT tRC = {};

	GetClientRect(&tRC);

	m_iWidth = tRC.right - tRC.left;
	m_iHeight = tRC.bottom - tRC.top;

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}
