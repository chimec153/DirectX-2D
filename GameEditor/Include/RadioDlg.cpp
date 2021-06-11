// RadioDlg.cpp: 구현 파일
//

#include "pch.h"
#include "GameEditor.h"
#include "RadioDlg.h"
#include "afxdialogex.h"


// CRadioDlg 대화 상자

IMPLEMENT_DYNAMIC(CRadioDlg, CDialogEx)

CRadioDlg::CRadioDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_RADIO, pParent)
{

}

CRadioDlg::~CRadioDlg()
{
}

void CRadioDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

int CRadioDlg::GetWidth() const
{
	return m_iWidth;
}

int CRadioDlg::GetHeight() const
{
	return m_iHeight;
}

void CRadioDlg::SetComponent(std::shared_ptr<class CComponent> pCom)
{
	m_pCom = pCom;
}


BEGIN_MESSAGE_MAP(CRadioDlg, CDialogEx)
END_MESSAGE_MAP()


// CRadioDlg 메시지 처리기


BOOL CRadioDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	RECT tRC = {};

	GetClientRect(&tRC);

	m_iWidth = tRC.right - tRC.left;
	m_iHeight = tRC.bottom - tRC.top;

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}
