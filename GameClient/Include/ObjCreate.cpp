#include "pch.h"
#include "GameEditor.h"
#include "ObjCreate.h"
#include "afxdialogex.h"
#include "Scene/SceneManager.h"
#include "TileMapDlg.h"
#include "ViewManager.h"
#include "DetailObj.h"
#include "Input.h"

#undef new
IMPLEMENT_DYNAMIC(CObjCreate, CDialogEx)

CObjCreate::CObjCreate(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_OBJ_CREATE, pParent)
	, m_pTileMapDlg(nullptr)
{

}

CObjCreate::~CObjCreate()
{
	SAFE_DELETE(m_pTileMapDlg)
}

void CObjCreate::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_PROTO, m_ProtoList);
}

void CObjCreate::AddProtoType(const std::string& strKey)
{
	CString pKey = CA2CT(strKey.c_str());

	m_ProtoList.AddString(pKey);
}

void CObjCreate::ClearProto()
{
	m_ProtoList.ResetContent();
}

BEGIN_MESSAGE_MAP(CObjCreate, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_CREATE, &CObjCreate::OnBnClickedButtonCreate)
	ON_LBN_SELCHANGE(IDC_LIST_PROTO, &CObjCreate::OnLbnSelchangeListProto)
	ON_BN_CLICKED(IDC_BUTTON_OPEN_TILEMAP, &CObjCreate::OnBnClickedButtonOpenTilemap)
END_MESSAGE_MAP()

void CObjCreate::OnBnClickedButtonCreate()
{
	int iSel = m_ProtoList.GetCurSel();

	CString strProto;

	m_ProtoList.GetText(iSel, strProto);

	CScene* pScene = GET_SINGLE(CSceneManager)->GetScene();

	CLayer* pLayer = pScene->FindLayer("Default");

	std::string pProto = CT2CA(strProto);

	GET_SINGLE(CSceneManager)->GetScene()->CreateCloneObj(pProto, pProto, pLayer);
}


void CObjCreate::OnLbnSelchangeListProto()
{
}

void CObjCreate::OnBnClickedButtonOpenTilemap()
{
	if (!m_pTileMapDlg)
	{
		m_pTileMapDlg = new CTileMapDlg;

		CDetailObj* pDlg = GET_SINGLE(CViewManager)->GetDetailDlg();

		m_pTileMapDlg->Create(IDD_DIALOG_TILEMAP, pDlg);

		m_pTileMapDlg->UpdateWindow();

		m_pTileMapDlg->MoveWindow(0, 290, 500, 300);
	}

	m_pTileMapDlg->ShowWindow(SW_SHOW);
}


LRESULT CObjCreate::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{

	switch (message)
	{
	case WM_ACTIVATE:
		if (LOWORD(wParam) == WA_ACTIVE || LOWORD(wParam) == WA_CLICKACTIVE)
		{
			if (!GET_SINGLE(CInput)->GetShow())
				GET_SINGLE(CInput)->SetShow(true);
		}
		break;
	default:
		break;
	}

	return CDialogEx::WindowProc(message, wParam, lParam);
}
