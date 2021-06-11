#include "CollisionSection.h"
#include "../Component/Collider.h"
#include "../Input.h"
#include "../UI/MouseObj.h"
#include "../UI/UIControl.h"

CCollisionSection::CCollisionSection()
{


}

CCollisionSection::~CCollisionSection()
{
}

bool CCollisionSection::Init(int x, int y, int z, int idx, Vector3 vSize)
{
	m_iX = x;
	m_iY = y;
	m_iZ = z;
	m_iIdx = idx;
	m_vSize = vSize;

	return true;
}

void CCollisionSection::SetCenter(const Vector3& vCenter)
{
	m_vMin.x = vCenter.x - m_vSize.x / 2.f;
	m_vMax.x = m_vMin.x + m_vSize.x;

	m_vMin.y = vCenter.y - m_vSize.y / 2.f;
	m_vMax.y = m_vMin.y + m_vSize.y;

	m_vMin.z = vCenter.z - m_vSize.z / 2.f;
	m_vMax.z = m_vMin.z + m_vSize.z;
}

void CCollisionSection::AddCollider(CCollider* pCol)
{
	pCol->AddSection(m_iIdx);
	m_ColliderList.push_back(pCol);
}

bool CCollisionSection::MouseCollision(float fTime)
{
	Vector2 vPos = GET_SINGLE(CInput)->GetMousePos();

	std::shared_ptr<CComponent> pPC = GET_SINGLE(CInput)->GetMouse()->GetCol();

	std::list<CCollider*>::iterator iter = m_ColliderList.begin();
	std::list<CCollider*>::iterator iterEnd = m_ColliderList.end();

	bool bCol = false;

	for (; iter != iterEnd; ++iter)
	{
		if ((*iter)->CollisionMouse(vPos) && !bCol)
		{
			bCol = true;

			if (!(*iter)->HasPrevCol((CCollider*)pPC.get()))
			{
				(*iter)->AddPrevCol((CCollider*)pPC.get());
				((CCollider*)pPC.get())->AddPrevCol((*iter));

				(*iter)->CallMouse(vPos, fTime, COLLISION_STATE::CS_INIT);
				((CCollider*)pPC.get())->CallMouse(vPos, fTime, COLLISION_STATE::CS_INIT);
			}

			else
			{
				(*iter)->CallMouse(vPos, fTime, COLLISION_STATE::CS_STAY);
				((CCollider*)pPC.get())->CallMouse(vPos, fTime, COLLISION_STATE::CS_STAY);
			}
		}

		else
		{
			if ((*iter)->HasPrevCol((CCollider*)pPC.get()))
			{
				(*iter)->DeletePrevCol((CCollider*)pPC.get());
				((CCollider*)pPC.get())->DeletePrevCol((*iter));

				(*iter)->CallMouse(vPos, fTime, COLLISION_STATE::CS_LAST);
				((CCollider*)pPC.get())->CallMouse(vPos, fTime, COLLISION_STATE::CS_LAST);
			}
		}
	}

	return bCol;
}

void CCollisionSection::Collision(float fTime)
{
	std::list<CCollider*>::iterator iterPrev = m_ColliderList.begin();
	std::list<CCollider*>::iterator iterPrevEnd = m_ColliderList.end();

	for (; iterPrev != iterPrevEnd; ++iterPrev)
	{
		if ((*iterPrev)->IsCheck())
			continue;

		(*iterPrev)->Check();
		(*iterPrev)->PrevColSecCheck(fTime);
	}

	if (m_ColliderList.size() < 2)
		return;

	m_ColliderList.sort(&CCollisionSection::SortZ);

	std::list<CCollider*>::iterator iter = m_ColliderList.begin();
	std::list<CCollider*>::iterator iterEnd = m_ColliderList.end();

	--iterEnd;

	for (; iter != iterEnd; ++iter)
	{
		std::list<CCollider*>::iterator iter1 = iter;
		std::list<CCollider*>::iterator iter1End = m_ColliderList.end();

		++iter1;

		for (; iter1 != iter1End; ++iter1)
		{
			if ((*iter)->HasCurCol((*iter1)))
				continue;

			PCollisionProfile pSrcProfile = (*iter)->GetProFile();
			PCollisionProfile pDestProfile = (*iter1)->GetProFile();

			bool bIgn1 = pSrcProfile->vecChannel[(int)pDestProfile->eChannel].eType == COLLISION_CHANNEL_TYPE::CCT_COL;
			bool bIgn2 = pDestProfile->vecChannel[(int)pSrcProfile->eChannel].eType == COLLISION_CHANNEL_TYPE::CCT_COL;

			if ((*iter)->IsColEnable() && 
				(*iter1)->IsColEnable() &&
				(*iter)->Collision((*iter1)))
			{
				(*iter1)->AddCurCol((*iter));
				(*iter)->AddCurCol((*iter1));

				if (!(*iter)->HasPrevCol((*iter1)))
				{
					if (bIgn1)
					{
						(*iter)->Call((*iter1), fTime, COLLISION_STATE::CS_INIT);
					}

					if (bIgn2)
					{
						(*iter1)->Call((*iter), fTime, COLLISION_STATE::CS_INIT);
					}

					(*iter)->AddPrevCol((*iter1));
					(*iter1)->AddPrevCol((*iter));
				}

				else
				{
					if (bIgn1)
						(*iter)->Call((*iter1), fTime, COLLISION_STATE::CS_STAY);

					if (bIgn2)
						(*iter1)->Call((*iter), fTime, COLLISION_STATE::CS_STAY);
				}
			}

			else
			{
				if ((*iter)->HasPrevCol((*iter1)))
				{
					if (bIgn1)
					{
						(*iter)->Call((*iter1), fTime, COLLISION_STATE::CS_LAST);
					}

					if (bIgn2)
					{
						(*iter1)->Call((*iter), fTime, COLLISION_STATE::CS_LAST);
					}

					(*iter)->DeletePrevCol((*iter1));
					(*iter1)->DeletePrevCol((*iter));
				}
			}
		}

	}
}

void CCollisionSection::Clear()
{
	std::list<CCollider*>::iterator iter = m_ColliderList.begin();
	std::list<CCollider*>::iterator iterEnd = m_ColliderList.end();

	for (; iter != iterEnd; ++iter)
	{
		if (!(*iter)->IsMouse())
		{
			(*iter)->Clear();
		}
		else
		{
			(*iter)->EnableCol(true);
		}
	}	

	m_ColliderList.clear();
}

bool CCollisionSection::SortZ(CCollider* pSrc, CCollider* pDst)
{
	CUIControl* pSrcParent = static_cast<CUIControl*>(pSrc->GetParent());
	CUIControl* pDestParent = static_cast<CUIControl*>(pDst->GetParent());

	if (pSrcParent && pDestParent)
	{
		return pSrcParent->GetZOrder() < pDestParent->GetZOrder();
	}

	return false;
}
