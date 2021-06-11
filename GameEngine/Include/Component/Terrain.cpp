#include "Terrain.h"
#include "../Resource/ResourceManager.h"
#include "../Object/GameObject.h"
#include "Renderer.h"
#include "../Device.h"
#include "../Resource/Texture.h"
#include "../PathManager.h"
#include "../Resource/ShaderManager.h"
#include "Collider.h"
#include "../Input.h"
#include "../Resource/Mesh.h"
#include "ColliderTerrain.h"
#include "../Scene/Scene.h"
#include "Decal.h"
#include <wincodec.h>
#include "../CameraManager.h"
#include "Camera.h"

std::unordered_map<CTerrain*, std::string	>CTerrain::m_mapTerrain;
std::unordered_map<std::string, std::shared_ptr<CTexture>> CTerrain::m_mapBrush;
CTerrain* CTerrain::m_pMainTerrain = nullptr;
static const int CellsPerPatch = 64;
CTerrain::CTerrain()	:
	m_iWidth(0)
	, m_iHeight(0)
	, m_pHeightMap(nullptr)
	, m_tCBuffer()
	, m_pHeightMapSRV(nullptr)
	, m_iNumPatchVertRows(0)
	, m_iNumPatchVertCols(0)
	, m_iNumPatchVertices(0)
	, m_iNumPatchQuadFaces(0)
	, m_fCellSpacing(1.f)
	, m_pQuadPatchIB(nullptr)
	, m_pImage(nullptr)
	, m_pTexture(nullptr)
	, m_bEditMode(false)
	, m_bBlurMode(false)
	, m_vBrushScale(1.f, 1.f, 1.f)
{
	m_eSceneComponentType = SCENE_COMPONENT_TYPE::SCT_3D;
	m_iSceneComponentClassType = static_cast<int>(SCENECOMPONENT_CLASS_TYPE::TERRAIN);
	m_tCBuffer.gWorldCellSpace = 50.f;
	m_tCBuffer.gMaxTess = 6.f;
	m_tCBuffer.gMinTess = 0.f;
	m_tCBuffer.fMaxDist = 500.f;
	m_tCBuffer.fMinDist = 20.;
}

CTerrain::CTerrain(const CTerrain& terrain)	:
	m_iWidth(terrain.m_iWidth)
	, m_iHeight(terrain.m_iHeight)
	, m_tCBuffer(terrain.m_tCBuffer)
	, m_pHeightMapSRV(terrain.m_pHeightMapSRV)
	, m_iNumPatchVertRows(terrain.m_iNumPatchVertRows)
	, m_iNumPatchVertCols(terrain.m_iNumPatchVertCols)
	, m_iNumPatchVertices(terrain.m_iNumPatchVertRows)
	, m_iNumPatchQuadFaces(terrain.m_iNumPatchVertRows)
	, m_fCellSpacing(terrain.m_fCellSpacing)
	, m_pQuadPatchIB(terrain.m_pQuadPatchIB)
	, m_pImage(terrain.m_pImage)
	, m_pTexture(terrain.m_pTexture)
	, m_bEditMode(terrain.m_bEditMode)
	, m_bBlurMode(terrain.m_bBlurMode)
	, m_vBrushScale(terrain.m_vBrushScale)
{
	CTerrain::AddTerrain(this);
	SetMainTerrain(this);

	if (m_pTexture)
	{
		m_pTexture->AddRef();
	}
}

CTerrain::~CTerrain()
{
	DeleteTerrain(this);
	SAFE_DELETE_ARRAY(m_pHeightMap);
	SAFE_RELEASE(m_pHeightMapSRV);
	SAFE_RELEASE(m_pQuadPatchIB);
	SAFE_RELEASE(m_pTexture);
	SAFE_DELETE(m_pImage);
	SAFE_DELETE_ARRAY(m_pHeightMap);
}

bool CTerrain::Init()
{
	if (!CSceneComponent::Init())
		return false;

	m_iNumPatchVertRows = (m_iHeight - 1) / CellsPerPatch + 1;
	m_iNumPatchVertCols = (m_iWidth - 1) / CellsPerPatch + 1;
	
	m_iNumPatchVertices = m_iNumPatchVertRows * m_iNumPatchVertCols;
	m_iNumPatchQuadFaces = (m_iNumPatchVertRows - 1) * (m_iNumPatchVertCols - 1);
	CTerrain::AddTerrain(this);


	return true;
}

void CTerrain::Start()
{
	CSceneComponent::Start();

}

void CTerrain::Update(float fTime)
{
	CSceneComponent::Update(fTime);

	CCamera* pCam = GET_SINGLE(CCameraManager)->GetMainCam();

	if (pCam)
	{
		const Matrix& matProj = pCam->GetProj();

		CTerrain::ExtractFrustumPlanes(m_tCBuffer.gWorldFrustumPlanes, const_cast<Matrix&>(matProj));
	}
}

void CTerrain::PostUpdate(float fTime)
{
	CSceneComponent::PostUpdate(fTime);
}

void CTerrain::Collision(float fTime)
{
	CSceneComponent::Collision(fTime);
}

void CTerrain::PreRender(float fTime)
{
	CSceneComponent::PreRender(fTime);
}

void CTerrain::Render(float fTime)
{
	CSceneComponent::Render(fTime);

	GET_SINGLE(CShaderManager)->UpdateCBuffer("Terrain", &m_tCBuffer);
}

void CTerrain::PostRender(float fTime)
{
	CSceneComponent::PostRender(fTime);
}

void CTerrain::Save(FILE * pFile)
{
	CSceneComponent::Save(pFile);

	fwrite(&m_iWidth, 4, 1, pFile);
	fwrite(&m_iHeight, 4, 1, pFile);
	int iSize = static_cast<int>(m_vecPos.size());
	fwrite(&iSize, 4, 1, pFile);

	fwrite(&m_vecPos[0], 12, iSize, pFile);
	fwrite(&m_tCBuffer, sizeof(TERRAINCBUFFER), 1, pFile);
	fwrite(&m_vecVtx[0], sizeof(VERTEX3D), iSize, pFile);

	int iFace = static_cast<int>(m_vecFaceNormal.size());

	fwrite(&iFace, 4, 1, pFile);
	fwrite(&m_vecFaceNormal[0], 12, iFace, pFile);
}

void CTerrain::Load(FILE* pFile)
{
	CSceneComponent::Load(pFile);

	fread(&m_iWidth, 4, 1, pFile);
	fread(&m_iHeight, 4, 1, pFile);
	int iSize = 0;
	fread(&iSize, 4, 1, pFile);
	m_vecPos.resize(iSize);
	m_vecVtx.resize(iSize);
	fread(&m_vecPos[0], 12, iSize, pFile);
	fread(&m_tCBuffer, sizeof(TERRAINCBUFFER), 1, pFile);
	fread(&m_vecVtx[0], sizeof(VERTEX3D), iSize, pFile);

	int iFace = 0;

	fread(&iFace, 4, 1, pFile);
	m_vecFaceNormal.resize(iFace);
	fread(&m_vecFaceNormal[0], 12, iFace, pFile);

	CTerrain::AddTerrain(this);
	SetMainTerrain(this);
}

void CTerrain::ColInit(CCollider* pSrc, CCollider* pDest, float fTime)
{
}

void CTerrain::ColStay(CCollider* pSrc, CCollider* pDest, float fTime)
{
	if (!m_bEditMode)
		return;

	if (pDest->GetColType() == COLLIDER_TYPE::RAY)
	{
		Vector3 vCrs = pSrc->GetCrs();
		m_pDecal->SetWorldPos(vCrs);

		if (GET_SINGLE(CInput)->IsPress(MOUSE_TYPE::LEFT))
		{
			int iIdx = GetIndex(vCrs);

			if (iIdx != -1)
			{
				/*m_vecVtx[iIdx].vPos.y += fTime * 2.f;
				m_vecPos[iIdx].y += fTime * 2.f;*/

				CGameObject* pObj = GetObj();

				if (pObj)
				{
					std::shared_ptr<CComponent> pRenderer = pObj->FindComponentByType<CRenderer>();
					if (pRenderer)
					{
						Vector3 vScale = static_cast<CSceneComponent*>(pRenderer.get())->GetWorldScale();
						Vector3 vPos = static_cast<CSceneComponent*>(pRenderer.get())->GetWorldPos();

						Vector3 vLocal = (vCrs - vPos) / vScale;

						unsigned int iWidth = m_pBrushTexture->GetWidth();
						unsigned int iHeight = m_pBrushTexture->GetHeight();

						int iStartX = static_cast<int>(vLocal.x - iWidth / 2);
						int iStartY = static_cast<int>(vLocal.z - iHeight / 2);

						int iEndX = static_cast<int>(vLocal.x + iWidth / 2);
						int iEndY = static_cast<int>(vLocal.z + iHeight / 2);

						iStartX = iStartX < 0 ? 0 : iStartX;
						iStartY = iStartY < 0 ? 0 : iStartY;

						iEndX = iEndX >= m_pImage->GetMetadata().width ? static_cast<int>(m_pImage->GetMetadata().width) - 1 : iEndX;
						iEndY = iEndY >= m_pImage->GetMetadata().height ? static_cast<int>(m_pImage->GetMetadata().height) - 1 : iEndY;
						size_t iHeightMapWidth = m_pImage->GetMetadata().width;
						uint8_t* pPixels = m_pBrushTexture->GetInfo()->pImage->GetPixels();

						uint8_t* pHeightMapPixels = m_pImage->GetPixels();

						if (m_bBlurMode)
						{
							int iCount = 0;

							float* pNewMap = new float[m_iHeight * m_iWidth];
							memset(pNewMap, 0, m_iHeight * m_iWidth * 4);
							int* pCount = new int[m_iHeight * m_iWidth];
							memset(pCount, 0, m_iHeight * m_iWidth * 4);
							int iSize = m_iHeight * m_iWidth * 4;

							for (int j = iStartY, a = 0; j <= iEndY; j++, a++)
							{
								for (int i = iStartX, b = 0; i <= iEndX; i++, b++)
								{
									for (int k = -1; k < 2; k++)
									{
										for (int l = -1; l < 2; l++)
										{
											if (pPixels[(a * iWidth + b) * 4] > 0)
											{
												/*Vector3 vScale = m_pDecal->GetWorldScale();

												int iIndex = GetIndex(vCrs + Vector3(
													static_cast<float>(b - static_cast<int>(iWidth / 2))* vScale.x,
													0.f,
													static_cast<float>(a - static_cast<int>(iHeight / 2)) * vScale.z));
												if (iIndex == -1)
													continue;*/

												int iIdx = j * static_cast<int>(iHeightMapWidth) + i;
												int iNewIdx = iIdx + k * static_cast<int>(iHeightMapWidth) + l;

												if (iNewIdx >= m_iHeight * m_iWidth||
													iNewIdx < 0)
													continue;

												float h = m_pHeightMap[iNewIdx];
												//pNewMap[iIdx] += m_pHeightMap[iNewIdx];
												pNewMap[iIdx] += m_vecHeight[iNewIdx];
												float f = pNewMap[iIdx];
												pCount[iIdx]++;
											}
										}
									}
								}
							}

							for (int j = iStartY, a = 0; j <= iEndY; j++, a++)
							{
								for (int i = iStartX, b = 0; i <= iEndX; i++, b++)
								{
									if (pPixels[(a * iWidth + b) * 4] > 0)
									{
										/*int iIndex = GetIndex(vCrs + Vector3(
											static_cast<float>(b - static_cast<int>(iWidth / 2)),
											0.f,
											static_cast<float>(a - static_cast<int>(iHeight / 2))));
										if (iIndex == -1)
											continue;*/

										int iIdx = j * static_cast<int>(iHeightMapWidth) + i;
										if (pCount[iIdx] == 0)
											continue;
										m_vecHeight[iIdx] = pNewMap[iIdx] / pCount[iIdx];

										int i = 0;
									}
								}
							}

							//memcpy_s(m_pHeightMap, 4 * m_iHeight * m_iWidth, pNewMap, 4 * m_iHeight * m_iWidth);

							for (int j = iStartY, a = 0; j <= iEndY; j++, a++)
							{
								for (int i = iStartX, b = 0; i <= iEndX; i++, b++)
								{
									if (pPixels[(a * iWidth + b) * 4] > 0)
									{
										/*int iIndex = GetIndex(vCrs + Vector3(
											static_cast<float>(b - static_cast<int>(iWidth / 2)),
											0.f,
											static_cast<float>(a - static_cast<int>(iHeight / 2))));
										if (iIndex == -1)
											continue;*/

										int iIdx = j * static_cast<int>(iHeightMapWidth) + i;
										int _iIdx = iIdx * 4;
										char r = pHeightMapPixels[_iIdx];
										char g = pHeightMapPixels[_iIdx+1];
										char b = pHeightMapPixels[_iIdx+2];
										float fY = m_vecPos[iIdx].y;
										pHeightMapPixels[_iIdx] =		static_cast<uint8_t>(m_vecHeight[iIdx]);
										pHeightMapPixels[_iIdx + 1] =	static_cast<uint8_t>(m_vecHeight[iIdx]);
										pHeightMapPixels[_iIdx + 2] =	static_cast<uint8_t>(m_vecHeight[iIdx]);

										m_vecPos[iIdx].y = m_vecHeight[iIdx];
										/*pHeightMapPixels[_iIdx] = m_vecHeight[iIndex];
										pHeightMapPixels[_iIdx + 1] = m_vecHeight[iIndex];
										pHeightMapPixels[_iIdx + 2] = m_vecHeight[iIndex];

										m_vecPos[iIndex].y = m_vecHeight[iIndex];*/
									}
								}
							}

							delete[] pNewMap;
							delete[] pCount;
						}
						else
						{
							for (int j = iStartY, a = 0; j <= iEndY; j++, a++)
							{
								for (int i = iStartX, b = 0; i <= iEndX; i++, b++)
								{
									if (pPixels[(a * iWidth + b) * 4] > 0)
									{
										/*int iIdx = j * static_cast<int>(iHeightMapWidth) + i;
										int _iIdx = iIdx * 4;
										pHeightMapPixels[_iIdx] += 1;
										pHeightMapPixels[_iIdx + 1] += 1;
										pHeightMapPixels[_iIdx + 2] += 1;

										m_pHeightMap[iIdx] += 1.f;
										m_vecHeight[iIdx] += 1.f;
										m_vecPos[iIdx].y += 1;*/
										Vector3 vScale = m_pDecal->GetWorldScale();

										int iIndex = GetIndex(vCrs + Vector3(
											static_cast<float>(b - static_cast<int>(iWidth / 2)) * vScale.x / iWidth,
											0.f,
											static_cast<float>(a - static_cast<int>(iHeight / 2)) * vScale.z / iHeight));

										if (iIndex == -1)
											continue;

										m_pHeightMap[iIndex] += fTime * 0.1f;
										m_vecHeight[iIndex] += fTime * 0.1f;
										m_vecPos[iIndex].y = m_vecHeight[iIndex];

										pHeightMapPixels[iIndex] = static_cast<uint8_t>(m_vecHeight[iIndex]);
										pHeightMapPixels[iIndex + 1] = static_cast<uint8_t>(m_vecHeight[iIndex]);
										pHeightMapPixels[iIndex + 2] = static_cast<uint8_t>(m_vecHeight[iIndex]);
									}
								}
							}
						}
						D3D11_MAPPED_SUBRESOURCE tSub = {};
						CONTEXT->Map(m_pTexture, 0, D3D11_MAP_WRITE_DISCARD, 0, &tSub);

						memcpy_s(tSub.pData, 4 * m_iHeight * m_iWidth, &m_vecHeight[0], 4 * m_iHeight * m_iWidth);

						CONTEXT->Unmap(m_pTexture, 0);

						std::shared_ptr<CComponent> pCollider = FindComponentByType<CColliderTerrain>();

						if (pCollider)
						{
							static_cast<CColliderTerrain*>(pCollider.get())->SetInfo(m_vecPos, m_iWidth, m_iHeight);
						}

						/*class CMesh* pMesh = static_cast<CRenderer*>(pRenderer.get())->GetMesh();

						if (pMesh)
						{
							pMesh->ChangeMeshBuffer(m_vecVtx);
						}*/
					}
				}
			}

		}
		if (GET_SINGLE(CInput)->IsPress(MOUSE_TYPE::RIGHT))
		{
			if (m_bBlurMode)
				return;

			int iIdx = GetIndex(vCrs);

			if (iIdx != -1)
			{
				/*m_vecVtx[iIdx].vPos.y += fTime * 2.f;
				m_vecPos[iIdx].y += fTime * 2.f;*/

				CGameObject* pObj = GetObj();

				if (pObj)
				{
					std::shared_ptr<CComponent> pRenderer = pObj->FindComponentByType<CRenderer>();
					if (pRenderer)
					{
						Vector3 vScale = static_cast<CSceneComponent*>(pRenderer.get())->GetWorldScale();
						Vector3 vPos = static_cast<CSceneComponent*>(pRenderer.get())->GetWorldPos();

						Vector3 vLocal = (vCrs - vPos) / vScale;

						unsigned int iWidth = m_pBrushTexture->GetWidth();
						unsigned int iHeight = m_pBrushTexture->GetHeight();

						int iStartX = static_cast<int>(vLocal.x - iWidth / 2);
						int iStartY = static_cast<int>(vLocal.z - iHeight / 2);

						int iEndX = static_cast<int>(vLocal.x + iWidth / 2);
						int iEndY = static_cast<int>(vLocal.z + iHeight / 2);

						iStartX = iStartX < 0 ? 0 : iStartX;
						iStartY = iStartY < 0 ? 0 : iStartY;

						iEndX = iEndX >= m_pImage->GetMetadata().width ? static_cast<int>(m_pImage->GetMetadata().width) - 1 : iEndX;
						iEndY = iEndY >= m_pImage->GetMetadata().height ? static_cast<int>(m_pImage->GetMetadata().height) - 1 : iEndY;
						size_t iHeightMapWidth = m_pImage->GetMetadata().width;
						uint8_t* pPixels = m_pBrushTexture->GetInfo()->pImage->GetPixels();

						uint8_t* pHeightMapPixels = m_pImage->GetPixels();

						for (int j = iStartY, a = 0; j <= iEndY; j++, a++)
						{
							for (int i = iStartX, b = 0; i <= iEndX; i++, b++)
							{
								if (pPixels[(a * iWidth + b) * 4] > 0)
								{
									Vector3 vScale = m_pDecal->GetWorldScale();

									int iIndex = GetIndex(vCrs + Vector3(
										static_cast<float>(b - static_cast<int>(iWidth / 2)) * vScale.x / iWidth,
										0.f,
										static_cast<float>(a - static_cast<int>(iHeight / 2)) * vScale.z / iHeight));

									if (iIndex == -1)
										continue;

									/*int iIdx = j * static_cast<int>(iHeightMapWidth) + i;
									int _iIdx = iIdx * 4;*/

									m_pHeightMap[iIndex] -= fTime * 0.1f;
									m_vecHeight[iIndex] -= fTime * 0.1f;
									m_vecPos[iIndex].y -= fTime * 0.1f;
									pHeightMapPixels[iIndex] = static_cast<uint8_t>(m_vecHeight[iIndex]);
									pHeightMapPixels[iIndex + 1] = static_cast<uint8_t>(m_vecHeight[iIndex]);
									pHeightMapPixels[iIndex + 2] = static_cast<uint8_t>(m_vecHeight[iIndex]);
								}
							}
						}
						D3D11_MAPPED_SUBRESOURCE tSub = {};
						CONTEXT->Map(m_pTexture, 0, D3D11_MAP_WRITE_DISCARD, 0, &tSub);

						memcpy_s(tSub.pData, 4 * m_iHeight * m_iWidth, &m_vecHeight[0], 4 * m_iHeight * m_iWidth);

						CONTEXT->Unmap(m_pTexture, 0);

						std::shared_ptr<CComponent> pCollider = FindComponentByType<CColliderTerrain>();

						if (pCollider)
						{
							static_cast<CColliderTerrain*>(pCollider.get())->SetInfo(m_vecPos, m_iWidth, m_iHeight);
						}

						/*class CMesh* pMesh = static_cast<CRenderer*>(pRenderer.get())->GetMesh();

						if (pMesh)
						{
							pMesh->ChangeMeshBuffer(m_vecVtx);
						}*/
					}
				}
			}
		}
	}
}

void CTerrain::ColLast(CCollider* pSrc, CCollider* pDest, float fTime)
{
}

CTerrain* CTerrain::FindTerrain(const std::string& strKey)
{
	std::unordered_map<CTerrain*, std::string>::iterator iter = m_mapTerrain.begin();
	std::unordered_map<CTerrain*,std::string>::iterator iterEnd = m_mapTerrain.end();

	for (; iter != iterEnd; iter++)
	{
		if (iter->second == strKey)
		{
			return iter->first;
		}
	}

	return nullptr;
}

void CTerrain::DeleteTerrain(CTerrain* pTerrain)
{
	std::unordered_map< CTerrain*, std::string>::iterator iter = m_mapTerrain.find(pTerrain);

	if (iter == m_mapTerrain.end())
		return;
	
	m_mapTerrain.erase(iter);

	if (m_pMainTerrain == pTerrain)
		m_pMainTerrain = nullptr;
}
CTerrain* CTerrain::GetMainTerrain()
{
	return m_pMainTerrain;
}

void CTerrain::SetMainTerrain(CTerrain* pTerrain)
{
	m_pMainTerrain = pTerrain;
}

void CTerrain::SetMainTerrain(const std::string& strKey)
{
	CTerrain* pTerrain = FindTerrain(strKey);

	if (!pTerrain)
		return;

	m_pMainTerrain = pTerrain;
}

void CTerrain::AddTerrain(CTerrain* pTerrain)
{
	if (!pTerrain)
		return;

	m_mapTerrain.insert(std::make_pair(pTerrain, pTerrain->GetName()));

	if (!m_pMainTerrain)
		m_pMainTerrain = pTerrain;
}

std::shared_ptr<class CTexture> CTerrain::FindBrush(const std::string& strKey)
{
	std::unordered_map<std::string, std::shared_ptr<CTexture>>::iterator iter = m_mapBrush.find(strKey);

	if (iter == m_mapBrush.end())
		return nullptr;

	return iter->second;
}

void CTerrain::ShowWindow()
{
	CSceneComponent::ShowWindow();

	std::string strName = GetName();

	if (strName == "")
	{
		strName = "None";
	}

	if (ImGui::Begin(strName.c_str()))
	{
		ImGui::Text("Height %d", m_iHeight);
		ImGui::Text("Width %d", m_iWidth);
		ImGui::InputFloat2("Tiling", &m_tCBuffer.fDetailX);
		ImGui::InputInt("Splating Count", &m_tCBuffer.iCount);

		if (ImGui::Button("Save Height Map"))
		{
			const char* pPath = GET_SINGLE(CPathManager)->FindMultiBytePath(TEXTURE_PATH);

			ImGuiFileDialog::Instance()->OpenDialog("Save Height Map", "Save Height Map", ".bmp", pPath);
		}

		if (ImGuiFileDialog::Instance()->Display("Save Height Map"))
		{
			if (ImGuiFileDialog::Instance()->IsOk())
			{
				std::string strPathName = ImGuiFileDialog::Instance()->GetFilePathName();

				TCHAR pPath[MAX_PATH] = {};
#ifdef UNICODE
				MultiByteToWideChar(CP_ACP, 0, strPathName.c_str(), -1, pPath, static_cast<int>(strPathName.length()));
#else
				strcpy_s(pPath, MAX_PATH, strPath.c_str());
#endif
				SaveHeightMap(pPath, "");
			}

			ImGuiFileDialog::Instance()->Close();
		}

		if (ImGui::Button("Load Height Map"))
		{
			const char* pPath = GET_SINGLE(CPathManager)->FindMultiBytePath(TEXTURE_PATH);

			ImGuiFileDialog::Instance()->OpenDialog("Load Height Map", "Load Height Map", ".bmp", pPath);
		}

		if (ImGuiFileDialog::Instance()->Display("Load Height Map"))
		{
			if (ImGuiFileDialog::Instance()->IsOk())
			{
				std::string strPathName = ImGuiFileDialog::Instance()->GetFilePathName();

				TCHAR pPath[MAX_PATH] = {};
#ifdef UNICODE
				MultiByteToWideChar(CP_ACP, 0, strPathName.c_str(), -1, pPath, static_cast<int>(strPathName.length()));
#else
				strcpy_s(pPath, MAX_PATH, strPath.c_str());
#endif
				BuildHeightMap("test", m_iWidth, m_iHeight, 255.f, pPath, "");
			}

			ImGuiFileDialog::Instance()->Close();
		}

		if (ImGui::Checkbox("Edit Mode", &m_bEditMode))
		{
			SetEditorMode(m_bEditMode);
		}

		if (ImGui::Checkbox("Blur Mode", &m_bBlurMode))
		{
			SetBlurMode(m_bBlurMode);
		}
		
		if (ImGui::InputFloat3("Brush Scale", &m_vBrushScale.x))
		{
			Vector3 vWorldScale = GetWorldScale();
			float fWidth = static_cast<float>(m_pBrushTexture->GetWidth());
			float fHeight = static_cast<float>(m_pBrushTexture->GetHeight());
			m_pDecal->SetWorldScale(m_vBrushScale.x * fWidth, vWorldScale.y * 255.f, m_vBrushScale.z *fHeight);

			 //std::shared_ptr<CComponent> pRenderer = m_pDecal->FindComponentByType<CRenderer>("");
			 //static_cast<CSceneComponent*>(pRenderer.get())->SetWorldScale(fWidth, 1.f, fHeight);
		}

		int iSize = static_cast<int>(m_mapBrush.size());

		std::vector<const char*> vecstrBrush(iSize);

		std::unordered_map<std::string, std::shared_ptr<CTexture>>::iterator iter = m_mapBrush.begin();
		std::unordered_map<std::string, std::shared_ptr<CTexture>>::iterator iterEnd = m_mapBrush.end();

		for (int i=0; iter != iterEnd; ++iter,i++)
		{
			vecstrBrush[i] = iter->second->GetName().c_str();
		}
		static int iSelect = -1;
		if (iSize > 0)
		{
			ImGui::ListBox("Brush List", &iSelect, &vecstrBrush[0], iSize);

			if (iSelect != -1 && iSelect < iSize)
			{
				SetBrushTexture(vecstrBrush[iSelect], L"");
			
			}

			if (m_pBrushTexture)
			{
				ImGui::Text("Brush Name %s", m_pBrushTexture->GetName().c_str());
			}
		}
	}
	ImGui::End();
}

void CTerrain::ExtractFrustumPlanes(Vector4 planes[6], Matrix& M)
{
	planes[0].x = M[0][3] + M[0][0];
	planes[0].y = M[1][3] + M[1][0];
	planes[0].z = M[2][3] + M[2][0];
	planes[0].w = M[3][3] + M[3][0];

	planes[1].x = M[0][3] - M[0][0];
	planes[1].y = M[1][3] - M[1][0];
	planes[1].z = M[2][3] - M[2][0];
	planes[1].w = M[3][3] - M[3][0];

	planes[2].x = M[0][3] + M[0][1];
	planes[2].y = M[1][3] + M[1][1];
	planes[2].z = M[2][3] + M[2][1];
	planes[2].w = M[3][3] + M[3][1];

	planes[3].x = M[0][3] - M[0][1];
	planes[3].y = M[1][3] - M[1][1];
	planes[3].z = M[2][3] - M[2][1];
	planes[3].w = M[3][3] - M[3][1];

	planes[4].x = M[0][2];
	planes[4].y = M[1][2];
	planes[4].z = M[2][2];
	planes[4].w = M[3][2];

	planes[5].x = M[0][3] - M[0][2];
	planes[5].y = M[1][3] - M[1][2];
	planes[5].z = M[2][3] - M[2][2];
	planes[5].w = M[3][3] - M[3][2];

	for (int i = 0; i < 6; ++i)
	{
		m_tCBuffer.gWorldFrustumPlanes[i] = XMPlaneNormalize(planes[i].Convert());
	}
}

void CTerrain::CalcAllPatchBoundsY()
{
	m_vecPatchBoundsY.resize(m_iNumPatchQuadFaces);

	for (int i = 0; i < m_iHeight - 1; i++)
	{
		for (int j = 0; j < m_iWidth-1; j++)
		{
			CalcPatchBoundsY(i, j);
		}
	}
}

void CTerrain::CalcPatchBoundsY(UINT i, UINT j)
{
	UINT x0 = j;
	UINT x1 = (j + 1);

	UINT y0 = i;
	UINT y1 = (i + 1);

	float minY = FLT_MAX;
	float maxY = -FLT_MAX;
	for (UINT y = y0; y <= y1; y++)
	{
		for (UINT x = x0; x <= x1; x++)
		{
			UINT k = y * m_iWidth + x;
			minY = min(minY, m_vecVtx[k].vPos.y);
			maxY = max(maxY, m_vecVtx[k].vPos.y);
		}
	}

	UINT patchID = i * (m_iWidth - 1) + j;
	m_vecPatchBoundsY[patchID] = Vector2(minY, maxY);
}

bool CTerrain::BuildHeightMap(const std::string& strMesh, int iWidth, int iHeight, float fMaxHeight,
	const TCHAR* pHeightMapPath, const std::string& strPathKey)
{
	SAFE_DELETE_ARRAY(m_pHeightMap);

	m_iWidth = iWidth;
	m_iHeight = iHeight;

	m_pHeightMap = new float[m_iWidth * m_iHeight];
	memset(m_pHeightMap, 0, m_iWidth * m_iHeight * 4);
	m_vecHeight.resize(m_iWidth * m_iHeight);

	m_tCBuffer.gTexelCellSpaceU = 1.f / m_iWidth;
	m_tCBuffer.gTexelCellSpaceV = 1.f / m_iHeight;

	if (pHeightMapPath)
	{
		TCHAR strFullPath[MAX_PATH] = {};

		const TCHAR* pPath = GET_SINGLE(CPathManager)->FindPath(strPathKey);

		if (pPath)
			lstrcpy(strFullPath, pPath);

		lstrcat(strFullPath, pHeightMapPath);

		SAFE_DELETE(m_pImage);
		m_pImage = new ScratchImage;

		TCHAR _pExt[_MAX_EXT] = {};

		_wsplitpath_s(strFullPath, nullptr, 0, nullptr, 0, nullptr, 0, _pExt, _MAX_EXT);

		_wcsupr_s(_pExt);

		if (!wcscmp(_pExt, TEXT(".DDS")))
		{
			if (FAILED(LoadFromDDSFile(strFullPath, DDS_FLAGS_NONE, nullptr, *m_pImage)))
			{
				SAFE_DELETE(m_pImage);
				return false;
			}
		}

		else if (!lstrcmp(_pExt, TEXT(".TGA")))
		{
			if (FAILED(LoadFromTGAFile(strFullPath, nullptr, *m_pImage)))
			{
				SAFE_DELETE(m_pImage);
				return false;
			}
		}

		else
		{
			if (FAILED(LoadFromWICFile(strFullPath, WIC_FLAGS_NONE, nullptr, *m_pImage)))
			{
				SAFE_DELETE(m_pImage);
				return false;
			}
		}

		const Image* _pImage = m_pImage->GetImages();
		for (int i = 0; i < m_iHeight; i++)
		{
			for (int j = 0; j < m_iWidth; j++)
			{
				int iIdx = i * m_iWidth + j;
				m_pHeightMap[iIdx] = _pImage->pixels[4 * iIdx] / 255.f * fMaxHeight;
				m_vecHeight[iIdx] = _pImage->pixels[4 * iIdx] / 255.f * fMaxHeight;
			}
		}
	}

	std::vector<Vector3>	vecPos(m_iHeight * m_iWidth);

	m_vecPos.resize(m_iHeight * m_iWidth);	
	m_vecVtx.resize(m_iWidth * m_iHeight);

	for (int i = 0; i < m_iHeight; i++)
	{
		for (int j = 0; j < m_iWidth; j++)
		{
			int iIdx = i * m_iWidth + j;
			VERTEX3D tVtx = {};
			tVtx.vNormal = Vector3(0.f, 1.f, 0.f);
			tVtx.vTangent = Vector3(1.f, 0.f, 0.f);
			tVtx.vBinormal = Vector3(0.f, 0.f, -1.f);
			tVtx.vUV = Vector2(j / static_cast<float>(m_iWidth - 1), (m_iHeight - i - 1) / static_cast<float>(m_iHeight - 1));
			m_vecVtx[iIdx] = tVtx;
			vecPos[iIdx] = Vector3(static_cast<float>(j), m_pHeightMap[iIdx], static_cast<float>(i));
		}
	}

	for (int i = 0; i < m_iHeight; i++)
	{
		for (int j = 0; j < m_iWidth; j++)
		{
			int iNum = 1;
			int iIdx = i * m_iWidth + j;
			float fHeight = vecPos[iIdx].y;
			int iLB = iIdx - m_iWidth - 1;
			if (j != 0 && i != 0)
			{
				fHeight += vecPos[iLB].y;
				iNum++;
			}
				
			int iB = iIdx - m_iWidth;
			if (i != 0)
			{
				fHeight += vecPos[iB].y;
				iNum++;
			}
				
			int iRB = iIdx - m_iWidth + 1;
			if (i != 0 && j != m_iWidth-1)
			{
				fHeight += vecPos[iRB].y;
				iNum++;
			}
				
			int iL = iIdx - 1;
			if (j != 0)
			{
				fHeight += vecPos[iL].y;
				iNum++;
			}
				
			int iR = iIdx + 1;
			if (j!= m_iWidth - 1)
			{
				fHeight += vecPos[iR].y;
				iNum++;
			}
				
			int iLT = iIdx + m_iWidth - 1;
			if (j != 0 && i!= m_iHeight-1)
			{
				fHeight += vecPos[iLT].y;
				iNum++;
			}
			int iT = iIdx + m_iWidth;
			if (i != m_iHeight - 1)
			{
				fHeight += vecPos[iT].y;
				iNum++;
			}
			int iRT = iIdx + m_iWidth + 1;
			if (i != m_iHeight - 1 && j != m_iWidth-1)
			{
				fHeight += vecPos[iRT].y;
				iNum++;
			}

			vecPos[iIdx].y = fHeight / iNum;

			m_vecPos[iIdx] = vecPos[iIdx];
			m_vecVtx[iIdx].vPos = vecPos[iIdx];
			m_pHeightMap[iIdx] = vecPos[iIdx].y;
			m_vecHeight[iIdx] = vecPos[iIdx].y;
		}
	}

	m_iNumPatchQuadFaces = (m_iHeight - 1) * (m_iWidth - 1) * 2;
	//CalcAllPatchBoundsY();

	//for (UINT i = 0; i < m_iHeight - 1; i++)
	//{
	//	for (UINT j = 0; j < m_iWidth - 1; j++)
	//	{
	//		UINT patchID = i * (m_iWidth - 1) + j;
	//		m_vecVtx[i * m_iWidth + j].vBoundsY = m_vecPatchBoundsY[patchID];
	//	}
	//}

	std::vector<unsigned int>	vecIdx;
	int iTris = 0;
	m_vecFaceNormal.resize(m_iNumPatchQuadFaces);
	for (int i = 0; i < m_iHeight-1; i++)
	{
		for (int j = 0; j < m_iWidth-1; j++)
		{
			vecIdx.push_back(j + m_iWidth * (i + 1));
			vecIdx.push_back(j + 1 + m_iWidth * (1 + i));
			vecIdx.push_back(j + 1 + m_iWidth * i);
			vecIdx.push_back(j + m_iWidth * (1 + i));
			vecIdx.push_back(j + 1 + m_iWidth * i);
			vecIdx.push_back(j + m_iWidth * i);

			int iIdx0 = j + m_iWidth * (i + 1);
			int iIdx1 = j + 1 + m_iWidth * (1 + i);
			int iIdx2 = j + 1 + m_iWidth * i;

			Vector3 vEdge1 = m_vecVtx[iIdx1].vPos - m_vecVtx[iIdx0].vPos;
			Vector3 vEdge2 = m_vecVtx[iIdx2].vPos - m_vecVtx[iIdx0].vPos;

			vEdge1.Normalize();
			vEdge2.Normalize();

			int iIdx = (i * (m_iHeight - 1) + j) * 2;
			m_vecFaceNormal[iTris] = vEdge1.Cross(vEdge2);

			m_vecFaceNormal[iTris++].Normalize();

			iIdx0 = j + m_iWidth * (1 + i);
			iIdx1 = j + 1 + m_iWidth * i;
			iIdx2 = j + m_iWidth * i;

			vEdge1 = m_vecVtx[iIdx1].vPos - m_vecVtx[iIdx0].vPos;
			vEdge2 = m_vecVtx[iIdx2].vPos - m_vecVtx[iIdx0].vPos;

			vEdge1.Normalize();
			vEdge2.Normalize();

			m_vecFaceNormal[iTris] = vEdge1.Cross(vEdge2);
			m_vecFaceNormal[iTris++].Normalize();
		}
	}

	ComputeNormal();
	ComputeTangent();

	D3D11_TEXTURE2D_DESC tDesc = {};

	tDesc.ArraySize = 1;
	tDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	tDesc.Format = DXGI_FORMAT_R32_FLOAT;
	tDesc.Height = m_iHeight;
	tDesc.Width = m_iWidth;
	tDesc.MipLevels = 1;
	tDesc.SampleDesc.Count = 1;
	tDesc.SampleDesc.Quality = 0;
	tDesc.Usage = D3D11_USAGE_DYNAMIC;
	tDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	D3D11_SUBRESOURCE_DATA tSub = {};
	tSub.pSysMem = m_pHeightMap;
	tSub.SysMemPitch = m_iWidth * 4;
	SAFE_RELEASE(m_pTexture);
	if (FAILED(DEVICE->CreateTexture2D(&tDesc, &tSub, &m_pTexture)))
		return false;
	D3D11_SHADER_RESOURCE_VIEW_DESC tSrv = {};
	tSrv.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	tSrv.Texture2D.MipLevels = -1;
	tSrv.Texture2D.MostDetailedMip = 0;

	tSrv.Format = tDesc.Format;
	SAFE_RELEASE(m_pHeightMapSRV);
	if (FAILED(DEVICE->CreateShaderResourceView(m_pTexture, &tSrv, &m_pHeightMapSRV)))
		return false;

	CONTEXT->VSSetShaderResources(20, 1, &m_pHeightMapSRV);

	GET_SINGLE(CResourceManager)->CreateMesh(strMesh, MESH_TYPE::MT_STATIC_MESH, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
		static_cast<int>(m_vecVtx.size()), static_cast<int>(sizeof(m_vecVtx[0])), D3D11_USAGE_DYNAMIC, &m_vecVtx[0],
		static_cast<int>(vecIdx.size()), 4, D3D11_USAGE_IMMUTABLE, DXGI_FORMAT_R32_UINT, &vecIdx[0], D3D11_CPU_ACCESS_WRITE);
	/*if (!GET_SINGLE(CResourceManager)->CreateMesh(strMesh, MESH_TYPE::MT_STATIC_MESH, D3D11_PRIMITIVE_TOPOLOGY_4_CONTROL_POINT_PATCHLIST,
		static_cast<int>(m_vecVtx.size()), static_cast<int>(sizeof(m_vecVtx[0])), D3D11_USAGE_DYNAMIC, &m_vecVtx[0],
		static_cast<int>(vecIdx.size()), 4, D3D11_USAGE_IMMUTABLE, DXGI_FORMAT_R32_UINT, &vecIdx[0], D3D11_CPU_ACCESS_WRITE))
		return false;*/

	SetMesh(strMesh);

	return true;
}

void CTerrain::SaveHeightMap(const TCHAR* pFilePath, const std::string& strPathKey)
{
	TCHAR strFullPath[MAX_PATH] = {};

	const TCHAR* pPath = GET_SINGLE(CPathManager)->FindPath(strPathKey);

	if (pPath)
		lstrcpy(strFullPath, pPath);

	lstrcat(strFullPath, pFilePath);
	
	if (FAILED(SaveToWICFile(*m_pImage->GetImages(), WIC_FLAGS_NONE, GUID_ContainerFormatBmp, strFullPath)))
		return;

	return;
}

void CTerrain::SetBrushTexture(const std::string& strKey, const TCHAR* pFilePath, 
	const std::string& strPathKey)
{
	GET_SINGLE(CResourceManager)->LoadTexture(strKey, pFilePath, strPathKey);

	m_pBrushTexture = GET_SINGLE(CResourceManager)->FindTexture(strKey);

	if (!m_pBrushTexture)
		return;

	std::shared_ptr<CTexture> pBrush = FindBrush(strKey);

	if (!pBrush)
	{
		m_mapBrush.insert(std::make_pair(strKey, m_pBrushTexture));
	}

	SetTexture(LINK_TYPE::LT_DIF, strKey);

	unsigned int iWidth = m_pBrushTexture->GetWidth();
	unsigned int iHeight = m_pBrushTexture->GetHeight();

	Vector3 vScale = GetWorldScale();
	m_pDecal->SetWorldScale(static_cast<float>(iWidth) * m_vBrushScale.x, 
		vScale.y * 255.f, static_cast<float>(iHeight) * m_vBrushScale.z);
}

int CTerrain::GetIndex(const Vector3& vPos)
{
	CGameObject* pObj = GetObj();
	Vector3 _vPos = GetWorldPos();
	Vector3 vScale = GetWorldScale();


	Vector3 vIndex = (vPos - _vPos) / vScale;

	if (vIndex.x < 0 || vIndex.z < 0 || vIndex.x >= m_iWidth-1 || vIndex.z >= m_iHeight-1)
		return -1;

	return (int)vIndex.z * m_iWidth + (int)vIndex.x;
}

float CTerrain::GetHeight(const Vector3& vPos)
{
	CGameObject* pObj = GetObj();
	Vector3 _vPos = GetWorldPos();
	Vector3 vScale = GetWorldScale();

	Vector3 vIndex = (vPos - _vPos) / vScale;

	if (vIndex.x < 0 || vIndex.z < 0 || vIndex.x >= m_iWidth - 1 || vIndex.z >= m_iHeight - 1)
		return 0.f;

	int iIdx = (int)vIndex.z * m_iWidth + (int)vIndex.x;

	if (iIdx < 0)
		return 0.f;

	Vector3 A = m_vecPos[iIdx];
	Vector3 B = m_vecPos[iIdx+1];
	Vector3 C = m_vecPos[iIdx + m_iWidth];
	Vector3 D = m_vecPos[iIdx + 1 + m_iWidth];

	Vector3 vRatio = vIndex - A;

	if (vRatio.x + vRatio.z >= 1.f)
	{
		return ((B.y - A.y) * vRatio.x + A.y + (C.y - A.y) * vRatio.z) * vScale.y + _vPos.y;
	}	

	return ((C.y - D.y) * (1.f - vRatio.x) + D.y + (B.y - D.y) * (1.f - vRatio.z)) * vScale.y + _vPos.y;
}

void CTerrain::SetTiling(float fX, float fY)
{
	m_tCBuffer.fDetailX = fX;
	m_tCBuffer.fDetailY = fY;
}

const std::vector<Vector3>& CTerrain::GetVecPos() const
{
	return m_vecPos;
}

void CTerrain::SetCollider(const std::shared_ptr<class CComponent>& pCol)
{
	static_cast<CCollider*>(pCol.get())->SetCallBack<CTerrain>(COLLISION_STATE::CS_INIT, this, &CTerrain::ColInit);
	static_cast<CCollider*>(pCol.get())->SetCallBack<CTerrain>(COLLISION_STATE::CS_STAY, this, &CTerrain::ColStay);
	static_cast<CCollider*>(pCol.get())->SetCallBack<CTerrain>(COLLISION_STATE::CS_LAST, this, &CTerrain::ColLast);
}

void CTerrain::SetSplatCount(int iCount)
{
	m_tCBuffer.iCount = iCount;
}

const Vector3 CTerrain::GetNormal(const Vector3& vPos) const
{
	CGameObject* pObj = GetObj();
	Vector3 _vPos = GetWorldPos();
	Vector3 vScale = GetWorldScale();

			
			

	Vector3 vIndex = (vPos - _vPos) / vScale;

	if (vIndex.x < 0 || vIndex.z < 0 || vIndex.x >= m_iWidth - 1 || vIndex.z >= m_iHeight - 1)
		return Vector3();

	int iIdx = (int)vIndex.z * m_iWidth + (int)vIndex.x;

	if (iIdx == -1)
		return Vector3();

	Vector3 A = m_vecVtx[iIdx].vNormal;
	Vector3 B = m_vecVtx[iIdx + 1].vNormal;
	Vector3 C = m_vecVtx[iIdx + m_iWidth].vNormal;
	Vector3 D = m_vecVtx[iIdx + 1 + m_iWidth].vNormal;

	Vector3 vRatio = vIndex - A;

	if (vRatio.x + vRatio.z >= 1.f)
	{
		Vector3 vNormal = ((B - A) * vRatio.x + A + (C - A) * vRatio.z) * vScale;

		vNormal.Normalize();

		return vNormal;
	}

	Vector3 vNormal = ((C - D) * (1.f - vRatio.x) + D + (B - D) * (1.f - vRatio.z)) * vScale;

	vNormal.Normalize();

	return vNormal;
}

const Vector3 CTerrain::GetTangent(const Vector3& vPos) const
{
	CGameObject* pObj = GetObj();
	Vector3 _vPos = GetWorldPos();
	Vector3 vScale = GetWorldScale();

	Vector3 vIndex = (vPos - _vPos) / vScale;

	if (vIndex.x < 0 || vIndex.z < 0 || vIndex.x >= m_iWidth - 1 || vIndex.z >= m_iHeight - 1)
		return Vector3();

	int iIdx = (int)vIndex.z * m_iWidth + (int)vIndex.x;

	if (iIdx == -1)
		return Vector3();

	Vector3 A = m_vecVtx[iIdx].vTangent;
	Vector3 B = m_vecVtx[iIdx + 1].vTangent;
	Vector3 C = m_vecVtx[iIdx + m_iWidth].vTangent;
	Vector3 D = m_vecVtx[iIdx + 1 + m_iWidth].vTangent;

	Vector3 vRatio = vIndex - A;

	if (vRatio.x + vRatio.z >= 1.f)
	{
		Vector3 vTangent = ((B - A) * vRatio.x + A + (C - A) * vRatio.z) * vScale;

		vTangent.Normalize();

		return vTangent;
	}

	Vector3 vTangent = ((C - D) * (1.f - vRatio.x) + D + (B - D) * (1.f - vRatio.z)) * vScale;

	vTangent.Normalize();

	return vTangent;
}

const Vector3 CTerrain::GetBinormal(const Vector3& vPos) const
{
	CGameObject* pObj = GetObj();
	Vector3 _vPos = GetWorldPos();
	Vector3 vScale = GetWorldScale();

	Vector3 vIndex = (vPos - _vPos) / vScale;

	if (vIndex.x < 0 || vIndex.z < 0 || vIndex.x >= m_iWidth - 1 || vIndex.z >= m_iHeight - 1)
		return Vector3();

	int iIdx = (int)vIndex.z * m_iWidth + (int)vIndex.x;

	if (iIdx == -1)
		return Vector3();

	Vector3 A = m_vecVtx[iIdx].vBinormal;
	Vector3 B = m_vecVtx[iIdx + 1].vBinormal;
	Vector3 C = m_vecVtx[iIdx + m_iWidth].vBinormal;
	Vector3 D = m_vecVtx[iIdx + 1 + m_iWidth].vBinormal;

	Vector3 vRatio = vIndex - A;

	if (vRatio.x + vRatio.z >= 1.f)
	{
		Vector3 vBinormal = ((B - A) * vRatio.x + A + (C - A) * vRatio.z) * vScale;

		vBinormal.Normalize();

		return vBinormal;
	}

	Vector3 vBinormal = ((C - D) * (1.f - vRatio.x) + D + (B - D) * (1.f - vRatio.z)) * vScale;

	vBinormal.Normalize();

	return vBinormal;
}

float CTerrain::GetSlope(const Vector3& vDir, const Vector3& vPos)	const
{
	Vector3 vTangent = GetTangent(vDir, vPos);

	if (vTangent.Length() == 0.f)
		return 0.f;

	float fDot = vTangent.Dot(vDir);

	if (fDot > 1.f)
		fDot = 1.f;

	float fAngle = acosf(fDot);

	return fAngle;
}

const Vector3 CTerrain::GetTangent(const Vector3& vDir, const Vector3& vPos) const
{
	CGameObject* pObj = GetObj();
	Vector3 _vPos = GetWorldPos();
	Vector3 vScale = GetWorldScale();

	Vector3 vIndex = (vPos - _vPos) / vScale;

	if (vIndex.x < 0 || vIndex.z < 0 || vIndex.x >= m_iWidth - 1 || vIndex.z >= m_iHeight - 1)
		return Vector3();

	int iIdx = (int)vIndex.z * m_iWidth + (int)vIndex.x;

	if (iIdx < 0)
		return Vector3();

	int iX = iIdx % m_iWidth;
	int iY = iIdx / m_iWidth;

	Vector3 A = m_vecPos[iIdx];
	Vector3 B = m_vecPos[iIdx + 1];
	Vector3 C = m_vecPos[iIdx + m_iWidth];
	Vector3 D = m_vecPos[iIdx + 1 + m_iWidth];

	Vector3 vRatio = vIndex - A;

	Vector3 vNormal = {};

	if (vRatio.x + vRatio.z >= 1.f)
	{
		Vector3 e1 = A - B;
		Vector3 e2 = D - B;

		e1.Normalize();
		e2.Normalize();

		vNormal = e1.Cross(e2);
	}
	else
	{
		Vector3 e1 = A - C;
		Vector3 e2 = D - C;

		e1.Normalize();
		e2.Normalize();

		vNormal = e1.Cross(e2);
	}

	Vector3 vBinormal = vNormal.Cross(vDir);

	vBinormal.Normalize();

	Vector3 vTangent = vBinormal.Cross(vNormal);

	return vTangent;
}

float CTerrain::GetHeightMapWidth() const
{
	return (m_iWidth - 1) * m_fCellSpacing;
}

float CTerrain::GetHeightMapDepth() const
{
	return (m_iHeight - 1) * m_fCellSpacing;
}

void CTerrain::BuildQuadPatch()
{
	std::vector<VERTEX3D> patchVertices(m_iNumPatchVertRows * m_iNumPatchVertCols);

	float halfWidth = 0.5f * GetHeightMapWidth();
	float halfDepth = 0.5f * GetHeightMapDepth();

	float patchWidth = GetHeightMapWidth() / (m_iNumPatchVertCols - 1);
	float patchDepth = GetHeightMapDepth() / (m_iNumPatchVertRows - 1);
	float du = 1.f / (m_iNumPatchVertCols - 1);
	float dv = 1.f / (m_iNumPatchVertRows - 1);
	for (UINT i = 0; i < m_iNumPatchVertRows; i++)
	{
		float z = halfDepth - i * patchDepth;
		for (UINT j = 0; j < m_iNumPatchVertCols; j++)
		{
			float x = -halfWidth + j * patchWidth;

			patchVertices[i * m_iNumPatchVertCols + j].vPos = Vector3(x, 0.f, z);

			patchVertices[i * m_iNumPatchVertCols + j].vUV.x = j * du;
			patchVertices[i * m_iNumPatchVertCols + j].vUV.y = i * dv;
		}
	}

	for (UINT i = 0; i < m_iNumPatchVertRows - 1; ++i)
	{
		for (UINT j = 0; j < m_iNumPatchVertCols - 1; ++j)
		{
			UINT patchID = i * (m_iNumPatchVertCols - 1) + j;
			//patchVertices[i * m_iNumPatchVertCols + j].vBoundsY;// = m_pPatchBoundsY[patchID];
		}
	}
}

void CTerrain::BuildQuadPatchIB()
{
	std::vector<USHORT> indices(m_iNumPatchQuadFaces * 4);

	int k = 0;

	for (UINT i = 0; i < m_iNumPatchVertRows - 1; ++i)
	{
		for (UINT j = 0; j < m_iNumPatchVertCols - 1; ++j)
		{
			indices[k] = i * m_iNumPatchVertCols + j;
			indices[k + 1] = i * m_iNumPatchVertCols + j + 1;

			indices[k+2] = (i+1) * m_iNumPatchVertCols + j;
			indices[k + 3] = (i+1) * m_iNumPatchVertCols + j + 1;
			k += 4;
		}
	}

	D3D11_BUFFER_DESC tDesc = {};

	tDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	tDesc.ByteWidth = 2 * static_cast<UINT>(indices.size());
	tDesc.Usage = D3D11_USAGE_IMMUTABLE;

	D3D11_SUBRESOURCE_DATA tSub = {};
	tSub.pSysMem = &indices[0];

	DEVICE->CreateBuffer(&tDesc, &tSub, &m_pQuadPatchIB);
}

void CTerrain::SetEditorMode(bool bEdit)
{
	m_bEditMode = bEdit;

	if (!m_pDecal)
	{
		m_pDecal = m_pScene->CreateObject<CGameObject>("Decal", m_pScene->FindLayer(DECAL_LAYER));

		std::shared_ptr<CComponent> pDecal = m_pDecal->CreateComponent<CDecal>("Decal");

		m_pDecal->SetRootComponent(pDecal);

		static_cast<CSceneComponent*>(pDecal.get())->SetWorldScale(100.f, 100.f, 100.f);

		static_cast<CSceneComponent*>(pDecal.get())->SetMesh("OBBMesh");
		static_cast<CSceneComponent*>(pDecal.get())->SetMaterial("Color");
		static_cast<CSceneComponent*>(pDecal.get())->SetShader("Decal");
		static_cast<CSceneComponent*>(pDecal.get())->SetPivot(0.5f, 0.5f, 0.5f);
	}

	m_pDecal->SetEnable(m_bEditMode);
}

void CTerrain::SetBlurMode(bool bBlur)
{
	m_bBlurMode = bBlur;

	//if(m_bBlurMode)
	//{
	//	if (m_pBrushTexture)
	//	{
	//		Vector3 vScale = GetWorldScale();
	//		
	//		m_pDecal->SetWorldScale(
	//			vScale.x * m_pBrushTexture->GetWidth(), 
	//			255.f * vScale.y, 
	//			m_pBrushTexture->GetHeight() * vScale.z);
	//	}
	//}
	//else
	//{
	//	if (m_pBrushTexture)
	//	{
	//		Vector3 vScale = GetWorldScale();

	//		m_pDecal->SetWorldScale(
	//			static_cast<float>(m_pBrushTexture->GetWidth()),
	//			255.f * vScale.y,
	//			static_cast<float>(m_pBrushTexture->GetHeight()));
	//	}
	//}
}

void CTerrain::ComputeNormal()
{
	size_t iSize = m_vecFaceNormal.size();

	for (int i = 0; i < m_iHeight - 1; i++)
	{
		for (int j = 0; j < m_iWidth - 1; j++)
		{
			int iIdx0 = j + m_iWidth * (i + 1);
			int iIdx1 = j + 1 + m_iWidth * (1 + i);
			int iIdx2 = j + 1 + m_iWidth * i;
			int iIdx3 = j + m_iWidth * (1 + i);
			int iIdx4 = j + 1 + m_iWidth * i;
			int iIdx5 = j + m_iWidth * i;

			int iTri = (i * (m_iWidth - 1) + j) * 2;

			m_vecVtx[iIdx0].vNormal += m_vecFaceNormal[iTri];
			m_vecVtx[iIdx1].vNormal += m_vecFaceNormal[iTri];
			m_vecVtx[iIdx2].vNormal += m_vecFaceNormal[iTri];
			m_vecVtx[iIdx3].vNormal += m_vecFaceNormal[iTri+1];
			m_vecVtx[iIdx4].vNormal += m_vecFaceNormal[iTri+1];
			m_vecVtx[iIdx5].vNormal += m_vecFaceNormal[iTri+1];
		}
	}

	for (int i = 0; i < m_iHeight; i++)
	{
		for (int j = 0; j < m_iWidth; j++)
		{
			m_vecVtx[i * m_iWidth + j].vNormal.Normalize();
		}
	}
}

void CTerrain::ComputeTangent()
{
	for (int i = 0; i < m_iHeight-1; i++)
	{
		for (int j = 0; j < m_iWidth-1; j++)
		{
			/*int iIdx = i * m_iWidth + j;			

			int iLeft = iIdx - 1;
			int iRight = iIdx + 1;

			m_vecVtx[iLeft].vPos;
			m_vecVtx[iIdx].vPos;
			m_vecVtx[iRight].vPos;

			m_vecVtx[iIdx].vTangent = Vector3(1.f, (m_vecVtx[iRight].vPos.y - m_vecVtx[iLeft].vPos.y) / 2.f, 0.f);
			m_vecVtx[iIdx].vTangent.Normalize();

			m_vecVtx[iIdx].vBinormal = m_vecVtx[iIdx].vNormal.Cross(m_vecVtx[iIdx].vTangent);*/

			int iIdx0 = j + m_iWidth * (i + 1);
			int iIdx1 = j + 1 + m_iWidth * (1 + i);
			int iIdx2 = j + 1 + m_iWidth * i;

			float fVtx1[3], fVtx2[3];
			float ftu[2], ftv[2];

			fVtx1[0] = m_vecVtx[iIdx1].vPos.x - m_vecVtx[iIdx0].vPos.x;
			fVtx1[1] = m_vecVtx[iIdx1].vPos.y - m_vecVtx[iIdx0].vPos.y;
			fVtx1[2] = m_vecVtx[iIdx1].vPos.z - m_vecVtx[iIdx0].vPos.z;

			fVtx2[0] = m_vecVtx[iIdx2].vPos.x - m_vecVtx[iIdx0].vPos.x;
			fVtx2[1] = m_vecVtx[iIdx2].vPos.y - m_vecVtx[iIdx0].vPos.y;
			fVtx2[2] = m_vecVtx[iIdx2].vPos.z - m_vecVtx[iIdx0].vPos.z;

			ftu[0] = m_vecVtx[iIdx1].vUV.x - m_vecVtx[iIdx0].vUV.x;
			ftu[1] = m_vecVtx[iIdx2].vUV.x - m_vecVtx[iIdx0].vUV.x;

			ftv[0] = m_vecVtx[iIdx1].vUV.y - m_vecVtx[iIdx0].vUV.y;
			ftv[1] = m_vecVtx[iIdx2].vUV.y - m_vecVtx[iIdx0].vUV.y;

			float fDen = 1.f / (ftu[0] * ftv[1] - ftu[1] * ftv[0]);

			Vector3 vTanget = {};

			vTanget.x = (ftv[0] * fVtx1[0] - ftv[1] * fVtx2[0])*fDen;
			vTanget.y = (ftv[0] * fVtx1[1] - ftv[1] * fVtx2[1])*fDen;
			vTanget.z = (ftv[0] * fVtx1[2] - ftv[1] * fVtx2[2])*fDen;

			vTanget.Normalize();

			m_vecVtx[iIdx0].vTangent = vTanget;
			m_vecVtx[iIdx1].vTangent = vTanget;
			m_vecVtx[iIdx2].vTangent = vTanget;

			m_vecVtx[iIdx0].vBinormal = m_vecVtx[iIdx0].vNormal.Cross(m_vecVtx[iIdx0].vTangent);
			m_vecVtx[iIdx1].vBinormal = m_vecVtx[iIdx1].vNormal.Cross(m_vecVtx[iIdx1].vTangent);
			m_vecVtx[iIdx2].vBinormal = m_vecVtx[iIdx2].vNormal.Cross(m_vecVtx[iIdx2].vTangent);

			m_vecVtx[iIdx0].vBinormal.Normalize();
			m_vecVtx[iIdx1].vBinormal.Normalize();
			m_vecVtx[iIdx2].vBinormal.Normalize();

			iIdx0 = j + m_iWidth * (1 + i);
			iIdx1 = j + 1 + m_iWidth * i;
			iIdx2 = j + m_iWidth * i;

			fVtx1[0] = m_vecVtx[iIdx1].vPos.x - m_vecVtx[iIdx0].vPos.x;
			fVtx1[1] = m_vecVtx[iIdx1].vPos.y - m_vecVtx[iIdx0].vPos.y;
			fVtx1[2] = m_vecVtx[iIdx1].vPos.z - m_vecVtx[iIdx0].vPos.z;

			fVtx2[0] = m_vecVtx[iIdx2].vPos.x - m_vecVtx[iIdx0].vPos.x;
			fVtx2[1] = m_vecVtx[iIdx2].vPos.y - m_vecVtx[iIdx0].vPos.y;
			fVtx2[2] = m_vecVtx[iIdx2].vPos.z - m_vecVtx[iIdx0].vPos.z;

			ftu[0] = m_vecVtx[iIdx1].vUV.x - m_vecVtx[iIdx0].vUV.x;
			ftu[1] = m_vecVtx[iIdx2].vUV.x - m_vecVtx[iIdx0].vUV.x;

			ftv[0] = m_vecVtx[iIdx1].vUV.y - m_vecVtx[iIdx0].vUV.y;
			ftv[1] = m_vecVtx[iIdx2].vUV.y - m_vecVtx[iIdx0].vUV.y;

			fDen = 1.f / (ftu[0] * ftv[1] - ftu[1] * ftv[0]);

			vTanget.x = (ftv[0] * fVtx1[0] - ftv[1] * fVtx2[0]) * fDen;
			vTanget.y = (ftv[0] * fVtx1[1] - ftv[1] * fVtx2[1]) * fDen;
			vTanget.z = (ftv[0] * fVtx1[2] - ftv[1] * fVtx2[2]) * fDen;

			vTanget.Normalize();

			m_vecVtx[iIdx0].vTangent = vTanget;
			m_vecVtx[iIdx1].vTangent = vTanget;
			m_vecVtx[iIdx2].vTangent = vTanget;

			m_vecVtx[iIdx0].vBinormal = m_vecVtx[iIdx0].vNormal.Cross(m_vecVtx[iIdx0].vTangent);
			m_vecVtx[iIdx1].vBinormal = m_vecVtx[iIdx1].vNormal.Cross(m_vecVtx[iIdx1].vTangent);
			m_vecVtx[iIdx2].vBinormal = m_vecVtx[iIdx2].vNormal.Cross(m_vecVtx[iIdx2].vTangent);

			m_vecVtx[iIdx0].vBinormal.Normalize();
			m_vecVtx[iIdx1].vBinormal.Normalize();
			m_vecVtx[iIdx2].vBinormal.Normalize();
		}
	}
}
