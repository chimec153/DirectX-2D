// CStaticMeshDlg.cpp: 구현 파일
//

#include "pch.h"
#include "GameEditor.h"
#include "CStaticMeshDlg.h"
#include "afxdialogex.h"


// CStaticMeshDlg 대화 상자

IMPLEMENT_DYNAMIC(CStaticMeshDlg, CDialogEx)

CStaticMeshDlg::CStaticMeshDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_STATICMESH, pParent)
{

}

CStaticMeshDlg::~CStaticMeshDlg()
{
}

void CStaticMeshDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

int CStaticMeshDlg::GetWidth() const
{
	return m_iWidth;
}

int CStaticMeshDlg::GetHeight() const
{
	return m_iHeight;
}

void CStaticMeshDlg::SetComponent(std::shared_ptr<class CComponent> pCom)
{
	m_pCom = pCom;
}


BEGIN_MESSAGE_MAP(CStaticMeshDlg, CDialogEx)
END_MESSAGE_MAP()


// CStaticMeshDlg 메시지 처리기


BOOL CStaticMeshDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	RECT tRC = {};

	GetClientRect(&tRC);

	m_iWidth = tRC.right - tRC.left;
	m_iHeight = tRC.bottom - tRC.top;

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}
