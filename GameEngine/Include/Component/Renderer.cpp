#include "Renderer.h"
#include "../Resource/Mesh.h"
#include "../Resource/Material.h"
#include "../Resource/Shader.h"
#include "../Resource/ResourceManager.h"
#include "../Resource/ShaderManager.h"
#include "../RenderManager.h"
#include "../Render/RenderState.h"
#include "../Resource/Mesh2D.h"
#include "../Resource/Animation.h"
#include "../Component/Transform.h"
#include "../UI/imgui/imgui.h"
#include "../Component/Camera.h"
#include "../Component/Transform.h"
#include "../Component/Renderer.h"
#include "../Resource/Material.h"
#include "../Input.h"
#include "../UI/imgui/ImGuiFileDialog.h"
#include "../PathManager.h"
#include "../Device.h"
#include "../Resource/StaticMesh.h"
#include "../Resource/AnimationMesh.h"

CRenderer::CRenderer()	:
	m_pMesh(nullptr)
	, m_pAnimation(nullptr)
	, m_bIndexMode(false)
	, m_iIndex(0)
	, m_iStartIndex(0)
	, m_pInstancing(nullptr)
	, m_iInstIndex(0)
	, m_pOwner(nullptr)
{
}

CRenderer::CRenderer(const CRenderer& com)	:
	CRef(com)
	, m_pMesh(com.m_pMesh)
	, m_pAnimation(com.m_pAnimation)
	, m_bIndexMode(com.m_bIndexMode)
	, m_iIndex(com.m_iIndex)
	, m_iStartIndex(com.m_iStartIndex)
	, m_pInstancing(com.m_pInstancing)
	, m_iInstIndex(com.m_iInstIndex)
	, m_pOwner(com.m_pOwner)
{
	if (m_pMesh)
	{
		m_pMesh->AddRef();
	}

	m_vecMaterial.clear();
	m_vecShader.clear();

	size_t iContainerSize = com.m_vecMaterial.size();
	m_vecMaterial.resize(iContainerSize);
	for (size_t i = 0; i < iContainerSize; i++)
	{
		size_t iSubsetSize = com.m_vecMaterial[i].size();

		for (size_t j = 0; j < iSubsetSize; j++)
		{
			m_vecMaterial[i].push_back(com.m_vecMaterial[i][j]->Clone());
		}
	}

	iContainerSize = com.m_vecShader.size();
	m_vecShader.resize(iContainerSize);
	for (size_t i = 0; i < iContainerSize; i++)
	{
		size_t iSubsetSize = com.m_vecShader[i].size();

		for (size_t j = 0; j < iSubsetSize; j++)
		{
			m_vecShader[i].push_back(com.m_vecShader[i][j]);
		}
	}

	if (m_pAnimation)
	{
		m_pAnimation = m_pAnimation->Clone();

		m_pAnimation->SetOwner(m_pOwner);
	}

	
	for (int i = 0; i < static_cast<int>(RENDER_STATE_TYPE::END); i++)
	{
		int iSize = static_cast<int>(com.m_vecRenderStates[i].size());
		for (int j = 0; j < iSize; j++)
		{
			m_vecRenderStates[i].resize(iSize);

			int iSub = static_cast<int>(com.m_vecRenderStates[i][j].size());

			for (int k = 0; k < iSub; k++)
			{
				m_vecRenderStates[i][j].resize(iSub);

				m_vecRenderStates[i][j][k] = com.m_vecRenderStates[i][j][k];

				if (m_vecRenderStates[i][j][k])
				{
					m_vecRenderStates[i][j][k]->AddRef();
				}
			}
		}
	}
}

CRenderer::~CRenderer()
{
	for (int i = 0; i < static_cast<int>(RENDER_STATE_TYPE::END); i++)
	{
		for (int j = 0; j < m_vecRenderStates[i].size(); j++)
		{
			for (int k = 0; k < m_vecRenderStates[i][j].size(); k++)
			{
				SAFE_RELEASE(m_vecRenderStates[i][j][k]);
			}
		}
	}
	SAFE_RELEASE(m_pAnimation);
	SAFE_RELEASE(m_pMesh);
}

void CRenderer::SetMesh(const std::string& strKey)
{
	SetMesh(GET_SINGLE(CResourceManager)->FindMesh(strKey));
}

void CRenderer::SetMaterial(const std::string& strKey, int iContainer, int iSubset)
{
	SetMaterial(GET_SINGLE(CResourceManager)->FindMaterial(strKey), iContainer, iSubset);
}

void CRenderer::SetShader(const std::string& strKey, int iContainer, int iSubset)
{
	SetShader(GET_SINGLE(CShaderManager)->FindShader(strKey), iContainer, iSubset);
}

void CRenderer::SetMesh(class CMesh* pMesh)
{
	SAFE_RELEASE(m_pMesh);

	m_vecMaterial.clear();
	m_vecShader.clear();
	for (int i = 0; i < static_cast<int>(RENDER_STATE_TYPE::END); i++)
	{
		m_vecRenderStates[i].clear();
	}
	m_pMesh = pMesh;

	if (m_pMesh)
	{
		m_pMesh->AddRef();

	const std::vector<PMESHCONTAINER>* pvecMeshContainer = m_pMesh->GetMeshContainer();

	size_t iSize = pvecMeshContainer->size();

	for (size_t i = 0; i < iSize; i++)
	{
		size_t iSubsetSize = (*pvecMeshContainer)[i]->vecIdx.size();

		std::vector<std::shared_ptr<CMaterial>> vecMaterial;
		m_vecMaterial.push_back(vecMaterial);
		std::vector<std::shared_ptr<CShader>> vecShader;
		m_vecShader.push_back(vecShader);
		for (int k = 0; k < static_cast<int>(RENDER_STATE_TYPE::END); k++)
		{
			std::vector<class CRenderState*> vecState;
			m_vecRenderStates[k].push_back(vecState);
			m_vecRenderStates[k][i].resize(iSubsetSize);
		}

		for (size_t j = 0; j < iSubsetSize; j++)
		{
			if ((*pvecMeshContainer)[i]->vecMaterial.size() > j && (*pvecMeshContainer)[i]->vecMaterial[j])
			{
				m_vecMaterial[i].push_back((*pvecMeshContainer)[i]->vecMaterial[j]->Clone());
			}

			else
			{
				m_vecMaterial[i].resize(iSubsetSize);
			}

			if ((*pvecMeshContainer)[i]->vecShader.size() > j && (*pvecMeshContainer)[i]->vecShader[j])
			{
				m_vecShader[i].push_back((*pvecMeshContainer)[i]->vecShader[j]);
			}

			else
			{
				m_vecShader[i].resize(iSubsetSize);
			}
		}
	}

		Vector3 vSize = m_pMesh->GetMax() - m_pMesh->GetMin();
		Vector3 vPos = (m_pMesh->GetMax() + m_pMesh->GetMin())/2.f;


		m_pOwner->SetMeshSize(vSize);
		m_pOwner->SetMeshPos(vPos);
	}
}

void CRenderer::SetMaterial(std::shared_ptr<class CMaterial> pMtrl, int iContainer, int iSubset)
{
	if (m_vecMaterial.size() <= iContainer)
		return;

	if (m_vecMaterial[iContainer].size() <= iSubset)
		return;

	m_vecMaterial[iContainer][iSubset] = pMtrl->Clone();
}

void CRenderer::SetShader(std::shared_ptr<class CShader> pShader, int iContainer , int iSubset)
{
	if (m_vecShader.size() <= iContainer)
		return;

	if (m_vecShader[iContainer].size() <= iSubset)
		return;

	m_vecShader[iContainer][iSubset] = pShader;
}

class CMesh* CRenderer::GetMesh() const
{
	return m_pMesh;
}

std::shared_ptr<class CShader> CRenderer::GetShader(int iContainer, int iSubset) const
{
	if (m_vecShader.size() <= iContainer)
		return nullptr;

	else if (m_vecShader[iContainer].size() <= iSubset)
		return nullptr;

	return m_vecShader[iContainer][iSubset];
}

std::shared_ptr<class CMaterial> CRenderer::GetMaterial(int iContainer, int iSubset) const
{
	if (m_vecMaterial.size() <= iContainer)
		return nullptr;

	else if (m_vecMaterial[iContainer].size() <= iSubset)
		return nullptr;

	return m_vecMaterial[iContainer][iSubset];
}

size_t CRenderer::GetMaterialSize() const
{
	return m_vecMaterial.size();
}

size_t CRenderer::GetMaterialSubsetSize(int idx) const
{
	return m_vecMaterial[idx].size();
}

void CRenderer::AddRenderState(const std::string& strKey, int iContainer, int iSubset)
{
	CRenderState* pState = GET_SINGLE(CRenderManager)->FindState(strKey);

	AddRenderState(pState, iContainer,iSubset);
}

void CRenderer::AddRenderState(CRenderState* pState, int iContainer, int iSubset)
{
	if (!pState)
		return;

	RENDER_STATE_TYPE eType = pState->GetType();

	if (m_vecRenderStates[static_cast<int>(eType)].size() <= iContainer)
		return;

	else if (m_vecRenderStates[static_cast<int>(eType)][iContainer].size() <= iSubset)
		return;

	SAFE_RELEASE(m_vecRenderStates[static_cast<int>(eType)][iContainer][iSubset]);

	m_vecRenderStates[static_cast<int>(eType)][iContainer][iSubset] = pState;
}

CRenderState* CRenderer::FindRenderState(RENDER_STATE_TYPE eType, int iContainer, int iSubset)
{
	if (m_vecRenderStates[static_cast<int>(eType)].size() <= iContainer)
		return nullptr;

	else if (m_vecRenderStates[static_cast<int>(eType)][iContainer].size() <= iSubset)
		return nullptr;

	if (m_vecRenderStates[static_cast<int>(eType)][iContainer][iSubset])
		m_vecRenderStates[static_cast<int>(eType)][iContainer][iSubset]->AddRef();

	return m_vecRenderStates[static_cast<int>(eType)][iContainer][iSubset];
}

void CRenderer::SetTexture(LINK_TYPE eType, std::shared_ptr<CTexture> pTexture, int iShaderType, int iRegister, int iContainer, int iSubset)
{
	if (m_vecMaterial.size() <= iContainer)
		return;
	else if (m_vecMaterial[iContainer].size() <= iSubset)
		return;
	m_vecMaterial[iContainer][iSubset]->SetTexture(eType, pTexture, iShaderType, iRegister);
}

void CRenderer::SetTexture(LINK_TYPE eType, const std::string& strTag, int iShaderType, int iRegister, int iContainer, int iSubset)
{
	if (m_vecMaterial.size() <= iContainer)
		return;
	else if (m_vecMaterial[iContainer].size() <= iSubset)
		return;

	m_vecMaterial[iContainer][iSubset]->SetTexture(eType, strTag, iShaderType, iRegister);
}

void CRenderer::SetTexture(LINK_TYPE eType, const std::string& strTag, const TCHAR* pFileName, const std::string& strRootPath, int iShaderType, int iRegister, int iContainer, int iSubset)
{
	if (m_vecMaterial.size() <= iContainer)
		return;
	else if (m_vecMaterial[iContainer].size() <= iSubset)
		return;
	m_vecMaterial[iContainer][iSubset]->SetTexture(eType, strTag, pFileName, strRootPath, iShaderType, iRegister);
}

void CRenderer::SetTexture(LINK_TYPE eType, const std::string& strTag, const TCHAR* pFileName, int iShaderType, int iRegister, int iContainer, int iSubset)
{
	if (m_vecMaterial.size() <= iContainer)
		return;
	else if (m_vecMaterial[iContainer].size() <= iSubset)
		return;
	m_vecMaterial[iContainer][iSubset]->SetTexture(eType, strTag, pFileName, iShaderType, iRegister);
}

void CRenderer::SetAnimation(class CAnimation* pAni)
{
	m_pAnimation = pAni;

	if (m_pAnimation)
	{
		if (!m_pAnimation->HasOwner())
		{
			m_pAnimation->SetOwner(m_pOwner);
		}
		m_pAnimation->AddRef();
	}
}

void CRenderer::ChangeSequence(const std::string& strKey)
{
	if (m_pAnimation)
	{
		m_pAnimation->ChangeSequence(strKey);
	}
}

CAnimation* CRenderer::GetAnimation() const
{
	if (m_pAnimation)
		m_pAnimation->AddRef();

	return m_pAnimation;
}

void CRenderer::SetInstance(bool bInst)
{
	m_pOwner->SetInstance(bInst);

	if (!bInst)
	{
		m_pInstancing = nullptr;
	}
}

void CRenderer::SetInstancing(CRenderInstancing* pInst)
{
	m_pInstancing = pInst;
}

CRenderInstancing* CRenderer::GetInstancing() const
{
	return m_pInstancing;
}

void CRenderer::SetInstIndex(int iIdx)
{
	m_iInstIndex = iIdx;
}

int CRenderer::GetInstIndex() const
{
	return m_iInstIndex;
}
CSceneComponent* CRenderer::GetOwner() const
{
	return m_pOwner;
}
void CRenderer::SetOwner(CSceneComponent* pCom)
{
	m_pOwner = pCom;
}
void CRenderer::ResizeMaterial(int iSize, int iContainer)
{
	if (m_vecMaterial.size() <= iContainer)
		return;

	m_vecMaterial[iContainer].resize(iSize);
}
void CRenderer::ResizeShader(int iSize, int iContainer)
{
	if (m_vecShader.size() <= iContainer)
		return;

	m_vecShader[iContainer].resize(iSize);
}
void CRenderer::ResizeRenderState(RENDER_STATE_TYPE eType, int iSize, int iContainer)
{
	if (m_vecRenderStates[static_cast<int>(eType)].size() <= iContainer)
		return;

	m_vecRenderStates[static_cast<int>(eType)][iContainer].resize(iSize);
}
bool CRenderer::Init()
{
	return true;
}

void CRenderer::Start()
{
	if (m_pAnimation && m_pAnimation->IsStart())
	{
		m_pAnimation->Start();
	}
}

void CRenderer::Update(float fTime)
{
	if (m_pAnimation)
	{
		if (!m_pAnimation->IsStart())
		{
			m_pAnimation->Start();
		}

		m_pAnimation->Update(fTime);
	}
}

void CRenderer::PostUpdate(float fTime)
{
	if (m_pAnimation)
	{
		m_pAnimation->PostUpdate(fTime);
	}
}

void CRenderer::Collision(float fTime)
{
}

void CRenderer::PreRender(float fTime)
{
}

void CRenderer::RenderShadow(float fTime)
{
	if (m_pAnimation)
	{
		if (!m_pAnimation->IsStart())
		{
			m_pAnimation->Start();
		}

		m_pAnimation->SetShaderResource();
	}


	if (m_pMesh)
	{
		const std::vector<PMESHCONTAINER>* pvecMeshContainer = m_pMesh->GetMeshContainer();

		size_t iSize = pvecMeshContainer->size();

		for (size_t i = 0; i < iSize; i++)
		{
			size_t iSubsetSize = (*pvecMeshContainer)[i]->vecIdx.size();

			for (size_t j = 0; j < iSubsetSize; j++)
			{
				for (int k = 0; k < static_cast<int>(RENDER_STATE_TYPE::END); k++)
				{
					if (m_vecRenderStates[k].size() <= i)
						continue;

					else if (m_vecRenderStates[k][i].size() <= j)
						continue;

					else if (m_vecRenderStates[k][i][j])
						m_vecRenderStates[k][i][j]->SetState();
				}

				if (m_vecMaterial[i].size() > j)
				{
					if (m_vecMaterial[i][j])
						m_vecMaterial[i][j]->SetMaterialNoShader();
				}

				if (!m_bIndexMode)
				{
					m_pMesh->Render(fTime, (int)i, (int)j);
				}
				else
				{
					m_pMesh->RenderIndex(fTime, m_iIndex, m_iStartIndex, static_cast<int>(i), static_cast<int>(j));
				}

				for (int k = 0; k < static_cast<int>(RENDER_STATE_TYPE::END); k++)
				{
					if (m_vecRenderStates[k].size() <= i)
						continue;

					else if (m_vecRenderStates[k][i].size() <= j)
						continue;

					else if (m_vecRenderStates[k][i][j])
						m_vecRenderStates[k][i][j]->ResetState();
				}
			}

			if (iSubsetSize == 0)
			{
				for (int k = 0; k < static_cast<int>(RENDER_STATE_TYPE::END); k++)
				{
					if (m_vecRenderStates[k][0][0])
						m_vecRenderStates[k][0][0]->SetState();
				}

				if (m_vecMaterial[0].size() > 0)
				{
					if (m_vecMaterial[0][0])
						m_vecMaterial[0][0]->SetMaterialNoShader();
				}

				m_pMesh->Render(fTime, static_cast<int>(i));

				for (int k = 0; k < static_cast<int>(RENDER_STATE_TYPE::END); k++)
				{
					if (m_vecRenderStates[k][0][0])
						m_vecRenderStates[k][0][0]->ResetState();
				}
			}

		}
	}

	if (m_pAnimation)
		m_pAnimation->ResetShaderResource();
}

void CRenderer::Render(float fTime)
{
	if (m_pAnimation)
	{
		if (!m_pAnimation->IsStart())
		{
			m_pAnimation->Start();
		}

		m_pAnimation->SetShaderResource();
	}


	if (m_pMesh)
	{
		const std::vector<PMESHCONTAINER>* pvecMeshContainer = m_pMesh->GetMeshContainer();

		size_t iSize = pvecMeshContainer->size();

		for (size_t i = 0; i < iSize; i++)
		{
			size_t iSubsetSize = (*pvecMeshContainer)[i]->vecIdx.size();

			for (size_t j = 0; j < iSubsetSize; j++)
			{
				for (int k = 0; k < static_cast<int>(RENDER_STATE_TYPE::END); k++)
				{
					if (m_vecRenderStates[k].size() <= i)
						continue;

					else if (m_vecRenderStates[k][i].size() <= j)
						continue;

					else if (m_vecRenderStates[k][i][j])
						m_vecRenderStates[k][i][j]->SetState();
				}

				if (m_vecMaterial.size() > i)
				{
					if (m_vecMaterial[i].size() > j)
					{
						if (m_vecMaterial[i][j])
							m_vecMaterial[i][j]->Render(fTime);
					}
				}

				if (m_vecShader.size() > i)
				{
					if (m_vecShader[i].size() > j)
					{
						if (m_vecShader[i][j])
							m_vecShader[i][j]->SetShader();
					}
				}

				if (!m_bIndexMode)
				{
					m_pMesh->Render(fTime, (int)i, (int)j);
				}
				else
				{
					m_pMesh->RenderIndex(fTime, m_iIndex, m_iStartIndex, static_cast<int>(i), static_cast<int>(j));
				}

				for (int k = 0; k < static_cast<int>(RENDER_STATE_TYPE::END); k++)
				{
					if (m_vecRenderStates[k].size() <= i)
						continue;

					else if (m_vecRenderStates[k][i].size() <= j)
						continue;

					else if (m_vecRenderStates[k][i][j])
						m_vecRenderStates[k][i][j]->ResetState();
				}
			}

			if (iSubsetSize == 0)
			{
				for (int k = 0; k < static_cast<int>(RENDER_STATE_TYPE::END); k++)
				{
					if (m_vecRenderStates[k][0][0])
						m_vecRenderStates[k][0][0]->SetState();
				}

				if (m_vecMaterial[0].size() > 0)
				{
					if (m_vecMaterial[0][0])
						m_vecMaterial[0][0]->Render(fTime);
				}

				if (m_vecShader[0].size() > 0)
				{
					if (m_vecShader[0][0])
						m_vecShader[0][0]->SetShader();
				}

				m_pMesh->Render(fTime, static_cast<int>(i));

				for (int k = 0; k < static_cast<int>(RENDER_STATE_TYPE::END); k++)
				{
					if (m_vecRenderStates[k][0][0])
						m_vecRenderStates[k][0][0]->ResetState();
				}
			}
				
		}
	}

	if (m_pAnimation)
		m_pAnimation->ResetShaderResource();
}

void CRenderer::RenderParticle(int iCount,float fTime)
{
	if (m_pMesh)
	{
		const std::vector<PMESHCONTAINER>* pvecMeshContainer = m_pMesh->GetMeshContainer();

		size_t iSize = pvecMeshContainer->size();

		for (size_t i = 0; i < iSize; i++)
		{
			size_t iSubsetSize = (*pvecMeshContainer)[i]->vecIdx.size();

			for (int k = 0; k < static_cast<int>(RENDER_STATE_TYPE::END); k++)
			{
				if (m_vecRenderStates[k].size() <= i)
					continue;

				else if (m_vecRenderStates[k][i].size() <= 0)
					continue;

				else if (m_vecRenderStates[k][i][0])
					m_vecRenderStates[k][i][0]->SetState();
			}

			if (m_vecMaterial[i].size() > 0)
			{
				if (m_vecMaterial[i][0])
					m_vecMaterial[i][0]->Render(fTime);
			}

			if (m_vecShader[i].size() > 0)
			{
				if (m_vecShader[i][0])
					m_vecShader[i][0]->SetShader();
			}

			m_pMesh->RenderParticle(iCount, fTime);

			for (int k = 0; k < static_cast<int>(RENDER_STATE_TYPE::END); k++)
			{
				if (m_vecRenderStates[k].size() <= i)
					continue;

				else if (m_vecRenderStates[k][i].size() <= 0)
					continue;

				else if (m_vecRenderStates[k][i][0])
					m_vecRenderStates[k][i][0]->ResetState();
			}
		}
	}
}

void CRenderer::PostRender(float fTime)
{
}

CRenderer* CRenderer::Clone()
{
	return new CRenderer(*this);
}

void CRenderer::Save(FILE* pFile)
{
	CRef::Save(pFile);

	bool bMesh = false;

	if (m_pMesh)
	{
		bMesh = true;
	}

	fwrite(&bMesh, 1, 1, pFile);

	if (m_pMesh)
	{
		/*MESH_TYPE eType = m_pMesh->GetMeshType();

		fwrite(&eType, 4, 1, pFile);
		m_pMesh->Save(pFile);*/
		std::string strMesh = m_pMesh->GetName();

		int iLength = static_cast<int>(strMesh.length());
		fwrite(&iLength, 4, 1, pFile);
		if (iLength > 0)
		{
			fwrite(strMesh.c_str(), 1, iLength, pFile);
		}
	}

	int iShaderSize = static_cast<int>(m_vecShader.size());

	fwrite(&iShaderSize, 4, 1, pFile);

	for (int i = 0; i < iShaderSize; i++)
	{
		int iShaderSubSize = static_cast<int>(m_vecShader[i].size());

		fwrite(&iShaderSubSize, 4, 1, pFile);

		for (int j = 0; j < iShaderSubSize; j++)
		{
			bool bShader = false;

			if (m_vecShader[i][j])
			{
				bShader = true;
			}

			fwrite(&bShader, 1, 1, pFile);

			if (m_vecShader[i][j])
			{
				std::string strShader = m_vecShader[i][j]->GetName();
				int iShaderLength = static_cast<int>(strShader.length());

				fwrite(&iShaderLength, 4, 1, pFile);
				if (iShaderLength > 0)
				{
					fwrite(strShader.c_str(), 1, iShaderLength, pFile);
				}
			}
		}
	}

	int iMtrlSize = static_cast<int>(m_vecMaterial.size());

	fwrite(&iMtrlSize, 4, 1, pFile);

	for (int i = 0; i < iMtrlSize; i++)
	{
		int iMtrlSubSize = static_cast<int>(m_vecMaterial[i].size());

		fwrite(&iMtrlSubSize, 4, 1, pFile);
		for (int j = 0; j < iMtrlSubSize; j++)
		{
			bool bMtrl = false;

			if (m_vecMaterial[i][j])
			{
				bMtrl = true;
			}

			fwrite(&bMtrl, 1, 1, pFile);

			if (m_vecMaterial[i][j])
			{
				m_vecMaterial[i][j]->Save(pFile);
			}
		}
	}

	for (int i = 0; i < static_cast<int>(RENDER_STATE_TYPE::END); i++)
	{
		int iSize = static_cast<int>(m_vecRenderStates[i].size());

		fwrite(&iSize, 4, 1, pFile);

		for (int j = 0; j < iSize; j++)
		{
			int iSub = static_cast<int>(m_vecRenderStates[i][j].size());

			fwrite(&iSub, 4, 1, pFile);

			for (int k = 0; k < iSub; k++)
			{
				bool bState = false;

				if (m_vecRenderStates[i][j][k])
				{
					bState = true;
				}

				fwrite(&bState, 1, 1, pFile);

				if (m_vecRenderStates[i][j][k])
				{
					int iLength = static_cast<int>(m_vecRenderStates[i][j][k]->GetName().length());

					fwrite(&iLength, 4, 1, pFile);

					fwrite(m_vecRenderStates[i][j][k]->GetName().c_str(), 1, iLength, pFile);
				}
			}
		}
	}

	bool bAni = false;

	if (m_pAnimation)
	{
		bAni = true;
	}

	fwrite(&bAni, 1, 1, pFile);

	if (m_pAnimation)
	{
		m_pAnimation->Save(pFile);
	}
}

void CRenderer::Load(FILE* pFile)
{
	CRef::Load(pFile);

	bool bMesh = false;

	fread(&bMesh, 1, 1, pFile);

	if (bMesh)
	{
		/*MESH_TYPE eType = MESH_TYPE::MT_MESH2D;
		fread(&eType, 4, 1, pFile);
		switch (eType)
		{
		case MESH_TYPE::MT_MESH2D:
		{
			m_pMesh = new CMesh2D);
		}
			break;
		case MESH_TYPE::MT_STATIC_MESH:
		{
			m_pMesh = new CStaticMesh);
		}
			break;
		case MESH_TYPE::MT_ANIMATION_MESH:
		{
			m_pMesh = new CAnimationMesh);
		}
			break;
		}
		m_pMesh->Load(pFile);*/

		char strMesh[MAX_PATH] = {};

		int iLength = 0;
		fread(&iLength, 4, 1, pFile);
		if (iLength > 0)
		{
			fread(strMesh, 1, iLength, pFile);
		}

		m_pMesh = GET_SINGLE(CResourceManager)->FindMesh(strMesh);
	}
	int iShaderSize = 0;

	fread(&iShaderSize, 4, 1, pFile);

	m_vecShader.resize(iShaderSize);

	for (int i = 0; i < iShaderSize; i++)
	{
		int iShaderSubSize = 0;

		fread(&iShaderSubSize, 4, 1, pFile);
		m_vecShader[i].resize(iShaderSubSize);
		for (int j = 0; j < iShaderSubSize; j++)
		{
			bool bShader = false;

			fread(&bShader, 1, 1, pFile);

			if (bShader)
			{
				char strShader[MAX_PATH] = {};
				int iShaderLength = 0;

				fread(&iShaderLength, 4, 1, pFile);
				if (iShaderLength > 0)
				{
					fread(strShader, 1, iShaderLength, pFile);

					m_vecShader[i][j] = GET_SINGLE(CShaderManager)->FindShader(strShader);
				}
			}
		}
	}

	int iMtrlSize = 0;

	fread(&iMtrlSize, 4, 1, pFile);
	m_vecMaterial.resize(iMtrlSize);
	for (int i = 0; i < iMtrlSize; i++)
	{
		int iMtrlSubSize = 0;

		fread(&iMtrlSubSize, 4, 1, pFile);
		m_vecMaterial[i].resize(iMtrlSubSize);
		for (int j = 0; j < iMtrlSubSize; j++)
		{
			bool bMtrl = false;

			fread(&bMtrl, 1, 1, pFile);
			if (bMtrl)
			{
				m_vecMaterial[i][j] = std::shared_ptr<CMaterial>(new CMaterial);
				m_vecMaterial[i][j]->Load(pFile);
			}
		}
	}


	for (int i = 0; i < static_cast<int>(RENDER_STATE_TYPE::END); i++)
	{
		int iSize = 0;

		fread(&iSize, 4, 1, pFile);

		for (int j = 0; j < iSize; j++)
		{
			int iSub = 0;

			fread(&iSub, 4, 1, pFile);

			for (int k = 0; k < iSub; k++)
			{
				bool bState = false;

				fread(&bState, 1, 1, pFile);

				if (bState)
				{
					int iLength = 0;

					fread(&iLength, 4, 1, pFile);

					char strName[MAX_PATH] = {};

					if (iLength > 0)
					{
						fread(strName, 1, iLength, pFile);
					}

					AddRenderState(strName, j,k);
				}
			}
		}
	}

	bool bAni = false;

	fread(&bAni, 1, 1, pFile);

	if (bAni)
	{
		SAFE_RELEASE(m_pAnimation);
		m_pAnimation = CAnimation::LoadAnimation(pFile);
		m_pAnimation->SetOwner(m_pOwner);
	}

}

bool CRenderer::ShowNode()
{
	return false;
}

void CRenderer::ShowWindow()
{
	CRef::ShowWindow();

	std::string strName = GetName();

	if (strName == "")
	{
		strName = "None";
	}

	if (ImGui::Begin(strName.c_str()))
	{
		if (m_pAnimation)
		{
			m_pAnimation->ShowWindow();
		}
		else
		{
			static char pText[MAX_PATH] = {};
			ImGui::InputText("Bone Name", pText, MAX_PATH);

			if (ImGui::Button("Create Animation"))
			{
				const char* pPath = GET_SINGLE(CPathManager)->FindMultiBytePath(MESH_PATH);
				ImGuiFileDialog::Instance()->OpenDialog("Load Bone", "Load Bone", ".bne", pPath);
			}

			if (ImGuiFileDialog::Instance()->Display("Load Bone"))
			{
				if (ImGuiFileDialog::Instance()->IsOk())
				{
					std::string strPathName = ImGuiFileDialog::Instance()->GetFilePathName();
					CAnimation* pAni = CAnimation::CreateAnimationFullPath<CAnimation>("Ani", pText, strPathName.c_str());
					
					SetAnimation(pAni);

					SAFE_RELEASE(pAni);

					memset(pText, 0, MAX_PATH);
				}
				ImGuiFileDialog::Instance()->Close();
			}

		}
		//if (ImGui::Begin("MeshContainer"))
		//{
		if (m_pMesh)
		{
			const std::vector<PMESHCONTAINER>* pvecCon = m_pMesh->GetMeshContainer();

			int iCount = static_cast<int>(pvecCon->size());
			std::vector<const char*> vecText;

			for (int i = 0; i < iCount; i++)
			{
				char* strText = new char[MAX_PATH];
				memset(strText, 0, MAX_PATH);
				sprintf_s(strText, MAX_PATH, "Mesh%d", i);

				vecText.push_back(strText);

				(*pvecCon)[i]->eTopology;
			}
			static int iSel = -1;

			if (iCount > 0)
			{
				ImGui::ListBox("Meshs", &iSel, &vecText[0], iCount);
			}

			for (int i = 0; i < iCount; i++)
			{
				delete[] vecText[i];
			}

			if (iSel != -1)
			{
				if (iSel < iCount)
				{
					//if (ImGui::Begin("Mesh"))
					//{
					ImGui::Text("Count: %d", (*pvecCon)[iSel]->tVtx.iCount);
					ImGui::Text("Size: %d", (*pvecCon)[iSel]->tVtx.iSize);
					ImGui::Text("Usage: %d", (*pvecCon)[iSel]->tVtx.iUsage);
					ImGui::Text("Topology: %d", (*pvecCon)[iSel]->eTopology);

					int iSub = static_cast<int>((*pvecCon)[iSel]->vecMaterial.size());

					std::vector<const char*> vecSub;

					for (int i = 0; i < iSub; i++)
					{
						if ((*pvecCon)[iSel]->vecMaterial[i])
						{
							vecSub.push_back((*pvecCon)[iSel]->vecMaterial[i]->GetName().c_str());
						}
						else
						{
							if (m_vecMaterial[iSel][i])
							{
								vecSub.push_back(m_vecMaterial[iSel][i]->GetName().c_str());
							}
						}
					}

					static int _iSel = -1;

					if (!vecSub.empty())
					{
						ImGui::ListBox("SubSet", &_iSel, &vecSub[0], iSub);
					}

					if (_iSel != -1)
					{
						if (m_vecMaterial[iSel].size() > _iSel)
						{
							if (m_vecMaterial[iSel][_iSel])
							{
								m_vecMaterial[iSel][_iSel]->ShowWindow();
							}
						}

						static char strShader[MAX_PATH] = {};
						ImGui::InputText("Shader: ", strShader, MAX_PATH);

						if (ImGui::Button("Change Shader"))
						{
							if (m_vecShader[iSel].size() > _iSel)
							{
								m_vecShader[iSel][_iSel] = GET_SINGLE(CShaderManager)->FindShader(strShader);
							}
						}

						if (m_vecShader[iSel].size() > _iSel)
						{
							if (m_vecShader[iSel][_iSel])
							{
								ImGui::Text("Current Shader: %s", m_vecShader[iSel][_iSel]->GetName().c_str());
							}
						}

						static char strState[MAX_PATH] = {};

						ImGui::InputText("Add Render State", strState, MAX_PATH);
						if (ImGui::Button("Add"))
						{
							AddRenderState(strState,iSel, _iSel);
						}

						std::vector<const char*> vecState;

						for (int i = 0; i < static_cast<int>(RENDER_STATE_TYPE::END); i++)
						{
							if (m_vecRenderStates[i].size() > iSel)
							{
								if (m_vecRenderStates[i][iSel].size() > _iSel)
								{
									if (m_vecRenderStates[i][iSel][_iSel])
									{
										vecState.push_back(m_vecRenderStates[i][iSel][_iSel]->GetName().c_str());
									}
								}
							}
						}
						int iCurrent = -1;

						if (!vecState.empty())
						{
							ImGui::Combo("Render States", &iCurrent, &vecState[0], static_cast<int>(vecState.size()));
						}

						if (ImGui::Button("Set New SubSet"))
						{
							m_pMesh->DeleteIndexBuffer(iSel, _iSel, m_iIndex);

							SetMesh(m_pMesh);
						}
						ImGui::Text("Change Container");
						static char strContainer[MAX_PATH] = {};
						ImGui::InputText("Mesh Name: ", strContainer, MAX_PATH);
						static int iContainer = -1;
						ImGui::InputInt("Container: ", &iContainer);
						static int iDestContainer = -1;
						ImGui::InputInt("DestContainer: ", &iDestContainer);
						if (ImGui::Button("Change Container"))
						{
							class CMesh* pMesh =  GET_SINGLE(CResourceManager)->FindMesh(strContainer);

							if (pMesh)
							{
								if (m_pMesh)
								{
									m_pMesh->ChangeContainer(pMesh, iContainer, iDestContainer);
									SetMesh(m_pMesh);
								}
							}
						}
					}
					//}
					//ImGui::End();
				}
			}

			if (ImGui::Button("Save Mesh"))
			{
				const char* pPath = GET_SINGLE(CPathManager)->FindMultiBytePath(MESH_PATH);
				ImGuiFileDialog::Instance()->OpenDialog("Save Mesh", "Save Mesh", ".msh,.FBX", pPath);
			}

			if (ImGuiFileDialog::Instance()->Display("Save Mesh"))
			{
				if (ImGuiFileDialog::Instance()->IsOk())
				{
					std::string strFilePath = ImGuiFileDialog::Instance()->GetFilePathName();
					std::string strPath = ImGuiFileDialog::Instance()->GetCurrentPath();

					size_t iSize = m_vecMaterial.size();
					for (size_t i = 0; i < iSize; i++)
					{
						size_t iSubSize = m_vecMaterial[i].size();
						for (size_t j = 0; j < iSubSize; j++)
						{
							m_pMesh->SetMaterial(m_vecMaterial[i][j], static_cast<int>(i), static_cast<int>(j));
							m_pMesh->SetShader(m_vecShader[i][j]->GetName(), static_cast<int>(i), static_cast<int>(j));
						}
					}
					m_pMesh->SaveMesh(strFilePath.c_str());
				}
				ImGuiFileDialog::Instance()->Close();
			}
		}

			static char pName[MAX_PATH] = {};
			ImGui::InputText("Mesh Name", pName, MAX_PATH);
			if (ImGui::Button("Load Mesh"))
			{
				const char* pPath = GET_SINGLE(CPathManager)->FindMultiBytePath(MESH_PATH);
				ImGuiFileDialog::Instance()->OpenDialog("Load Mesh", "Load Mesh", ".msh,.FBX", pPath);
			}
			static bool bAni = false;
			ImGui::Checkbox("Animation Mesh", &bAni);

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
					MESH_TYPE eType = MESH_TYPE::MT_STATIC_MESH;

					if (bAni)
					{
						eType = MESH_TYPE::MT_ANIMATION_MESH;
					}

					if (GET_SINGLE(CResourceManager)->LoadMeshFromFull(eType, pName, pFilePath))
					{
						SetMesh(pName);
					}
					memset(pName, 0, MAX_PATH);
				}
				ImGuiFileDialog::Instance()->Close();
			}
			static bool bSet = false;
			if (ImGui::Button("Set Mesh"))
			{
				bSet = true;
			}
			if (bSet)
			{
				std::string strMesh;
				if (ImGui::Begin("Select Mesh"))
				{
					strMesh = GET_SINGLE(CResourceManager)->ShowMesh();

					if (ImGui::Button("Select"))
					{
						SetMesh(strMesh);
						bSet = false;
					}
					if (ImGui::Button("Cancel"))
					{
						bSet = false;
					}
				}
				ImGui::End();
			}

		//}
		//ImGui::End();
		/*
				int iSize = static_cast<int>(m_vecMaterial.size());

				for (int i = 0; i < iSize; i++)
				{
					int iSub = m_vecMaterial[i].size();

					for (int j = 0; j < iSub; j++)
					{
						if(m_vecMaterial[i][j])
						m_vecMaterial[i][j]->ShowWindow();
					}
				}*/

		if (ImGui::InputInt("StartIndex", &m_iStartIndex))
		{
			m_bIndexMode = true;
		}
		if (ImGui::InputInt("IndexCount", &m_iIndex))
		{
			m_bIndexMode = true;
		}

	}
	ImGui::End();
}
