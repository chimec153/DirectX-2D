// ColliderPointDlg.cpp: 구현 파일
//

#include "pch.h"
#include "GameEditor.h"
#include "ColliderPointDlg.h"
#include "afxdialogex.h"


// CColliderPointDlg 대화 상자

IMPLEMENT_DYNAMIC(CColliderPointDlg, CDialogEx)

CColliderPointDlg::CColliderPointDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_COLLIDER_POINT, pParent)
{

}

CColliderPointDlg::~CColliderPointDlg()
{
}

void CColliderPointDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

int CColliderPointDlg::GetWidth() const
{
	return m_iWidth;
}

int CColliderPointDlg::GetHeight() const
{
	return m_iHeight;
}

void CColliderPointDlg::SetComponent(std::shared_ptr<class CComponent> pCom)
{
	m_pCom = pCom;
}


BEGIN_MESSAGE_MAP(CColliderPointDlg, CDialogEx)
END_MESSAGE_MAP()


// CColliderPointDlg 메시지 처리기


BOOL CColliderPointDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	RECT tRC = {};

	GetClientRect(&tRC);

	m_iWidth = tRC.right - tRC.left;
	m_iHeight = tRC.bottom - tRC.top;

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}
