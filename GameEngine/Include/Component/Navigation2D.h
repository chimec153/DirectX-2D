#pragma once
#include "Navigation.h"

enum class TILE_DIR
{
	LT = -4,
	T,
	RT,
	L,
	C,
	R,
	LB,
	B,
	RB,
	END
};

class CNavigation2D :
	public CNavigation
{
	friend class CGameObject;

public:
	CNavigation2D();
	CNavigation2D(const CNavigation2D& nav);
	virtual ~CNavigation2D();

private:
	static bool compare(const class CTile* tSrc, const class CTile* tDest);

private:
	std::shared_ptr<class CComponent> m_pTileMap;
	std::priority_queue<class CTile*, std::vector<class CTile*>, decltype(&compare) > m_queTile;
	std::vector<class CTile*>	m_vecClose;
	std::stack<class CTile*>	m_stTile;

public:
	virtual bool FindPath(const Vector3& vStart, const Vector3& vEnd);
	void SetTileMap(const std::shared_ptr<CComponent>& pMap);
	const std::stack<class CTile*>& GetStack()	const;
	void Clear();

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

