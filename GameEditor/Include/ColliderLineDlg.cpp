// ColliderLineDlg.cpp: 구현 파일
//

#include "pch.h"
#include "GameEditor.h"
#include "ColliderLineDlg.h"
#include "afxdialogex.h"


// CColliderLineDlg 대화 상자

IMPLEMENT_DYNAMIC(CColliderLineDlg, CDialogEx)

CColliderLineDlg::CColliderLineDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_COLLIDER_LINE, pParent)
{

}

CColliderLineDlg::~CColliderLineDlg()
{
}

void CColliderLineDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

int CColliderLineDlg::GetWidth() const
{
	return m_iWidth;
}

int CColliderLineDlg::GetHeight() const
{
	return m_iHeight;
}

void CColliderLineDlg::SetComponent(std::shared_ptr<class CComponent> pCom)
{
	m_pCom = pCom;
}


BEGIN_MESSAGE_MAP(CColliderLineDlg, CDialogEx)
END_MESSAGE_MAP()


// CColliderLineDlg 메시지 처리기


BOOL CColliderLineDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	RECT tRC = {};

	GetClientRect(&tRC);

	m_iWidth = tRC.right - tRC.left;
	m_iHeight = tRC.bottom - tRC.top;

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}
