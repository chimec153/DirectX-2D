#include "Tile.h"
#include "Layer/Layer.h"
#include "LayerInstance.h"
#include "Resource/ShaderManager.h"
#include "Resource/ResourceManager.h"
#include "Resource/Mesh2D.h"
#include "Resource/Material.h"
#include "Resource/Texture.h"
#include "RenderManager.h"
#include "Resource/ShaderManager.h"

CTile::CTile()	:
	m_eTileType(TILE_TYPE::ISOMETRIC),
	m_eOption(TILE_OPTION::NONE),
	m_tCBuffer(),
	m_iTileIdx(0),
	m_iHeight(0),
	m_pParentTile(),
	m_fDist(0.f),
	m_fPathDist(0.f),
	m_iX(0),
	m_iY(0),
	m_iIdx(0),
	m_bClose(false),
	m_bOpen(false)
{
	m_iSceneComponentClassType = (int)SCENECOMPONENT_CLASS_TYPE::TILE;
	m_eSceneComponentType = SCENE_COMPONENT_TYPE::SCT_2D;
}

CTile::CTile(const CTile& tile)	:
	CSceneComponent(tile)
{
	m_eTileType = tile.m_eTileType;
	m_eOption = tile.m_eOption;
	m_tCBuffer = tile.m_tCBuffer;

	memcpy(&m_tCBuffer, &tile.m_tCBuffer, sizeof(AniCBuffer));

	m_iTileIdx = tile.m_iTileIdx;

	m_iHeight = tile.m_iHeight;

	m_pParentTile = nullptr;

	m_fDist = 0.f;
	m_fPathDist = 0.f;

	m_iX = tile.m_iX;
	m_iY = tile.m_iY;
	m_iIdx = tile.m_iIdx;
	m_bClose = false;
	m_bOpen = false;
}

CTile::~CTile()
{
}

TILE_OPTION CTile::GetTileOption() const
{
	return m_eOption;
}

int CTile::GetTileIndex() const
{
	return m_iTileIdx;
}

void CTile::SetImageType(IMAGE_TYPE eType)
{
	m_tCBuffer.eImageType = (int)eType;
}

void CTile::SetTileTextureIdx(int idx)
{
	m_iTileIdx = idx;
}

void CTile::SetTileOption(TILE_OPTION eOp)
{
	m_eOption = eOp;

	if (m_eOption == TILE_OPTION::NOMOVE)
	{
		
		
		GetMaterial()->SetDiffuseColor(1.f, 0.f, 0.f, 1.f);
	}

	else if(m_eOption == TILE_OPTION::WATER)
	{
			GetMaterial()->SetDiffuseColor(0.f, 0.f, 1.f, 1.f);
	}

	else
	{
			GetMaterial()->SetDiffuseColor(1.f, 1.f, 1.f, 1.f);
	}
}

void CTile::SetTileHeight(int iHeight)
{
	m_iHeight = iHeight;
}

void CTile::SetTileParent(CTile* pParent)
{
	m_pParentTile = pParent;
}

void CTile::SetDist(float fDist)
{
	m_fDist = fDist;
}

float CTile::GetDist() const
{
	return m_fDist;
}

void CTile::SetPathDist(float fDist)
{
	m_fPathDist = fDist;
}

float CTile::GetPathDist() const
{
	return m_fPathDist;
}

int CTile::GetIndexX() const
{
	return m_iX;
}

int CTile::GetIndexY() const
{
	return m_iY;
}

int CTile::GetIndex() const
{
	return m_iIdx;
}

bool CTile::IsClosed() const
{
	return m_bClose;
}

void CTile::SetClose(bool bClose)
{
	m_bClose = bClose;
}

CTile* CTile::GetTileParent() const
{
	return m_pParentTile;
}

void CTile::SetOpen(bool bOpen)
{
	m_bOpen = bOpen;
}

bool CTile::IsOpened() const
{
	return m_bOpen;
}

bool CTile::Init()
{
	if (!CSceneComponent::Init())
		return false;

	return true;
}

void CTile::Start()
{
}

void CTile::Update(float fTime)
{
}

void CTile::PostUpdate(float fTime)
{
}

void CTile::Collision(float fTime)
{
}

void CTile::PreRender(float fTime)
{
}

void CTile::Render(float fTime)
{
	GET_SINGLE(CShaderManager)->UpdateCBuffer("Sprite", &m_tCBuffer);

	CSceneComponent::Render(fTime);

	//m_pMesh->Render(fTime);
}

void CTile::PostRender(float fTime)
{
}

std::shared_ptr<CComponent> CTile::Clone()
{
	return std::shared_ptr<CComponent>(new CTile(*this));
}

void CTile::Save(FILE* pFile)
{
	CSceneComponent::Save(pFile);

	fwrite(&m_eTileType, 4, 1, pFile);
	fwrite(&m_eOption, 4, 1, pFile);
	fwrite(&m_tCBuffer, sizeof(m_tCBuffer), 1, pFile);
	fwrite(&m_iTileIdx, 4, 1, pFile);
	fwrite(&m_iHeight, 4, 1, pFile);
}

void CTile::Load(FILE* pFile)
{
	CSceneComponent::Load(pFile);

	fread(&m_eTileType, 4, 1, pFile);
	fread(&m_eOption, 4, 1, pFile);
	fread(&m_tCBuffer, sizeof(m_tCBuffer), 1, pFile);
	fread(&m_iTileIdx, 4, 1, pFile);
	fread(&m_iHeight, 4, 1, pFile);

	CSceneComponent::PostUpdate(0.f);
}
