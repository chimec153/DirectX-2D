// AniEditor.cpp: 구현 파일
//

#include "pch.h"
#include "GameEditor.h"
#include "AniEditor.h"
#include "afxdialogex.h"
#include "PathManager.h"
#include "Resource/ResourceManager.h"
#include "Resource/Texture.h"
#include "Resource/Animation2DSequence.h"
#include "Object/GameObject.h"
#include "Scene/SceneManager.h"
#include "Scene/Scene.h"
#include "Component/SpriteComponent.h"

// CAniEditor 대화 상자

IMPLEMENT_DYNAMIC(CAniEditor, CDialogEx)

CAniEditor::CAniEditor(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_ANIEDIT, pParent)
	, m_strTag(_T(""))
	, m_strTex(_T(""))
	, m_fRate(0)
	, m_fMaxTime(0)
{

}

CAniEditor::~CAniEditor()
{
}

void CAniEditor::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_TAG, m_strTag);
	DDX_Text(pDX, IDC_EDIT_TEX_TAG, m_strTex);
	DDX_Control(pDX, IDC_LIST_SEQUENCE, m_hList);
	DDX_Text(pDX, IDC_EDIT_RATE, m_fRate);
	DDX_Text(pDX, IDC_EDIT_TIME, m_fMaxTime);
}


BEGIN_MESSAGE_MAP(CAniEditor, CDialogEx)
	ON_EN_CHANGE(IDC_EDIT_TAG, &CAniEditor::OnEnChangeEditTag)
	ON_EN_CHANGE(IDC_EDIT_TEX_TAG, &CAniEditor::OnEnChangeEditTexTag)
	ON_BN_CLICKED(IDC_BUTTON_TEXTURE_LOAD, &CAniEditor::OnBnClickedButtonTextureLoad)
	ON_BN_CLICKED(IDC_BUTTON_ADD_SEQUENCE, &CAniEditor::OnBnClickedButtonAddSequence)
	ON_LBN_SELCHANGE(IDC_LIST_SEQUENCE, &CAniEditor::OnLbnSelchangeListSequence)
	ON_EN_CHANGE(IDC_EDIT_RATE, &CAniEditor::OnEnChangeEditRate)
	ON_EN_CHANGE(IDC_EDIT_TIME, &CAniEditor::OnEnChangeEditTime)
	ON_BN_CLICKED(IDC_BUTTON_SAVE_SEQUENCE, &CAniEditor::OnBnClickedButtonSaveSequence)
	ON_BN_CLICKED(IDC_BUTTON_LOAD_SEQUENCE, &CAniEditor::OnBnClickedButtonLoadSequence)
END_MESSAGE_MAP()


// CAniEditor 메시지 처리기


void CAniEditor::OnEnChangeEditTag()
{
}

void CAniEditor::OnEnChangeEditTexTag()
{
}

void CAniEditor::OnBnClickedButtonTextureLoad()
{
	CFileDialog dlg(TRUE, TEXT(""), TEXT(""), OFN_HIDEREADONLY | OFN_ALLOWMULTISELECT, 
		TEXT("이미지 파일(*.JPG,*.BMP,*.PNG,*.DDS)|*.jpg;*.bmp;*.png;*.dds;*.JPG;*.BMP;*.PNG;*.DDS|모든 파일(*.*)|*.*||"));

	TCHAR strFile[2048] = {};

	dlg.m_ofn.lpstrInitialDir = GET_SINGLE(CPathManager)->FindPath(TEXTURE_PATH);
	dlg.m_ofn.nMaxFile = 2048;
	dlg.m_ofn.lpstrFile = strFile;

	if (dlg.DoModal() == IDOK)
	{
		POSITION pos = dlg.GetStartPosition();

		std::vector<const TCHAR*> vecFullPath;

		while (pos)
		{
			CString strName = dlg.GetNextPathName(pos);

			std::wstring strTag = CT2CW(strName);

			TCHAR* pFullPath = new TCHAR[MAX_PATH];

			memset(pFullPath, 0, sizeof(TCHAR) * MAX_PATH);

			lstrcpy(pFullPath, strTag.c_str());

			vecFullPath.push_back(pFullPath);
		}

		UpdateData(TRUE);

		std::string strTag = CT2CA(m_strTex);

		GET_SINGLE(CResourceManager)->LoadTextureFromFullPath(strTag, vecFullPath);

		SAFE_DELETE_ARRAY_VECLIST(vecFullPath);
	}
}


void CAniEditor::OnBnClickedButtonAddSequence()
{
	UpdateData(TRUE);

	std::string strTex = CT2CA(m_strTex);
	std::string strTag = CT2CA(m_strTag);

	std::shared_ptr<CTexture> pTex = GET_SINGLE(CResourceManager)->FindTexture(strTex);

	if(GET_SINGLE(CResourceManager)->CreateAni2DSequence(strTag, pTex, m_fRate, m_fMaxTime))
		m_hList.AddString(m_strTag);

	
}

void CAniEditor::OnLbnSelchangeListSequence()
{
	int iSel = m_hList.GetCurSel();

	CString strTag;

	m_hList.GetText(iSel, strTag);

	std::string strName = CT2CA(strTag);

	std::shared_ptr<CAnimation2DSequence> pSeq = GET_SINGLE(CResourceManager)->FindAni2DSequence(strName);

	if (pSeq)
	{
		m_fMaxTime = pSeq->GetMaxTime();
		m_fRate = pSeq->GetPlayRate();
		m_strTag = CA2CT(pSeq->GetName().c_str());

		std::shared_ptr<CTexture> pTex = pSeq->GetTexture();

		if (pTex)
		{
			m_strTex = CA2CT(pTex->GetName().c_str());
		}

		CLayer* pLayer = GET_SINGLE(CSceneManager)->GetScene()->FindLayer("Default");

		std::shared_ptr<CGameObject> pObj = pLayer->FindObj("Minion");

		if (pObj)
		{
			std::shared_ptr<CComponent> pCom = pObj->FindComponent("Mesh");

			if (pCom)
			{
				((CSpriteComponent*)pCom.get())->AddAni2DInfo(pSeq->GetName(), pSeq, m_fRate, true);

				((CSpriteComponent*)pCom.get())->ChangeClip(pSeq->GetName());
			}
		}
	}

	UpdateData(FALSE);
}


void CAniEditor::OnEnChangeEditRate()
{
}


void CAniEditor::OnEnChangeEditTime()
{
}


void CAniEditor::OnBnClickedButtonSaveSequence()
{
	const TCHAR strFilter[] = TEXT("시퀀스 파일(*.rcs)|*.rcs|모든 파일(*.*)|*.*||");

	CFileDialog dlg(FALSE, TEXT(""), TEXT(""), OFN_HIDEREADONLY, strFilter);

	dlg.m_ofn.lpstrInitialDir = GET_SINGLE(CPathManager)->FindPath(DATA_PATH);

	if (dlg.DoModal() == IDOK)
	{
		POSITION pos = dlg.GetStartPosition();

		 CString strFullPath = dlg.GetNextPathName(pos);

		std::wstring pFullPath = CT2CW(strFullPath);

		int iSel = m_hList.GetCurSel();

		CString strSeq;

		m_hList.GetText(iSel, strSeq);

		std::string strSequence = CT2CA(strSeq);

		GET_SINGLE(CResourceManager)->SaveAni2DSeqFromFullPath(strSequence.c_str(), pFullPath.c_str());
	}
}


void CAniEditor::OnBnClickedButtonLoadSequence()
{
	const TCHAR strFilter[] = TEXT("시퀀스 파일(*.rcs)|*.rcs|모든 파일(*.*)|*.*||");

	CFileDialog dlg(TRUE, TEXT(""), TEXT(""), OFN_HIDEREADONLY, strFilter);

	dlg.m_ofn.lpstrInitialDir = GET_SINGLE(CPathManager)->FindPath(DATA_PATH);

	if (dlg.DoModal() == IDOK)
	{
		POSITION pos = dlg.GetStartPosition();

		CString strFullPath = dlg.GetNextPathName(pos);

		std::wstring strFull = CT2CW(strFullPath);

		std::shared_ptr<CAnimation2DSequence> pSeq = GET_SINGLE(CResourceManager)->LoadAni2DSeqFromFullPath(strFull.c_str());

		if (pSeq)
		{
			CString strTag = CA2CT(pSeq->GetName().c_str());
			m_hList.AddString(strTag);
		}
	}
}
