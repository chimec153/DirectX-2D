#include "Camera.h"
#include "../Device.h"
#include "../CameraManager.h"
#include "Renderer.h"
#include "../Resource/Mesh.h"
#include "../Resource/Material.h"
#include "../UI/imgui/imgui.h"
#include "Transform.h"

CCamera::CCamera()	:
	m_matView(),
	m_matProj(),
	m_fAngle(90.f),
	m_fDist(30000.f),
	m_pTarget(nullptr),
	m_matVP(),
	m_vTargetOffset(0.f, 35.f, 0.f),
	m_bPosTarget(false)
	, m_vTargetPos()
	, m_bShake(false)
	, m_fShakeTime()
	, m_fShakeTimeMax(1.f)
	, m_fShakePower(0.f)
	, m_fShakeMass(1.f)
	, m_fFrequency(PI * 2.f)
{
	SetCameraType(CAMERA_TYPE::CT_3D);
	m_iSceneComponentClassType = (int)SCENECOMPONENT_CLASS_TYPE::SCT_CAMERA;

	m_vProjPoint[0] = Vector4(-0.3f, 0.3f, 0.f, 0.3f);
	m_vProjPoint[1] = Vector4(0.3f, 0.3f, 0.f, 0.3f);
	m_vProjPoint[2] = Vector4(-0.3f, -0.3f, 0.f, 0.3f);
	m_vProjPoint[3] = Vector4(0.3f, -0.3f, 0.f, 0.3f);

	m_vProjPoint[4] = Vector4(-m_fDist, m_fDist, m_fDist, m_fDist);
	m_vProjPoint[5] = Vector4(m_fDist, m_fDist, m_fDist, m_fDist);
	m_vProjPoint[6] = Vector4(-m_fDist, -m_fDist, m_fDist, m_fDist);
	m_vProjPoint[7] = Vector4(m_fDist, -m_fDist, m_fDist, m_fDist);
}

CCamera::CCamera(const CCamera& cam)	:
	CSceneComponent(cam)
	, m_vTargetOffset(cam.m_vTargetOffset)
	, m_bPosTarget(cam.m_bPosTarget)
	, m_vTargetPos(cam.m_vTargetPos)
	, m_bShake(cam.m_bShake)
	, m_fShakeTime(cam.m_fShakeTime)
	, m_fShakeTimeMax(cam.m_fShakeTimeMax)
	, m_fShakePower(cam.m_fShakePower)
	, m_fShakeMass(cam.m_fShakeMass)
	, m_fFrequency(cam.m_fFrequency)
{
	m_matView = cam.m_matView;
	m_matProj = cam.m_matProj;
	m_fAngle = cam.m_fAngle;
	m_fDist = cam.m_fDist;
	m_eType = cam.m_eType;
	m_pTarget = cam.m_pTarget;
	m_matVP = cam.m_matVP;
	
}

CCamera::~CCamera()
{
	GET_SINGLE(CCameraManager)->EraseCam(this);
}

const Matrix& CCamera::GetView() const
{
	return m_matView;
}

const Matrix& CCamera::GetProj() const
{
	return m_matProj;
}

const Matrix& CCamera::GetVP() const
{
	return m_matVP;
}

void CCamera::SetCameraType(CAMERA_TYPE eType)
{
	Resolution tRS = RESOLUTION;

	m_eType = eType;

	m_matView.Identity();

	//W^-1 = (RT)^-1 = T^-1 * R'

	switch (m_eType)
	{
	case CAMERA_TYPE::CT_3D:

		m_matProj = DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(m_fAngle), 
			tRS.iWidth / (float)tRS.iHeight, 0.3f, m_fDist);
		break;
	case CAMERA_TYPE::CT_2D:
	case CAMERA_TYPE::CT_UI:

		m_matProj = DirectX::XMMatrixOrthographicOffCenterLH(-tRS.iWidth / 2.f, tRS.iWidth/2.f,- tRS.iHeight / 2.f, tRS.iHeight / 2.f, 0.f, m_fDist);
		break;
	}
}

CAMERA_TYPE CCamera::GetCameraType() const
{
	return m_eType;
}

void CCamera::SetAngle(float fAngle)
{
	m_fAngle = fAngle;

	if (m_eType == CAMERA_TYPE::CT_3D)
		SetCameraType(m_eType);
}

void CCamera::SetDist(float fDist)
{
	m_fDist = fDist;

	SetCameraType(m_eType);
}

float CCamera::GetAngle() const
{
	return m_fAngle;
}

float CCamera::GetDist() const
{
	return m_fDist;
}

void CCamera::SetTarget(CSceneComponent* pCom)
{
	m_pTarget = pCom;
}

void CCamera::LookAt(Vector3 vCenter, Vector3 vTarget, Vector3 vUp)
{
	m_matView = XMMatrixLookAtLH(vCenter.Convert(), vTarget.Convert(), vUp.Convert());
}

void CCamera::SetLens(float fAngle, float fRatio, float fNear, float fFar)
{
	m_matProj = XMMatrixPerspectiveFovLH(fAngle, fRatio, fNear, fFar);
}

const Vector4* CCamera::GetPlanes() const
{
	return m_Plane;
}

void CCamera::SetTargetOffset(const Vector3& vOffset)
{
	m_vTargetOffset = vOffset;
}

const Vector3& CCamera::GetTargetOffset() const
{
	return m_vTargetOffset;
}

void CCamera::SetPositionTargetMode(bool bTarget)
{
	m_bPosTarget = bTarget;
}

void CCamera::SetPositionTarget(const Vector3& vPos)
{
	m_vTargetPos = vPos;
}

void CCamera::SetShake(bool bShake)
{
	m_bShake = bShake;
}

void CCamera::SetShakePower(float fPower)
{
	m_fShakePower = fPower;
}

void CCamera::SetShakeMass(float fMass)
{
	m_fShakeMass = fMass;
}

void CCamera::SetFrequency(float fFrequency)
{
	m_fFrequency = fFrequency;
}

bool CCamera::Init()
{
	if (!CSceneComponent::Init())
		return false;

	return true;
}

void CCamera::Start()
{
	GET_SINGLE(CCameraManager)->AddCam(this);
}

void CCamera::Update(float fTime)
{
	CSceneComponent::Update(fTime);

	if (m_bShake)
	{
		m_fShakeTime += fTime;
	
		m_vShakeDistMax += Vector3(0.f, sinf(m_fShakeTime * m_fFrequency) * m_fShakePower, 0.f);

		m_fShakePower -= m_fShakeMass * fTime;

		if (m_fShakePower <= 0.f)
		{
			m_bShake = false;
			m_fShakePower = 0.f;
			m_vShakeDistMax = 0.f;
		}
	}

	Resolution tRS = RESOLUTION;

	m_matView.Identity();

	switch (m_eType)
	{
	case CAMERA_TYPE::CT_3D:

		if (m_pTarget)
		{
			Vector3 vTargetPos = m_pTarget->GetWorldPos() + m_vTargetOffset + m_vShakeDistMax;
			Matrix matBone = m_pTarget->GetTransform()->GetMatBone();

			Vector3 vTargetBonePos = {};
			memcpy_s(&vTargetBonePos.x, 12, &matBone[3][0], 12);
			vTargetPos += vTargetBonePos;
			Vector3 vBonePos = {};
			matBone = GetTransform()->GetMatBone();
			memcpy_s(&vBonePos.x, 12, &matBone[3][0], 12);

			Vector3 vPos = GetWorldPos() + vBonePos  + m_vShakeDistMax;
			if (vTargetPos != vPos)
			{
				m_matView.m = XMMatrixLookAtLH(vPos.Convert(), vTargetPos.Convert(), XMVectorSet(0.f, 1.f, 0.f, 0.f));
			}
		}
		else if (m_bPosTarget)
		{
			Vector3 vPos = GetBoneWorldPos() + m_vShakeDistMax;

			m_matView.m = XMMatrixLookAtLH(vPos.Convert(), (m_vTargetPos+ m_vShakeDistMax).Convert(), Vector3(0.f, 1.f, 0.f).Convert());
		}
		else
		{
			Vector3 vAxis[3] = {};

			vAxis[AXIS_X] = GetWorldAxis(AXIS_X);
			vAxis[AXIS_Y] = GetWorldAxis(AXIS_Y);
			vAxis[AXIS_Z] = GetWorldAxis(AXIS_Z);

			for (int i = 0; i < AXIS_END; ++i)
				memcpy(&m_matView.v[i], &vAxis[i], sizeof(float) * 3);

			m_matView.Transpose();

			for (int i = 0; i < AXIS_END; ++i)
				m_matView.v[3][i] = (GetWorldPos() + m_vShakeDistMax).Dot(vAxis[i]) * -1.f;
		}
		break;
	case CAMERA_TYPE::CT_2D:
	case CAMERA_TYPE::CT_UI:
	{
		Vector3 vAxis[3] = {};

		vAxis[AXIS_X] = GetWorldAxis(AXIS_X);
		vAxis[AXIS_Y] = GetWorldAxis(AXIS_Y);
		vAxis[AXIS_Z] = GetWorldAxis(AXIS_Z);

		for (int i = 0; i < AXIS_END; ++i)
			memcpy(&m_matView.v[i], &vAxis[i], sizeof(float) * 3);

		m_matView.Transpose();

		Vector3 vRS = Vector3((float)tRS.iWidth, (float)tRS.iHeight, 0.f);

		for (int i = 0; i < AXIS_END; ++i)
			m_matView.v[3][i] = vAxis[i].Dot(GetWorldPos() - vRS * GetPivot()) * -1.f;
	}
		break;
	}

	m_matVP = m_matView * m_matProj;
}

void CCamera::PostUpdate(float fTime)
{
	CSceneComponent::PostUpdate(fTime);

	Matrix matInvVP = m_matVP;

	matInvVP.Inverse();

	Vector3 vWorldPos[8] = {};
	for (int i = 0; i < 8; i++)
	{
		vWorldPos[i].x = 
			m_vProjPoint[i].x * matInvVP[0][0] + 
			m_vProjPoint[i].y * matInvVP[1][0] + 
			m_vProjPoint[i].z * matInvVP[2][0] + 
			m_vProjPoint[i].w * matInvVP[3][0];
		vWorldPos[i].y = 
			m_vProjPoint[i].x * matInvVP[0][1] + 
			m_vProjPoint[i].y * matInvVP[1][1] + 
			m_vProjPoint[i].z * matInvVP[2][1] + 
			m_vProjPoint[i].w * matInvVP[3][1];
		vWorldPos[i].z = 
			m_vProjPoint[i].x * matInvVP[0][2] + 
			m_vProjPoint[i].y * matInvVP[1][2] + 
			m_vProjPoint[i].z * matInvVP[2][2] + 
			m_vProjPoint[i].w * matInvVP[3][2];
	}

	m_Plane[static_cast<int>(PLANE_TYPE::PT_LEFT)] =
		XMPlaneFromPoints(vWorldPos[0].Convert(), vWorldPos[6].Convert(), vWorldPos[4].Convert());
	m_Plane[static_cast<int>(PLANE_TYPE::PT_RIGHT)] =
		XMPlaneFromPoints(vWorldPos[1].Convert(), vWorldPos[5].Convert(), vWorldPos[7].Convert());
	m_Plane[static_cast<int>(PLANE_TYPE::PT_TOP)] =
		XMPlaneFromPoints(vWorldPos[1].Convert(), vWorldPos[0].Convert(), vWorldPos[4].Convert());
	m_Plane[static_cast<int>(PLANE_TYPE::PT_BOTTOM)] =
		XMPlaneFromPoints(vWorldPos[2].Convert(), vWorldPos[3].Convert(), vWorldPos[6].Convert());
	m_Plane[static_cast<int>(PLANE_TYPE::PT_NEAR)] =
		XMPlaneFromPoints(vWorldPos[0].Convert(), vWorldPos[1].Convert(), vWorldPos[3].Convert());
	m_Plane[static_cast<int>(PLANE_TYPE::PT_FAR)] =
		XMPlaneFromPoints(vWorldPos[5].Convert(), vWorldPos[4].Convert(), vWorldPos[6].Convert());
}

void CCamera::Collision(float fTime)
{
	CSceneComponent::Collision(fTime);
}

void CCamera::PreRender(float fTime)
{
	CSceneComponent::PreRender(fTime);
}

void CCamera::Render(float fTime)
{
	CSceneComponent::Render(fTime);
}

void CCamera::PostRender(float fTime)
{
	CSceneComponent::PostRender(fTime);
}

std::shared_ptr<CComponent> CCamera::Clone()
{
	return std::shared_ptr<CComponent>(new CCamera(*this));
}

void CCamera::Save(FILE* pFile)
{
	CSceneComponent::Save(pFile);

	fwrite(&m_fAngle, 4, 1, pFile);
	fwrite(&m_fDist, 4, 1, pFile);
	fwrite(&m_eType, 4, 1, pFile);
}

void CCamera::Load(FILE* pFile)
{
	CSceneComponent::Load(pFile);

	fread(&m_fAngle, 4, 1, pFile);
	fread(&m_fDist, 4, 1, pFile);
	fread(&m_eType, 4, 1, pFile);
}

void CCamera::ShowWindow()
{
	CSceneComponent::ShowWindow();

	std::string strName = GetName();

	if (strName == "")
	{
		strName = "None";
	}

	if(ImGui::Begin(strName.c_str()))
	{
		ImGui::InputInt("Type", reinterpret_cast<int*>(&m_eType));
		ImGui::InputFloat("Angle", &m_fAngle);
		ImGui::InputFloat("Dist", &m_fDist);
		ImGui::InputFloat3("TargetPos", &m_vTargetPos.x);
		ImGui::Checkbox("TargetPosMode", &m_bPosTarget);
		
	}
	ImGui::End();
}

bool CCamera::FrustumInPoint(const Vector3& vPos)
{
	for (int i = 0; i < static_cast<int>(PLANE_TYPE::PT_END); i++)
	{
		if (XMVectorGetX(XMPlaneDotCoord(m_Plane[i].Convert(), vPos.Convert())) > 0.f)
			return false;
	}

	return true;
}

bool CCamera::FrustumInSphere(const Vector3& vPos, float fRadius)
{
	for (int i = 0; i < static_cast<int>(PLANE_TYPE::PT_END); i++)
	{
		if (XMVectorGetX(XMPlaneDotCoord(m_Plane[i].Convert(), vPos.Convert())) > fRadius)
			return false;
	}

	return true;
}
