#include "pch.h"
#include "GameEditor.h"
#include "WorldObj.h"
#include "ViewManager.h"
#include "DetailObj.h"
#include "Object/GameObject.h"
#include "Scene/Scene.h"
#include "Scene/SceneManager.h"

#undef new
IMPLEMENT_DYNCREATE(CWorldObj, CFormView)

CWorldObj::CWorldObj()	
	:	CFormView(IDD_WORLDOBJ)
{

}

CWorldObj::~CWorldObj()
{
	GET_SINGLE(CSceneManager)->GetScene()->DeleteEditorDelete();
}

void CWorldObj::AddObject(const std::string& strTag)
{
	TreeItem tItem;

	tItem.strTag = CA2CT(strTag.c_str());
	tItem.hItem = m_RootItemtest.InsertItem(tItem.strTag, 0, 0, m_RootItem, TVI_LAST);

	m_vecItem.push_back(tItem);
}

void CWorldObj::DeleteObject(const std::string& strTag)
{
	size_t iSize = m_vecItem.size();

	for (size_t i = 0; i < iSize; ++i)
	{
		std::string strName = CT2CA(m_vecItem[i].strTag);

		if (strName == strTag)
		{
			m_RootItemtest.DeleteItem(m_vecItem[i].hItem);

			std::vector<TreeItem>::iterator iter = m_vecItem.begin() + i;

			m_vecItem.erase(iter);

			GET_SINGLE(CViewManager)->GetDetailDlg()->DeleteObject(strName);
			
			break;
		}

	}
}

BEGIN_MESSAGE_MAP(CWorldObj, CFormView)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE_CONTROL, &CWorldObj::OnTvnSelchangedTreeControl)
	ON_NOTIFY(NM_CLICK, IDC_TREE_CONTROL, &CWorldObj::OnNMClickTreeControl)
END_MESSAGE_MAP()


void CWorldObj::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	m_RootItem = m_RootItemtest.InsertItem(TEXT("Root"), 0, 0, TVI_ROOT, TVI_LAST);

	GET_SINGLE(CViewManager)->GetDetailDlg()->SetObject("Player");
}

void CWorldObj::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
}

#ifdef _DEBUG
void CWorldObj::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CWorldObj::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif

void CWorldObj::OnTvnSelchangedTreeControl(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);

	*pResult = 0;
}


void CWorldObj::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE_CONTROL, m_RootItemtest);
}


void CWorldObj::OnNMClickTreeControl(NMHDR* pNMHDR, LRESULT* pResult)
{
	*pResult = 0;

	TV_HITTESTINFO	hInfo;

	::GetCursorPos(&hInfo.pt);

	::ScreenToClient(m_RootItemtest.m_hWnd, &hInfo.pt);

	HTREEITEM hItem = m_RootItemtest.HitTest(hInfo.pt);

	if (hItem)
	{
		m_RootItemtest.Select(hItem, TVGN_CARET);

		std::string strTag = CT2CA(m_RootItemtest.GetItemText(hItem));

		GET_SINGLE(CViewManager)->GetDetailDlg()->SetObject(strTag);
	}
}
