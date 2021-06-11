#include "Collider.h"
#include "../Resource/Mesh2D.h"
#include "../Resource/ResourceManager.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneCollision.h"
#include "../Resource/Material.h"
#include "../Collision/CollisionManager.h"
#include "../RenderManager.h"
#ifdef _DEBUG
#include "../Engine.h"
#endif
#include "../Resource/Shader.h"

CCollider::CCollider()	:
	m_eColType(COLLIDER_TYPE::CT_RECT),
	m_vCross(),
	m_bUI(false),
	m_b2D(false),
	m_vMin(),
	m_vMax(),
	m_bCheck(false),
	m_bEnableCol(true),
	m_bMouse(false)
{
	m_strProFile = "Base";
	m_pProFile = GET_SINGLE(CCollisionManager)->FindProfile(m_strProFile);

#ifdef _DEBUG
	m_pDebugMesh = nullptr;
	m_pDebugMtrl = nullptr;
	m_pDebugShader = nullptr;
	m_pRasterizer = nullptr;
	m_pDepthStencilState = nullptr;
#endif
}

CCollider::CCollider(const CCollider& col)	:
	CSceneComponent(col)
{
	m_eColType = col.m_eColType;

#ifdef _DEBUG
	m_pDebugMesh = col.m_pDebugMesh;

	if (col.m_pDebugMtrl)
		m_pDebugMtrl = col.m_pDebugMtrl->Clone();

	else
		m_pDebugMtrl = nullptr;

	m_pDebugShader = col.m_pDebugShader;

	m_pRasterizer = col.m_pRasterizer;

	m_pDepthStencilState = col.m_pDepthStencilState;
#endif

	m_bUI = col.m_bUI;
	m_b2D = col.m_b2D;
	m_vMin = col.m_vMin;
	m_vMax = col.m_vMax;

	m_SectionList.clear();
	m_PrevList.clear();
	m_CurList.clear();

	for (int i = 0; i < (int)COLLISION_STATE::CS_END; ++i)
	{
		size_t iSize = col.m_vecCallBack[i].size();

		for (size_t j = 0; j < iSize; ++j)
		{
			m_vecCallBack[i].push_back(col.m_vecCallBack[i][j]);
		}
	}

	m_bEnableCol = col.m_bEnableCol;
	m_pProFile = col.m_pProFile;
	m_bMouse = col.m_bMouse;
}

CCollider::~CCollider()
{
	std::list<CCollider*>::iterator iter = m_PrevList.begin();
	std::list<CCollider*>::iterator iterEnd = m_PrevList.end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->DeletePrevCol(this);
		(*iter)->Call(this, 0.f, COLLISION_STATE::CS_LAST);
		//Call((*iter), 0.f, COLLISION_STATE::CS_LAST);
	}		
}

COLLIDER_TYPE CCollider::GetColType() const
{
	return m_eColType;
}

void CCollider::SetCross(const Vector3& v)
{
	m_vCross = v;
}

const Vector3& CCollider::GetCrs() const
{
	return m_vCross;
}

bool CCollider::IsUI() const
{
	return m_bUI;
}

bool CCollider::Is2D() const
{
	return m_b2D;
}

const Vector3& CCollider::GetMax() const
{
	return m_vMax;
}

const Vector3& CCollider::GetMin() const
{
	return m_vMin;
}

void CCollider::AddSection(int iSec)
{
	m_SectionList.push_back(iSec);
}

void CCollider::Clear()
{
	m_bCheck = false;
	m_SectionList.clear();
	m_CurList.clear();
}

void CCollider::AddPrevCol(CCollider* pCol)
{
	m_PrevList.push_back(pCol);
}

bool CCollider::HasPrevCol(CCollider* pCol)
{
	std::list<CCollider*>::iterator iter = m_PrevList.begin();
	std::list<CCollider*>::iterator iterEnd = m_PrevList.end();

	for (; iter != iterEnd; ++iter)
	{
		if ((*iter) == pCol)
			return true;
	}

	return false;
}

void CCollider::AddCurCol(CCollider* pCol)
{
	m_CurList.push_back(pCol);
}

bool CCollider::HasCurCol(CCollider* pCol)
{
	std::list<CCollider*>::iterator iter = m_CurList.begin();
	std::list<CCollider*>::iterator iterEnd = m_CurList.end();

	for (; iter != iterEnd; ++iter)
	{
		if ((*iter) == pCol)
			return true;
	}

	return false;
}

void CCollider::DeletePrevCol(CCollider* pCol)
{
	std::list<CCollider*>::iterator iter = m_PrevList.begin();
	std::list<CCollider*>::iterator iterEnd = m_PrevList.end();

	for (; iter != iterEnd; ++iter)
	{
		if ((*iter) == pCol)
		{
			m_PrevList.erase(iter);
			return;
		}
	}
}

void CCollider::Call(CCollider* pDest, float fTime, COLLISION_STATE eStat)
{
#ifdef _DEBUG
	if (eStat == COLLISION_STATE::CS_INIT)
	{
		m_pDebugMtrl->SetDiffuseColor(1.f, 0.f, 0.f, 1.f);
	}
		

	else if (eStat == COLLISION_STATE::CS_STAY)
	{
		m_pDebugMtrl->SetDiffuseColor(1.f, 0.f, 0.f, 1.f);
	}

	else if (eStat == COLLISION_STATE::CS_LAST)
	{
		m_pDebugMtrl->SetDiffuseColor(0.f, 1.f, 0.f, 1.f);
	}
#endif

	size_t iSize = m_vecCallBack[(int)eStat].size();

	for (size_t i = 0; i < iSize; ++i)
		m_vecCallBack[(int)eStat][i](this, pDest, fTime);
}

void CCollider::CallMouse(const Vector2& v, float fTime, COLLISION_STATE eStat)
{
#ifdef _DEBUG
	if (eStat == COLLISION_STATE::CS_INIT)
	{
		m_pDebugMtrl->SetDiffuseColor(1.f, 0.f, 0.f, 1.f);
	}


	else if (eStat == COLLISION_STATE::CS_STAY)
	{
		m_pDebugMtrl->SetDiffuseColor(1.f, 0.f, 0.f, 1.f);
	}

	else if (eStat == COLLISION_STATE::CS_LAST)
	{
		m_pDebugMtrl->SetDiffuseColor(0.f, 1.f, 0.f, 1.f);
	}
#endif

	size_t iSize = m_vecMouseCallBack[(int)eStat].size();

	for (size_t i = 0; i < iSize; ++i)
		m_vecMouseCallBack[(int)eStat][i](this, v, fTime);
}

void CCollider::SetProFile(const std::string& strTag)
{
	m_strProFile = strTag;
	m_pProFile = GET_SINGLE(CCollisionManager)->FindProfile(strTag);
}

PCollisionProfile CCollider::GetProFile() const
{
	return m_pProFile;
}

bool CCollider::IsCheck() const
{
	return m_bCheck;
}

void CCollider::Check()
{
	m_bCheck = true;
}

void CCollider::PrevColSecCheck(float fTime)
{
	std::list<CCollider*>::iterator iter = m_PrevList.begin();
	std::list<CCollider*>::iterator iterEnd = m_PrevList.end();

	for (; iter != iterEnd;)
	{
		std::list<int>::iterator iterSec = m_SectionList.begin();
		std::list<int>::iterator iterSecEnd = m_SectionList.end();

		bool bCheck = false;

		for (; iterSec != iterSecEnd; ++iterSec)
		{
			std::list<int>::iterator iterSec1 = (*iter)->m_SectionList.begin();
			std::list<int>::iterator iterSec1End = (*iter)->m_SectionList.end();

			for (; iterSec1 != iterSec1End; ++iterSec1)
			{
				if ((*iterSec) == (*iterSec1))
				{
					bCheck = true;
					break;
				}
			}

			if (bCheck)
				break;
		}

		if (!bCheck)
		{
			Call((*iter), fTime, COLLISION_STATE::CS_LAST);
			(*iter)->Call(this, fTime, COLLISION_STATE::CS_LAST);
			(*iter)->DeletePrevCol(this);
			iter = m_PrevList.erase(iter);
			iterEnd = m_PrevList.end();
			continue;
		}

		++iter;
	}
}

bool CCollider::IsColEnable() const
{
	return m_bEnableCol;
}

bool CCollider::CheckCurSec(CCollider* pCol)
{
	if (!pCol->HasCurCol(pCol))
		return false;

	std::list<int>::iterator iter = pCol->m_CurSecList.begin();
	std::list<int>::iterator iterEnd = pCol->m_CurSecList.end();

	std::list<int>::iterator iter1 = m_CurSecList.begin();
	std::list<int>::iterator iter1End = m_CurSecList.end();

	for (; iter != iterEnd; ++iter)
	{
		for (; iter1 != iter1End; ++iter1)
		{
			if (*iter == *iter1)
				return true;
		}
	}

	return false;
}

void CCollider::SetUI()
{
	m_bUI = true;
}

bool CCollider::IsMouse() const
{
	return m_bMouse;
}

void CCollider::SetMouse()
{
	m_bMouse = true;
}

void CCollider::EnableCol(bool bCol)
{
	m_bEnableCol = bCol;
}
#ifdef _DEBUG
class CMesh* CCollider::GetDebugMesh() const
{
	return m_pDebugMesh;
}
#endif

bool CCollider::Init()
{
	if (!CSceneComponent::Init())
		return false;

#ifdef _DEBUG
	m_pDebugMtrl = GET_SINGLE(CResourceManager)->FindMaterial("Collider");
	m_pDebugMtrl->SetDiffuseColor(0.f, 1.f, 0.f, 1.f);
#endif

	m_pProFile = GET_SINGLE(CCollisionManager)->FindProfile("Base");

	return true;
}

void CCollider::Start()
{
	CSceneComponent::Start();
}

void CCollider::Update(float fTime)
{
	CSceneComponent::Update(fTime);
}

void CCollider::PostUpdate(float fTime)
{
	CSceneComponent::PostUpdate(fTime);
}

void CCollider::Collision(float fTime)
{
	CSceneComponent::Collision(fTime);

	m_pScene->GetCollision()->AddCollider(this);
}

void CCollider::PreRender(float fTime)
{
	CSceneComponent::PreRender(fTime);

#ifdef _DEBUG

	if (GET_SINGLE(CEngine)->IsColliderEnabled())
	{

		if (m_bInstance)
		{
			switch (GetSceneComponentType())
			{
			case SCENE_COMPONENT_TYPE::SCT_2D:
			case SCENE_COMPONENT_TYPE::SCT_UI:
				GET_SINGLE(CRenderManager)->AddColliderInstancing(this);
				break;
			case SCENE_COMPONENT_TYPE::SCT_3D:
				break;
			}
		}
	}
		
#endif
}

void CCollider::Render(float fTime)
{
	CSceneComponent::Render(fTime);

#ifdef _DEBUG
	if (GET_SINGLE(CEngine)->IsColliderEnabled())
	{
		if(m_pDebugMtrl)
		m_pDebugMtrl->Render(fTime);
		if (m_pDebugShader)
		m_pDebugShader->SetShader();
		if (m_pDebugMesh)
			m_pDebugMesh->Render(fTime);
	}
#endif
}

void CCollider::PostRender(float fTime)
{
	CSceneComponent::PostRender(fTime);
}

std::shared_ptr<CComponent> CCollider::Clone()
{
	return nullptr;
}

void CCollider::Save(FILE* pFile)
{
	CSceneComponent::Save(pFile);

	fwrite(&m_eColType, 4, 1, pFile);
	fwrite(&m_vCross, sizeof(Vector3), 1, pFile);
	fwrite(&m_bUI, 1, 1, pFile);
	fwrite(&m_b2D, 1, 1, pFile);
	fwrite(&m_vMin, sizeof(Vector3), 1, pFile);
	fwrite(&m_vMax, sizeof(Vector3), 1, pFile);
	fwrite(&m_bCheck, 1, 1, pFile);
	fwrite(&m_bEnable, 1, 1, pFile);

	int iLength = (int)m_strProFile.length();

	fwrite(&iLength, 4, 1, pFile);
	fwrite(m_strProFile.c_str(), 1, iLength, pFile);

	fwrite(&m_bMouse, 1, 1, pFile);
}

void CCollider::Load(FILE* pFile)
{
	CSceneComponent::Load(pFile);

	fread(&m_eColType, 4, 1, pFile);
	fread(&m_vCross, sizeof(Vector3), 1, pFile);
	fread(&m_bUI, 1, 1, pFile);
	fread(&m_b2D, 1, 1, pFile);
	fread(&m_vMin, sizeof(Vector3), 1, pFile);
	fread(&m_vMax, sizeof(Vector3), 1, pFile);
	fread(&m_bCheck, 1, 1, pFile);
	fread(&m_bEnable, 1, 1, pFile);

	int iLength = 0;

	fread(&iLength, 4, 1, pFile);

	char strTag[256] = {};
	if (iLength > 0)
	{
		fread(strTag, 1, iLength, pFile);
	}

	m_strProFile = strTag;

	m_pProFile = GET_SINGLE(CCollisionManager)->FindProfile(m_strProFile);

	fread(&m_bMouse, 1, 1, pFile);
}

bool CCollider::Collision(CCollider* pDest)
{
	return false;
}
