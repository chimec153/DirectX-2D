
#include "pch.h"
#include "framework.h"
#include "GameEditor.h"

#include "MainFrm.h"
#include "WorldObj.h"
#include "DetailObj.h"
#include "GameEditorView.h"
#include "ViewManager.h"
#include <sstream>
#include "AniEditor.h"
#include "ColliderInfo.h"
#include "ObjCreate.h"
#include "Scene/SceneManager.h"

#ifdef _DEBUG
#undef new
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_COMMAND(ID_32773, &CMainFrame::OnAniEdit)
	ON_COMMAND(ID_OBJ_CREATE, &CMainFrame::OnObjCreate)
END_MESSAGE_MAP()


CMainFrame::CMainFrame() noexcept	:
	m_pAniEditDlg(nullptr),
	m_pObjCreateDlg(nullptr)
{
}

CMainFrame::~CMainFrame()
{
	/*if(m_pAniEditDlg)
	m_pAniEditDlg->CloseWindow();*/
	SAFE_DELETE(m_pObjCreateDlg);
	SAFE_DELETE(m_pAniEditDlg);
}

void CMainFrame::ClearCreateDlg()
{
	m_pObjCreateDlg->ClearProto();
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;

	cs.style &= ~(LONG)FWS_ADDTOTITLE;
	cs.cx = 1280;
	cs.cy = 1080;
	cs.y = 0;
	cs.x = 0;

	return TRUE;
}

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}
#endif

BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
	m_hSplit.CreateStatic(this, 1, 2);

	m_hSplitChild.CreateStatic(&m_hSplit, 2, 1,
		WS_VISIBLE | WS_CHILD | WS_BORDER, m_hSplit.IdFromRowCol(0, 1));

	/*m_hSplitChild1.CreateStatic(&m_hSplit, 2, 1,
		WS_VISIBLE | WS_CHILD | WS_BORDER, m_hSplit.IdFromRowCol(0, 0));

	m_hSplitChild1.CreateView(0, 0, RUNTIME_CLASS(CColliderInfo),
		CSize(1400, 200), pContext);*/

	m_hSplit.CreateView(0, 0, RUNTIME_CLASS(CGameEditorView),
		CSize(1280, 1080), pContext);

	m_hSplitChild.CreateView(0, 0, RUNTIME_CLASS(CWorldObj), 
		CSize(520, 300), pContext);

	m_hSplitChild.CreateView(1, 0, RUNTIME_CLASS(CDetailObj), 
		CSize(520, 1080), pContext);

	SetActiveView((CView*)m_hSplit.GetPane(0, 0));

	GET_SINGLE(CViewManager)->Init((CWorldObj*)m_hSplitChild.GetPane(0, 0), 
		(CDetailObj*)m_hSplitChild.GetPane(1, 0));

	m_pObjCreateDlg = new CObjCreate;

	m_pObjCreateDlg->Create(IDD_DIALOG_OBJ_CREATE);

	GET_SINGLE(CSceneManager)->SetEditorProtoCreateFunc(m_pObjCreateDlg, &CObjCreate::AddProtoType);

	return TRUE;
}


void CMainFrame::OnUpdateFrameTitle(BOOL bAddToTitle)
{
	std::wostringstream outs;
	outs.precision(6);

	outs << TEXT("Assort30th");

	SetWindowText(outs.str().c_str());

	//CFrameWnd::OnUpdateFrameTitle(bAddToTitle);
}


void CMainFrame::OnAniEdit()
{
	if (!m_pAniEditDlg)
	{
		m_pAniEditDlg = new CAniEditor;

		m_pAniEditDlg->Create(IDD_DIALOG_ANIEDIT);
	}

	m_pAniEditDlg->ShowWindow(SW_SHOW);

	
}


void CMainFrame::OnObjCreate()
{
	if (!m_pObjCreateDlg)
	{
		m_pObjCreateDlg = new CObjCreate;

		m_pObjCreateDlg->Create(IDD_DIALOG_OBJ_CREATE);
	}

	m_pObjCreateDlg->ShowWindow(SW_SHOW);
}