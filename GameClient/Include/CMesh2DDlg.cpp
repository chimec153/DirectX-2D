// CMesh2DDlg.cpp: 구현 파일
//

#include "pch.h"
#include "GameEditor.h"
#include "CMesh2DDlg.h"
#include "afxdialogex.h"


// CMesh2DDlg 대화 상자

IMPLEMENT_DYNAMIC(CMesh2DDlg, CDialogEx)

CMesh2DDlg::CMesh2DDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_MESH2D, pParent)
{

}

CMesh2DDlg::~CMesh2DDlg()
{
}

void CMesh2DDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

int CMesh2DDlg::GetWidth() const
{
	return m_iWidth;
}

int CMesh2DDlg::GetHeight() const
{
	return m_iHeight;
}

void CMesh2DDlg::SetComponent(std::shared_ptr<class CComponent> pCom)
{
	m_pCom = pCom;
}


BEGIN_MESSAGE_MAP(CMesh2DDlg, CDialogEx)
END_MESSAGE_MAP()


// CMesh2DDlg 메시지 처리기


BOOL CMesh2DDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();


	RECT tRC = {};

	GetClientRect(&tRC);

	m_iWidth = tRC.right - tRC.left;
	m_iHeight = tRC.bottom - tRC.top;

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}
