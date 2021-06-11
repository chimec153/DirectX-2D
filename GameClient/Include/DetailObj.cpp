// DetailObj.cpp: 구현 파일
//

#include "pch.h"
#include "GameEditor.h"
#include "DetailObj.h"
#include "Scene/SceneManager.h"
#include "Scene/Scene.h"
#include "Object/GameObject.h"
#include "Component/SceneComponent.h"
#include "GameEngine.h"
#include "ColliderInfo.h"
#include "Component/ColliderRect.h"
#include "Component/ColliderCircle.h"
#include "Component/ColliderOBB2D.h"
#include "Layer/Layer.h"
#include "TileMapDlg.h"
#include "Component/TileMap.h"
#include "ColliderInfo.h"
#include "CMesh2DDlg.h"
#include "CameraDlg.h"
#include "ColliderPointDlg.h"
#include "ColliderLineDlg.h"
#include "PixelColDlg.h"
#include "ColliderCircleDlg.h"
#include "FontDlg.h"
#include "PanelDlg.h"
#include "RendererDlg.h"
#include "UISliderDlg.h"
#include "SoundDlg.h"
#include "SpriteDlg.h"
#include "CStaticMeshDlg.h"
#include "UITabDlg.h"
#include "UIBarDlg.h"
#include "UIButtonDlg.h"
#include "CheckBoxDlg.h"
#include "UIControlDlg.h"
#include "UIImageDlg.h"
#include "UIListDlg.h"
#include "UITitleDlg.h"
#include "RadioDlg.h"
#include "CColliderOBBDlg.h"

// CDetailObj

#undef new
IMPLEMENT_DYNCREATE(CDetailObj, CFormView)

CDetailObj::CDetailObj()
	:	CFormView(IDD_OBJDETAIL)
	, m_iOffset(290)
	, m_pSelectObj(nullptr)
	, m_fRadius(0)
	, m_fExtentX(0)
	, m_fExtentY(0)
{
	m_vScale = Vector3(1.f, 1.f, 1.f);
}

CDetailObj::~CDetailObj()
{
	SAFE_DELETE_VECLIST(m_vecDialog);
}

BEGIN_MESSAGE_MAP(CDetailObj, CFormView)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE_COMPONENT, &CDetailObj::OnTvnSelchangedTreeComponent)
	ON_EN_CHANGE(IDC_EDIT_POSX, &CDetailObj::OnEnChangeEditPosx)
	ON_EN_CHANGE(IDC_EDIT_SCALEX, &CDetailObj::OnEnChangeEditScalex)
	ON_EN_CHANGE(IDC_EDIT_ROTX, &CDetailObj::OnEnChangeEditRotx)
	ON_EN_CHANGE(IDC_EDIT_POSY, &CDetailObj::OnEnChangeEditPosy)
	ON_EN_CHANGE(IDC_EDIT_SCALEY, &CDetailObj::OnEnChangeEditScaley)
	ON_EN_CHANGE(IDC_EDIT_ROTY, &CDetailObj::OnEnChangeEditRoty)
	ON_EN_CHANGE(IDC_EDIT_POSZ, &CDetailObj::OnEnChangeEditPosz)
	ON_EN_CHANGE(IDC_EDIT_SCALEZ, &CDetailObj::OnEnChangeEditScalez)
	ON_EN_CHANGE(IDC_EDIT_ROTZ, &CDetailObj::OnEnChangeEditRotz)
	ON_NOTIFY(NM_CLICK, IDC_TREE_COMPONENT, &CDetailObj::OnNMClickTreeComponent)
END_MESSAGE_MAP()


// CDetailObj 그리기

void CDetailObj::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: 여기에 그리기 코드를 추가합니다.
}


// CDetailObj 진단

#ifndef _WIN32_WCE
#ifdef _DEBUG
void CDetailObj::AssertValid() const
{
	CView::AssertValid();
}

void CDetailObj::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG

void CDetailObj::SetObject(const std::string& strTag)
{
	CScene* pScene = GET_SINGLE(CSceneManager)->GetScene();

	if (!pScene)
		return;

	CLayer* pLayer = pScene->FindLayer("Default");

	m_pSelectObj = pLayer->FindObj(strTag);

	if (!m_pSelectObj)
	{
		pLayer = pScene->FindLayer("Back");

		m_pSelectObj = pLayer->FindObj(strTag);

		if (!m_pSelectObj)
		{
			pLayer = pScene->FindLayer("UI");

			m_pSelectObj = pLayer->FindObj(strTag);

			if (!m_pSelectObj)
			{
				return;
			}
		}
			
	}		

	m_Component.DeleteAllItems();
	m_vecItem.clear();

	std::vector<Hierarchy> vecHierarchy;

	m_pSelectObj->GetAllComponentName(vecHierarchy);

	std::vector<Hierarchy>::iterator iter = vecHierarchy.begin();
	std::vector<Hierarchy>::iterator iterEnd = vecHierarchy.end();

	for (; iter != iterEnd; ++iter)
	{
		CString strName = CA2CT((*iter).strTag.c_str());

		if ((*iter).strParentTag.empty())
		{
			TreeItem tItem;

			tItem.strTag = CA2CT((*iter).strTag.c_str());
			tItem.hItem = m_Component.InsertItem(tItem.strTag, 0 , 0, TVI_ROOT,TVI_LAST);

			m_vecItem.push_back(tItem);
		}

		else
		{
			CString strParentName = CA2CT((*iter).strParentTag.c_str());

			size_t iSize = m_vecItem.size();

			TreeItem tItem;

			for (size_t i = 0; i < iSize; ++i)
			{
				if (m_vecItem[i].strTag == strParentName)
				{
					tItem.hItem = m_Component.InsertItem(strName, m_vecItem[i].hItem);
					break;
				}
			}

			tItem.strTag = strName;

			m_vecItem.push_back(tItem);
		}
	}

	CCreateContext tContext = {};

	tContext.m_pNewViewClass = RUNTIME_CLASS(CColliderInfo);

	bool bResult = m_hWnd.Create(TEXT("Collider"), TEXT("Collider"), WS_CHILD | WS_BORDER | WS_VISIBLE, CRect(0, 0,300, 300), this, IDD_COLLIDER_RECT, &tContext);

	//m_hWnd.OnInit
}

void CDetailObj::DeleteObject(const std::string& strTag)
{
	if (!m_pSelectObj)
		return;

	if (m_pSelectObj->GetName() == strTag)
		m_pSelectObj = nullptr;
}

#endif


// CDetailObj 메시지 처리기


void CDetailObj::OnTvnSelchangedTreeComponent(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;
}


void CDetailObj::DoDataExchange(CDataExchange* pDX)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE_COMPONENT, m_Component);
	DDX_Text(pDX, IDC_EDIT_POSX, m_vPos.x);
	DDX_Text(pDX, IDC_EDIT_POSY, m_vPos.y);
	DDX_Text(pDX, IDC_EDIT_POSZ, m_vPos.z);
	DDX_Text(pDX, IDC_EDIT_SCALEX, m_vScale.x);
	DDX_Text(pDX, IDC_EDIT_SCALEY, m_vScale.y);
	DDX_Text(pDX, IDC_EDIT_SCALEZ, m_vScale.z);
	DDX_Text(pDX, IDC_EDIT_ROTX, m_vRot.x);
	DDX_Text(pDX, IDC_EDIT_ROTY, m_vRot.y);
	DDX_Text(pDX, IDC_EDIT_ROTZ, m_vRot.z);
}


void CDetailObj::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	UpdateData(FALSE);
}


void CDetailObj::OnEnChangeEditPosx()
{
	UpdateData(TRUE);

	if (m_pSelectObj)
	{
		HTREEITEM hItem = m_Component.GetSelectedItem();

		CString strCom = m_Component.GetItemText(hItem);

		std::string strTag = CT2CA(strCom);

		std::shared_ptr<CComponent> pCom = m_pSelectObj->FindComponent(strTag.c_str());

		if (pCom)
			((CSceneComponent*)pCom.get())->SetWorldPos(m_vPos);

		else
			m_pSelectObj->SetWorldPos(m_vPos);
	}
		
}


void CDetailObj::OnEnChangeEditScalex()
{
	UpdateData(TRUE);

	if (m_pSelectObj)
	{
		HTREEITEM hItem = m_Component.GetSelectedItem();

		CString strCom = m_Component.GetItemText(hItem);

		std::string strTag = CT2CA(strCom);

		std::shared_ptr<CComponent> pCom = m_pSelectObj->FindComponent(strTag.c_str());

		if (pCom)
			((CSceneComponent*)pCom.get())->SetWorldScale(m_vScale);

		else
			m_pSelectObj->SetWorldScale(m_vScale);
	}
}


void CDetailObj::OnEnChangeEditRotx()
{
	UpdateData(TRUE);

	if (m_pSelectObj)
	{
		HTREEITEM hItem = m_Component.GetSelectedItem();

		CString strCom = m_Component.GetItemText(hItem);

		std::string strTag = CT2CA(strCom);

		std::shared_ptr<CComponent> pCom = m_pSelectObj->FindComponent(strTag.c_str());

		if (pCom)
			((CSceneComponent*)pCom.get())->SetWorldPos(m_vPos);

		else
			m_pSelectObj->SetWorldRot(m_vRot);
	}
}


void CDetailObj::OnEnChangeEditPosy()
{
	UpdateData(TRUE);

	if (m_pSelectObj)
	{
		HTREEITEM hItem = m_Component.GetSelectedItem();

		CString strCom = m_Component.GetItemText(hItem);

		std::string strTag = CT2CA(strCom);

		std::shared_ptr<CComponent> pCom = m_pSelectObj->FindComponent(strTag.c_str());

		if (pCom)
			((CSceneComponent*)pCom.get())->SetWorldPos(m_vPos);

		else
			m_pSelectObj->SetWorldPos(m_vPos);
	}
}


void CDetailObj::OnEnChangeEditScaley()
{
	UpdateData(TRUE);

	if (m_pSelectObj)
	{
		HTREEITEM hItem = m_Component.GetSelectedItem();

		CString strCom = m_Component.GetItemText(hItem);

		std::string strTag = CT2CA(strCom);

		std::shared_ptr<CComponent> pCom = m_pSelectObj->FindComponent(strTag.c_str());

		if (pCom)
			((CSceneComponent*)pCom.get())->SetWorldPos(m_vPos);

		else
			m_pSelectObj->SetWorldScale(m_vScale);
	}
}


void CDetailObj::OnEnChangeEditRoty()
{
	UpdateData(TRUE);

	if (m_pSelectObj)
	{
		HTREEITEM hItem = m_Component.GetSelectedItem();

		CString strCom = m_Component.GetItemText(hItem);

		std::string strTag = CT2CA(strCom);

		std::shared_ptr<CComponent> pCom = m_pSelectObj->FindComponent(strTag.c_str());

		if (pCom)
			((CSceneComponent*)pCom.get())->SetWorldPos(m_vPos);

		else
			m_pSelectObj->SetWorldRot(m_vRot);
	}
}


void CDetailObj::OnEnChangeEditPosz()
{
	UpdateData(TRUE);

	if (m_pSelectObj)
	{
		HTREEITEM hItem = m_Component.GetSelectedItem();

		CString strCom = m_Component.GetItemText(hItem);

		std::string strTag = CT2CA(strCom);

		std::shared_ptr<CComponent> pCom = m_pSelectObj->FindComponent(strTag.c_str());

		if (pCom)
			((CSceneComponent*)pCom.get())->SetWorldPos(m_vPos);

		else
			m_pSelectObj->SetWorldPos(m_vPos);
	}
}


void CDetailObj::OnEnChangeEditScalez()
{
	UpdateData(TRUE);

	if (m_pSelectObj)
	{
		HTREEITEM hItem = m_Component.GetSelectedItem();

		CString strCom = m_Component.GetItemText(hItem);

		std::string strTag = CT2CA(strCom);

		std::shared_ptr<CComponent> pCom = m_pSelectObj->FindComponent(strTag.c_str());

		if (pCom)
			((CSceneComponent*)pCom.get())->SetWorldPos(m_vPos);

		else
			m_pSelectObj->SetWorldScale(m_vScale);
	}
}


void CDetailObj::OnEnChangeEditRotz()
{
	UpdateData(TRUE);

	if (m_pSelectObj)
	{
		HTREEITEM hItem = m_Component.GetSelectedItem();

		CString strCom = m_Component.GetItemText(hItem);

		std::string strTag = CT2CA(strCom);

		std::shared_ptr<CComponent> pCom = m_pSelectObj->FindComponent(strTag.c_str());

		if (pCom)
			((CSceneComponent*)pCom.get())->SetWorldPos(m_vPos);

		else
			m_pSelectObj->SetWorldRot(m_vRot);
	}
}

void CDetailObj::Update(float fTime)
{
	HTREEITEM hItem = m_Component.GetSelectedItem();

	CString strCom = m_Component.GetItemText(hItem);

	std::string strTag = CT2CA(strCom);

	if (m_pSelectObj && hItem)
	{
		std::shared_ptr<CComponent> pCom = m_pSelectObj->FindComponent(strTag.c_str());

		m_vPos = ((CSceneComponent*)pCom.get())->GetWorldPos();
		m_vRot = ((CSceneComponent*)pCom.get())->GetWorldRot();
		m_vScale = ((CSceneComponent*)pCom.get())->GetWorldScale();

		UpdateData(FALSE);
	}
}


void CDetailObj::OnNMClickTreeComponent(NMHDR* pNMHDR, LRESULT* pResult)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;

	TV_HITTESTINFO tInfo;

	::GetCursorPos(&tInfo.pt);

	::ScreenToClient(m_Component.m_hWnd, &tInfo.pt);

	HTREEITEM hItem = m_Component.HitTest(tInfo.pt);

	if (hItem)
	{
		CString strTag = m_Component.GetItemText(hItem);

		std::string strName = CT2CA(strTag);

		if (m_pSelectObj)
		{
			size_t iSz = m_vecDialog.size();

			for (size_t i = 0; i < iSz; ++i)
			{
				m_vecDialog[i]->DestroyWindow();
				delete m_vecDialog[i];
			}

			m_vecDialog.clear();

			m_iOffset = 290;

			std::shared_ptr<CComponent> pCom = m_pSelectObj->FindComponent(strName.c_str());

			m_vPos = ((CSceneComponent*)pCom.get())->GetWorldPos();
			m_vRot = ((CSceneComponent*)pCom.get())->GetWorldRot();
			m_vScale = ((CSceneComponent*)pCom.get())->GetWorldScale();

			int iType = ((CSceneComponent*)pCom.get())->GetSceneComponentClassType();

			RectInfo tRI = {};
			SphereInfo tSI = {};
			OBBInfo tOI = {};

			switch (SCENECOMPONENT_CLASS_TYPE(iType))
			{
			case SCENECOMPONENT_CLASS_TYPE::SCT_MESH2D:
			{
				CRendererDlg* pRenDlg = new CRendererDlg;

				pRenDlg->Create(IDD_DIALOG_RENDERER, this);

				pRenDlg->UpdateWindow();

				pRenDlg->MoveWindow(0, m_iOffset, pRenDlg->GetWidth(), pRenDlg->GetHeight());

				pRenDlg->SetComponent(pCom);

				pRenDlg->ShowWindow(SW_SHOW);

				m_iOffset += pRenDlg->GetHeight();

				m_vecDialog.push_back(pRenDlg);

				CMesh2DDlg* pDlg = new CMesh2DDlg;

				pDlg->Create(IDD_DIALOG_MESH2D, this);

				pDlg->UpdateWindow();

				pDlg->MoveWindow(0, m_iOffset, pDlg->GetWidth(), pDlg->GetHeight());

				pDlg->SetComponent(pCom);

				pDlg->ShowWindow(SW_SHOW);

				m_iOffset += pDlg->GetHeight();

				m_vecDialog.push_back(pDlg);
			}
				break;
			case SCENECOMPONENT_CLASS_TYPE::SCT_STATICMESH:
			{
				CRendererDlg* pRenDlg = new CRendererDlg;

				pRenDlg->Create(IDD_DIALOG_RENDERER, this);

				pRenDlg->UpdateWindow();

				pRenDlg->MoveWindow(0, m_iOffset, pRenDlg->GetWidth(), pRenDlg->GetHeight());

				pRenDlg->SetComponent(pCom);

				pRenDlg->ShowWindow(SW_SHOW);

				m_iOffset += pRenDlg->GetHeight();

				m_vecDialog.push_back(pRenDlg);

				CStaticMeshDlg* pDlg = new CStaticMeshDlg;

				pDlg->Create(IDD_DIALOG_STATICMESH, this);

				pDlg->UpdateWindow();

				pDlg->MoveWindow(0, m_iOffset, pDlg->GetWidth(), pDlg->GetHeight());

				pDlg->SetComponent(pCom);

				pDlg->ShowWindow(SW_SHOW);

				m_iOffset += pDlg->GetHeight();

				m_vecDialog.push_back(pDlg);
			}
				break;
			case SCENECOMPONENT_CLASS_TYPE::SCT_CAMERA:
			{
				CCameraDlg* pDlg = new CCameraDlg;

				pDlg->Create(IDD_DIALOG_CAMERA, this);

				pDlg->UpdateWindow();

				pDlg->MoveWindow(0, m_iOffset, pDlg->GetWidth(), pDlg->GetHeight());

				pDlg->SetComponent(pCom);

				pDlg->ShowWindow(SW_SHOW);

				m_iOffset += pDlg->GetHeight();

				m_vecDialog.push_back(pDlg);
			}
				break;
			case SCENECOMPONENT_CLASS_TYPE::SCT_SPRITE:
			{
				CRendererDlg* pRenDlg = new CRendererDlg;

				pRenDlg->Create(IDD_DIALOG_RENDERER, this);

				pRenDlg->UpdateWindow();

				pRenDlg->MoveWindow(0, m_iOffset, pRenDlg->GetWidth(), pRenDlg->GetHeight());

				pRenDlg->SetComponent(pCom);

				pRenDlg->ShowWindow(SW_SHOW);

				m_iOffset += pRenDlg->GetHeight();

				m_vecDialog.push_back(pRenDlg);

				CSpriteDlg* pDlg = new CSpriteDlg;

				pDlg->Create(IDD_DIALOG_SPRITE, this);

				pDlg->UpdateWindow();

				pDlg->MoveWindow(0, m_iOffset, pDlg->GetWidth(), pDlg->GetHeight());

				pDlg->SetComponent(pCom);

				pDlg->ShowWindow(SW_SHOW);

				m_iOffset += pDlg->GetHeight();

				m_vecDialog.push_back(pDlg);
			}
				break;
			case SCENECOMPONENT_CLASS_TYPE::SCT_COLLIDER_RECT:
			{
				CColliderInfo* pInfo = new CColliderInfo;

				pInfo->Create(IDD_COLLIDER_RECT, this);

				pInfo->SetCollider(pCom);

				pInfo->UpdateWindow();

				int iHeight = pInfo->GetHeight();

				int iWidth = pInfo->GetWidth();

				pInfo->MoveWindow(0, m_iOffset, iWidth, iHeight);

				pInfo->ShowWindow(SW_SHOW);

				m_vecDialog.push_back(pInfo);

				m_iOffset += pInfo->GetHeight();
			}
				break;
			case SCENECOMPONENT_CLASS_TYPE::SCT_COLLIDER_LINE:
			{
				CColliderLineDlg* pDlg = new CColliderLineDlg;

				pDlg->Create(IDD_DIALOG_MESH2D, this);

				pDlg->UpdateWindow();

				pDlg->MoveWindow(0, m_iOffset, pDlg->GetWidth(), pDlg->GetHeight());

				pDlg->SetComponent(pCom);

				pDlg->ShowWindow(SW_SHOW);

				m_iOffset += pDlg->GetHeight();

				m_vecDialog.push_back(pDlg);
			}
				break;
			case SCENECOMPONENT_CLASS_TYPE::SCT_COLLIDER_CIRCLE:
			{
				CColliderCircleDlg* pDlg = new CColliderCircleDlg;

				pDlg->Create(IDD_DIALOG_COLLIDERCIRCLE, this);

				pDlg->UpdateWindow();

				pDlg->MoveWindow(0, m_iOffset, pDlg->GetWidth(), pDlg->GetHeight());

				pDlg->SetCollider(pCom);

				pDlg->ShowWindow(SW_SHOW);

				m_iOffset += pDlg->GetHeight();

				m_vecDialog.push_back(pDlg);
			}
				break;
			case SCENECOMPONENT_CLASS_TYPE::SCT_COLLIDER_OBB2D:
			{
				CRendererDlg* pRenDlg = new CRendererDlg;

				pRenDlg->Create(IDD_DIALOG_RENDERER, this);

				pRenDlg->UpdateWindow();

				pRenDlg->MoveWindow(0, m_iOffset, pRenDlg->GetWidth(), pRenDlg->GetHeight());

				pRenDlg->SetComponent(pCom);

				pRenDlg->ShowWindow(SW_SHOW);

				m_iOffset += pRenDlg->GetHeight();

				m_vecDialog.push_back(pRenDlg);

				CColliderOBBDlg* pDlg = new CColliderOBBDlg;

				pDlg->Create(IDD_DIALOG_COLLIDER_OBB2D, this);

				pDlg->UpdateWindow();

				pDlg->MoveWindow(0, m_iOffset, pDlg->GetWidth(), pDlg->GetHeight());

				pDlg->SetCollider(pCom);

				pDlg->ShowWindow(SW_SHOW);

				m_iOffset += pDlg->GetHeight();

				m_vecDialog.push_back(pDlg);
			}
				break;
			case SCENECOMPONENT_CLASS_TYPE::SCT_COLLIDER_PIXEL:
			{
				CPixelColDlg* pDlg = new CPixelColDlg;

				pDlg->Create(IDD_DIALOG_COLLIDER_PIXEL, this);

				pDlg->UpdateWindow();

				pDlg->MoveWindow(0, m_iOffset, pDlg->GetWidth(), pDlg->GetHeight());

				pDlg->SetComponent(pCom);

				pDlg->ShowWindow(SW_SHOW);

				m_iOffset += pDlg->GetHeight();

				m_vecDialog.push_back(pDlg);
			}
			break;
			case SCENECOMPONENT_CLASS_TYPE::COLLIDER_POINT:
			{
				CColliderPointDlg* pDlg = new CColliderPointDlg;

				pDlg->Create(IDD_DIALOG_COLLIDER_POINT, this);

				pDlg->UpdateWindow();

				pDlg->MoveWindow(0, m_iOffset, pDlg->GetWidth(), pDlg->GetHeight());

				pDlg->SetComponent(pCom);

				pDlg->ShowWindow(SW_SHOW);

				m_iOffset += pDlg->GetHeight();

				m_vecDialog.push_back(pDlg);
			}
			break;
			case SCENECOMPONENT_CLASS_TYPE::UI_BAR:
			{
				CRendererDlg* pRenDlg = new CRendererDlg;

				pRenDlg->Create(IDD_DIALOG_RENDERER, this);

				pRenDlg->UpdateWindow();

				pRenDlg->MoveWindow(0, m_iOffset, pRenDlg->GetWidth(), pRenDlg->GetHeight());

				pRenDlg->SetComponent(pCom);

				pRenDlg->ShowWindow(SW_SHOW);

				m_iOffset += pRenDlg->GetHeight();

				m_vecDialog.push_back(pRenDlg);

				CUIControlDlg* pConDlg = new CUIControlDlg;

				pConDlg->Create(IDD_DIALOG_UI_CONTROL, this);

				pConDlg->UpdateWindow();

				pConDlg->MoveWindow(0, m_iOffset, pConDlg->GetWidth(), pConDlg->GetHeight());

				pConDlg->SetComponent(pCom);

				pConDlg->ShowWindow(SW_SHOW);

				m_iOffset += pConDlg->GetHeight();

				m_vecDialog.push_back(pConDlg);

				CUIBarDlg* pDlg = new CUIBarDlg;

				pDlg->Create(IDD_DIALOG_UI_BAR, this);

				pDlg->UpdateWindow();

				pDlg->MoveWindow(0, m_iOffset, pDlg->GetWidth(), pDlg->GetHeight());

				pDlg->SetComponent(pCom);

				pDlg->ShowWindow(SW_SHOW);

				m_iOffset += pDlg->GetHeight();

				m_vecDialog.push_back(pDlg);
			}
			break;
			case SCENECOMPONENT_CLASS_TYPE::UI_CHECK:
			{
				CRendererDlg* pRenDlg = new CRendererDlg;

				pRenDlg->Create(IDD_DIALOG_RENDERER, this);

				pRenDlg->UpdateWindow();

				pRenDlg->MoveWindow(0, m_iOffset, pRenDlg->GetWidth(), pRenDlg->GetHeight());

				pRenDlg->SetComponent(pCom);

				pRenDlg->ShowWindow(SW_SHOW);

				m_iOffset += pRenDlg->GetHeight();

				m_vecDialog.push_back(pRenDlg);

				CUIControlDlg* pConDlg = new CUIControlDlg;

				pConDlg->Create(IDD_DIALOG_UI_CONTROL, this);

				pConDlg->UpdateWindow();

				pConDlg->MoveWindow(0, m_iOffset, pConDlg->GetWidth(), pConDlg->GetHeight());

				pConDlg->SetComponent(pCom);

				pConDlg->ShowWindow(SW_SHOW);

				m_iOffset += pConDlg->GetHeight();

				m_vecDialog.push_back(pConDlg);

				CCheckBoxDlg* pDlg = new CCheckBoxDlg;

				pDlg->Create(IDD_DIALOG_UI_CHECK, this);

				pDlg->UpdateWindow();

				pDlg->MoveWindow(0, m_iOffset, pDlg->GetWidth(), pDlg->GetHeight());

				pDlg->SetComponent(pCom);

				pDlg->ShowWindow(SW_SHOW);

				m_iOffset += pDlg->GetHeight();

				m_vecDialog.push_back(pDlg);
			}
			break;
			case SCENECOMPONENT_CLASS_TYPE::UI_BUTTON:
			{
				CRendererDlg* pRenDlg = new CRendererDlg;

				pRenDlg->Create(IDD_DIALOG_RENDERER, this);

				pRenDlg->UpdateWindow();

				pRenDlg->MoveWindow(0, m_iOffset, pRenDlg->GetWidth(), pRenDlg->GetHeight());

				pRenDlg->SetComponent(pCom);

				pRenDlg->ShowWindow(SW_SHOW);

				m_iOffset += pRenDlg->GetHeight();

				m_vecDialog.push_back(pRenDlg);

				CUIControlDlg* pConDlg = new CUIControlDlg;

				pConDlg->Create(IDD_DIALOG_UI_CONTROL, this);

				pConDlg->UpdateWindow();

				pConDlg->MoveWindow(0, m_iOffset, pConDlg->GetWidth(), pConDlg->GetHeight());

				pConDlg->SetComponent(pCom);

				pConDlg->ShowWindow(SW_SHOW);

				m_iOffset += pConDlg->GetHeight();

				m_vecDialog.push_back(pConDlg);

				CUIButtonDlg* pDlg = new CUIButtonDlg;

				pDlg->Create(IDD_DIALOG_UI_BUTTON, this);

				pDlg->UpdateWindow();

				pDlg->MoveWindow(0, m_iOffset, pDlg->GetWidth(), pDlg->GetHeight());

				pDlg->SetComponent(pCom);

				pDlg->ShowWindow(SW_SHOW);

				m_iOffset += pDlg->GetHeight();

				m_vecDialog.push_back(pDlg);
			}
			break;
			case SCENECOMPONENT_CLASS_TYPE::UI_RADIO:
			{
				CRendererDlg* pRenDlg = new CRendererDlg;

				pRenDlg->Create(IDD_DIALOG_RENDERER, this);

				pRenDlg->UpdateWindow();

				pRenDlg->MoveWindow(0, m_iOffset, pRenDlg->GetWidth(), pRenDlg->GetHeight());

				pRenDlg->SetComponent(pCom);

				pRenDlg->ShowWindow(SW_SHOW);

				m_iOffset += pRenDlg->GetHeight();

				m_vecDialog.push_back(pRenDlg);

				CUIControlDlg* pConDlg = new CUIControlDlg;

				pConDlg->Create(IDD_DIALOG_UI_CONTROL, this);

				pConDlg->UpdateWindow();

				pConDlg->MoveWindow(0, m_iOffset, pConDlg->GetWidth(), pConDlg->GetHeight());

				pConDlg->SetComponent(pCom);

				pConDlg->ShowWindow(SW_SHOW);

				m_iOffset += pConDlg->GetHeight();

				m_vecDialog.push_back(pConDlg);

				CRadioDlg* pDlg = new CRadioDlg;

				pDlg->Create(IDD_DIALOG_RADIO, this);

				pDlg->UpdateWindow();

				pDlg->MoveWindow(0, m_iOffset, pDlg->GetWidth(), pDlg->GetHeight());

				pDlg->SetComponent(pCom);

				pDlg->ShowWindow(SW_SHOW);

				m_iOffset += pDlg->GetHeight();

				m_vecDialog.push_back(pDlg);
			}
			break;
			case SCENECOMPONENT_CLASS_TYPE::UI_PANEL:
			{
				CRendererDlg* pRenDlg = new CRendererDlg;

				pRenDlg->Create(IDD_DIALOG_RENDERER, this);

				pRenDlg->UpdateWindow();

				pRenDlg->MoveWindow(0, m_iOffset, pRenDlg->GetWidth(), pRenDlg->GetHeight());

				pRenDlg->SetComponent(pCom);

				pRenDlg->ShowWindow(SW_SHOW);

				m_iOffset += pRenDlg->GetHeight();

				m_vecDialog.push_back(pRenDlg);

				CUIControlDlg* pConDlg = new CUIControlDlg;

				pConDlg->Create(IDD_DIALOG_UI_CONTROL, this);

				pConDlg->UpdateWindow();

				pConDlg->MoveWindow(0, m_iOffset, pConDlg->GetWidth(), pConDlg->GetHeight());

				pConDlg->SetComponent(pCom);

				pConDlg->ShowWindow(SW_SHOW);

				m_iOffset += pConDlg->GetHeight();

				m_vecDialog.push_back(pConDlg);

				CPanelDlg* pDlg = new CPanelDlg;

				pDlg->Create(IDD_DIALOG_PANEL, this);

				pDlg->UpdateWindow();

				pDlg->MoveWindow(0, m_iOffset, pDlg->GetWidth(), pDlg->GetHeight());

				pDlg->SetComponent(pCom);

				pDlg->ShowWindow(SW_SHOW);

				m_iOffset += pDlg->GetHeight();

				m_vecDialog.push_back(pDlg);
			}
			break;
			case SCENECOMPONENT_CLASS_TYPE::UI_SLIDER:
			{
				CRendererDlg* pRenDlg = new CRendererDlg;

				pRenDlg->Create(IDD_DIALOG_RENDERER, this);

				pRenDlg->UpdateWindow();

				pRenDlg->MoveWindow(0, m_iOffset, pRenDlg->GetWidth(), pRenDlg->GetHeight());

				pRenDlg->SetComponent(pCom);

				pRenDlg->ShowWindow(SW_SHOW);

				m_iOffset += pRenDlg->GetHeight();

				m_vecDialog.push_back(pRenDlg);

				CUIControlDlg* pConDlg = new CUIControlDlg;

				pConDlg->Create(IDD_DIALOG_UI_CONTROL, this);

				pConDlg->UpdateWindow();

				pConDlg->MoveWindow(0, m_iOffset, pConDlg->GetWidth(), pConDlg->GetHeight());

				pConDlg->SetComponent(pCom);

				pConDlg->ShowWindow(SW_SHOW);

				m_iOffset += pConDlg->GetHeight();

				m_vecDialog.push_back(pConDlg);

				CUISliderDlg* pDlg = new CUISliderDlg;

				pDlg->Create(IDD_DIALOG_SLIDER, this);

				pDlg->UpdateWindow();

				pDlg->MoveWindow(0, m_iOffset, pDlg->GetWidth(), pDlg->GetHeight());

				pDlg->SetComponent(pCom);

				pDlg->ShowWindow(SW_SHOW);

				m_iOffset += pDlg->GetHeight();

				m_vecDialog.push_back(pDlg);
			}
			break;
			case SCENECOMPONENT_CLASS_TYPE::UI_FONT:
			{
				CRendererDlg* pRenDlg = new CRendererDlg;

				pRenDlg->Create(IDD_DIALOG_RENDERER, this);

				pRenDlg->UpdateWindow();

				pRenDlg->MoveWindow(0, m_iOffset, pRenDlg->GetWidth(), pRenDlg->GetHeight());

				pRenDlg->SetComponent(pCom);

				pRenDlg->ShowWindow(SW_SHOW);

				m_iOffset += pRenDlg->GetHeight();

				m_vecDialog.push_back(pRenDlg);

				CUIControlDlg* pConDlg = new CUIControlDlg;

				pConDlg->Create(IDD_DIALOG_UI_CONTROL, this);

				pConDlg->UpdateWindow();

				pConDlg->MoveWindow(0, m_iOffset, pConDlg->GetWidth(), pConDlg->GetHeight());

				pConDlg->SetComponent(pCom);

				pConDlg->ShowWindow(SW_SHOW);

				m_iOffset += pConDlg->GetHeight();

				m_vecDialog.push_back(pConDlg);

				CFontDlg* pDlg = new CFontDlg;

				pDlg->Create(IDD_DIALOG_FONT, this);

				pDlg->UpdateWindow();

				pDlg->MoveWindow(0, m_iOffset, pDlg->GetWidth(), pDlg->GetHeight());

				pDlg->SetComponent(pCom);

				pDlg->ShowWindow(SW_SHOW);

				m_iOffset += pDlg->GetHeight();

				m_vecDialog.push_back(pDlg);
			}
			break;
			case SCENECOMPONENT_CLASS_TYPE::UI_IMAGE:
			{
				CRendererDlg* pRenDlg = new CRendererDlg;

				pRenDlg->Create(IDD_DIALOG_RENDERER, this);

				pRenDlg->UpdateWindow();

				pRenDlg->MoveWindow(0, m_iOffset, pRenDlg->GetWidth(), pRenDlg->GetHeight());

				pRenDlg->SetComponent(pCom);

				pRenDlg->ShowWindow(SW_SHOW);

				m_iOffset += pRenDlg->GetHeight();

				m_vecDialog.push_back(pRenDlg);

				CUIControlDlg* pConDlg = new CUIControlDlg;

				pConDlg->Create(IDD_DIALOG_UI_CONTROL, this);

				pConDlg->UpdateWindow();

				pConDlg->MoveWindow(0, m_iOffset, pConDlg->GetWidth(), pConDlg->GetHeight());

				pConDlg->SetComponent(pCom);

				pConDlg->ShowWindow(SW_SHOW);

				m_iOffset += pConDlg->GetHeight();

				m_vecDialog.push_back(pConDlg);

				CUIImageDlg* pDlg = new CUIImageDlg;

				pDlg->Create(IDD_DIALOG_UI_IMAGE, this);

				pDlg->UpdateWindow();

				pDlg->MoveWindow(0, m_iOffset, pDlg->GetWidth(), pDlg->GetHeight());

				pDlg->SetComponent(pCom);

				pDlg->ShowWindow(SW_SHOW);

				m_iOffset += pDlg->GetHeight();

				m_vecDialog.push_back(pDlg);
			}
			break;
			case SCENECOMPONENT_CLASS_TYPE::UI_TITLE:
			{
				CRendererDlg* pRenDlg = new CRendererDlg;

				pRenDlg->Create(IDD_DIALOG_RENDERER, this);

				pRenDlg->UpdateWindow();

				pRenDlg->MoveWindow(0, m_iOffset, pRenDlg->GetWidth(), pRenDlg->GetHeight());

				pRenDlg->SetComponent(pCom);

				pRenDlg->ShowWindow(SW_SHOW);

				m_iOffset += pRenDlg->GetHeight();

				m_vecDialog.push_back(pRenDlg);

				CUIControlDlg* pConDlg = new CUIControlDlg;

				pConDlg->Create(IDD_DIALOG_UI_CONTROL, this);

				pConDlg->UpdateWindow();

				pConDlg->MoveWindow(0, m_iOffset, pConDlg->GetWidth(), pConDlg->GetHeight());

				pConDlg->SetComponent(pCom);

				pConDlg->ShowWindow(SW_SHOW);

				m_iOffset += pConDlg->GetHeight();

				m_vecDialog.push_back(pConDlg);

				CUITitleDlg* pDlg = new CUITitleDlg;

				pDlg->Create(IDD_DIALOG_UI_TITLE, this);

				pDlg->UpdateWindow();

				pDlg->MoveWindow(0, m_iOffset, pDlg->GetWidth(), pDlg->GetHeight());

				pDlg->SetComponent(pCom);

				pDlg->ShowWindow(SW_SHOW);

				m_iOffset += pDlg->GetHeight();

				m_vecDialog.push_back(pDlg);
			}
			break;
			case SCENECOMPONENT_CLASS_TYPE::SOUND:
			{
				CSoundDlg* pDlg = new CSoundDlg;

				pDlg->Create(IDD_DIALOG_SOUND, this);

				pDlg->UpdateWindow();

				pDlg->MoveWindow(0, m_iOffset, pDlg->GetWidth(), pDlg->GetHeight());

				pDlg->SetComponent(pCom);

				pDlg->ShowWindow(SW_SHOW);

				m_iOffset += pDlg->GetHeight();

				m_vecDialog.push_back(pDlg);
			}
				break;
			case SCENECOMPONENT_CLASS_TYPE::TILEMAP:
			{
				CTileMapDlg* pDlg = new CTileMapDlg;

				pDlg->Create(IDD_DIALOG_TILEMAP, this);

				pDlg->SetTileMap(pCom);

				CWnd* pWnd = pDlg->GetWindow(0);

				LONG iStyle = GetWindowLong(pWnd->m_hWnd, GWL_STYLE);

				iStyle |= WS_POPUP;

				iStyle &= ~WS_CHILD;

				SetWindowLong(pWnd->m_hWnd, GWL_STYLE, iStyle); // & WS_CHILD

				RECT	tRC = {};

				GetClientRect(&tRC);

				pDlg->MoveWindow(0, m_iOffset, tRC.right - tRC.left, tRC.bottom - tRC.top);

				pDlg->ShowWindow(SW_SHOW);

				m_vecDialog.push_back(pDlg);

				m_iOffset += pDlg->GetHeight();
			}
				break;
			case SCENECOMPONENT_CLASS_TYPE::TILE:
			{

			}
				break;
			}

			UpdateData(FALSE);
		}
	}
}

void CDetailObj::OnEnChangeEditColRadius()
{
	UpdateData(TRUE);

	HTREEITEM tItem = m_Component.GetSelectedItem();

	CString strTag = m_Component.GetItemText(tItem);

	std::string strCom = CT2CA(strTag);

	if (m_pSelectObj)
	{
		std::shared_ptr<CComponent> pCom = m_pSelectObj->FindComponent(strCom);

		if (!pCom)
		{
			pCom = m_pSelectObj->FindComponentByType<CColliderCircle>();

			if (pCom)
			{
				((CColliderCircle*)pCom.get())->SetRadius(m_fRadius);
			}
		}

		else
		{
			((CColliderCircle*)pCom.get())->SetRadius(m_fRadius);
		}
	}
}


void CDetailObj::OnEnChangeEditColExtentX()
{
	UpdateData(TRUE);

	HTREEITEM tItem = m_Component.GetSelectedItem();

	CString strTag = m_Component.GetItemText(tItem);

	std::string strCom = CT2CA(strTag);

	if (m_pSelectObj)
	{
		std::shared_ptr<CComponent> pCom = m_pSelectObj->FindComponent(strCom);
		
		if (!pCom)
		{
			pCom = m_pSelectObj->FindComponentByType<CColliderRect>();

			if (pCom)
			{
				((CColliderRect*)pCom.get())->SetExtend(m_fExtentX, m_fExtentY);
			}

			else
			{
				pCom = m_pSelectObj->FindComponentByType<CColliderOBB2D>();

				if (pCom)
				{
					((CColliderOBB2D*)pCom.get())->SetExtend(m_fExtentX, m_fExtentY, 0.f);
				}
			}
		}

		else
		{
			int iType = ((CSceneComponent*)pCom.get())->GetSceneComponentClassType();

			switch (iType)
			{
			case (int)SCENECOMPONENT_CLASS_TYPE::SCT_COLLIDER_RECT:
				((CColliderRect*)pCom.get())->SetExtend(m_fExtentX, m_fExtentY);
				break;
			case (int)SCENECOMPONENT_CLASS_TYPE::SCT_COLLIDER_OBB2D:
				((CColliderOBB2D*)pCom.get())->SetExtend(m_fExtentX, m_fExtentY, 0.f);
				break;
			}
		}
	}
}


void CDetailObj::OnEnChangeEditColExtenty()
{
	UpdateData(TRUE);

	HTREEITEM tItem = m_Component.GetSelectedItem();

	CString strTag = m_Component.GetItemText(tItem);

	std::string strCom = CT2CA(strTag);

	if (m_pSelectObj)
	{
		std::shared_ptr<CComponent> pCom = m_pSelectObj->FindComponent(strCom);

		if (!pCom)
		{
			pCom = m_pSelectObj->FindComponentByType<CColliderRect>();

			if (pCom)
			{
				((CColliderRect*)pCom.get())->SetExtend(m_fExtentX, m_fExtentY);
			}

			else
			{
				pCom = m_pSelectObj->FindComponentByType<CColliderOBB2D>();

				if (pCom)
				{
					((CColliderOBB2D*)pCom.get())->SetExtend(m_fExtentX, m_fExtentY, 0.f);
				}
			}
		}

		else
		{
			int iType = ((CSceneComponent*)pCom.get())->GetSceneComponentClassType();

			switch (iType)
			{
			case (int)SCENECOMPONENT_CLASS_TYPE::SCT_COLLIDER_RECT:
				((CColliderRect*)pCom.get())->SetExtend(m_fExtentX, m_fExtentY);
				break;
			case (int)SCENECOMPONENT_CLASS_TYPE::SCT_COLLIDER_OBB2D:
				((CColliderOBB2D*)pCom.get())->SetExtend(m_fExtentX, m_fExtentY, 0.f);
				break;
			}
		}
	}
}
