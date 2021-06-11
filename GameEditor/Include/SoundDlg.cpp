// SoundDlg.cpp: 구현 파일
//

#include "pch.h"
#include "GameEditor.h"
#include "SoundDlg.h"
#include "afxdialogex.h"


// CSoundDlg 대화 상자

IMPLEMENT_DYNAMIC(CSoundDlg, CDialogEx)

CSoundDlg::CSoundDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_SOUND, pParent)
{

}

CSoundDlg::~CSoundDlg()
{
}

void CSoundDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

int CSoundDlg::GetWidth() const
{
	return m_iWidth;
}

int CSoundDlg::GetHeight() const
{
	return m_iHeight;
}

void CSoundDlg::SetComponent(std::shared_ptr<class CComponent> pCom)
{
	m_pCom = pCom;
}


BEGIN_MESSAGE_MAP(CSoundDlg, CDialogEx)
END_MESSAGE_MAP()


// CSoundDlg 메시지 처리기


BOOL CSoundDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	RECT tRC = {};

	GetClientRect(&tRC);

	m_iWidth = tRC.right - tRC.left;
	m_iHeight = tRC.bottom - tRC.top;

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}
