#include "ResourceManager.h"
#include "Mesh2D.h"
#include "ShaderManager.h"
#include "Material.h"
#include "Texture.h"
#include "Sample.h"
#include "../Device.h"
#include "Animation2DSequence.h"
#include "StaticMesh.h"
#include "AnimationMesh.h"
#include "AnimationSequence.h"
#include "Skeleton.h"
#include "../UI/imgui/imgui.h"
#include "../PathManager.h"
#include "../UI/imgui/ImGuiFileDialog.h"
#include "../Sync.h"

DEFINITION_SINGLE(CResourceManager)

CResourceManager::CResourceManager()	:
	m_pDefaultMesh2D(nullptr),
	m_pDefaultMeshUI(nullptr),
	m_pDefaultTexUI(nullptr)
{
	m_pCRT = new CRITICAL_SECTION;
	InitializeCriticalSection(m_pCRT);
}

CResourceManager::~CResourceManager()
{
	DeleteCriticalSection(m_pCRT);
	delete m_pCRT;
	Safe_Release_Map(m_mapMesh);
	Safe_Release_Map(m_mapSample);
	Safe_Release_Map(m_mapSequence);
	Safe_Release_Map(m_mapSkeleton);
	DESTROY_SINGLE(CShaderManager);
	SAFE_RELEASE(m_pDefaultMesh2D);
	SAFE_RELEASE(m_pDefaultMeshUI)
}

class CMesh* CResourceManager::GetDefaultMesh() const
{
	if (m_pDefaultMesh2D)
	{
		m_pDefaultMesh2D->AddRef();
	}

	return m_pDefaultMesh2D;
}

class CMesh* CResourceManager::GetDefaultUIMesh() const
{
	if (m_pDefaultMeshUI)
	{
		m_pDefaultMeshUI->AddRef();
	}

	return m_pDefaultMeshUI;
}

std::shared_ptr<class CTexture> CResourceManager::GetDefaultTexture() const
{
	return m_pDefaultTexUI;
}

bool CResourceManager::Init()
{
#ifdef _DEBUG
	if (_heapchk() != _HEAPOK)
		DebugBreak();
#endif
	//	셰이더 매니저 초기화
	if (!GET_SINGLE(CShaderManager)->Init())
		return false;

	m_pDefaultMesh2D = new CMesh2D;

	m_pDefaultMesh2D->SetName("DefaultMesh2D");

	if (!((CMesh2D*)m_pDefaultMesh2D)->CreateMesh())
		return false;

	m_mapMesh.insert(std::make_pair("DefaultMesh2D", m_pDefaultMesh2D));

	std::shared_ptr<class CMaterial> pMaterial = CreateMaterial("Color");

	pMaterial->SetShader("Standard2D");

	m_pDefaultMesh2D->SetMaterial(pMaterial);

	m_pDefaultMeshUI = new CMesh2D;

	m_pDefaultMeshUI->SetName("DefaultMeshUI");

	if (!((CMesh2D*)m_pDefaultMeshUI)->CreateMesh())
		return false;

	m_mapMesh.insert(std::make_pair("DefaultMeshUI", m_pDefaultMeshUI));

	std::shared_ptr<class CMaterial> pMtrl = CreateMaterial("UI");

	pMtrl->SetShader("UI");

	m_pDefaultMeshUI->SetMaterial(pMtrl);

	std::shared_ptr<class CMaterial> pInstMtrl = CreateMaterial("Inst");

	pInstMtrl->SetShader("Inst");

	std::shared_ptr<class CMaterial> pInstAni2DMtrl = CreateMaterial("Anim2DInst");

	pInstAni2DMtrl->SetShader("Anim2DInst");

	std::shared_ptr<class CMaterial> pInstColMtrl = CreateMaterial("ColliderInst");

	pInstColMtrl->SetShader("ColliderInst");

	float pColor[4] = { 0.f, 0.f, 0.f, 0.f };

	class CSample* pPoint = CreateSample("point", D3D11_FILTER_MIN_MAG_MIP_POINT);
	class CSample* pLinear = CreateSample("linear", D3D11_FILTER_MIN_MAG_MIP_LINEAR);
	class CSample* pAni = CreateSample("anisotropic", D3D11_FILTER_ANISOTROPIC,
		D3D11_TEXTURE_ADDRESS_WRAP,
		D3D11_TEXTURE_ADDRESS_WRAP,
		D3D11_TEXTURE_ADDRESS_WRAP,
		D3D11_COMPARISON_NEVER,
		1, pColor);
	class CSample* pProjLinear = CreateSample("ProjLinear", D3D11_FILTER_MIN_MAG_MIP_LINEAR, D3D11_TEXTURE_ADDRESS_WRAP, D3D11_TEXTURE_ADDRESS_WRAP, D3D11_TEXTURE_ADDRESS_WRAP,
		D3D11_COMPARISON_ALWAYS, 1);
	class CSample* pShadow = CreateSample("Shadow", D3D11_FILTER_COMPARISON_MIN_MAG_LINEAR_MIP_POINT,
		D3D11_TEXTURE_ADDRESS_CLAMP,
		D3D11_TEXTURE_ADDRESS_CLAMP,
		D3D11_TEXTURE_ADDRESS_CLAMP,
		D3D11_COMPARISON_LESS_EQUAL,
		1, pColor);

	pPoint->SetSampler(0, static_cast<int>(SHADER_CBUFFER_TYPE::ALL));
	pLinear->SetSampler(1, static_cast<int>(SHADER_CBUFFER_TYPE::ALL));
	pAni->SetSampler(2, static_cast<int>(SHADER_CBUFFER_TYPE::ALL));
	pProjLinear->SetSampler(3, static_cast<int>(SHADER_CBUFFER_TYPE::ALL));
	pShadow->SetSampler(4, static_cast<int>(SHADER_CBUFFER_TYPE::ALL));

	SAFE_RELEASE(pPoint);
	SAFE_RELEASE(pLinear);
	SAFE_RELEASE(pAni);
	SAFE_RELEASE(pProjLinear);
	SAFE_RELEASE(pShadow);

	std::shared_ptr<class CMaterial> pColMat = CreateMaterial("Collider");

	pColMat->SetShader("Collider2D");

	std::shared_ptr<class CMaterial> pBarMat = CreateMaterial("Bar");

	pBarMat->SetShader("Bar");

	Vector3 vPos[] =
	{
		Vector3(-0.5f, 0.5f, 0.f),
		Vector3(0.5f, 0.5f, 0.f),
		Vector3(0.5f, -0.5f, 0.f),
		Vector3(-0.5f, -0.5f, 0.f),
		Vector3(-0.5f, 0.5f, 0.f)
	};

	if (!CreateMesh("Collider2DRect", MESH_TYPE::MT_MESH2D, D3D_PRIMITIVE_TOPOLOGY_LINESTRIP, 5,
		sizeof(Vector3), D3D11_USAGE_IMMUTABLE, (void*)vPos))
		return false;

	Vector3 vLine[] =
	{
		Vector3(0.f, 0.f, 0.f),
		Vector3(0.f, 1.f, 0.f),
	};

	if (!CreateMesh("Collider2DLine", MESH_TYPE::MT_MESH2D, D3D_PRIMITIVE_TOPOLOGY_LINESTRIP, 2,
		sizeof(Vector3), D3D11_USAGE_IMMUTABLE, (void*)vLine))
		return false;

	Vector3 vCircle[361] = {};

	vCircle[0].x = 1.f;
	vCircle[0].y = 0.f;

	for (int i = 1; i <= 360; ++i)
	{
		vCircle[i].x = cosf(DirectX::XMConvertToRadians((float)i));
		vCircle[i].y = sinf(DirectX::XMConvertToRadians((float)i));
	}

	if (!CreateMesh("ColliderCircle2D", MESH_TYPE::MT_MESH2D, D3D_PRIMITIVE_TOPOLOGY_LINESTRIP, 361,
		sizeof(Vector3), D3D11_USAGE_IMMUTABLE, (void*)vCircle))
		return false;

	BASIC pBasic[4] = {};

	pBasic[0].vPos = Vector3(0.f, 1.f, 0.f);
	pBasic[1].vPos = Vector3(1.f, 1.f, 0.f);
	pBasic[2].vPos = Vector3(0.f, 0.f, 0.f);
	pBasic[3].vPos = Vector3(1.f, 0.f, 0.f);

	pBasic[0].vUV = Vector2(0.f, 0.f);
	pBasic[1].vUV = Vector2(1.f, 0.f);
	pBasic[2].vUV = Vector2(0.f, 1.f);
	pBasic[3].vUV = Vector2(1.f, 1.f);

	unsigned short iBIdx[6] =
	{
		0, 1, 3, 
		0, 3, 2
	};

	if (!CreateMesh("Basic", MESH_TYPE::MT_MESH2D, D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST, 4,
		sizeof(BASIC), D3D11_USAGE_IMMUTABLE, pBasic, 6,
		sizeof(short), D3D11_USAGE_IMMUTABLE, DXGI_FORMAT_R16_UINT, iBIdx))
		return false;

	BASIC tPoint = {};

	if (!CreateMesh("PointMesh", MESH_TYPE::MT_MESH2D, D3D11_PRIMITIVE_TOPOLOGY_POINTLIST, 1, sizeof(BASIC), D3D11_USAGE_IMMUTABLE, &tPoint))
		return false;

	Vector3 vPointPos = {};

	if (!CreateMesh("Point", MESH_TYPE::MT_STATIC_MESH, D3D11_PRIMITIVE_TOPOLOGY_POINTLIST, 1, 12, D3D11_USAGE_IMMUTABLE, &vPointPos))
		return false;

	//float fGoldenRatio = 1.618f;

	Vector3 vertices[12] =
	{
		Vector3(0.f, 1.f, 1.618f),
		Vector3(0.f, -1.f, 1.618f),
		Vector3(0.f, 1.f, -1.618f),
		Vector3(0.f, -1.f, -1.618f),
		Vector3(1.f, 1.618f, 0.f),
		Vector3(-1.f, 1.618f, 0.f),
		Vector3(1.f, -1.618f, 0.f),
		Vector3(-1.f, -1.618f, 0.f),
		Vector3(1.618f, 0.f, 1.f),
		Vector3(-1.618f, 0.f, 1.f),
		Vector3(1.618f, 0.f, -1.f),
		Vector3(-1.618f, 0.f, -1.f),
	};

	unsigned short Idx[60] =
	{
		5, 0, 4,
		5, 9, 0,
		5, 4, 2,
		5,2,11,
		5,11,9,
		2,4,10,
		2,10,3,
		2,3,11,
		11,3,7,
		11,7,9,

		8, 10, 4,
		8, 4, 0,
		8,0,1,
		8,1,6,
		8,6,10,
		1,0,9,
		1,9,7,
		1,7,6,
		6,7,3,
		6,3,10
	};

	if (!CreateMesh("Test", MESH_TYPE::MT_STATIC_MESH, D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST,
		12, (int)sizeof(Vector3), D3D11_USAGE_DEFAULT, vertices, 60, 2,
		D3D11_USAGE_IMMUTABLE, DXGI_FORMAT_R16_UINT, Idx))
		return false;

	if (!CreateMesh("Test2", MESH_TYPE::MT_STATIC_MESH, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
		12, (int)sizeof(Vector3), D3D11_USAGE_DEFAULT, vertices, 60, 2,
		D3D11_USAGE_IMMUTABLE, DXGI_FORMAT_R16_UINT, Idx))
		return false;
	VERTEX3D vecOBB[8] = {};
	vecOBB[0].vPos = Vector3(-0.5f, -0.5f, -0.5f);
	vecOBB[1].vPos = Vector3(-0.5f, -0.5f, 0.5f);
	vecOBB[2].vPos = Vector3(-0.5f, 0.5f, -0.5f);
	vecOBB[3].vPos = Vector3(-0.5f, 0.5f, 0.5f);
	vecOBB[4].vPos = Vector3(0.5f, -0.5f, -0.5f);
	vecOBB[5].vPos = Vector3(0.5f, -0.5f, 0.5f);
	vecOBB[6].vPos = Vector3(0.5f, 0.5f, -0.5f);
	vecOBB[7].vPos = Vector3(0.5f, 0.5f, 0.5f);

	unsigned short pOBBIdx[36] = {
		0,1,2,
		1,3,2,
		0,2,4,
		1,0,4,
		5,1,4,
		5,3,1,
		4,6,5,
		2,6,4,
		2,3,6,
		3,7,6,
		7,3,5,
		5,6,7
	};

	if (!CreateMesh("OBBMesh", MESH_TYPE::MT_STATIC_MESH, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, 
		8, static_cast<int>(sizeof(VERTEX3D)), D3D11_USAGE_IMMUTABLE, vecOBB,
		36, 2, D3D11_USAGE_IMMUTABLE, DXGI_FORMAT_R16_UINT, pOBBIdx))
		return false;

	if (!LoadTexture("noise", TEXT("noise_01.png")))
	{
		return false;
	}
	FindTexture("noise")->SetShader(98, (int)SHADER_CBUFFER_TYPE::ALL);

	if (!LoadTexture("DistortNormal", TEXT("DefaultDistortionNormal1.png")))
	{
		return false;
	}
	FindTexture("DistortNormal")->SetShader(101, (int)SHADER_CBUFFER_TYPE::CBUFFER_PIXEL);
	
	if (!LoadTexture("DistortFilter", TEXT("DefaultDistortionFilter1.png")))
	{
		return false;
	}
	FindTexture("DistortFilter")->SetShader(102, (int)SHADER_CBUFFER_TYPE::CBUFFER_PIXEL);

	std::vector<const TCHAR*> vecstrMouse;

	for (int i = 0; i < 13; i++)
	{
		TCHAR*strNum = new TCHAR[MAX_PATH];

		memset(strNum, 0, MAX_PATH * sizeof(TCHAR));

		swprintf_s(strNum,MAX_PATH, L"UI\\Mouse\\Default\\%d.png", i);

		vecstrMouse.push_back(strNum);
	}

	LoadTextureArray("Mouse", vecstrMouse);

	CreateAni2DSequence("Mouse", "Mouse");

	for (int i = 0; i < 13; i++)
	{
		AddFrame("Mouse", Vector2(), Vector2());
	}

	SAFE_DELETE_ARRAY_VECLIST(vecstrMouse);

#ifdef _DEBUG
	if (_heapchk() != _HEAPOK)
		DebugBreak();
#endif
	return true;
}

class CMesh* CResourceManager::FindMesh(const std::string& strName)
{
	std::unordered_map<std::string, class CMesh*>::iterator iter = m_mapMesh.find(strName);

	if (iter == m_mapMesh.end())
		return nullptr;

	return iter->second;
}

bool CResourceManager::CreateMesh(const std::string& strTag, MESH_TYPE eType, D3D11_PRIMITIVE_TOPOLOGY eTopo,
	int iVtxCount, int iVtxSize, D3D11_USAGE eVtxUsage, void* pVtxData, 
	int iIdxCount, int iIdxSize, D3D11_USAGE eIdxUsage, DXGI_FORMAT eFmt, void* pIdxData,
	D3D11_CPU_ACCESS_FLAG eCpuFlag,
	D3D11_CPU_ACCESS_FLAG eIdxCpuFlag)
{
	class CMesh* pMesh = FindMesh(strTag);

	if (pMesh)
	{
		return false;
	}

	switch (eType)
	{
	case MESH_TYPE::MT_MESH2D:
		pMesh = new CMesh2D;
		break;
	case MESH_TYPE::MT_STATIC_MESH:
		pMesh = new CStaticMesh;
		break;
	case MESH_TYPE::MT_ANIMATION_MESH:
		pMesh = new CAnimationMesh;
		break;
	}

	pMesh->SetName(strTag);

	if (!pMesh->CreateMesh(eTopo,
		iVtxCount, iVtxSize, eVtxUsage, pVtxData,
		iIdxCount, iIdxSize, eIdxUsage, eFmt,  pIdxData, eCpuFlag, eIdxCpuFlag))
	{
		return false;
	}

	m_mapMesh.insert(std::make_pair(strTag, pMesh));

	return true;
}

bool CResourceManager::LoadMesh(MESH_TYPE eType, const std::string& strKey, const TCHAR* pFilePath, 
	const std::string& strPathKey, bool bLoop, int iIndex)
{
	class CMesh* pMesh = FindMesh(strKey);

	if (pMesh)
	{
		return false;
	}

	CMesh* _pMesh = nullptr;

	switch (eType)
	{
	case MESH_TYPE::MT_MESH2D:
		pMesh = new CMesh2D;
		break;
	case MESH_TYPE::MT_STATIC_MESH:
		pMesh = new CStaticMesh;
		break;
	case MESH_TYPE::MT_ANIMATION_MESH:
		pMesh = new CAnimationMesh;
		break;
	}

	if (!pMesh->LoadMesh(strKey, pFilePath, strPathKey,bLoop, iIndex))
	{
		return false;
	}

	m_mapMesh.insert(std::make_pair(strKey, pMesh));

	return true;
}

bool CResourceManager::LoadMeshFromFull(MESH_TYPE eType, const std::string& strKey, const TCHAR* pFullPath)
{
	class CMesh* pMesh = FindMesh(strKey);

	if (pMesh)
	{
		return false;
	}

	switch (eType)
	{
	case MESH_TYPE::MT_MESH2D:
		pMesh = new CMesh2D;
		break;
	case MESH_TYPE::MT_STATIC_MESH:
		pMesh = new CStaticMesh;
		break;
	case MESH_TYPE::MT_ANIMATION_MESH:
		pMesh = new CAnimationMesh;
		break;
	}

	if (!pMesh->LoadMeshFromFull(strKey, pFullPath))
	{
		return false;
	}

	m_mapMesh.insert(std::make_pair(strKey, pMesh));

	return true;
}

void CResourceManager::DeleteMesh(const std::string& strTag)
{
	std::unordered_map<std::string, class CMesh*>::iterator iter = m_mapMesh.find(strTag);

	if (iter == m_mapMesh.end())
		return;

	m_mapMesh.erase(iter);
}

std::shared_ptr<class CMaterial> CResourceManager::FindMaterial(const std::string& strName)
{
	std::unordered_map<std::string, std::shared_ptr<class CMaterial>>::iterator iter = m_mapMaterial.find(strName);

	if (iter == m_mapMaterial.end())
		return nullptr;

	return iter->second;
}


std::shared_ptr<class CMaterial> CResourceManager::CreateMaterial(const std::string& strName)
{
	std::shared_ptr<class CMaterial> pMaterial = FindMaterial(strName);

	if (pMaterial)
		return pMaterial;

	pMaterial = std::shared_ptr<class CMaterial>(new CMaterial);

	pMaterial->SetName(strName);

	m_mapMaterial.insert(std::make_pair(strName, pMaterial));

	return pMaterial;
}

std::shared_ptr<class CTexture> CResourceManager::FindTexture(const std::string& strName)
{
	CSync sync(m_pCRT);
	std::unordered_map<std::string, std::shared_ptr<class CTexture>>::iterator iter = m_mapTexture.find(strName);

	if (iter == m_mapTexture.end())
		return nullptr;

	return iter->second;
}


bool CResourceManager::LoadTexture(const std::string& strTag, const TCHAR* pFileName, const std::string& strRootPath)
{
	std::shared_ptr<class CTexture> pTexture = FindTexture(strTag);

	CSync sync(m_pCRT);

	if (pTexture)
	{
		return false;
	}

	pTexture = std::shared_ptr<class CTexture>(new CTexture);

	if (!pTexture->LoadTexture(strTag, pFileName, strRootPath))
	{
		return false;
	}

	m_mapTexture.insert(std::make_pair(strTag, pTexture));

	return true;
}

bool CResourceManager::LoadTextureFromFullPath(const std::string& strTag, const TCHAR* pFullPath)
{
	std::shared_ptr<class CTexture> pTexture = FindTexture(strTag);

	CSync sync(m_pCRT);
	if (pTexture)
	{
		return false;
	}

	pTexture = std::shared_ptr<class CTexture>(new CTexture);

	if (!pTexture->LoadTextureFromFullPath(strTag, pFullPath))
	{
		return false;
	}

	m_mapTexture.insert(std::make_pair(strTag, pTexture));

	return true;
}

bool CResourceManager::LoadTexture(const std::string& strTag, const std::vector<const TCHAR*>& vecFileName, const std::string& strRootPath)
{
	std::shared_ptr<class CTexture> pTexture = FindTexture(strTag);

	CSync sync(m_pCRT);
	if (pTexture)
	{
		return false;
	}

	pTexture = std::shared_ptr<class CTexture>(new CTexture);

	if (!pTexture->LoadTexture(strTag, vecFileName, strRootPath))
	{
		return false;
	}

	m_mapTexture.insert(std::make_pair(strTag, pTexture));

	return true;
}

bool CResourceManager::LoadTextureFromFullPath(const std::string& strTag, const std::vector<const TCHAR*>& vecFullPath)
{
	std::shared_ptr<class CTexture> pTexture = FindTexture(strTag);

	CSync sync(m_pCRT);
	if (pTexture)
	{
		return false;
	}

	pTexture = std::shared_ptr<class CTexture>(new CTexture);

	if (!pTexture->LoadTextureFromFullPath(strTag, vecFullPath))
	{
		return false;
	}

	m_mapTexture.insert(std::make_pair(strTag, pTexture));

	return true;
}

bool CResourceManager::LoadTextureArray(const std::string& strTag, const std::vector<const TCHAR*>& vecFileName, const std::string& strRootPath)
{
	std::shared_ptr<class CTexture> pTexture = FindTexture(strTag);

	CSync sync(m_pCRT);
	if (pTexture)
	{
		return false;
	}

	pTexture = std::shared_ptr<class CTexture>(new CTexture);

	if (!pTexture->LoadTextureArray(strTag, vecFileName, strRootPath))
	{
		return false;
	}

	m_mapTexture.insert(std::make_pair(strTag, pTexture));

	return true;
}

bool CResourceManager::LoadTextureArrayFromFullPath(const std::string& strTag, const std::vector<const TCHAR*>& vecFullPath)
{
	std::shared_ptr<CTexture> pTexture = FindTexture(strTag);

	CSync sync(m_pCRT);
	if (pTexture)
	{
		return false;
	}

	pTexture = std::shared_ptr<CTexture>(new CTexture);

	if (!pTexture->LoadTextureArrayFromFullPath(strTag, vecFullPath))
	{
		return false;
	}

	m_mapTexture.insert(std::make_pair(strTag, pTexture));

	return true;
}

std::shared_ptr<class CTexture> CResourceManager::LoadTexture(FILE* pFile)
{
	std::shared_ptr<CTexture> pTex = std::shared_ptr<CTexture>(new CTexture);

	CSync sync(m_pCRT);
	pTex->Load(pFile);

	m_mapTexture.insert(std::make_pair(pTex->GetName(), pTex));

	return pTex;
}

class CSample* CResourceManager::CreateSample(const std::string& strTag, D3D11_FILTER eFilter,
	D3D11_TEXTURE_ADDRESS_MODE eUMode ,
	D3D11_TEXTURE_ADDRESS_MODE eVMode ,
	D3D11_TEXTURE_ADDRESS_MODE eWMode ,
	D3D11_COMPARISON_FUNC eFunc,
	int iMaxanisotropy,
	float pColor[4])
{
	class CSample* pSam = FindSample(strTag);

	if (pSam)
		return pSam;

	pSam = new CSample;

	if (!pSam->CreateSample(eFilter,
		eUMode,
		eVMode,
		eWMode,
		eFunc,
		iMaxanisotropy,
		pColor))
	{
		return nullptr;
	}

	m_mapSample.insert(std::make_pair(strTag, pSam));

	return pSam;
}

class CSample* CResourceManager::FindSample(const std::string& strTag)
{
	std::unordered_map<std::string, CSample*>::iterator iter = m_mapSample.find(strTag);

	if (iter == m_mapSample.end())
		return nullptr;

	iter->second->AddRef();

	return iter->second;
}

bool CResourceManager::CreateAni2DSequence(const std::string& strTag, std::shared_ptr<CTexture> pTex, float fPlayRate, float fMaxTime)
{
	std::shared_ptr<CAnimation2DSequence> pSequence = FindAni2DSequence(strTag);

	if (pSequence)
	{
		return false;
	}

	pSequence = std::shared_ptr<CAnimation2DSequence>(new CAnimation2DSequence);

	pSequence->SetName(strTag);

	if (!pSequence->Init(pTex, fPlayRate, fMaxTime))
	{
		return false;
	}

	m_mapAni2DSequence.insert(make_pair(strTag, pSequence));

	return true;
}

bool CResourceManager::CreateAni2DSequence(const std::string& strTag, const std::string& strTex, float fPlayRate, float fMaxTime)
{
	std::shared_ptr<CAnimation2DSequence> pSequence = FindAni2DSequence(strTag);

	if (pSequence)
	{
		return false;
	}

	pSequence = std::shared_ptr<CAnimation2DSequence>( new CAnimation2DSequence);

	pSequence->SetName(strTag);

	if (!pSequence->Init(strTex, fPlayRate, fMaxTime))
	{
		return false;
	}

	m_mapAni2DSequence.insert(make_pair(strTag, pSequence));

	return true;
}

bool CResourceManager::LoadAni2DSequence(const TCHAR* pFileName, const std::string& strPathName)
{
	std::shared_ptr<CAnimation2DSequence> pSequence = std::shared_ptr<CAnimation2DSequence>(new CAnimation2DSequence);

	pSequence->Init(pFileName, strPathName);
		
	m_mapAni2DSequence.insert(std::make_pair(pSequence->GetName(), pSequence));

	return true;
}

std::shared_ptr<CAnimation2DSequence> CResourceManager::LoadAni2DSeqFromFullPath(const TCHAR* pFileName)
{
	std::shared_ptr<CAnimation2DSequence> pSequence = std::shared_ptr<CAnimation2DSequence>(new CAnimation2DSequence);

	pSequence->LoadFromFullPath(pFileName);

	m_mapAni2DSequence.insert(std::make_pair(pSequence->GetName(), pSequence));

	return pSequence;
}

void CResourceManager::SaveAni2DSeqFromFullPath(const std::string& strTag, const TCHAR* pFileName)
{
	std::shared_ptr<CAnimation2DSequence> pSequence = FindAni2DSequence(strTag);

	if (!pSequence)
		return;

	pSequence->SaveFromFullPath(pFileName);
}


void CResourceManager::AddFrame(const std::string& strTag, const Vector2& vStart, const Vector2& vEnd)
{
	std::shared_ptr<CAnimation2DSequence> pSequence = FindAni2DSequence(strTag);

	if (!pSequence)
		return;

	pSequence->AddFrame(vStart, vEnd);
}

std::shared_ptr<CAnimation2DSequence> CResourceManager::FindAni2DSequence(const std::string& strTag)
{
	std::unordered_map<std::string, std::shared_ptr<CAnimation2DSequence>>::iterator iter = m_mapAni2DSequence.find(strTag);

	if (iter == m_mapAni2DSequence.end())
		return nullptr;

	return iter->second;
}

void CResourceManager::AddAni2DNotify(const std::string& strSeq, const std::string& strNot, int iFrame)
{
	std::shared_ptr<CAnimation2DSequence> pSeq = FindAni2DSequence(strSeq);

	if (!pSeq)
		return;

	pSeq->AddAni2DNotify(strNot, iFrame);
}

bool CResourceManager::LoadAnimationSequence(const std::string& strKey,bool bLoop, const TCHAR* pFilePath, const std::string& strPathKey, bool bRootMotionRemove)
{
	CAnimationSequence* pSeq = FindSequence(strKey);

	if (pSeq)
	{
		SAFE_RELEASE(pSeq);
		return false;
	}

	pSeq = new CAnimationSequence;

	if (!pSeq->CreateSequence(strKey, bLoop, pFilePath, strPathKey,bRootMotionRemove))
	{
		SAFE_RELEASE(pSeq);
		return false;
	}

	m_mapSequence.insert(std::make_pair(strKey, pSeq));

	return true;
}

bool CResourceManager::LoadAnimationSequence(const std::string& strKey, bool bLoop, _tagFbxAnimationClip* pClip)
{
	CAnimationSequence* pSequence = FindSequence(strKey);

	if (pSequence)
	{
		SAFE_RELEASE(pSequence);
		return false;
	}

	pSequence = new CAnimationSequence;

	if (!pSequence->CreateSequence(bLoop, pClip))
	{
		SAFE_RELEASE(pSequence);
		return false;
	}

	m_mapSequence.insert(std::make_pair(strKey, pSequence));
	
	return true;
}

CAnimationSequence* CResourceManager::FindSequence(const std::string& strKey)
{
	std::unordered_map<std::string, CAnimationSequence*>::iterator iter = m_mapSequence.find(strKey);

	if (iter == m_mapSequence.end())
		return nullptr;

	iter->second->AddRef();

	return iter->second;
}

bool CResourceManager::LoadAnimationSequenceFullPath(const std::string& strKey, bool bLoop, const TCHAR* pFullPath)
{
	CAnimationSequence* pSequence = FindSequence(strKey);

	if (pSequence)
	{
		SAFE_RELEASE(pSequence);
		return false;
	}

	pSequence = new CAnimationSequence;

	pSequence->SetName(strKey);

	if (!pSequence->CreateSequenceFullPath(bLoop, pFullPath))
	{
		SAFE_RELEASE(pSequence);
		return false;
	}

	m_mapSequence.insert(std::make_pair(strKey, pSequence));

	return true;
}

void CResourceManager::AddAnimationSequence(const std::string& strSrc, const std::string& strDest, 
	const char* pFilePath, const std::string& strPathKey)
{
	CAnimationSequence* pSrc = FindSequence(strSrc); 

	if (!pSrc)
		return;

	CAnimationSequence* pDest = FindSequence(strDest);

	if (!pDest)
	{
		SAFE_RELEASE(pSrc);
		return;
	}

	pDest->AddSequence(pSrc);

	if (pFilePath)
	{
		char strFullPath[MAX_PATH] = {};

		const char* pPath = GET_SINGLE(CPathManager)->FindMultiBytePath(strPathKey);

		if (pPath)
			strcpy_s(strFullPath, pPath);

		strcat_s(strFullPath, pFilePath);

		pDest->SaveFullPath(strFullPath);
	}

	SAFE_RELEASE(pSrc);
	SAFE_RELEASE(pDest);
}

bool CResourceManager::LoadSkeleton(const std::string& strKey, const char* pFilePath, const std::string& strPathKey)
{
	CSkeleton* pSkel = FindSkeleton(strKey);

	if (pSkel)
	{
		SAFE_RELEASE(pSkel);
		return false;
	}

	pSkel = new CSkeleton;

	pSkel->SetName(strKey);

	if (!pSkel->LoadSkeleton(strKey, pFilePath, strPathKey))
	{
		SAFE_RELEASE(pSkel);
		return false;
	}

	m_mapSkeleton.insert(std::make_pair(strKey, pSkel));

	return true;
}

bool CResourceManager::LoadSkeletonFullPath(const std::string& strKey, const char* pFullPath)
{
	CSkeleton* pSkel = FindSkeleton(strKey);

	if (pSkel)
	{
		SAFE_RELEASE(pSkel);
		return false;
	}

	pSkel = new CSkeleton;
	pSkel->SetName(strKey);

	if (!pSkel->LoadSkeletonMultiByte(strKey, pFullPath))
	{
		SAFE_RELEASE(pSkel);
		return false;
	}

	m_mapSkeleton.insert(std::make_pair(strKey, pSkel));

	return true;
}

void CResourceManager::SetSkeleton(const std::string& strKey, CSkeleton* pSkeleton)
{
	CSkeleton* _pSkeleton = FindSkeleton(strKey);

	if (_pSkeleton)
	{
		SAFE_RELEASE(_pSkeleton);
		return;
	}

	pSkeleton->SetName(strKey);

	m_mapSkeleton.insert(std::make_pair(strKey, pSkeleton));
}

CSkeleton* CResourceManager::FindSkeleton(const std::string& strKey)
{
	std::unordered_map<std::string, CSkeleton*>::iterator iter = m_mapSkeleton.find(strKey);

	if (iter == m_mapSkeleton.end())
		return nullptr;

	iter->second->AddRef();

	return iter->second;
}

CSkeleton* CResourceManager::LoadSkeleton(FILE* pFile)
{
	CSkeleton* pSkeleton = new CSkeleton;

	pSkeleton->Load(pFile);

	m_mapSkeleton.insert(std::make_pair(pSkeleton->GetName(), pSkeleton));

	return pSkeleton;
}

void CResourceManager::AddSkeleton(const std::string& strDest, const std::string& strSrc, const char* pFilePath, const std::string& strPathKey)
{
	CSkeleton* pDest = FindSkeleton(strDest);

	if (!pDest)
		return;

	CSkeleton* pSrc = FindSkeleton(strSrc);

	if (!pSrc)
	{
		SAFE_RELEASE(pDest);
		return;
	}

	pDest->AddSkeleton(pSrc);

	if (pFilePath)
	{
		pDest->SaveSkeleton(pFilePath, strPathKey);
	}

	SAFE_RELEASE(pSrc);
	SAFE_RELEASE(pDest);
}

void CResourceManager::CreateSphere(const std::string& strName, float radius, UINT sliceCount, UINT stackCount)
{
	std::vector<VERTEX3D> vecVtx;
	std::vector<UINT>	vecIdx;

	//
	// Compute the vertices stating at the top pole and moving down the stacks.
	//

	// Poles: note that there will be texture coordinate distortion as there is
	// not a unique point on the texture map to assign to the pole when mapping
	// a rectangular texture onto a sphere.
	VERTEX3D topVertex =
	{
		Vector3(0.f, radius, 0.f),
		Vector3(0.0f, +1.0f, 0.0f),
		Vector2(0.0f, 0.0f) ,
		Vector3(1.0f, 0.0f, 0.0f)
	};
	VERTEX3D bottomVertex =
	{
		Vector3(0.f, -radius, 0.f),
		Vector3(0.0f, -1.0f, 0.0f),
		Vector2(0.0f, 1.f) ,
		Vector3(1.0f, 0.0f, 0.0f)
	};
	vecVtx.push_back(topVertex);

	float phiStep = XM_PI / stackCount;
	float thetaStep = 2.0f * XM_PI / sliceCount;

	// Compute vertices for each stack ring (do not count the poles as rings).
	for (UINT i = 1; i <= stackCount - 1; ++i)
	{
		float phi = i * phiStep;

		// Vertices of ring.
		for (UINT j = 0; j <= sliceCount; ++j)
		{
			float theta = j * thetaStep;

			VERTEX3D v;

			// spherical to cartesian
			v.vPos.x = radius * sinf(phi) * cosf(theta);
			v.vPos.y = radius * cosf(phi);
			v.vPos.z = radius * sinf(phi) * sinf(theta);

			// Partial derivative of P with respect to theta
			v.vTangent.x = -radius * sinf(phi) * sinf(theta);
			v.vTangent.y = 0.0f;
			v.vTangent.z = +radius * sinf(phi) * cosf(theta);

			v.vTangent.Normalize();

			v.vNormal = v.vPos;
			v.vNormal.Normalize();

			v.vUV.x = theta / XM_2PI;
			v.vUV.y = phi / XM_PI;

			vecVtx.push_back(v);
		}
	}

	vecVtx.push_back(bottomVertex);

	//
	// Compute indices for top stack.  The top stack was written first to the vertex buffer
	// and connects the top pole to the first ring.
	//

	for (UINT i = 1; i <= sliceCount; ++i)
	{
		vecIdx.push_back(0);
		vecIdx.push_back(i + 1);
		vecIdx.push_back(i);
	}

	//
	// Compute indices for inner stacks (not connected to poles).
	//

	// Offset the indices to the index of the first vertex in the first ring.
	// This is just skipping the top pole vertex.
	UINT baseIndex = 1;
	UINT ringVertexCount = sliceCount + 1;
	for (UINT i = 0; i < stackCount - 2; ++i)
	{
		for (UINT j = 0; j < sliceCount; ++j)
		{
			vecIdx.push_back(baseIndex + i * ringVertexCount + j);
			vecIdx.push_back(baseIndex + i * ringVertexCount + j + 1);
			vecIdx.push_back(baseIndex + (i + 1) * ringVertexCount + j);

			vecIdx.push_back(baseIndex + (i + 1) * ringVertexCount + j);
			vecIdx.push_back(baseIndex + i * ringVertexCount + j + 1);
			vecIdx.push_back(baseIndex + (i + 1) * ringVertexCount + j + 1);
		}
	}

	//
	// Compute indices for bottom stack.  The bottom stack was written last to the vertex buffer
	// and connects the bottom pole to the bottom ring.
	//

	// South pole vertex was added last.
	UINT southPoleIndex = (UINT)vecVtx.size() - 1;

	// Offset the indices to the index of the first vertex in the last ring.
	baseIndex = southPoleIndex - ringVertexCount;

	for (UINT i = 0; i < sliceCount; ++i)
	{
		vecIdx.push_back(southPoleIndex);
		vecIdx.push_back(baseIndex + i);
		vecIdx.push_back(baseIndex + i + 1);
	}

	CreateMesh(strName, MESH_TYPE::MT_STATIC_MESH, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
		static_cast<int>(vecVtx.size()), static_cast<int>(sizeof(VERTEX3D)), D3D11_USAGE_IMMUTABLE,
		&vecVtx[0], static_cast<int>(vecIdx.size()), 4, D3D11_USAGE_IMMUTABLE, DXGI_FORMAT_R32_UINT, &vecIdx[0]);
}

void CResourceManager::ShowWindow()
{
	if (ImGui::Begin("Resource Manager"))
	{
		std::string strMesh = ShowMesh();
		class CMesh* pMesh = FindMesh(strMesh);

		if (pMesh)
		{
			pMesh->ShowWindow();
		}

		static char pName[MAX_PATH] = {};
		ImGui::InputText("Name", pName, MAX_PATH);
		static bool bStatic = false;
		ImGui::Checkbox("Static", &bStatic);
		if (ImGui::Button("Load Mesh"))
		{
			const char* pPath = GET_SINGLE(CPathManager)->FindMultiBytePath(MESH_PATH);
			ImGuiFileDialog::Instance()->OpenDialog("Load Mesh", "Load Mesh", ".msh,.FBX", pPath);
		}

		if (ImGuiFileDialog::Instance()->Display("Load Mesh"))
		{
			if (ImGuiFileDialog::Instance()->IsOk())
			{
				std::string strFilePath = ImGuiFileDialog::Instance()->GetFilePathName();
				std::string strPath = ImGuiFileDialog::Instance()->GetCurrentPath();

				TCHAR pFilePath[MAX_PATH] = {};
#ifdef UNICODE
				MultiByteToWideChar(CP_ACP, 0, strFilePath.c_str(), -1, pFilePath, static_cast<int>(strFilePath.length()));
#else
				strcpy_s(pFilePath, MAX_PATH, strFilePath.c_str());
#endif
				if (!bStatic)
				{
					GET_SINGLE(CResourceManager)->LoadMeshFromFull(MESH_TYPE::MT_ANIMATION_MESH, pName, pFilePath);
				}
				else
				{
					GET_SINGLE(CResourceManager)->LoadMeshFromFull(MESH_TYPE::MT_STATIC_MESH, pName, pFilePath);
				}

				memset(pName, 0, MAX_PATH);
			}
			ImGuiFileDialog::Instance()->Close();
		}
	}
	ImGui::End();
}

const std::string CResourceManager::ShowMesh()
{
	size_t iMesh = m_mapMesh.size();

	std::unordered_map<std::string, class CMesh*>::iterator iter = m_mapMesh.begin();
	std::unordered_map<std::string, class CMesh*>::iterator iterEnd = m_mapMesh.end();
	std::vector<const char*> vecstrMesh(iMesh);
	for (int i = 0; iter != iterEnd; ++iter, ++i)
	{
		if (vecstrMesh.size() <= i)
			break;
		vecstrMesh[i] = iter->first.c_str();
	}

	static int iSel = -1;

	if (iMesh > 0)
	{
		ImGui::ListBox("Meshs", &iSel, &vecstrMesh[0], static_cast<int>(iMesh));
	}

	if (iSel != -1 && iMesh > iSel)
	{
		return vecstrMesh[iSel];
	}

	return std::string();
}
