#include "pch.h"
#include "ViewManager.h"
#include "DetailObj.h"
#include "Scene/SceneManager.h"
#include "WorldObj.h"
#include "MainFrm.h"

DEFINITION_SINGLE(CViewManager)

CViewManager::CViewManager()	:
	m_pWorldObj(nullptr),
	m_pDetailObj(nullptr)
{

}

CViewManager::~CViewManager()
{

}

bool CViewManager::Init(CWorldObj* pWorldObj, CDetailObj* pDetailObj)
{
	m_pWorldObj = pWorldObj;
	m_pDetailObj = pDetailObj;

	GET_SINGLE(CSceneManager)->SetEditorUpdateFunc(m_pDetailObj, &CDetailObj::Update);
	GET_SINGLE(CSceneManager)->SetEditorCreateFunc(m_pWorldObj, &CWorldObj::AddObject);
	GET_SINGLE(CSceneManager)->SetEditorDeleteFunc(m_pWorldObj, &CWorldObj::DeleteObject);

	return true;
}

void CViewManager::ClearProtoList()
{
	CWnd* pWnd = AfxGetMainWnd();

	if(pWnd)
	((CMainFrame*)pWnd)->ClearCreateDlg();
}
