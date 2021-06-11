#include "Navigation2D.h"
#include "TileMap.h"
#include "../Tile.h"

CNavigation2D::CNavigation2D()	:
	m_queTile(compare)
{
	m_eType = COMPONENT_TYPE::CT_OBJECT;
	SetClassType(OBJ_COMPONENT_CLASS_TYPE::NAVIGATION2D);
}

CNavigation2D::CNavigation2D(const CNavigation2D& nav)	:
	CNavigation(nav)
{
	m_pTileMap = nav.m_pTileMap;
}

CNavigation2D::~CNavigation2D()
{
}

bool CNavigation2D::compare(const CTile* tSrc, const CTile* tDest)
{
	return tSrc->GetDist() + tSrc->GetPathDist() > tDest->GetDist() + tDest->GetPathDist();
}

bool CNavigation2D::FindPath(const Vector3& vStart, const Vector3& vEnd)
{
	if (!m_pTileMap)
		return false;	

	Clear();

	CTile* pStartTile = ((CTileMap*)m_pTileMap.get())->GetTile(vStart);
	CTile* pEndTile = ((CTileMap*)m_pTileMap.get())->GetTile(vEnd);

	if (!pStartTile || !pEndTile)
		return false;

	m_queTile.push(pStartTile);
	//	간선 비용 + 휴리스틱 값이 가장 작은 타일을 열린 목록에서 꺼내서 닫힌 목록에 추가한다.

	while (true)
	{
		if (m_queTile.empty())
			break;

		CTile* _pTile = m_queTile.top();

		if (!_pTile)
			break;

		m_queTile.pop();

		_pTile->SetClose(true);

		m_vecClose.push_back(_pTile);

		int iX = _pTile->GetIndexX();
		int iY = _pTile->GetIndexY();

		for (int y = -1; y < 2; ++y)
		{
			for (int x = -1; x < 2; ++x)
			{
				int iCurX = iX + x;
				int iCurY = iY + y;

				CTile* pTile = ((CTileMap*)m_pTileMap.get())->GetTile(iCurX, iCurY);
				CTile* pAdjTile1 = nullptr;
				CTile* pAdjTile2 = nullptr;

				switch (TILE_DIR(y * 3 + x))
				{
				case TILE_DIR::LT:
					pAdjTile1 = ((CTileMap*)m_pTileMap.get())->GetTile(iCurX, iCurY + 1);
					pAdjTile2 = ((CTileMap*)m_pTileMap.get())->GetTile(iCurX + 1, iCurY);
					break;
				case TILE_DIR::RT:
					pAdjTile1 = ((CTileMap*)m_pTileMap.get())->GetTile(iCurX, iCurY + 1);
					pAdjTile2 = ((CTileMap*)m_pTileMap.get())->GetTile(iCurX - 1, iCurY);
					break;
				case TILE_DIR::LB:
					pAdjTile1 = ((CTileMap*)m_pTileMap.get())->GetTile(iCurX, iCurY - 1);
					pAdjTile2 = ((CTileMap*)m_pTileMap.get())->GetTile(iCurX + 1, iCurY);
					break;
				case TILE_DIR::RB:
					pAdjTile1 = ((CTileMap*)m_pTileMap.get())->GetTile(iCurX, iCurY - 1);
					pAdjTile2 = ((CTileMap*)m_pTileMap.get())->GetTile(iCurX - 1, iCurY);
					break;
				}

				if (!pTile)
					continue;

				bool bClose = pTile->IsClosed();

				if (!bClose)
				{
					TILE_OPTION eOp = pTile->GetTileOption();

					switch (TILE_DIR(y * 3 + x))
					{
					case TILE_DIR::LT:
					case TILE_DIR::RT:
					case TILE_DIR::LB:
					case TILE_DIR::RB:
					{
						if (pAdjTile1 && pAdjTile2)
						{
							if (pAdjTile1->GetTileOption() == TILE_OPTION::NOMOVE ||
								pAdjTile2->GetTileOption() == TILE_OPTION::NOMOVE)
								continue;
						}
						else
						{
							continue;
						}
					}
						break;
					}

					if (eOp != TILE_OPTION::NOMOVE)
					{
						CTile* pParent = pTile->GetTileParent();

						if (pParent)
						{
							float fPath = pTile->GetPathDist();

							float _fPath = _pTile->GetPathDist();

							float fNew = _fPath + 1.f + 0.4f * ((x + y + 3) % 2);

							if (fPath > fNew)	//	간선 비용이 이전보다 더 적을 경우 현재 닫힌 목록에 추가된 타일을 부모 타일로 추가한다.
							{
								pTile->SetPathDist(fNew);
								pTile->SetTileParent(_pTile);
							}
						}

						else
						{
							float _fPath = _pTile->GetPathDist();

							float fNew = _fPath + 1.f + 0.4f * ((x + y + 3) % 2);

							pTile->SetPathDist(fNew);
							pTile->SetTileParent(_pTile);

							int iEndX = pEndTile->GetIndexX();
							int iEndY = pEndTile->GetIndexY();

							int dx = iCurX > iEndX ? iCurX - iEndX : iEndX - iCurX;
							int dy = iCurY > iEndY ? iCurY - iEndY : iEndY - iCurY;

							pTile->SetDist(dx > dy ? dx + dy * 0.4f : dy + dx * 0.4f);

							m_queTile.push(pTile);
						}

						if (pTile == pEndTile)
						{
							pTile->SetTileTextureIdx(10);

							m_stTile.push(pTile);

							CTile* pParent = pTile->GetTileParent();

							while (true)
							{
								if (!pParent)
									break;

								pParent->SetTileTextureIdx(8);

								m_stTile.push(pParent);

								pParent = pParent->GetTileParent();
							}

							return true;
						}
					}
				}
			}
		}
	}

	Clear();

	return false;
}

void CNavigation2D::SetTileMap(const std::shared_ptr<CComponent>& pMap)
{
	m_pTileMap = pMap;
}

const std::stack<class CTile*>& CNavigation2D::GetStack() const
{
	return m_stTile;
}

void CNavigation2D::Clear()
{
	while (!m_queTile.empty())
	{
		CTile* pTile = m_queTile.top();

		pTile->SetClose(false);
		pTile->SetOpen(false);
		pTile->SetTileParent(nullptr);
		pTile->SetDist(0.f);
		pTile->SetPathDist(0.f);

		pTile->SetTileTextureIdx(0);

		m_queTile.pop();
	}

	size_t iSz = m_vecClose.size();

	for (size_t i = 0; i < iSz; ++i)
	{
		m_vecClose[i]->SetClose(false);
		m_vecClose[i]->SetOpen(false);
		m_vecClose[i]->SetTileParent(nullptr);
		m_vecClose[i]->SetDist(0.f);
		m_vecClose[i]->SetPathDist(0.f);

		m_vecClose[i]->SetTileTextureIdx(0);
	}

	m_vecClose.clear();	

	while (!m_stTile.empty())
	{
		CTile* pTile = m_stTile.top();

		pTile->SetClose(false);
		pTile->SetOpen(false);
		pTile->SetTileParent(nullptr);
		pTile->SetDist(0.f);
		pTile->SetPathDist(0.f);

		pTile->SetTileTextureIdx(0);

		m_stTile.pop();
	}
}

bool CNavigation2D::Init()
{
	if (!CNavigation::Init())
		return false;

	return true;
}

void CNavigation2D::Start()
{
	CNavigation::Start();
}

void CNavigation2D::Update(float fTime)
{
	CNavigation::Update(fTime);
}

void CNavigation2D::PostUpdate(float fTime)
{
	CNavigation::PostUpdate(fTime);
}

void CNavigation2D::Collision(float fTime)
{
	CNavigation::Collision(fTime);
}

void CNavigation2D::PreRender(float fTime)
{
	CNavigation::PreRender(fTime);
}

void CNavigation2D::Render(float fTime)
{
	CNavigation::Render(fTime);
}

void CNavigation2D::PostRender(float fTime)
{
	CNavigation::PostRender(fTime);
}

std::shared_ptr<CComponent> CNavigation2D::Clone()
{
	return std::shared_ptr<CComponent>(new CNavigation2D(*this));
}

void CNavigation2D::Save(FILE* pFile)
{
	CNavigation::Save(pFile);
}

void CNavigation2D::Load(FILE* pFile)
{
	CNavigation::Load(pFile);
}
