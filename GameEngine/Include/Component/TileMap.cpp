#include "TileMap.h"
#include "../Tile.h"
#include "../Scene/Scene.h"
#include "../CameraManager.h"
#include "Camera.h"
#include "../Device.h"
#include "../LayerInstance.h"
#include "../Resource/Texture.h"
#include "../Resource/ResourceManager.h"
#include "../RenderManager.h"
#include "../Resource/Mesh2D.h"
#include "../Resource/Material.h"
#include "../Input.h"

CTileMap::CTileMap()	:
	m_iCountX(0),
	m_iCountY(0),
	m_iSize(0),
	m_vTileSize(),
	m_eTileType(TILE_TYPE::ISOMETRIC),
	m_pTexture(nullptr),
	m_iIndex(0),
	m_bEdit(false),
	m_eTileOption(TILE_OPTION::NONE),
	m_iHeight(0),
	m_bArrayTex(true)
{
	m_iSceneComponentClassType = (int)SCENECOMPONENT_CLASS_TYPE::TILEMAP;
	m_eSceneComponentType = SCENE_COMPONENT_TYPE::SCT_2D;
}

CTileMap::CTileMap(const CTileMap& map)	:
	CSceneComponent(map)
{
	m_iCountX = map.m_iCountX;
	m_iCountY = map.m_iCountY;
	m_iSize = map.m_iSize;
	m_eTileType = map.m_eTileType;
	m_vTileSize = map.m_vTileSize;

	m_pTexture = map.m_pTexture;
	m_iIndex = map.m_iIndex;
	m_bEdit = map.m_bEdit;
	m_eTileOption = map.m_eTileOption;
	m_iHeight = map.m_iHeight;

	m_vecTile.clear();

	size_t iSz = map.m_vecTile.size();

	for (size_t i = 0; i < iSz; ++i)
	{
		std::shared_ptr<CComponent> pTile = std::shared_ptr<CComponent>(new CTile);

		pTile = map.m_vecTile[i]->Clone();

		m_vecTile.push_back(pTile);
	}

	m_pTile = map.m_pTile->Clone();
}

CTileMap::~CTileMap()
{
}

void CTileMap::SetTexture(const std::string& strKey)
{
	m_pTexture = GET_SINGLE(CResourceManager)->FindTexture(strKey);
}

void CTileMap::SetTexture(std::shared_ptr<class CTexture> pTex)
{
	m_pTexture = pTex;
}

void CTileMap::SetTileTexture(IMAGE_TYPE eType, const std::string& strKey)
{
	size_t iSz = m_vecTile.size();

	switch (eType)
	{
	case IMAGE_TYPE::IT_ATLAS:
		for (size_t i = 0; i < iSz; ++i)
		{
			((CTile*)m_vecTile[i].get())->SetTexture(LINK_TYPE::LT_DIF, strKey, (int)SHADER_CBUFFER_TYPE::CBUFFER_PIXEL, 0);
			((CTile*)m_vecTile[i].get())->SetImageType(eType);
		}
		break;
	case IMAGE_TYPE::IT_ARRAY:
		for (size_t i = 0; i < iSz; ++i)
		{
			((CTile*)m_vecTile[i].get())->SetTexture(LINK_TYPE::LT_CUSTOM, strKey, (int)SHADER_CBUFFER_TYPE::CBUFFER_PIXEL, 9);
			((CTile*)m_vecTile[i].get())->SetImageType(eType);
		}
		break;
	}
}

void CTileMap::SetTileTexture(IMAGE_TYPE eType, std::shared_ptr<CTexture> pTex)
{
	size_t iSz = m_vecTile.size();

	switch (eType)
	{
	case IMAGE_TYPE::IT_ATLAS:
		for (size_t i = 0; i < iSz; ++i)
		{
			((CTile*)m_vecTile[i].get())->SetTexture(LINK_TYPE::LT_DIF, pTex, (int)SHADER_CBUFFER_TYPE::CBUFFER_PIXEL, 0);
			((CTile*)m_vecTile[i].get())->SetImageType(eType);
		}
			
		break;
	case IMAGE_TYPE::IT_ARRAY:
		for (size_t i = 0; i < iSz; ++i)
		{
			((CTile*)m_vecTile[i].get())->SetTexture(LINK_TYPE::LT_DIF, pTex, (int)SHADER_CBUFFER_TYPE::CBUFFER_PIXEL, 9);
			((CTile*)m_vecTile[i].get())->SetImageType(eType);
		}
		break;
	}
}

void CTileMap::SetEdit(bool bEdit)
{
	m_bEdit = bEdit;
}

void CTileMap::SetOption(TILE_OPTION eOp)
{
	m_eTileOption = eOp;
}

void CTileMap::SetTileIdx(int idx)
{
	m_iIndex = idx;
}

void CTileMap::SetTileHeight(int iHeight)
{
	m_iHeight = iHeight;
}

void CTileMap::SetTileSize(const Vector2& vSz)
{
	m_vTileSize = Vector3(vSz.x, vSz.y, 0.f);
}

void CTileMap::SetArrayTex(bool bArray)
{
	m_bArrayTex = bArray;
}

void CTileMap::SetTileType(TILE_TYPE eType)
{
	m_eTileType = eType;
}

TILE_OPTION CTileMap::GetTileOption() const
{
	return m_eTileOption;
}

int CTileMap::GetCountX() const
{
	return m_iCountX;
}

int CTileMap::GetCountY() const
{
	return m_iCountY;
}

const Vector3& CTileMap::GetTileSize() const
{
	return m_vTileSize;
}

bool CTileMap::CreateTile(TILE_TYPE eType, int iCntX, int iCntY, const Vector3& vStart, const Vector2& vSize, TILE_OPTION eOp, int idx)
{
	m_vecTile.clear();

	m_iCountX = iCntX;
	m_iCountY = iCntY;

	m_iSize = iCntX * iCntY;

	m_eTileType = eType;

	m_vTileSize = Vector3(vSize.x, vSize.y, 1.f);

	SetWorldPos(vStart);

	class CMesh* pMesh = GET_SINGLE(CResourceManager)->GetDefaultMesh();

	std::shared_ptr<CMaterial> pMtrl = GET_SINGLE(CResourceManager)->FindMaterial("Color");

	switch (eType)
	{
	case TILE_TYPE::RECT:
		for (int j = 0; j < m_iCountY; ++j)
		{
			for (int i = 0; i < m_iCountX; ++i)
			{
				std::shared_ptr<CComponent> pTile = std::shared_ptr<CComponent>(new CTile);

				Vector2 vPos = Vector2(i * m_vTileSize.x, j * m_vTileSize.y);

				((CTile*)pTile.get())->SetWorldPos(Vector3(vPos.x, vPos.y, 0.f));
				((CTile*)pTile.get())->SetWorldScale(vSize.x, vSize.y, 1.f);
				((CTile*)pTile.get())->SetMesh(pMesh);

				((CTile*)pTile.get())->SetTileOption(eOp);
				((CTile*)pTile.get())->SetTileTextureIdx(idx);

				((CTile*)pTile.get())->m_iX = i;
				((CTile*)pTile.get())->m_iY = j;
				((CTile*)pTile.get())->m_iIdx = j * m_iCountX + i;

				((CTile*)pTile.get())->CSceneComponent::PostUpdate(0.f);
				((CTile*)pTile.get())->m_pLayer = m_pLayer;

				m_vecTile.push_back(pTile);
			}
		}
		break;
	case TILE_TYPE::ISOMETRIC:

		for (int j = 0; j < m_iCountY; ++j)
		{
			for (int i = 0; i < m_iCountX; ++i)
			{
				//CGameObject* pObj = m_pScene->CreateObject<CGameObject>("Tile", m_pLayer);

				//float fX = 0.f;//(i % 2) * vSize.x / 2.f;
				//pObj->SetWorldPos(i * vSize.x + fX, j * vSize.y / 2.f, 0.f);
				//pObj->SetWorldScale(vSize.x, vSize.y, 1.f);

				std::shared_ptr<CComponent> pTile = std::shared_ptr<CComponent>(new CTile);//pObj->CreateComponent<CTile>("Tile");
				Vector2 vU = Vector2(vSize.x /2.f, -vSize.y / 2.f);
				Vector2 vV = Vector2(vSize.x / 2.f, vSize.y / 2.f);

				//Vector2 vPos = (float)i * (vU * vSize + Vector2(4.f, -0.5f)) + (float)j * (vV * vSize + Vector2(4.f, 0.5f));
				Vector2 vPos = (float)i * vU + (float)j * vV;
				//vPos.x += i * 7.f;
				((CTile*)pTile.get())->SetWorldPos(Vector3(vPos.x, vPos.y, 0.f));
				((CTile*)pTile.get())->SetWorldScale(vSize.x, vSize.y, 1.f);
				((CTile*)pTile.get())->SetMesh(pMesh);

				((CTile*)pTile.get())->m_iX = i;
				((CTile*)pTile.get())->m_iY = j;
				((CTile*)pTile.get())->m_iIdx = j * m_iCountX + i;

				((CTile*)pTile.get())->SetTileOption(eOp);
				((CTile*)pTile.get())->SetTileTextureIdx(idx);

				((CTile*)pTile.get())->CSceneComponent::PostUpdate(0.f);
				((CTile*)pTile.get())->m_pLayer = m_pLayer;
				//pObj->SetRootComponent(pTile);

				m_vecTile.push_back(pTile);

				//pObj->SetEnable(false);
			}
		}

		m_pTile = std::shared_ptr<CComponent>(new CTile);
		((CTile*)m_pTile.get())->SetWorldScale(vSize.x, vSize.y, 1.f);
		((CTile*)m_pTile.get())->SetMesh(pMesh);
		((CTile*)m_pTile.get())->m_pLayer = m_pLayer;
		((CTile*)m_pTile.get())->SetTileTextureIdx(26);
		break;
	}

	SAFE_RELEASE(pMesh);

	return true;
}

int CTileMap::GetIndex(const Vector2& vPos)
{
	switch (m_eTileType)
	{
	case TILE_TYPE::RECT:
	{
		Vector2 _vPos = vPos;

		Vector3 vWorldPos = GetWorldPos();

		_vPos -= Vector2(vWorldPos.x, vWorldPos.y);

		_vPos /= Vector2(m_vTileSize.x, m_vTileSize.y);

		if (_vPos.x < 0.f || _vPos.y < 0.f ||
			(int)_vPos.x >= m_iCountX || _vPos.y >= m_iCountY)
			return -1;

		return (int)_vPos.y * m_iCountX + (int)_vPos.x;
	}
		break;
	case TILE_TYPE::ISOMETRIC:
	{
		Vector2 _vPos = vPos;

		Vector3 vWorldPos = GetWorldPos();

		_vPos -= Vector2(vWorldPos.x, vWorldPos.y + m_vTileSize.y /2.f);

		int iA = (int)(_vPos.x / m_vTileSize.x- _vPos.y / m_vTileSize.y);
		int iB = (int)(_vPos.x / m_vTileSize.x+ _vPos.y / m_vTileSize.y);

		if (iA < 0 || iA >= m_iCountX ||
			iB < 0 || iB >= m_iCountY)
			return -1;

		return iB * m_iCountX + iA;
	}
		break;
	}

	return -1;
}

int CTileMap::GetIndex(int x, int y)
{
	return y * m_iCountX + x;
}

TILE_TYPE CTileMap::GetTileType() const
{
	return m_eTileType;
}

bool CTileMap::IsArrayTex() const
{
	return m_bArrayTex;
}

CTile* CTileMap::GetTile(const Vector3& vPos)
{
	int idx = GetIndex(Vector2(vPos.x, vPos.y));

	return GetTile(idx);
}

CTile* CTileMap::GetTile(int idx)
{
	if (idx < 0)
		return nullptr;

	else if (idx >= m_vecTile.size())
		return nullptr;

	return (CTile*)m_vecTile[idx].get();
}

CTile* CTileMap::GetTile(int x, int y)
{
	int idx = GetIndex(x, y);

	return GetTile(idx);
}

bool CTileMap::Init()
{
	if (!CSceneComponent::Init())
		return false;

	return true;
}

void CTileMap::Start()
{
	CSceneComponent::Start();
}

void CTileMap::Update(float fTime)
{
	CSceneComponent::Update(fTime);

	Vector2 vRatio = RATIO;

	int idx = GetIndex(GET_SINGLE(CInput)->GetMouseWorldPos() / vRatio);

	bool bShow = GET_SINGLE(CInput)->GetShow();

	if (m_bEdit)
	{
		if (GET_SINGLE(CInput)->IsDown(MOUSE_TYPE::LEFT))
		{
			if (idx > -1 && idx < m_iCountX * m_iCountY && !bShow)
			{
				((CTile*)m_vecTile[idx].get())->SetTileTextureIdx(m_iIndex);
				((CTile*)m_vecTile[idx].get())->SetTileOption(m_eTileOption);
				((CTile*)m_vecTile[idx].get())->SetTileHeight(m_iHeight);
			}
		}
	}

	if (idx > -1 && idx < m_iCountX * m_iCountY && !bShow)
	{
		Vector3 vPos = ((CSceneComponent*)m_vecTile[idx].get())->GetWorldPos();
		((CTile*)m_pTile.get())->SetWorldPos(vPos);
		((CTile*)m_pTile.get())->CSceneComponent::PostUpdate(fTime);
		((CTile*)m_pTile.get())->SetEnable(true);
	}

	else
	{
		((CTile*)m_pTile.get())->SetEnable(false);
	}
}

void CTileMap::PostUpdate(float fTime)
{
	CSceneComponent::PostUpdate(fTime);

	size_t iSz = m_vecTile.size();

	for (size_t i = 0; i < iSz; ++i)
		((CTile*)m_vecTile[i].get())->PostUpdate(fTime);
}

void CTileMap::Collision(float fTime)
{
	CSceneComponent::Collision(fTime);
}

void CTileMap::PreRender(float fTime)
{
	CSceneComponent::PreRender(fTime);

	CCamera* pCam = GET_SINGLE(CCameraManager)->GetMainCam();

	Vector3 vCamPos = pCam->GetWorldPos();

	Resolution tRS = RESOLUTION;

	Vector3 vLB = vCamPos -Vector3((float)tRS.iWidth, (float)tRS.iHeight, 0.f) / 2.f;

	Vector3 vRT = vLB + Vector3((float)tRS.iWidth, (float)tRS.iHeight, 0.f);

	Vector3 vPos = GetWorldPos();

	vLB -= vPos + m_vTileSize;
	vRT -= vPos - m_vTileSize;

/*

	int iStartX = (int)(vLB.x / m_vTileSize.x);
	int iStartY = (int)(vLB.y / (m_vTileSize.y / 2.f));

	int iEndX = (int)(vRT.x / m_vTileSize.x);
	int iEndY = (int)(vRT.y /( m_vTileSize.y / 2.f));

	if (iStartX < 0)
		iStartX = 0;

	if (iStartY < 0)
		iStartY = 0;

	if (iEndX >= m_iCountX)
		iEndX = m_iCountX - 1;

	if (iEndY >= m_iCountY)
		iEndY = m_iCountY - 1;*/

	for (int y = 0; y < m_iCountY; ++y)
	{
		for (int x = 0; x < m_iCountX; ++x)
		{
			int idx = y * m_iCountX + x;

			Vector3 vPos = ((CTile*)m_vecTile[idx].get())->GetWorldPos();

			if (vPos.x < vLB.x)
				continue;

			if (vPos.x > vRT.x)
				continue;

			if (vPos.y < vLB.y)
				continue;

			if (vPos.y > vRT.y)
				continue;

			GET_SINGLE(CRenderManager)->AddTileInstancing((CTile*)m_vecTile[idx].get());
		}
	}

	if (m_pTile.get()->IsEnable())
	{
		GET_SINGLE(CRenderManager)->AddTileInstancing((CTile*)m_pTile.get());
	}
}

void CTileMap::Render(float fTime)
{
	CSceneComponent::Render(fTime);
}


void CTileMap::PostRender(float fTime)
{
	CSceneComponent::PostRender(fTime);
}

std::shared_ptr<CComponent> CTileMap::Clone()
{
	return std::shared_ptr<CComponent>(new CTileMap(*this));
}

void CTileMap::Save(FILE* pFile)
{
	CSceneComponent::Save(pFile);

	fwrite(&m_iCountX, 4, 1, pFile);
	fwrite(&m_iCountY, 4, 1, pFile);
	fwrite(&m_iSize, 4, 1, pFile);
	fwrite(&m_vTileSize, sizeof(Vector3), 1, pFile);
	fwrite(&m_eTileType, 4, 1, pFile);
	fwrite(&m_iIndex, 4, 1, pFile);
	fwrite(&m_bEdit, 4, 1, pFile);
	fwrite(&m_eTileOption, 4, 1, pFile);
	fwrite(&m_iHeight, 4, 1, pFile);

	for (int i = 0; i < m_iSize; ++i)
	{
		m_vecTile[i]->Save(pFile);
	}

	bool bTex = false;

	if (m_pTexture)
	{
		bTex = true;
	}

	fwrite(&bTex, 1, 1, pFile);

	if (m_pTexture)
	{
		m_pTexture->Save(pFile);
	}
}

void CTileMap::Load(FILE* pFile)
{
	CSceneComponent::Load(pFile);

	fread(&m_iCountX, 4, 1, pFile);
	fread(&m_iCountY, 4, 1, pFile);
	fread(&m_iSize, 4, 1, pFile);
	fread(&m_vTileSize, sizeof(Vector3), 1, pFile);
	fread(&m_eTileType, 4, 1, pFile);
	fread(&m_iIndex, 4, 1, pFile);
	fread(&m_bEdit, 4, 1, pFile);
	fread(&m_eTileOption, 4, 1, pFile);
	fread(&m_iHeight, 4, 1, pFile);

	m_vecTile.clear();

	for (int i = 0; i < m_iSize; ++i)
	{
		std::shared_ptr<CComponent> pTile = std::shared_ptr<CComponent>(new CTile);

		pTile->Load(pFile);

		m_vecTile.push_back(pTile);
	}

	bool bTex = false;

	fread(&bTex, 1, 1, pFile);

	if (bTex)
	{
		m_pTexture = GET_SINGLE(CResourceManager)->LoadTexture(pFile);
	}
}