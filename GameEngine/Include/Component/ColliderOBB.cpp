#include "ColliderOBB.h"
#include "Collision.h"
#include "../Resource/ResourceManager.h"
#include "../Resource/ShaderManager.h"
#include "../RenderManager.h"
#include "../Render/RenderState.h"
#include "../Resource/Mesh.h"
#include "ColliderRay.h"
#include "../Resource/Material.h"
#include "ColliderSphere.h"
#include "Transform.h"
#ifdef _DEBUG
#include "../Engine.h"
#endif

CColliderOBB::CColliderOBB()	:
	m_tInfo()
{
	m_eColType = COLLIDER_TYPE::OBB;
	m_eSceneComponentType = SCENE_COMPONENT_TYPE::SCT_3D;
	m_iSceneComponentClassType = static_cast<int>(SCENECOMPONENT_CLASS_TYPE::COLLIDER_OBB);
}

CColliderOBB::CColliderOBB(const CColliderOBB& obb)	:
	CCollider(obb)
	, m_tInfo(obb.m_tInfo)
{
}

CColliderOBB::~CColliderOBB()
{
}

void CColliderOBB::SetExtent(const Vector3& vExtent)
{
	memcpy_s(&m_tInfo.fLength[0], 12, &vExtent.x, 12);
}

const OBBInfo& CColliderOBB::GetInfo() const
{
	return m_tInfo;
}

bool CColliderOBB::Init()
{
	if (!CCollider::Init())
		return false;

#ifdef _DEBUG
	m_pDebugMesh = GET_SINGLE(CResourceManager)->FindMesh("OBBMesh");
	m_pDebugShader = GET_SINGLE(CShaderManager)->FindShader("DebugCollider");
	m_pRasterizer = GET_SINGLE(CRenderManager)->FindState(WIRE_FRAME);
	m_pDepthStencilState = GET_SINGLE(CRenderManager)->FindState(NO_DEPTH);
#endif

	return true;
}

void CColliderOBB::Start()
{
	CCollider::Start();
}

void CColliderOBB::Update(float fTime)
{
	CCollider::Update(fTime);

	std::shared_ptr<CTransform> pTransform = GetTransform();
	m_tInfo.vPos = pTransform->GetBoneWorldPos();
	m_tInfo.vAxis[AXIS_X] = GetWorldAxis(AXIS_X);
	m_tInfo.vAxis[AXIS_Y] = GetWorldAxis(AXIS_Y);
	m_tInfo.vAxis[AXIS_Z] = GetWorldAxis(AXIS_Z);
	
	Vector3 vScale = {};
	memcpy_s(&vScale.x, 12, &m_tInfo.fLength[0], 12);
	vScale *= 2.f;
	SetWorldScale(vScale);

	/*m_vMin = Vector3(FLT_MAX, FLT_MAX, FLT_MAX);
	m_vMax = Vector3(FLT_MIN, FLT_MIN, FLT_MIN);

	Vector3 vLBN = m_tInfo.vPos - m_tInfo.vAxis[AXIS_X] * m_tInfo.fLength[AXIS_X] - m_tInfo.vAxis[AXIS_Y] * m_tInfo.fLength[AXIS_Y] - m_tInfo.vAxis[AXIS_Z] * m_tInfo.fLength[AXIS_Z];
	Vector3 vLBF = m_tInfo.vPos - m_tInfo.vAxis[AXIS_X] * m_tInfo.fLength[AXIS_X] - m_tInfo.vAxis[AXIS_Y] * m_tInfo.fLength[AXIS_Y] + m_tInfo.vAxis[AXIS_Z] * m_tInfo.fLength[AXIS_Z];
	Vector3 vLTN = m_tInfo.vPos - m_tInfo.vAxis[AXIS_X] * m_tInfo.fLength[AXIS_X] + m_tInfo.vAxis[AXIS_Y] * m_tInfo.fLength[AXIS_Y] - m_tInfo.vAxis[AXIS_Z] * m_tInfo.fLength[AXIS_Z];
	Vector3 vLTF = m_tInfo.vPos - m_tInfo.vAxis[AXIS_X] * m_tInfo.fLength[AXIS_X] + m_tInfo.vAxis[AXIS_Y] * m_tInfo.fLength[AXIS_Y] + m_tInfo.vAxis[AXIS_Z] * m_tInfo.fLength[AXIS_Z];
	Vector3 vRBN = m_tInfo.vPos + m_tInfo.vAxis[AXIS_X] * m_tInfo.fLength[AXIS_X] - m_tInfo.vAxis[AXIS_Y] * m_tInfo.fLength[AXIS_Y] - m_tInfo.vAxis[AXIS_Z] * m_tInfo.fLength[AXIS_Z];
	Vector3 vRBF = m_tInfo.vPos + m_tInfo.vAxis[AXIS_X] * m_tInfo.fLength[AXIS_X] - m_tInfo.vAxis[AXIS_Y] * m_tInfo.fLength[AXIS_Y] + m_tInfo.vAxis[AXIS_Z] * m_tInfo.fLength[AXIS_Z];
	Vector3 vRTN = m_tInfo.vPos + m_tInfo.vAxis[AXIS_X] * m_tInfo.fLength[AXIS_X] + m_tInfo.vAxis[AXIS_Y] * m_tInfo.fLength[AXIS_Y] - m_tInfo.vAxis[AXIS_Z] * m_tInfo.fLength[AXIS_Z];
	Vector3 vRTF = m_tInfo.vPos + m_tInfo.vAxis[AXIS_X] * m_tInfo.fLength[AXIS_X] + m_tInfo.vAxis[AXIS_Y] * m_tInfo.fLength[AXIS_Y] + m_tInfo.vAxis[AXIS_Z] * m_tInfo.fLength[AXIS_Z];

	m_vMin.x = m_vMin.x < vLBN.x ? m_vMin.x : vLBN.x;
	m_vMin.y = m_vMin.y < vLBN.y ? m_vMin.y : vLBN.y;
	m_vMin.z = m_vMin.z < vLBN.z ? m_vMin.z : vLBN.z;
	m_vMin.x = m_vMin.x < vLBF.x ? m_vMin.x : vLBF.x;
	m_vMin.y = m_vMin.y < vLBF.y ? m_vMin.y : vLBF.y;
	m_vMin.z = m_vMin.z < vLBF.z ? m_vMin.z : vLBF.z;
	m_vMin.x = m_vMin.x < vLTN.x ? m_vMin.x : vLTN.x;
	m_vMin.y = m_vMin.y < vLTN.y ? m_vMin.y : vLTN.y;
	m_vMin.z = m_vMin.z < vLTN.z ? m_vMin.z : vLTN.z;
	m_vMin.x = m_vMin.x < vLTF.x ? m_vMin.x : vLTF.x;
	m_vMin.y = m_vMin.y < vLTF.y ? m_vMin.y : vLTF.y;
	m_vMin.z = m_vMin.z < vLTF.z ? m_vMin.z : vLTF.z;
	m_vMin.x = m_vMin.x < vRBN.x ? m_vMin.x : vRBN.x;
	m_vMin.y = m_vMin.y < vRBN.y ? m_vMin.y : vRBN.y;
	m_vMin.z = m_vMin.z < vRBN.z ? m_vMin.z : vRBN.z;
	m_vMin.x = m_vMin.x < vRBF.x ? m_vMin.x : vRBF.x;
	m_vMin.y = m_vMin.y < vRBF.y ? m_vMin.y : vRBF.y;
	m_vMin.z = m_vMin.z < vRBF.z ? m_vMin.z : vRBF.z;
	m_vMin.x = m_vMin.x < vRTN.x ? m_vMin.x : vRTN.x;
	m_vMin.y = m_vMin.y < vRTN.y ? m_vMin.y : vRTN.y;
	m_vMin.z = m_vMin.z < vRTN.z ? m_vMin.z : vRTN.z;
	m_vMin.x = m_vMin.x < vRTF.x ? m_vMin.x : vRTF.x;
	m_vMin.y = m_vMin.y < vRTF.y ? m_vMin.y : vRTF.y;
	m_vMin.z = m_vMin.z < vRTF.z ? m_vMin.z : vRTF.z;

	m_vMax.x = m_vMax.x > vLBN.x ? m_vMax.x : vLBN.x;
	m_vMax.y = m_vMax.y > vLBN.y ? m_vMax.y : vLBN.y;
	m_vMax.z = m_vMax.z > vLBN.z ? m_vMax.z : vLBN.z;
	m_vMax.x = m_vMax.x > vLBF.x ? m_vMax.x : vLBF.x;
	m_vMax.y = m_vMax.y > vLBF.y ? m_vMax.y : vLBF.y;
	m_vMax.z = m_vMax.z > vLBF.z ? m_vMax.z : vLBF.z;
	m_vMax.x = m_vMax.x > vLTN.x ? m_vMax.x : vLTN.x;
	m_vMax.y = m_vMax.y > vLTN.y ? m_vMax.y : vLTN.y;
	m_vMax.z = m_vMax.z > vLTN.z ? m_vMax.z : vLTN.z;
	m_vMax.x = m_vMax.x > vLTF.x ? m_vMax.x : vLTF.x;
	m_vMax.y = m_vMax.y > vLTF.y ? m_vMax.y : vLTF.y;
	m_vMax.z = m_vMax.z > vLTF.z ? m_vMax.z : vLTF.z;
	m_vMax.x = m_vMax.x > vRBN.x ? m_vMax.x : vRBN.x;
	m_vMax.y = m_vMax.y > vRBN.y ? m_vMax.y : vRBN.y;
	m_vMax.z = m_vMax.z > vRBN.z ? m_vMax.z : vRBN.z;
	m_vMax.x = m_vMax.x > vRBF.x ? m_vMax.x : vRBF.x;
	m_vMax.y = m_vMax.y > vRBF.y ? m_vMax.y : vRBF.y;
	m_vMax.z = m_vMax.z > vRBF.z ? m_vMax.z : vRBF.z;
	m_vMax.x = m_vMax.x > vRTN.x ? m_vMax.x : vRTN.x;
	m_vMax.y = m_vMax.y > vRTN.y ? m_vMax.y : vRTN.y;
	m_vMax.z = m_vMax.z > vRTN.z ? m_vMax.z : vRTN.z;
	m_vMax.x = m_vMax.x > vRTF.x ? m_vMax.x : vRTF.x;
	m_vMax.y = m_vMax.y > vRTF.y ? m_vMax.y : vRTF.y;
	m_vMax.z = m_vMax.z > vRTF.z ? m_vMax.z : vRTF.z;*/
}

void CColliderOBB::PostUpdate(float fTime)
{
	CCollider::PostUpdate(fTime);
}

void CColliderOBB::Collision(float fTime)
{
	CCollider::Collision(fTime);
}

void CColliderOBB::PreRender(float fTime)
{
	CCollider::PreRender(fTime);
}

void CColliderOBB::Render(float fTime)
{
	CSceneComponent::Render(fTime);

#ifdef _DEBUG
	if (GET_SINGLE(CEngine)->IsColliderEnabled())
	{
		m_pDepthStencilState->SetState();
		m_pRasterizer->SetState();
		m_pDebugMtrl->Render(fTime);
		m_pDebugShader->SetShader();
		m_pDebugMesh->Render(fTime);
		m_pRasterizer->ResetState();
		m_pDepthStencilState->ResetState();
	}
#endif
}

void CColliderOBB::PostRender(float fTime)
{
	CCollider::PostRender(fTime);
}

std::shared_ptr<class CComponent> CColliderOBB::Clone()
{
	return std::shared_ptr<class CComponent>(new CColliderOBB(*this));
}

bool CColliderOBB::Collision(CCollider* pDest)
{
	switch (pDest->GetColType())
	{
	case COLLIDER_TYPE::CT_RECT:
		break;
	case COLLIDER_TYPE::CT_SPHERE2D:
		break;
	case COLLIDER_TYPE::CT_POINT:
		break;
	case COLLIDER_TYPE::CT_OBB2D:
		break;
	case COLLIDER_TYPE::CT_PIXEL:
		break;
	case COLLIDER_TYPE::CT_LINE:
		break;
	case COLLIDER_TYPE::SPHERE:
		return CCollision::CollisionSphereToOBB(static_cast<CColliderSphere*>(pDest), this);
	case COLLIDER_TYPE::RAY:
		return CCollision::CollisionRayToOBB(static_cast<CColliderRay*>(pDest), this);
	case COLLIDER_TYPE::TERRAIN:
		break;
	case COLLIDER_TYPE::OBB:
		return CCollision::CollisionOBBToOBB(this, static_cast<CColliderOBB*>(pDest));
	}

	return false;
}

bool CColliderOBB::CollisionMouse(const Vector2& vMousePos)
{
	return false;
}

void CColliderOBB::Save(FILE* pFile)
{
	CCollider::Save(pFile);
}

void CColliderOBB::Load(FILE* pFile)
{
	CCollider::Load(pFile);


#ifdef _DEBUG
	if (!m_pRasterizer)
	{
		m_pRasterizer = GET_SINGLE(CRenderManager)->FindState(WIRE_FRAME);
	}
	if (!m_pDepthStencilState)
	{
		m_pDepthStencilState = GET_SINGLE(CRenderManager)->FindState(NO_DEPTH);
	}
	if(!m_pDebugShader)
	m_pDebugShader = GET_SINGLE(CShaderManager)->FindShader("DebugCollider");
#endif
}

void CColliderOBB::ShowWindow()
{
	CCollider::ShowWindow(); 

	std::string strName = GetName();

	if (strName == "")
	{
		strName = "None";
	}

	if (ImGui::Begin(strName.c_str()))
	{
		ImGui::InputFloat3("Length", m_tInfo.fLength);
		ImGui::InputFloat3("Axis X", &m_tInfo.vAxis[AXIS_X].x);
		ImGui::InputFloat3("Axis Y", &m_tInfo.vAxis[AXIS_Y].x);
		ImGui::InputFloat3("Axis Z", &m_tInfo.vAxis[AXIS_Z].x);
		ImGui::InputFloat3("Position", &m_tInfo.vPos.x);
	}
	ImGui::End();
}
