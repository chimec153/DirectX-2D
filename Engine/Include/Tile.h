#pragma once
#include "Component/SceneComponent.h"
class CTile :
	public CSceneComponent
{
	friend class CTileMap;
	friend class CGameObject;

public:
	CTile();
	CTile(const CTile& tile);
	virtual ~CTile();

private:
	TILE_TYPE	m_eTileType;
	TILE_OPTION	m_eOption;
	AniCBuffer	m_tCBuffer;
	int			m_iTileIdx;
	int			m_iHeight;
	CTile*		m_pParentTile;
	float		m_fDist;
	float		m_fPathDist;
	int			m_iX;
	int			m_iY;
	int			m_iIdx;
	bool		m_bClose;
	bool		m_bOpen;

public:
	TILE_OPTION GetTileOption()	const;
	int GetTileIndex()	const;
	void SetImageType(IMAGE_TYPE eType);
	void SetTileTextureIdx(int idx);
	void SetTileOption(TILE_OPTION eOp);
	void SetTileHeight(int iHeight);
	void SetTileParent(CTile* pParent);
	void SetDist(float fDist);
	float GetDist()	const;
	void SetPathDist(float fDist);
	float GetPathDist()	const;
	int GetIndexX()	const;
	int GetIndexY()	const;
	int GetIndex()	const;
	bool IsClosed()	const;
	void SetClose(bool bClose);
	CTile* GetTileParent()	const;
	void SetOpen(bool bOpen);
	bool IsOpened()	const;

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

