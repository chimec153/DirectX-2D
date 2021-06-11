#pragma once

#include "PrimitiveComponent.h"

class CTileMap :
	public CSceneComponent
{
	friend class CGameObject;

public:
	CTileMap();
	CTileMap(const CTileMap& map);
	virtual ~CTileMap();

private:
	int								m_iCountX;
	int								m_iCountY;
	int								m_iSize;
	Vector3							m_vTileSize;
	std::vector<std::shared_ptr<class CComponent>>	m_vecTile;
	TILE_TYPE						m_eTileType;
	std::shared_ptr<class CTexture>	m_pTexture;
	int								m_iIndex;
	bool							m_bEdit;
	TILE_OPTION						m_eTileOption;
	int								m_iHeight;
	bool							m_bArrayTex;
	std::shared_ptr<class CComponent>	m_pTile;

public:
	void SetTexture(const std::string& strKey);
	void SetTexture(std::shared_ptr<class CTexture> pTex);
	void SetTileTexture(IMAGE_TYPE eType, const std::string& strKey);
	void SetTileTexture(IMAGE_TYPE eType, std::shared_ptr<CTexture> pTex);
	void SetEdit(bool bEdit);
	void SetOption(TILE_OPTION eOp);
	void SetTileIdx(int idx);
	void SetTileHeight(int iHeight);
	void SetTileSize(const Vector2& vSz);
	void SetArrayTex(bool bArray);
	void SetTileType(TILE_TYPE eType);
	TILE_OPTION GetTileOption()	const;
	int GetCountX()	const;
	int GetCountY()	const;
	const Vector3& GetTileSize()	const;

public:
	bool CreateTile(TILE_TYPE eType, int iCntX, int iCntY, const Vector3& vStart, const Vector2& vSize, TILE_OPTION eOp, int idx);
	int GetIndex(const Vector2& vPos);
	int GetIndex(int x, int y);
	TILE_TYPE GetTileType()	const;
	bool IsArrayTex()	const;
	class CTile* GetTile(const Vector3& vPos);
	class CTile* GetTile(int idx);
	class CTile* GetTile(int x, int y);

public:
	virtual bool Init();
	virtual void Start();
	virtual void Update(float fTime);
	virtual void PostUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void PreRender(float fTime);
	virtual void Render(float fTime);
	virtual void PostRender(float fTime);
	virtual std::shared_ptr<CComponent> Clone();

public:
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);
};

