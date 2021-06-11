// RendererDlg.cpp: 구현 파일
//

#include "pch.h"
#include "GameEditor.h"
#include "RendererDlg.h"
#include "afxdialogex.h"
#include "Component/SceneComponent.h"
#include "Resource/Mesh.h"
#include "Resource/Material.h"
#include "Resource/Texture.h"
#include "Resource/Shader.h"
#include "Layer/Layer.h"
#include "Scene/Scene.h"

// CRendererDlg 대화 상자

IMPLEMENT_DYNAMIC(CRendererDlg, CDialogEx)

CRendererDlg::CRendererDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_RENDERER, pParent)
	, m_bInst(FALSE)
	, m_iInstCount(0)
	, m_MeshMatReg(0)
	, m_iMeshLinkType(0)
	, m_fMeshShaderType(0)
	, m_iMeshDescSize(0)
	, m_iMeshInstSize(0)
	, m_iMeshShaderType(0)
	, m_iMatReg(0)
	, m_iMatLinkType(0)
	, m_iMatTexType(0)
	, m_iMatDescSize(0)
	, m_iMatInstSize(0)
	, m_iShaderType(0)
	, m_strMeshMatTex(_T(""))
	, m_strMeshTag(_T(""))
	, m_strMeshMatTag(_T(""))
	, m_strMeshShaderTag(_T(""))
	, m_strMatTag(_T(""))
	, m_strShaderTag(_T(""))
	, m_strMatTex(_T(""))
	, m_strLayer(_T(""))
	, m_iScene(0)
{

}

CRendererDlg::~CRendererDlg()
{
}

void CRendererDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECK_INST, m_bInst);
	DDX_Text(pDX, IDC_EDIT_MIN_X, m_vMin.x);
	DDX_Text(pDX, IDC_EDIT_MIN_Y, m_vMin.y);
	DDX_Text(pDX, IDC_EDIT_MIN_Z, m_vMin.z);
	DDX_Text(pDX, IDC_EDIT_MAX_X, m_vMax.x);
	DDX_Text(pDX, IDC_EDIT_MAX_Y, m_vMax.y);
	DDX_Text(pDX, IDC_EDIT_MAX_Z, m_vMax.z);
	DDX_Text(pDX, IDC_EDIT_INST_COUNT, m_iInstCount);
	DDX_Control(pDX, IDC_COMBO_MESH_TEXTUREINFO, m_MeshMatTexInfo);
	DDX_Text(pDX, IDC_EDIT_MESH_MAT_REGISTER, m_MeshMatReg);
	DDX_Text(pDX, IDC_EDIT_MESH_MAT_LINK_TYPE, m_iMeshLinkType);
	DDX_Text(pDX, IDC_EDIT_MESH_MAT_SHADER_TYPE, m_fMeshShaderType);
	DDX_Text(pDX, IDC_EDIT_MESH_MAT_SHADER_DESC_SIZE, m_iMeshDescSize);
	DDX_Text(pDX, IDC_EDIT_MESH_MAT_SHADER_INST_SIZE, m_iMeshInstSize);
	DDX_Text(pDX, IDC_EDIT_MESH_MAT_SHADERTYPE, m_iMeshShaderType);
	DDX_Control(pDX, IDC_COMBO_MAT_TEX_INFO, m_MatTexInfo);
	DDX_Text(pDX, IDC_EDIT_MAT_REGISTER, m_iMatReg);
	DDX_Text(pDX, IDC_EDIT_MAT_LINE_TYPE, m_iMatLinkType);
	DDX_Text(pDX, IDC_EDIT_MAT_SHADER_TYPE, m_iMatTexType);
	DDX_Text(pDX, IDC_EDIT_MAT_SHADER_DESC_SIZE, m_iMatDescSize);
	DDX_Text(pDX, IDC_EDIT_MAT_SHADER_INST_SIZE, m_iMatInstSize);
	DDX_Text(pDX, IDC_EDIT_MAT_SHADERTYPE, m_iShaderType);
	DDX_Text(pDX, IDC_EDIT_MESH_MAT_TEX, m_strMeshMatTex);
	DDX_Text(pDX, IDC_EDIT_MESH_NAME, m_strMeshTag);
	DDX_Text(pDX, IDC_EDIT_MESH_MAT_NAME, m_strMeshMatTag);
	DDX_Text(pDX, IDC_EDIT_MESH_SHADER_NAME, m_strMeshShaderTag);
	DDX_Text(pDX, IDC_EDIT_MAT_NAME, m_strMatTag);
	DDX_Text(pDX, IDC_EDIT_SHADER_NAME, m_strShaderTag);
	DDX_Text(pDX, IDC_EDIT_MAT_TEX, m_strMatTex);
	DDX_Text(pDX, IDC_EDIT_LAYER, m_strLayer);
	DDX_Text(pDX, IDC_EDIT_SCENE, m_iScene);
}

int CRendererDlg::GetWidth() const
{
	return m_iWidth;
}

int CRendererDlg::GetHeight() const
{
	return m_iHeight;
}

void CRendererDlg::SetComponent(std::shared_ptr<class CComponent> pCom)
{
	m_pCom = pCom;

	std::shared_ptr<CMesh> pMesh = ((CSceneComponent*)pCom.get())->GetMesh();

	m_strMeshTag = CA2CT(pMesh->GetName().c_str());

	m_vMax = pMesh->GetMax();
	m_vMin = pMesh->GetMin();
	m_iInstCount = pMesh->GetInstCount();

	m_bInst = ((CSceneComponent*)pCom.get())->IsInstance();

	std::shared_ptr<CMaterial> pMeshMat = pMesh->GetMaterial();

	if (pMeshMat)
	{
		m_strMeshMatTag = CA2CT(pMeshMat->GetName().c_str());

		std::shared_ptr<CShader> pShader = pMeshMat->GetShader();

		m_strMeshShaderTag = CA2CT(pShader->GetName().c_str());

		m_iMeshDescSize = pShader->GetDescSize();

		m_iMeshInstSize = pShader->GetInstSize();

		m_iMeshShaderType = (int)pShader->GetShaderType();

		size_t iSz = pMeshMat->GetInfoSize();

		for (size_t i = 0; i < iSz; ++i)
		{
			PMaterialTextureInfo pInfo = pMeshMat->GetTexInfo((int)i);

			std::string strTag = pInfo->pTexture->GetName();

			CString pTag = CA2CT(strTag.c_str());

			m_MeshMatTexInfo.AddString(pTag);

			if (i == 0)
			{
				m_strMeshMatTex = CA2CT(strTag.c_str());
				m_iMeshLinkType = (int)pInfo->eLink;
				m_iMeshShaderType = pInfo->iShaderType;
				m_MeshMatReg = pInfo->iRegister;
			}
		}

		std::shared_ptr<CMaterial> pMat = ((CSceneComponent*)pCom.get())->GetMaterial();

		m_strMatTag = CA2CT(pMat->GetName().c_str());

		iSz = pMat->GetInfoSize();

		for (size_t i = 0; i < iSz; ++i)
		{
			PMaterialTextureInfo pInfo = pMat->GetTexInfo((int)i);

			std::string strTag = pInfo->pTexture->GetName();

			CString pTag = CA2CT(strTag.c_str());

			m_MatTexInfo.AddString(pTag);

			if (i == 0)
			{
				m_strMatTex = CA2CT(strTag.c_str());
				m_iMatLinkType = (int)pInfo->eLink;
				m_iMatTexType = pInfo->iShaderType;
				m_iMatReg = pInfo->iRegister;
			}
		}

		pShader = pMat->GetShader();

		m_strShaderTag = CA2CT(pShader->GetName().c_str());

		m_iMatDescSize = pShader->GetDescSize();

		m_iMatInstSize = pShader->GetInstSize();

		m_iShaderType = (int)pShader->GetShaderType();
	}

	CLayer* pLayer =pCom->GetLayer();

	m_strLayer = CA2CT(pLayer->GetName().c_str());

	CScene* pScene = pCom->GetScene();

	m_iScene = (int)pScene->GetSceneType();

	UpdateData(FALSE);
}


BEGIN_MESSAGE_MAP(CRendererDlg, CDialogEx)
	ON_BN_CLICKED(IDC_CHECK_INST, &CRendererDlg::OnBnClickedCheckInst)
	ON_EN_CHANGE(IDC_EDIT_MIN_X, &CRendererDlg::OnEnChangeEditMinX)
	ON_EN_CHANGE(IDC_EDIT_MIN_Y, &CRendererDlg::OnEnChangeEditMinY)
	ON_EN_CHANGE(IDC_EDIT_MIN_Z, &CRendererDlg::OnEnChangeEditMinZ)
	ON_EN_CHANGE(IDC_EDIT_MAX_X, &CRendererDlg::OnEnChangeEditMaxX)
	ON_EN_CHANGE(IDC_EDIT_MAX_Y, &CRendererDlg::OnEnChangeEditMaxY)
	ON_EN_CHANGE(IDC_EDIT_MAX_Z, &CRendererDlg::OnEnChangeEditMaxZ)
	ON_EN_CHANGE(IDC_EDIT_INST_COUNT, &CRendererDlg::OnEnChangeEditInstCount)
	ON_CBN_SELCHANGE(IDC_COMBO_MESH_TEXTUREINFO, &CRendererDlg::OnCbnSelchangeComboMeshTextureinfo)
	ON_EN_CHANGE(IDC_EDIT_MESH_MAT_REGISTER, &CRendererDlg::OnEnChangeEditMeshMatRegister)
	ON_EN_CHANGE(IDC_EDIT_MESH_MAT_LINK_TYPE, &CRendererDlg::OnEnChangeEditMeshMatLinkType)
	ON_EN_CHANGE(IDC_EDIT_MESH_MAT_SHADER_TYPE, &CRendererDlg::OnEnChangeEditMeshMatShaderType)
	ON_EN_CHANGE(IDC_EDIT_MESH_MAT_SHADER_DESC_SIZE, &CRendererDlg::OnEnChangeEditMeshMatShaderDescSize)
	ON_EN_CHANGE(IDC_EDIT_MESH_MAT_SHADER_INST_SIZE, &CRendererDlg::OnEnChangeEditMeshMatShaderInstSize)
	ON_EN_CHANGE(IDC_EDIT_MESH_MAT_SHADERTYPE, &CRendererDlg::OnEnChangeEditMeshMatShadertype)
	ON_CBN_SELCHANGE(IDC_COMBO_MAT_TEX_INFO, &CRendererDlg::OnCbnSelchangeComboMatTexInfo)
	ON_EN_CHANGE(IDC_EDIT_MAT_REGISTER, &CRendererDlg::OnEnChangeEditMatRegister)
	ON_EN_CHANGE(IDC_EDIT_MAT_LINE_TYPE, &CRendererDlg::OnEnChangeEditMatLineType)
	ON_EN_CHANGE(IDC_EDIT_MAT_SHADER_TYPE, &CRendererDlg::OnEnChangeEditMatShaderType)
	ON_EN_CHANGE(IDC_EDIT_MAT_SHADER_DESC_SIZE, &CRendererDlg::OnEnChangeEditMatShaderDescSize)
	ON_EN_CHANGE(IDC_EDIT_MAT_SHADER_INST_SIZE, &CRendererDlg::OnEnChangeEditMatShaderInstSize)
	ON_EN_CHANGE(IDC_EDIT_MAT_SHADERTYPE, &CRendererDlg::OnEnChangeEditMatShadertype)
	ON_EN_CHANGE(IDC_EDIT_MESH_MAT_TEX, &CRendererDlg::OnEnChangeEditMeshMatTex)
	ON_EN_CHANGE(IDC_EDIT_MAT_TEX, &CRendererDlg::OnEnChangeEditMatTex)
	ON_BN_CLICKED(IDC_BUTTON_LOAD_MESH_TEX, &CRendererDlg::OnBnClickedButtonLoadMeshTex)
	ON_BN_CLICKED(IDC_BUTTON_LOAD_TEX, &CRendererDlg::OnBnClickedButtonLoadTex)
	ON_EN_CHANGE(IDC_EDIT_MESH_NAME, &CRendererDlg::OnEnChangeEditMeshName)
	ON_EN_CHANGE(IDC_EDIT_MESH_MAT_NAME, &CRendererDlg::OnEnChangeEditMeshMatName)
	ON_EN_CHANGE(IDC_EDIT_MESH_SHADER_NAME, &CRendererDlg::OnEnChangeEditMeshShaderName)
	ON_EN_CHANGE(IDC_EDIT_MAT_NAME, &CRendererDlg::OnEnChangeEditMatName)
	ON_EN_CHANGE(IDC_EDIT_SHADER_NAME, &CRendererDlg::OnEnChangeEditShaderName)
	ON_EN_CHANGE(IDC_EDIT_LAYER, &CRendererDlg::OnEnChangeEditLayer)
	ON_EN_CHANGE(IDC_EDIT_SCENE, &CRendererDlg::OnEnChangeEditScene)
END_MESSAGE_MAP()


void CRendererDlg::OnBnClickedCheckInst()
{
	UpdateData(TRUE);
}


void CRendererDlg::OnEnChangeEditMinX()
{
	UpdateData(TRUE);
}


void CRendererDlg::OnEnChangeEditMinY()
{
	UpdateData(TRUE);
}


void CRendererDlg::OnEnChangeEditMinZ()
{
	UpdateData(TRUE);
}


void CRendererDlg::OnEnChangeEditMaxX()
{
	UpdateData(TRUE);
}


void CRendererDlg::OnEnChangeEditMaxY()
{
	UpdateData(TRUE);
}


void CRendererDlg::OnEnChangeEditMaxZ()
{
	UpdateData(TRUE);
}


void CRendererDlg::OnEnChangeEditInstCount()
{
	UpdateData(TRUE);
}


void CRendererDlg::OnCbnSelchangeComboMeshTextureinfo()
{
}


void CRendererDlg::OnEnChangeEditMeshMatRegister()
{
	UpdateData(TRUE);
}


void CRendererDlg::OnEnChangeEditMeshMatLinkType()
{
	UpdateData(TRUE);
}


void CRendererDlg::OnEnChangeEditMeshMatShaderType()
{
	UpdateData(TRUE);
}


void CRendererDlg::OnEnChangeEditMeshMatShaderDescSize()
{
	UpdateData(TRUE);
}


void CRendererDlg::OnEnChangeEditMeshMatShaderInstSize()
{
	UpdateData(TRUE);
}


void CRendererDlg::OnEnChangeEditMeshMatShadertype()
{
	UpdateData(TRUE);
}


void CRendererDlg::OnCbnSelchangeComboMatTexInfo()
{
	UpdateData(TRUE);
}


void CRendererDlg::OnEnChangeEditMatRegister()
{
	UpdateData(TRUE);
}


void CRendererDlg::OnEnChangeEditMatLineType()
{
	UpdateData(TRUE);
}


void CRendererDlg::OnEnChangeEditMatShaderType()
{
	UpdateData(TRUE);
}


void CRendererDlg::OnEnChangeEditMatShaderDescSize()
{
	UpdateData(TRUE);
}


void CRendererDlg::OnEnChangeEditMatShaderInstSize()
{
	UpdateData(TRUE);
}


void CRendererDlg::OnEnChangeEditMatShadertype()
{
	UpdateData(TRUE);
}


BOOL CRendererDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	RECT tRC = {};

	GetClientRect(&tRC);

	m_iWidth = tRC.right - tRC.left;
	m_iHeight = tRC.bottom - tRC.top;

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}



void CRendererDlg::OnEnChangeEditMeshMatTex()
{
}


void CRendererDlg::OnEnChangeEditMatTex()
{
}


void CRendererDlg::OnBnClickedButtonLoadMeshTex()
{
}


void CRendererDlg::OnBnClickedButtonLoadTex()
{
}


void CRendererDlg::OnEnChangeEditMeshName()
{
	UpdateData(TRUE);
}


void CRendererDlg::OnEnChangeEditMeshMatName()
{
	UpdateData(TRUE);
}


void CRendererDlg::OnEnChangeEditMeshShaderName()
{
	UpdateData(TRUE);
}


void CRendererDlg::OnEnChangeEditMatName()
{
	UpdateData(TRUE);
}


void CRendererDlg::OnEnChangeEditShaderName()
{
	UpdateData(TRUE);
}


void CRendererDlg::OnEnChangeEditLayer()
{
	UpdateData(TRUE);
}


void CRendererDlg::OnEnChangeEditScene()
{
	UpdateData(TRUE);
}
