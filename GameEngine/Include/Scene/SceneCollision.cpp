#include "SceneCollision.h"
#include "CollisionSection.h"
#include "../Device.h"
#include "../Component/Collider.h"
#include "../Input.h"
#include "../UI/MouseObj.h"
#include "../Component/ColliderPoint.h"
#include "../CameraManager.h"
#include "../Component/Camera.h"

CSceneCollision::CSceneCollision()	:
	m_p2DSection(nullptr),
	m_p3DSection(nullptr),
	m_pUISection(nullptr),
	m_bMouseCol(false)
{
}

CSceneCollision::~CSceneCollision()
{
	size_t iSize = m_p2DSection->vecSection.size();

	for (size_t i = 0; i < iSize; ++i)
		SAFE_DELETE(m_p2DSection->vecSection[i]);

	SAFE_DELETE(m_p2DSection);

	iSize = m_p3DSection->vecSection.size();

	for (size_t i = 0; i < iSize; ++i)
		SAFE_DELETE(m_p3DSection->vecSection[i]);

	SAFE_DELETE(m_p3DSection);

	iSize = m_pUISection->vecSection.size();

	for (size_t i = 0; i < iSize; ++i)
		SAFE_DELETE(m_pUISection->vecSection[i]);

	SAFE_DELETE(m_pUISection);
}

bool CSceneCollision::Init()
{
	m_p2DSection = CreateSection(1, 1, 1, Vector3(1500.f, 1500.f, 10000.f), Vector3::Zero);

	m_p3DSection = CreateSection(1, 1, 1, Vector3(20000.f, 20000.f, 20000.f), Vector3::Zero);

	m_pUISection = CreateSection(1, 1, 1, Vector3(20000.f, 20000.f, 20000.f), Vector3::Zero);

	return true;
}

PSectionInfo CSceneCollision::CreateSection(int iNumX, int iNumY, int iNumZ, const Vector3& vSize, const Vector3& vCenter)
{
	PSectionInfo pInfo = new SectionInfo;

	pInfo->iNumX = iNumX;
	pInfo->iNumY = iNumY;
	pInfo->iNumZ = iNumZ;
	pInfo->vSize = vSize;

	Vector3 vSecSize;

	vSecSize.x = vSize.x / iNumX;
	vSecSize.y = vSize.y / iNumY;
	vSecSize.z = vSize.z / iNumZ;

	for (int z = 0; z < iNumZ; ++z)
	{
		for (int y = 0; y < iNumY; ++y)
		{
			for (int x = 0; x < iNumX; ++x)
			{
				CCollisionSection* pSec = new CCollisionSection;

				int idx = z * iNumY * iNumX + y * iNumX + x;

				pSec->Init(x, y, z, idx, vSecSize);

				pSec->SetCenter(Vector3(x * vSecSize.x - pInfo->vSize.x / 2.f + vSecSize.x /2.f, 
					y * vSecSize.y - pInfo->vSize.y/2.f + vSecSize.y / 2.f, 
					z * vSecSize.z - pInfo->vSize.z/2.f + +vSecSize.z / 2.f));

				pInfo->vecSection.push_back(pSec);
			}
		}
	}

	return pInfo;
}

void CSceneCollision::SetCenter(const Vector3& vCenter)
{
	m_p2DSection->vCenter = vCenter;

	size_t iSize = m_p2DSection->vecSection.size();

	for (size_t i = 0; i < iSize; ++i)
	{
		m_p2DSection->vecSection[i]->SetCenter(Vector3(((i % (m_p2DSection->iNumY * m_p2DSection->iNumX)) % m_p2DSection->iNumX) * 
			m_p2DSection->vecSection[i]->m_vSize.x - m_p2DSection->vSize.x / 2.f + m_p2DSection->vecSection[i]->m_vSize.x / 2.f,
			((i % (m_p2DSection->iNumY * m_p2DSection->iNumX)) / m_p2DSection->iNumX) * 
			m_p2DSection->vecSection[i]->m_vSize.y - m_p2DSection->vSize.y / 2.f + m_p2DSection->vecSection[i]->m_vSize.y / 2.f,
			(i / (m_p2DSection->iNumY * m_p2DSection->iNumX)) * 
			m_p2DSection->vecSection[i]->m_vSize.z - m_p2DSection->vSize.z / 2.f + m_p2DSection->vecSection[i]->m_vSize.z / 2.f));
	}

	m_pUISection->vCenter = vCenter;
	m_pUISection->vecSection[0]->SetCenter(vCenter);
}

void CSceneCollision::AddCollider(CCollider* pCol)
{
	PSectionInfo pInfo = m_p3DSection;

	if (pCol->IsMouse())
	{
		pInfo = m_pUISection;

		Vector3 vMax = pCol->GetMax();
		Vector3 vMin = pCol->GetMin();

		int iStartX, iStartY, iStartZ;
		int iEndX, iEndY, iEndZ;

		vMin -= pInfo->vCenter - pInfo->vSize / 2.f;
		vMax -= pInfo->vCenter - pInfo->vSize / 2.f;

		iStartX = (int)(vMin.x / (pInfo->vSize.x / pInfo->iNumX));
		iStartY = (int)(vMin.y / (pInfo->vSize.y / pInfo->iNumY));
		iStartZ = (int)(vMin.z / (pInfo->vSize.z / pInfo->iNumZ));

		iEndX = (int)(vMax.x / (pInfo->vSize.x / pInfo->iNumX));
		iEndY = (int)(vMax.y / (pInfo->vSize.y / pInfo->iNumY));
		iEndZ = (int)(vMax.z / (pInfo->vSize.z / pInfo->iNumZ));

		iStartX = iStartX < 0 ? 0 : iStartX;
		iStartY = iStartY < 0 ? 0 : iStartY;
		iStartZ = iStartZ < 0 ? 0 : iStartZ;

		iEndX = iEndX >= pInfo->iNumX ? pInfo->iNumX - 1 : iEndX;
		iEndY = iEndY >= pInfo->iNumY ? pInfo->iNumY - 1 : iEndY;
		iEndZ = iEndZ >= pInfo->iNumZ ? pInfo->iNumZ - 1 : iEndZ;

		for (int x = iStartX; x <= iEndX; ++x)
		{
			for (int y = iStartY; y <= iEndY; ++y)
			{
				for (int z = iStartZ; z <= iEndZ; ++z)
				{
					int idx = z * pInfo->iNumX * pInfo->iNumY + y * pInfo->iNumX + x;

					pCol->AddSection(idx);

					pInfo->vecSection[idx]->AddCollider(pCol);
				}
			}
		}

		pInfo = m_p3DSection;
	}

	Vector3 vMax = pCol->GetMax();
	Vector3 vMin = pCol->GetMin();

	if (pCol->IsUI())
	{
		pInfo = m_pUISection;

		CCamera* pCam = GET_SINGLE(CCameraManager)->GetMainCam();

		Resolution tRS = RESOLUTION;

		Vector3 vCamPos = pCam->GetWorldPos() - Vector3((float)tRS.iWidth, (float)tRS.iHeight, 0.f) * pCam->GetPivot();

		vCamPos.z = 0;

		vMax += vCamPos;
		vMin += vCamPos;
	}

	else if (pCol->Is2D())		
		pInfo = m_p2DSection;

	int iStartX, iStartY, iStartZ;
	int iEndX, iEndY, iEndZ;

	vMin -= pInfo->vCenter - pInfo->vSize / 2.f;
	vMax -= pInfo->vCenter - pInfo->vSize / 2.f;

	iStartX = (int)(vMin.x / (pInfo->vSize.x / pInfo->iNumX));
	iStartY = (int)(vMin.y / (pInfo->vSize.y / pInfo->iNumY));
	iStartZ = (int)(vMin.z / (pInfo->vSize.z / pInfo->iNumZ));

	iEndX = (int)(vMax.x / (pInfo->vSize.x / pInfo->iNumX));
	iEndY = (int)(vMax.y / (pInfo->vSize.y / pInfo->iNumY));
	iEndZ = (int)(vMax.z / (pInfo->vSize.z / pInfo->iNumZ));

	iStartX = iStartX < 0 ? 0 : iStartX;
	iStartY = iStartY < 0 ? 0 : iStartY;
	iStartZ = iStartZ < 0 ? 0 : iStartZ;

	iEndX = iEndX >= pInfo->iNumX ? pInfo->iNumX - 1 : iEndX;
	iEndY = iEndY >= pInfo->iNumY ? pInfo->iNumY - 1 : iEndY;
	iEndZ = iEndZ >= pInfo->iNumZ ? pInfo->iNumZ - 1 : iEndZ;

	for (int x = iStartX; x <= iEndX; ++x)
	{
		for (int y = iStartY; y <= iEndY; ++y)
		{
			for (int z = iStartZ; z <= iEndZ; ++z)
			{
				int idx = z * pInfo->iNumX * pInfo->iNumY + y * pInfo->iNumX + x;

				pCol->AddSection(idx);

				pInfo->vecSection[idx]->AddCollider(pCol);
			}
		}
	}
}

void CSceneCollision::Collision(float fTime)
{/*
	if (!m_pUISection->vecSection[0]->MouseCollision(fTime))
	{
		int idx = GetIdx(GET_SINGLE(CInput)->GetMouseWorldPos(), m_p2DSection);

		m_p2DSection->vecSection[idx]->MouseCollision(fTime);
	}*/

	m_pUISection->vecSection[0]->Collision(fTime);

	size_t i2DSize = m_p2DSection->vecSection.size();

	for (size_t i = 0; i < i2DSize; ++i)
		m_p2DSection->vecSection[i]->Collision(fTime);


	size_t i3DSize = m_p3DSection->vecSection.size();

	for (size_t i = 0; i < i3DSize; ++i)
		m_p3DSection->vecSection[i]->Collision(fTime);

	Clear();
}

int CSceneCollision::GetIdx(const Vector2& vPos, PSectionInfo pInfo)
{
	int iStartX, iStartY;

	Vector3 _vPos = Vector3(vPos.x, vPos.y, 0.f);

	_vPos -= pInfo->vCenter - pInfo->vSize / 2.f;

	iStartX = (int)(_vPos.x / (pInfo->vSize.x / pInfo->iNumX));
	iStartY = (int)(_vPos.y / (pInfo->vSize.y / pInfo->iNumY));

	iStartX = iStartX < 0 ? 0 : iStartX;
	iStartY = iStartY < 0 ? 0 : iStartY;

	return iStartY * pInfo->iNumX + iStartX;
}

void CSceneCollision::Clear()
{
	m_pUISection->vecSection[0]->Clear();

	size_t i2DSize = m_p2DSection->vecSection.size();

	for (size_t i = 0; i < i2DSize; ++i)
		m_p2DSection->vecSection[i]->Clear();

	size_t i3DSize = m_p3DSection->vecSection.size();

	for (size_t i = 0; i < i3DSize; ++i)
		m_p3DSection->vecSection[i]->Clear();

	((CCollider*)GET_SINGLE(CInput)->GetMouse()->GetCol().get())->Clear();
}
