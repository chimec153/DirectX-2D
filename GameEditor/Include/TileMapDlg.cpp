// TileMap.cpp: 구현 파일
//

#include "pch.h"
#include "GameEditor.h"
#include "TileMapDlg.h"
#include "afxdialogex.h"
#include "Component/TileMap.h"
#include "PathManager.h"
#include "Resource/ResourceManager.h"
#include "RenderManager.h"
#include "Scene/Scene.h"
#include "Scene/SceneManager.h"
#include "Tile.h"
#include "Resource/Material.h"
#include "Resource/Texture.h"

// CTileMap 대화 상자

IMPLEMENT_DYNAMIC(CTileMapDlg, CDialogEx)

CTileMapDlg::CTileMapDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_TILEMAP, pParent)
	, m_bModify(FALSE)
	, m_iTileHeight(0)
	, m_eOption(TILE_OPTION::NONE)
	, m_eType(TILE_TYPE::RECT)
	, m_iIdx(0)
	, m_pTileMap(nullptr)
	, m_iWidth(0)
	, m_iHeight(0)
	, m_fSizex(0)
	, m_strTex(_T(""))
{

}

CTileMapDlg::~CTileMapDlg()
{
}

void CTileMapDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECK_MODIFY, m_bModify);
	DDX_Text(pDX, IDC_EDIT_HEIGHT, m_iTileHeight);
	DDX_Control(pDX, IDC_COMBO_TILE, m_TileCombo);
	DDX_Control(pDX, IDC_COMBO_TILE_INDEX, m_IndexCombo);
	DDX_Control(pDX, IDC_COMBO_TILE_OPTION, m_OptionCombo);
	DDX_Text(pDX, IDC_EDIT_TILE_SIZE_X, m_vTileSize.x);
	DDX_Text(pDX, IDC_EDIT_TILE_SIZE_Y, m_vTileSize.y);
	DDX_Text(pDX, IDC_EDIT_TILE_CNT_X, m_iCountX);
	DDX_Text(pDX, IDC_EDIT_TILE_CNT_Y, m_iCountY);
	DDX_Text(pDX, IDC_EDIT_TILE_TEXTURE, m_strTex);
}


BEGIN_MESSAGE_MAP(CTileMapDlg, CDialogEx)
	ON_CBN_SELCHANGE(IDC_COMBO_TILE, &CTileMapDlg::OnCbnSelchangeComboTile)
	ON_CBN_SELCHANGE(IDC_COMBO_TILE_INDEX, &CTileMapDlg::OnCbnSelchangeComboTileIndex)
	ON_CBN_SELCHANGE(IDC_COMBO_TILE_OPTION, &CTileMapDlg::OnCbnSelchangeComboTileOption)
	ON_BN_CLICKED(IDC_CHECK_MODIFY, &CTileMapDlg::OnBnClickedCheckModify)
	ON_EN_CHANGE(IDC_EDIT_HEIGHT, &CTileMapDlg::OnEnChangeEditHeight)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, &CTileMapDlg::OnBnClickedButtonSave)
	ON_BN_CLICKED(IDC_BUTTON_LOAD, &CTileMapDlg::OnBnClickedButtonLoad)
	ON_EN_CHANGE(IDC_EDIT_TILE_SIZE_X, &CTileMapDlg::OnEnChangeEditTileSizeX)
	ON_EN_CHANGE(IDC_EDIT_TILE_SIZE_Y, &CTileMapDlg::OnEnChangeEditTileSizeY)
	ON_EN_CHANGE(IDC_EDIT_TILE_CNT_X, &CTileMapDlg::OnEnChangeEditTileCntX)
	ON_BN_CLICKED(IDC_BUTTON_CREATE_TILE, &CTileMapDlg::OnBnClickedButtonCreateTile)
	ON_EN_CHANGE(IDC_EDIT_TILE_TEXTURE, &CTileMapDlg::OnEnChangeEditTileTexture)
	ON_BN_CLICKED(IDC_BUTTON_LOAD_TILE_TEXTURE, &CTileMapDlg::OnBnClickedButtonLoadTileTexture)
END_MESSAGE_MAP()

void CTileMapDlg::OnCbnSelchangeComboTile()
{
	m_eType = (TILE_TYPE)m_TileCombo.GetCurSel();

	if (m_pTileMap)
	{
		((CTileMap*)m_pTileMap.get())->SetTileType(m_eType);
	}
}

void CTileMapDlg::OnCbnSelchangeComboTileIndex()
{
	m_iIdx = m_IndexCombo.GetCurSel();

	if (m_pTileMap)
	{
		((CTileMap*)m_pTileMap.get())->SetTileIdx(m_iIdx);
	}
}

void CTileMapDlg::OnCbnSelchangeComboTileOption()
{
	m_eOption = (TILE_OPTION)m_OptionCombo.GetCurSel();

	if (m_pTileMap)
	{
		((CTileMap*)m_pTileMap.get())->SetOption(m_eOption);
	}
}

void CTileMapDlg::OnBnClickedCheckModify()
{
	UpdateData(TRUE);

	if (m_pTileMap)
	{
		((CTileMap*)m_pTileMap.get())->SetEdit(m_bModify);
	}
}

void CTileMapDlg::OnEnChangeEditHeight()
{
	UpdateData(TRUE);

	if (m_pTileMap)
	{
		((CTileMap*)m_pTileMap.get())->SetTileHeight(m_iTileHeight);
	}
}

void CTileMapDlg::SetTileMap(std::shared_ptr<CComponent> pCom)
{
	m_pTileMap = pCom;

	m_eType = ((CTileMap*)m_pTileMap.get())->GetTileType();
	
	m_eOption = ((CTileMap*)m_pTileMap.get())->GetTileOption();

	Vector3 vSize = ((CTileMap*)m_pTileMap.get())->GetTileSize();

	m_vTileSize = Vector2(vSize.x, vSize.y);

	m_iCountX = ((CTileMap*)m_pTileMap.get())->GetCountX();
	m_iCountY = ((CTileMap*)m_pTileMap.get())->GetCountY();

	CTile* pTile = ((CTileMap*)m_pTileMap.get())->GetTile(0);

	std::shared_ptr<CMaterial> pMtrl = pTile->GetMaterial();

	PMaterialTextureInfo pInfo = pMtrl->GetTexInfo(0);

	m_strTex = CA2CT(pInfo->pTexture->GetName().c_str());

	m_TileCombo.SetCurSel((int)m_eType);
	m_OptionCombo.SetCurSel((int)m_eOption);

	int iTex = pTile->GetTileIndex();

	m_IndexCombo.SetCurSel(iTex);

	UpdateData(FALSE);
}


BOOL CTileMapDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	for (int i = 0; i < 41; ++i)
	{
		CString strNum;

		char pNum[32] = {};

		sprintf_s(pNum, "%d", i);

		strNum = CA2CT(pNum);

		m_IndexCombo.AddString(strNum);
	}

	RECT tRC = {};

	GetClientRect(&tRC);

	m_iWidth = tRC.right - tRC.left;
	m_iHeight = tRC.bottom - tRC.top;

	return TRUE;
}

int CTileMapDlg::GetWidth() const
{
	return m_iWidth;
}

int CTileMapDlg::GetHeight() const
{
	return m_iHeight;
}


void CTileMapDlg::OnBnClickedButtonSave()
{
	TCHAR strFilter[MAX_PATH] = TEXT("맵 파일(*.rtm)|*.rtm|모든 파일(*.*)|*.*||");

	CFileDialog dlg(FALSE, TEXT(""),TEXT(""), OFN_OVERWRITEPROMPT, strFilter);

	dlg.m_ofn.lpstrInitialDir = GET_SINGLE(CPathManager)->FindPath(DATA_PATH);

	if (dlg.DoModal() == IDOK)
	{
		CString strPath = dlg.GetPathName();

		std::string pPath = CT2CA(strPath);

		FILE* pFile = nullptr;

		fopen_s(&pFile, pPath.c_str(), "wb");

		if (pFile)
		{
			((CTileMap*)m_pTileMap.get())->Save(pFile);

			fclose(pFile);
		}
	}
}


void CTileMapDlg::OnBnClickedButtonLoad()
{
	TCHAR strFilter[MAX_PATH] = TEXT("맵 파일(*.rtm)|*.rtm|모든 파일(*.*)|*.*||");

	CFileDialog dlg(TRUE, TEXT(""), TEXT(""), OFN_HIDEREADONLY, strFilter);

	dlg.m_ofn.lpstrInitialDir = GET_SINGLE(CPathManager)->FindPath(DATA_PATH);

	if (dlg.DoModal() == IDOK)
	{
		CString strPath = dlg.GetPathName();

		std::string pPath = CT2CA(strPath);

		FILE* pFile = nullptr;

		fopen_s(&pFile, pPath.c_str(), "rb");

		if (pFile)
		{
			((CTileMap*)m_pTileMap.get())->Load(pFile);

			fclose(pFile);
		}
	}
}


void CTileMapDlg::OnEnChangeEditTileSizeX()
{
	UpdateData(TRUE);
}


void CTileMapDlg::OnEnChangeEditTileSizeY()
{
	UpdateData(TRUE);
}


void CTileMapDlg::OnEnChangeEditTileCntX()
{
	UpdateData(TRUE);
}

void CTileMapDlg::OnEnChangeEditTileCntY()
{
	UpdateData(TRUE);
}

void CTileMapDlg::OnBnClickedButtonCreateTile()
{
	if (!m_pTileMap)
	{
		CScene* pScene = GET_SINGLE(CSceneManager)->GetScene();

		CLayer* pLayer = pScene->FindLayer("Back");

		std::shared_ptr<CGameObject> pObj = pScene->CreateCloneObj("TileMap", "TileMap", pLayer);

		if (!pObj)
			return;

		m_pTileMap = pObj->FindComponentByType<CTileMap>();
	}

	Vector3 vPos = ((CSceneComponent*)m_pTileMap.get())->GetWorldPos();

	((CTileMap*)m_pTileMap.get())->CreateTile(m_eType, m_iCountX, m_iCountY, vPos, m_vTileSize, m_eOption, m_iIdx);
}

void CTileMapDlg::OnEnChangeEditTileTexture()
{
	UpdateData(TRUE);
}

void CTileMapDlg::OnBnClickedButtonLoadTileTexture()
{
	UpdateData(TRUE);

	TCHAR strFilter[MAX_PATH] = TEXT("이미지 파일(*.bmp, *.jpg, *.png)|*.bmp;*.png;*.jpg|모든 파일(*.*)|*.*||");

	CFileDialog dlg(TRUE, TEXT(""), TEXT(""), OFN_HIDEREADONLY | OFN_ALLOWMULTISELECT, strFilter);

	CString strFileName;

	TCHAR* pFileName = strFileName.GetBuffer((99 * (MAX_PATH + 1)) + 1);

	const int iFileSize = (99 * (MAX_PATH + 1)) + 1;

	dlg.m_ofn.nMaxFile = iFileSize;
	dlg.m_ofn.lpstrFile = pFileName;
	dlg.m_ofn.lpstrInitialDir = GET_SINGLE(CPathManager)->FindPath(TEXTURE_PATH);

	if (dlg.DoModal() == IDOK)
	{
		strFileName.ReleaseBuffer();

		POSITION tPos = dlg.GetStartPosition();

		std::vector<const TCHAR*>	vecFilePath;

		while (tPos)
		{
			CString strPath = dlg.GetNextPathName(tPos);

			 TCHAR* pPath = new TCHAR[MAX_PATH];

			memset(pPath, 0, sizeof(TCHAR) * MAX_PATH);
			
			const TCHAR* pPathName = CT2CW(strPath);

			memcpy(pPath, pPathName, sizeof(TCHAR) * MAX_PATH);

			vecFilePath.push_back(pPath);
		}

		std::string pTag = CT2CA(m_strTex);

		if (vecFilePath.size() == 1)
		{
			GET_SINGLE(CResourceManager)->LoadTextureFromFullPath(pTag, vecFilePath[0]);

			if(m_pTileMap)
			((CTileMap*)m_pTileMap.get())->SetArrayTex(false);
		}			

		else
		{
			GET_SINGLE(CResourceManager)->LoadTextureArrayFromFullPath(pTag, vecFilePath);

			if (m_pTileMap)
			((CTileMap*)m_pTileMap.get())->SetArrayTex(true);
		}

		if (m_pTileMap)
		GET_SINGLE(CRenderManager)->SetTileInstTex(pTag, ((CTileMap*)m_pTileMap.get())->IsArrayTex());

		SAFE_DELETE_ARRAY_VECLIST(vecFilePath);
	}
}