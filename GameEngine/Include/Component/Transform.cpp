#include "Transform.h"
#include "../Device.h"
#include "../Resource/ShaderManager.h"
#include "../CameraManager.h"
#include "Camera.h"
#include "../UI/imgui/imgui.h"
#include "../LightManager.h"
#include "Light.h"

#define GRAVITY	-1600.f

CTransform::CTransform()	:
	m_vVelocityScale(),
	m_vVelocityRot(),
	m_pScene(nullptr),
	m_pOwner(nullptr),
	m_pParent(nullptr),
	m_vRelativeScale(1.f, 1.f, 1.f),
	m_bInheritScale(true),
	m_bInheritRotX(true),
	m_bInheritRotY(true),
	m_bInheritRotZ(true),
	m_bInheritPos(true),
	m_bUpdateScale(true),
	m_bUpdateRot(true),
	m_bUpdatePos(true),
	m_bBone(false),
	m_bQuaternion(false),
	m_vRelativeQuaternion(0.f, 0.f, 0.f, 1.f),
	m_vWorldScale(1.f, 1.f, 1.f),
	m_vWorldQuaternion(0.f, 0.f, 0.f, 1.f),
	m_bGravity(false),
	m_vSpeed(0.f, 0.f, 0.f)
{
	for (int i = 0; i < AXIS_END; ++i)
	{
		m_vRelativeAxis[i] = Vector3::Axis[i];
		m_vWorldAxis[i] = Vector3::Axis[i];
	}

	m_vMeshSize = Vector3(1.f, 1.f, 1.f);
}

CTransform::CTransform(const CTransform& transform)
{
	*this = transform;

	m_ChildList.clear();
	m_pScene = nullptr;
	m_pOwner = nullptr;
	m_pParent = nullptr;

	m_bUpdateScale = true;
	m_bUpdateRot = true;
	m_bUpdatePos = true;
}

CTransform::~CTransform()
{
}

const Matrix& CTransform::GetMatScale() const
{
	return m_matScale;
}

const Matrix& CTransform::GetMatRot() const
{
	return m_matRot;
}

const Matrix& CTransform::GetMatPos() const
{
	return m_matPos;
}

const Matrix& CTransform::GetMatWorld() const
{
	return m_matWorld;
}

const Matrix& CTransform::GetMatBone() const
{
	return m_matBone;
}

const Matrix& CTransform::GetMatWVP() const
{
	return m_tCBuffer.matWVP;
}

void CTransform::SetBoneMatrix(const Matrix& mat)
{
	m_matBone = mat;
	m_bBone = true;

	std::list<CTransform*>::iterator iter = m_ChildList.begin();
	std::list<CTransform*>::iterator iterEnd = m_ChildList.end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->SetBoneMatrix(mat);
	}
}

void CTransform::DisableBoneMatrix()
{
	Vector3 vPos = {};
	memcpy_s(&vPos.x, 12, &m_matBone[3][0], 12);

	//m_vWorldPos += vPos;

	m_matBone = Matrix();
	m_bBone = false;

	std::list<CTransform*>::iterator iter = m_ChildList.begin();
	std::list<CTransform*>::iterator iterEnd = m_ChildList.end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->DisableBoneMatrix();
	}
}

void CTransform::SetWorldMatrix()
{
	if (m_bUpdatePos)
		m_matPos.Translate(m_vWorldPos);
	m_matWorld = m_matScale * m_matRot * m_matPos * m_matBone;
}

void CTransform::SetWorldMatrix(const Matrix& matWorld)
{
	m_matWorld = matWorld;
}

void CTransform::SetTexTransformMatrix(const Matrix& mat)
{
	m_tCBuffer.matTexTransform = mat;
	m_tCBuffer.matTexTransform.Transpose();
}

void CTransform::SetParentMatrix(const Matrix& mat)
{
	m_matParent = mat;
}

void CTransform::Start()
{
	m_vVelocityScale = Vector3::Zero;
	m_vVelocityRot = Vector3::Zero;
	m_vVelocity = Vector3::Zero;
	CCamera* pCam = GET_SINGLE(CCameraManager)->GetMainCam();
	if (pCam)
	{
		m_tCBuffer.matProjInv = pCam->GetProj();
		m_tCBuffer.matProjInv.Inverse();
		m_tCBuffer.matProjInv.Transpose();
	}
}

void CTransform::Update(float fTime)
{
}

void CTransform::PostUpdate(float fTime)
{
	if (m_bUpdateScale)
		m_matScale.Scaling(m_vWorldScale);

	if (m_bUpdateRot)
	{
		if (m_bQuaternion)
		{
			m_matRot = XMMatrixRotationQuaternion(m_vWorldQuaternion.Convert());
		}
		else
		{
			m_matRot.Rotation(m_vWorldRot);
		}
	}

	if (m_bUpdatePos)
		m_matPos.Translate(m_vWorldPos);

	m_matWorld = m_matScale * m_matRot * m_matPos * m_matBone;	//	크기 자전 이동 공전 부모

	if (m_bGravity)
	{
		m_vSpeed.y -= fTime * 500.f;

		if (m_vSpeed.y < GRAVITY)
			m_vSpeed.y = GRAVITY;

		AddWorldPos(m_vSpeed * fTime);
	}
}

void CTransform::SetTransform()
{
	Resolution tRS = RESOLUTION;
	CCamera* pCam = GET_SINGLE(CCameraManager)->GetMainCam();

	if (m_pOwner->GetSceneComponentType() == SCENE_COMPONENT_TYPE::SCT_UI)
	{
		pCam = GET_SINGLE(CCameraManager)->GetUICam();
	}		

	if (!pCam)
		return;

	m_tCBuffer.matWorld = m_matWorld;
	m_tCBuffer.matWorldInvTrans = m_matWorld;

	m_tCBuffer.matWorldInvTrans.m.r[3] = XMVectorSet(0.f, 0.f, 0.f, 1.f);
	m_tCBuffer.matWorldInvTrans.Inverse();
	m_tCBuffer.matWorldInvTrans.Transpose();

	m_tCBuffer.matView = pCam->GetView();
	m_tCBuffer.matViewInv = m_tCBuffer.matView;
	m_tCBuffer.matViewInv.Inverse();

	m_tCBuffer.matProj = pCam->GetProj();
	m_tCBuffer.matProjInv = m_tCBuffer.matProj;
	m_tCBuffer.matProjInv.Inverse();

	CLight* pLight = GET_SINGLE(CLightManager)->GetMainLight();

	Matrix matTex = {};

	if (pLight)
	{
		Matrix matLightVP = pLight->GetVP();
		m_tCBuffer.matShadowViewProj = matLightVP;

		m_tCBuffer.matLightWVPT = m_matWorld * matLightVP * matTex;
	}
	
	m_tCBuffer.matWV = m_matWorld * m_tCBuffer.matView;
	m_tCBuffer.matWVP = m_tCBuffer.matWV * m_tCBuffer.matProj;
	m_tCBuffer.vPivot = m_vPivot;
	m_tCBuffer.vMeshSize = m_vMeshSize;

	m_tCBuffer.matView.Transpose();
	m_tCBuffer.matWorld.Transpose();
	m_tCBuffer.matProj.Transpose();
	m_tCBuffer.matWV.Transpose();
	m_tCBuffer.matWVP.Transpose();
	m_tCBuffer.matLightWVPT.Transpose();
	m_tCBuffer.matShadowViewProj.Transpose();
	m_tCBuffer.matProjInv.Transpose();
	m_tCBuffer.matViewInv.Transpose();

	GET_SINGLE(CShaderManager)->UpdateCBuffer("Transform", &m_tCBuffer);

	m_vVelocityScale = Vector3::Zero;
	m_vVelocityRot = Vector3::Zero;
	m_vVelocity = Vector3::Zero;
}

void CTransform::SetTransformShadow()
{
	Resolution tRS = RESOLUTION;

	CLight* pLight = GET_SINGLE(CLightManager)->GetMainLight();

	m_tCBuffer.matWorld = m_matWorld;
	m_tCBuffer.matWorldInvTrans = m_matWorld;

	m_tCBuffer.matWorldInvTrans.m.r[3] = XMVectorSet(0.f, 0.f, 0.f, 1.f);
	m_tCBuffer.matWorldInvTrans.Inverse();
	m_tCBuffer.matWorldInvTrans.Transpose();

	if (pLight)
	{
		m_tCBuffer.matView = pLight->GetView();
		m_tCBuffer.matProj = pLight->GetProj();
	}

	m_tCBuffer.matWV = m_matWorld * m_tCBuffer.matView;
	m_tCBuffer.matWVP = m_tCBuffer.matWV * m_tCBuffer.matProj;
	m_tCBuffer.vPivot = m_vPivot;
	m_tCBuffer.vMeshSize = m_vMeshSize;

	m_tCBuffer.matView.Transpose();
	m_tCBuffer.matWorld.Transpose();
	m_tCBuffer.matProj.Transpose();
	m_tCBuffer.matWV.Transpose();
	m_tCBuffer.matWVP.Transpose();

	GET_SINGLE(CShaderManager)->UpdateCBuffer("Transform", &m_tCBuffer);

	m_vVelocityScale = Vector3::Zero;
	m_vVelocityRot = Vector3::Zero;
	m_vVelocity = Vector3::Zero;
}

std::shared_ptr<CTransform> CTransform::Clone()
{
	return std::shared_ptr<CTransform>(new CTransform(*this));
}

void CTransform::Save(FILE* pFile)
{
	fwrite(&m_vVelocityScale, sizeof(Vector3), 1, pFile);
	fwrite(&m_vVelocityRot, sizeof(Vector3), 1, pFile);
	fwrite(&m_vVelocity, sizeof(Vector3), 1, pFile);
	fwrite(&m_vRelativeScale, sizeof(Vector3), 1, pFile);
	fwrite(&m_vRelativeRot, sizeof(Vector3), 1, pFile);
	fwrite(&m_vRelativePos, sizeof(Vector3), 1, pFile);
	fwrite(m_vRelativeAxis, sizeof(Vector3), AXIS_END, pFile);
	fwrite(&m_bInheritScale, sizeof(bool), 1, pFile);
	fwrite(&m_bInheritRotX, sizeof(bool), 1, pFile);
	fwrite(&m_bInheritRotY, sizeof(bool), 1, pFile);
	fwrite(&m_bInheritRotZ, sizeof(bool), 1, pFile);

	fwrite(&m_vWorldScale, sizeof(Vector3), 1, pFile);
	fwrite(&m_vWorldRot, sizeof(Vector3), 1, pFile);
	fwrite(&m_vWorldPos, sizeof(Vector3), 1, pFile);
	fwrite(m_vWorldAxis, sizeof(Vector3), AXIS_END, pFile);
	fwrite(&m_vPivot, sizeof(Vector3), 1, pFile);
	fwrite(&m_vMeshSize, sizeof(Vector3), 1, pFile);
}

void CTransform::Load(FILE* pFile)
{
	fread(&m_vVelocityScale, sizeof(Vector3), 1, pFile);
	fread(&m_vVelocityRot, sizeof(Vector3), 1, pFile);
	fread(&m_vVelocity, sizeof(Vector3), 1, pFile);
	fread(&m_vRelativeScale, sizeof(Vector3), 1, pFile);
	fread(&m_vRelativeRot, sizeof(Vector3), 1, pFile);
	fread(&m_vRelativePos, sizeof(Vector3), 1, pFile);
	fread(m_vRelativeAxis, sizeof(Vector3), AXIS_END, pFile);
	fread(&m_bInheritScale, sizeof(bool), 1, pFile);
	fread(&m_bInheritRotX, sizeof(bool), 1, pFile);
	fread(&m_bInheritRotY, sizeof(bool), 1, pFile);
	fread(&m_bInheritRotZ, sizeof(bool), 1, pFile);

	fread(&m_vWorldScale, sizeof(Vector3), 1, pFile);
	fread(&m_vWorldRot, sizeof(Vector3), 1, pFile);
	fread(&m_vWorldPos, sizeof(Vector3), 1, pFile);
	fread(m_vWorldAxis, sizeof(Vector3), AXIS_END, pFile);
	fread(&m_vPivot, sizeof(Vector3), 1, pFile);
	fread(&m_vMeshSize, sizeof(Vector3), 1, pFile);

	m_bUpdatePos = true;
	m_bUpdateRot = true;
	m_bUpdateScale = true;
}

void CTransform::ShowWindow()
{
	const char* pName = m_pOwner->GetName().c_str();

	if (!strcmp(pName, ""))
	{
		pName = "NONE";
	}
	//if (ImGui::Begin("Transform"))
	//if (ImGui::Begin(pName))
	{
		ImGui::Text("Transform");
		Vector3 vRelativePos = GetRelativePos();
		if (ImGui::InputFloat3("Relative Position", &vRelativePos.x))
		{
			SetRelativePos(vRelativePos);
		}
		Vector3 vRelativeScale = GetRelativeScale();
		if (ImGui::InputFloat3("Relative Scale", &vRelativeScale.x))
		{
			SetRelativeScale(vRelativeScale);
		}
		Vector3 vRelativeRotation = GetRelativeRot();
		if (ImGui::InputFloat3("Relative Rotation", &vRelativeRotation.x))
		{
			SetRelativeRot(vRelativeRotation);
		}
		Vector3 vPosition = GetWorldPos();
		if (ImGui::InputFloat3("Position", &vPosition.x))
		{
			SetWorldPos(vPosition);
		}
		Vector3 vScale = GetWorldScale();
		if (ImGui::InputFloat3("Scale", &vScale.x))
		{
			SetWorldScale(vScale);
		}
		Vector3 vRotation = GetWorldRot();
		if (ImGui::InputFloat3("Rotation", &vRotation.x))
		{
			SetWorldRot(vRotation);
		}
		Vector3 vPivot = GetPivot();
		if (ImGui::InputFloat3("Pivot", &vPivot.x))
		{
			SetPivot(vPivot);
		}
		Vector3 vMeshSize = GetMeshSize();
		if (ImGui::InputFloat3("MeshSize", &vMeshSize.x))
		{
			SetMeshSize(vMeshSize);
		}

		ImGui::InputFloat4("World Matrix row: 0", &m_matWorld[0][0]);
		ImGui::InputFloat4("World Matrix row: 1", &m_matWorld[1][0]);
		ImGui::InputFloat4("World Matrix row: 2", &m_matWorld[2][0]);
		ImGui::InputFloat4("World Matrix row: 3", &m_matWorld[3][0]);

		ImGui::InputFloat4("Scale Matrix row: 0", &m_matScale[0][0]);
		ImGui::InputFloat4("Scale Matrix row: 1", &m_matScale[1][0]);
		ImGui::InputFloat4("Scale Matrix row: 2", &m_matScale[2][0]);
		ImGui::InputFloat4("Scale Matrix row: 3", &m_matScale[3][0]);

		ImGui::InputFloat4("Rotation Matrix row: 0", &m_matRot[0][0]);
		ImGui::InputFloat4("Rotation Matrix row: 1", &m_matRot[1][0]);
		ImGui::InputFloat4("Rotation Matrix row: 2", &m_matRot[2][0]);
		ImGui::InputFloat4("Rotation Matrix row: 3", &m_matRot[3][0]);

		ImGui::InputFloat4("Bone Matrix row: 0", &m_matBone[0][0]);
		ImGui::InputFloat4("Bone Matrix row: 1", &m_matBone[1][0]);
		ImGui::InputFloat4("Bone Matrix row: 2", &m_matBone[2][0]);
		ImGui::InputFloat4("Bone Matrix row: 3", &m_matBone[3][0]);

		ImGui::InputFloat4("WVP Matrix row: 0", &m_tCBuffer.matWVP[0][0]);
		ImGui::InputFloat4("WVP Matrix row: 1", &m_tCBuffer.matWVP[1][0]);
		ImGui::InputFloat4("WVP Matrix row: 2", &m_tCBuffer.matWVP[2][0]);
		ImGui::InputFloat4("WVP Matrix row: 3", &m_tCBuffer.matWVP[3][0]);
		bool bScale = IsInheritScale();
		if (ImGui::Checkbox("InheritScale", reinterpret_cast<bool*>(&bScale)))
		{
			SetInheritScale(bScale);
		}
		bool bRotX = IsInheritRotX();
		if (ImGui::Checkbox("InheritRotX", reinterpret_cast<bool*>(&m_bInheritRotX)))
		{
			SetInheritRotX(bRotX);
		}
		bool bRotY = IsInheritRotY();
		if (ImGui::Checkbox("InheritRotY", reinterpret_cast<bool*>(&m_bInheritRotY)))
		{
			SetInheritRotY(bRotY);
		}
		bool bRotZ = IsInheritRotZ();
		if (ImGui::Checkbox("InheritRotZ", reinterpret_cast<bool*>(&m_bInheritRotZ)))
		{
			SetInheritRotZ(bRotZ);
		}
	}
	//ImGui::End();
}

void CTransform::InheritScale()
{
	if (m_bInheritScale)
		m_vWorldScale = m_vRelativeScale * m_pParent->GetWorldScale();

	m_bUpdateScale = true;
	std::list<CTransform*>::iterator iter = m_ChildList.begin();
	std::list<CTransform*>::iterator iterEnd = m_ChildList.end();

	for (;iter!=iterEnd;++iter)
	{
		(*iter)->InheritScale();
	}
}

void CTransform::InheritRot()
{
	Matrix matRelativeRot;
	Matrix matWorldRot;

	if (m_pParent->IsQuaternion())
	{
		m_bQuaternion = true;

		const Vector4& vQ = m_pParent->GetWorldQRot();

		m_vWorldQuaternion = XMQuaternionMultiply(m_vRelativeQuaternion.Convert(), vQ.Convert());

		matRelativeRot = XMMatrixRotationQuaternion(m_vRelativeQuaternion.Convert());
		matWorldRot = XMMatrixRotationQuaternion(m_vWorldQuaternion.Convert());
	}
	else
	{

		if (m_bInheritRotX)
			m_vWorldRot.x = m_vRelativeRot.x + m_pParent->GetWorldRot().x;

		if (m_bInheritRotY)
			m_vWorldRot.y = m_vRelativeRot.y + m_pParent->GetWorldRot().y;

		if (m_bInheritRotZ)
			m_vWorldRot.z = m_vRelativeRot.z + m_pParent->GetWorldRot().z;

		matRelativeRot.Rotation(m_vRelativeRot);
		matWorldRot.Rotation(m_vWorldRot);
	}

	m_vRelativeAxis[AXIS_X] = Vector3::Axis[AXIS_X].TranslationNorm(matRelativeRot);
	m_vRelativeAxis[AXIS_X].Normalize();
	m_vWorldAxis[AXIS_X] = Vector3::Axis[AXIS_X].TranslationNorm(matWorldRot);
	m_vWorldAxis[AXIS_X].Normalize();

	m_vRelativeAxis[AXIS_Y] = Vector3::Axis[AXIS_Y].TranslationNorm(matRelativeRot);
	m_vRelativeAxis[AXIS_Y].Normalize();
	m_vWorldAxis[AXIS_Y] = Vector3::Axis[AXIS_Y].TranslationNorm(matWorldRot);
	m_vWorldAxis[AXIS_Y].Normalize();

	m_vRelativeAxis[AXIS_Z] = Vector3::Axis[AXIS_Z].TranslationNorm(matRelativeRot);
	m_vRelativeAxis[AXIS_Z].Normalize();
	m_vWorldAxis[AXIS_Z] = Vector3::Axis[AXIS_Z].TranslationNorm(matWorldRot);
	m_vWorldAxis[AXIS_Z].Normalize();

	if (m_pParent)
		InheritPos();

	m_bUpdateRot = true;

	std::list<CTransform*>::iterator iter = m_ChildList.begin();
	std::list<CTransform*>::iterator iterEnd = m_ChildList.end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->InheritRot();
	}
}

void CTransform::InheritPos()
{
	Matrix mat;

	if (m_bInheritPos)
	{
		if (m_pParent->IsQuaternion())
		{
			mat = XMMatrixRotationQuaternion(m_pParent->GetWorldQRot().Convert());
		}

		else
		{
			mat.Rotation(m_pParent->GetWorldRot());
		}

		Vector3 vPos = m_pParent->GetWorldPos();

		memcpy_s(&mat._41, 16, &vPos, 12);

		m_vWorldPos = m_vRelativePos.TranslationCoor(mat);
	}
	else
	{
		m_vWorldPos = m_vRelativePos + m_pParent->GetWorldPos();
	}

	m_bUpdatePos = true;

	std::list<CTransform*>::iterator iter = m_ChildList.begin();
	std::list<CTransform*>::iterator iterEnd = m_ChildList.end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->InheritPos();
	}
}

void CTransform::SetRelativeScale(const Vector3& v)
{
	m_vVelocityScale += v - m_vRelativeScale;

	m_vRelativeScale = v;

	m_vWorldScale = m_vRelativeScale;

	if (m_pParent && m_bInheritScale)
		m_vWorldScale = m_vRelativeScale * m_pParent->GetWorldScale();

	m_bUpdateScale = true;

	std::list<CTransform*>::iterator iter = m_ChildList.begin();
	std::list<CTransform*>::iterator iterEnd = m_ChildList.end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->InheritScale();
	}
}

void CTransform::SetRelativeScale(float x, float y, float z)
{
	m_vVelocityScale += Vector3(x,y,z) - m_vRelativeScale;

	m_vRelativeScale = Vector3(x, y, z);

	m_vWorldScale = m_vRelativeScale;

	if (m_pParent && m_bInheritScale)
		m_vWorldScale = m_vRelativeScale * m_pParent->GetWorldScale();

	m_bUpdateScale = true;

	std::list<CTransform*>::iterator iter = m_ChildList.begin();
	std::list<CTransform*>::iterator iterEnd = m_ChildList.end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->InheritScale();
	}
}

void CTransform::SetRelativeRot(const Vector3& v)
{
	m_vVelocityRot += v - m_vRelativeRot;

	m_vRelativeRot = v;

	m_vWorldRot = m_vRelativeRot;

	if (m_pParent)
	{
		if (m_bInheritRotX)
			m_vWorldRot.x = m_vRelativeRot.x + m_pParent->GetWorldRot().x;

		if (m_bInheritRotY)
			m_vWorldRot.y = m_vRelativeRot.y + m_pParent->GetWorldRot().y;

		if (m_bInheritRotZ)
			m_vWorldRot.z = m_vRelativeRot.z + m_pParent->GetWorldRot().z;
	}

	m_bUpdateRot = true;

	Matrix matRot;

	matRot.Rotation(m_vRelativeRot);

	for (int i = 0; i < AXIS_END; ++i)
	{
		m_vRelativeAxis[i] = Vector3::Axis[i].TranslationNorm(matRot);
		m_vRelativeAxis[i].Normalize();
	}

	matRot.Rotation(m_vWorldRot);

	for (int i = 0; i < AXIS_END; ++i)
	{
		m_vWorldAxis[i] = Vector3::Axis[i].TranslationNorm(matRot);
		m_vWorldAxis[i].Normalize();
	}

	std::list<CTransform*>::iterator iter = m_ChildList.begin();
	std::list<CTransform*>::iterator iterEnd = m_ChildList.end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->InheritRot();
	}
}

void CTransform::SetRelativeRot(float x, float y, float z)
{
	m_vVelocityRot += Vector3(x, y, z) - m_vRelativeRot;

	m_vRelativeRot = Vector3(x, y, z);

	m_vWorldRot = m_vRelativeRot;

	if (m_pParent)
	{
		if (m_bInheritRotX)
			m_vWorldRot.x = m_vRelativeRot.x + m_pParent->GetWorldRot().x;

		if (m_bInheritRotY)
			m_vWorldRot.y = m_vRelativeRot.y + m_pParent->GetWorldRot().y;

		if (m_bInheritRotZ)
			m_vWorldRot.z = m_vRelativeRot.z + m_pParent->GetWorldRot().z;
	}

	m_bUpdateRot = true;

	Matrix matRot;

	matRot.Rotation(m_vRelativeRot);

	for (int i = 0; i < AXIS_END; ++i)
	{
		m_vRelativeAxis[i] = Vector3::Axis[i].TranslationNorm(matRot);
		m_vRelativeAxis[i].Normalize();
	}

	matRot.Rotation(m_vWorldRot);

	for (int i = 0; i < AXIS_END; ++i)
	{
		m_vWorldAxis[i] = Vector3::Axis[i].TranslationNorm(matRot);
		m_vWorldAxis[i].Normalize();
	}

	std::list<CTransform*>::iterator iter = m_ChildList.begin();
	std::list<CTransform*>::iterator iterEnd = m_ChildList.end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->InheritRot();
	}
}

void CTransform::SetRelativeRotX(float x)
{
	m_vVelocityRot.x += x - m_vRelativeRot.x;

	m_vRelativeRot.x = x;

	m_vWorldRot.x = m_vRelativeRot.x;

	if (m_pParent)
	{
		if (m_bInheritRotX)
			m_vWorldRot.x = m_vRelativeRot.x + m_pParent->GetWorldRot().x;
	}

	m_bUpdateRot = true;

	Matrix matRot;

	matRot.Rotation(m_vRelativeRot);

	for (int i = 0; i < AXIS_END; ++i)
	{
		m_vRelativeAxis[i] = Vector3::Axis[i].TranslationNorm(matRot);
		m_vRelativeAxis[i].Normalize();
	}

	matRot.Rotation(m_vWorldRot);

	for (int i = 0; i < AXIS_END; ++i)
	{
		m_vWorldAxis[i] = Vector3::Axis[i].TranslationNorm(matRot);
		m_vWorldAxis[i].Normalize();
	}

	std::list<CTransform*>::iterator iter = m_ChildList.begin();
	std::list<CTransform*>::iterator iterEnd = m_ChildList.end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->InheritRot();
	}
}

void CTransform::SetRelativeRotY(float y)
{
	m_vVelocityRot.y += y - m_vRelativeRot.y;

	m_vRelativeRot.y = y;

	m_vWorldRot.y = m_vRelativeRot.y;

	if (m_pParent)
	{
		if (m_bInheritRotY)
			m_vWorldRot.y = m_vRelativeRot.y + m_pParent->GetWorldRot().y;
	}

	m_bUpdateRot = true;

	Matrix matRot;

	matRot.Rotation(m_vRelativeRot);

	for (int i = 0; i < AXIS_END; ++i)
	{
		m_vRelativeAxis[i] = Vector3::Axis[i].TranslationNorm(matRot);
		m_vRelativeAxis[i].Normalize();
	}

	matRot.Rotation(m_vWorldRot);

	for (int i = 0; i < AXIS_END; ++i)
	{
		m_vWorldAxis[i] = Vector3::Axis[i].TranslationNorm(matRot);
		m_vWorldAxis[i].Normalize();
	}

	std::list<CTransform*>::iterator iter = m_ChildList.begin();
	std::list<CTransform*>::iterator iterEnd = m_ChildList.end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->InheritRot();
	}
}

void CTransform::SetRelativeRotZ(float z)
{
	m_vVelocityRot.z += z - m_vRelativeRot.z;

	m_vRelativeRot.z = z;

	m_vWorldRot.z = m_vRelativeRot.z;

	if (m_pParent)
	{
		if (m_bInheritRotZ)
			m_vWorldRot.z = m_vRelativeRot.z + m_pParent->GetWorldRot().z;
	}

	m_bUpdateRot = true;

	Matrix matRot;

	matRot.Rotation(m_vRelativeRot);

	for (int i = 0; i < AXIS_END; ++i)
	{
		m_vRelativeAxis[i] = Vector3::Axis[i].TranslationNorm(matRot);
		m_vRelativeAxis[i].Normalize();
	}

	matRot.Rotation(m_vWorldRot);

	for (int i = 0; i < AXIS_END; ++i)
	{
		m_vWorldAxis[i] = Vector3::Axis[i].TranslationNorm(matRot);
		m_vWorldAxis[i].Normalize();
	}

	std::list<CTransform*>::iterator iter = m_ChildList.begin();
	std::list<CTransform*>::iterator iterEnd = m_ChildList.end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->InheritRot();
	}
}

void CTransform::SetRelativePos(const Vector3& v)
{
	m_vVelocity += v - m_vRelativePos;

	m_vRelativePos = v;

	m_vWorldPos = m_vRelativePos;

	if (m_pParent)
	{
		Matrix mat;

		mat.Rotation(m_pParent->GetWorldRot());

		Vector3 vPos = m_pParent->GetWorldPos();

		memcpy_s(&mat._41, 16, &vPos, 12);

		m_vWorldPos = m_vRelativePos.TranslationCoor(mat);
	}

	m_bUpdatePos = true;

	std::list<CTransform*>::iterator iter = m_ChildList.begin();
	std::list<CTransform*>::iterator iterEnd = m_ChildList.end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->InheritPos();
	}
}

void CTransform::SetRelativePos(float x, float y, float z)
{
	m_vVelocity += Vector3(x,y,z) - m_vRelativePos;

	m_vRelativePos = Vector3(x, y, z);

	m_vWorldPos = m_vRelativePos;

	if (m_pParent)
	{
		Matrix mat;

		mat.Rotation(m_pParent->GetWorldRot());

		Vector3 vPos = m_pParent->GetWorldPos();

		memcpy_s(&mat._41, 16, &vPos, 12);

		m_vWorldPos = m_vRelativePos.TranslationCoor(mat);
	}	

	m_bUpdatePos = true;

	std::list<CTransform*>::iterator iter = m_ChildList.begin();
	std::list<CTransform*>::iterator iterEnd = m_ChildList.end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->InheritPos();
	}
}

void CTransform::SetRelativePosX(float x)
{
	m_vVelocity.x += x - m_vRelativePos.x;

	m_vRelativePos.x = x;

	m_vWorldPos = m_vRelativePos;

	if (m_pParent)
	{
		Matrix mat;

		mat.Rotation(m_pParent->GetWorldRot());

		Vector3 vPos = m_pParent->GetWorldPos();

		memcpy_s(&mat._41, 16, &vPos, 12);

		m_vWorldPos = m_vRelativePos.TranslationCoor(mat);
	}

	m_bUpdatePos = true;

	std::list<CTransform*>::iterator iter = m_ChildList.begin();
	std::list<CTransform*>::iterator iterEnd = m_ChildList.end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->InheritPos();
	}
}

void CTransform::SetRelativePosY(float y)
{
	m_vVelocity.y += y - m_vRelativePos.y;

	m_vRelativePos.y = y;

	m_vWorldPos = m_vRelativePos;

	if (m_pParent)
	{
		Matrix mat;

		mat.Rotation(m_pParent->GetWorldRot());

		Vector3 vPos = m_pParent->GetWorldPos();

		memcpy_s(&mat._41, 16, &vPos, 12);

		m_vWorldPos = m_vRelativePos.TranslationCoor(mat);
	}

	m_bUpdatePos = true;

	std::list<CTransform*>::iterator iter = m_ChildList.begin();
	std::list<CTransform*>::iterator iterEnd = m_ChildList.end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->InheritPos();
	}
}

void CTransform::SetRelativePosZ(float z)
{
	m_vVelocity.z += z - m_vRelativePos.z;

	m_vRelativePos.z = z;

	m_vWorldPos = m_vRelativePos;

	if (m_pParent)
	{
		Matrix mat;

		mat.Rotation(m_pParent->GetWorldRot());

		Vector3 vPos = m_pParent->GetWorldPos();

		memcpy_s(&mat._41, 16, &vPos, 12);

		m_vWorldPos = m_vRelativePos.TranslationCoor(mat);
	}

	m_bUpdatePos = true;

	std::list<CTransform*>::iterator iter = m_ChildList.begin();
	std::list<CTransform*>::iterator iterEnd = m_ChildList.end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->InheritPos();
	}
}

void CTransform::AddRelativeScale(const Vector3& v)
{
	m_vVelocityScale += v;

	m_vRelativeScale += v;

	m_vWorldScale = m_vRelativeScale;

	if (m_pParent && m_bInheritScale)
		m_vWorldScale = m_vRelativeScale * m_pParent->GetWorldScale();

	m_bUpdateScale = true;

	std::list<CTransform*>::iterator iter = m_ChildList.begin();
	std::list<CTransform*>::iterator iterEnd = m_ChildList.end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->InheritScale();
	}
}

void CTransform::AddRelativeScale(float x, float y, float z)
{
	m_vVelocityScale += Vector3(x, y, z);

	m_vRelativeScale += Vector3(x, y, z);

	m_vWorldScale = m_vRelativeScale;

	if (m_pParent && m_bInheritScale)
		m_vWorldScale = m_vRelativeScale * m_pParent->GetWorldScale();

	m_bUpdateScale = true;

	std::list<CTransform*>::iterator iter = m_ChildList.begin();
	std::list<CTransform*>::iterator iterEnd = m_ChildList.end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->InheritScale();
	}
}

void CTransform::AddRelativeRot(const Vector3& v)
{
	m_vVelocityRot += v;

	m_vRelativeRot += v;

	m_vWorldRot = m_vRelativeRot;

	if (m_pParent)
	{
		if (m_bInheritRotX)
			m_vWorldRot.x = m_vRelativeRot.x + m_pParent->GetWorldRot().x;

		if (m_bInheritRotY)
			m_vWorldRot.y = m_vRelativeRot.y + m_pParent->GetWorldRot().y;

		if (m_bInheritRotX)
			m_vWorldRot.z = m_vRelativeRot.z + m_pParent->GetWorldRot().z;
	}		

	m_bUpdateRot = true;

	Matrix matRot;

	matRot.Rotation(m_vRelativeRot);

	for (int i = 0; i < AXIS_END; ++i)
	{
		m_vRelativeAxis[i] = Vector3::Axis[i].TranslationNorm(matRot);
		m_vRelativeAxis[i].Normalize();
	}

	matRot.Rotation(m_vWorldRot);

	for (int i = 0; i < AXIS_END; ++i)
	{
		m_vWorldAxis[i] = Vector3::Axis[i].TranslationNorm(matRot);
		m_vWorldAxis[i].Normalize();
	}

	std::list<CTransform*>::iterator iter = m_ChildList.begin();
	std::list<CTransform*>::iterator iterEnd = m_ChildList.end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->InheritRot();
	}
}

void CTransform::AddRelativeRot(float x, float y, float z)
{
	m_vVelocityRot += Vector3(x, y, z);

	m_vRelativeRot += Vector3(x, y, z);

	m_vWorldRot = m_vRelativeRot;

	if (m_pParent)
	{
		if (m_bInheritRotX)
			m_vWorldRot.x = m_vRelativeRot.x + m_pParent->GetWorldRot().x;

		if (m_bInheritRotY)
			m_vWorldRot.y = m_vRelativeRot.y + m_pParent->GetWorldRot().y;

		if (m_bInheritRotX)
			m_vWorldRot.z = m_vRelativeRot.z + m_pParent->GetWorldRot().z;
	}

	m_bUpdateRot = true;

	Matrix matRot;

	matRot.Rotation(m_vRelativeRot);

	for (int i = 0; i < AXIS_END; ++i)
	{
		m_vRelativeAxis[i] = Vector3::Axis[i].TranslationNorm(matRot);
		m_vRelativeAxis[i].Normalize();
	}

	matRot.Rotation(m_vWorldRot);

	for (int i = 0; i < AXIS_END; ++i)
	{
		m_vWorldAxis[i] = Vector3::Axis[i].TranslationNorm(matRot);
		m_vWorldAxis[i].Normalize();
	}

	std::list<CTransform*>::iterator iter = m_ChildList.begin();
	std::list<CTransform*>::iterator iterEnd = m_ChildList.end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->InheritRot();
	}
}

void CTransform::AddRelativeRotX(float x)
{
	m_vVelocityRot.x += x;

	m_vRelativeRot.x += x;

	m_vWorldRot = m_vRelativeRot;

	if (m_pParent && m_bInheritRotX)
		m_vWorldRot.x = m_vRelativeRot.x + m_pParent->GetWorldRot().x;

	if (m_vWorldRot.x > 360.f)
		m_vWorldRot.x -= 360.f;

	else if (m_vWorldRot.x < -360.f)
		m_vWorldRot.x += 360.f;

	if (m_vRelativeRot.x > 360.f)
		m_vRelativeRot.x -= 360.f;

	else if (m_vRelativeRot.x < -360.f)
		m_vRelativeRot.x += 360.f;

	m_bUpdateRot = true;

	Matrix matRot;

	matRot.Rotation(m_vRelativeRot);

	for (int i = 0; i < AXIS_END; ++i)
	{
		m_vRelativeAxis[i] = Vector3::Axis[i].TranslationNorm(matRot);
		m_vRelativeAxis[i].Normalize();
	}

	matRot.Rotation(m_vWorldRot);

	for (int i = 0; i < AXIS_END; ++i)
	{
		m_vWorldAxis[i] = Vector3::Axis[i].TranslationNorm(matRot);
		m_vWorldAxis[i].Normalize();
	}

	std::list<CTransform*>::iterator iter = m_ChildList.begin();
	std::list<CTransform*>::iterator iterEnd = m_ChildList.end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->InheritRot();
	}
}

void CTransform::AddRelativeRotY(float y)
{
	m_vVelocityRot.y += y;

	m_vRelativeRot.y += y;

	m_vWorldRot = m_vRelativeRot;

	if (m_pParent && m_bInheritRotY)
		m_vWorldRot.y = m_vRelativeRot.y + m_pParent->GetWorldRot().y;

	if (m_vWorldRot.y > 360.f)
		m_vWorldRot.y -= 360.f;

	else if (m_vWorldRot.y < -360.f)
		m_vWorldRot.y += 360.f;

	if (m_vRelativeRot.y > 360.f)
		m_vRelativeRot.y -= 360.f;

	else if (m_vRelativeRot.y < -360.f)
		m_vRelativeRot.y += 360.f;

	m_bUpdateRot = true;

	Matrix matRot;

	matRot.Rotation(m_vRelativeRot);

	for (int i = 0; i < AXIS_END; ++i)
	{
		m_vRelativeAxis[i] = Vector3::Axis[i].TranslationNorm(matRot);
		m_vRelativeAxis[i].Normalize();
	}

	matRot.Rotation(m_vWorldRot);

	for (int i = 0; i < AXIS_END; ++i)
	{
		m_vWorldAxis[i] = Vector3::Axis[i].TranslationNorm(matRot);
		m_vWorldAxis[i].Normalize();
	}

	std::list<CTransform*>::iterator iter = m_ChildList.begin();
	std::list<CTransform*>::iterator iterEnd = m_ChildList.end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->InheritRot();
	}
}

void CTransform::AddRelativeRotZ(float z)
{
	m_vVelocityRot.z += z;

	m_vRelativeRot.z += z;

	m_vWorldRot = m_vRelativeRot;

	if (m_pParent && m_bInheritRotZ)
		m_vWorldRot.z = m_vRelativeRot.z + m_pParent->GetWorldRot().z;

	if (m_vWorldRot.z > 360.f)
		m_vWorldRot.z -= 360.f;

	else if (m_vWorldRot.z < -360.f)
		m_vWorldRot.z += 360.f;

	if (m_vRelativeRot.z > 360.f)
		m_vRelativeRot.z -= 360.f;

	else if (m_vRelativeRot.z < -360.f)
		m_vRelativeRot.z += 360.f;

	m_bUpdateRot = true;

	Matrix matRot;

	matRot.Rotation(m_vRelativeRot);

	for (int i = 0; i < AXIS_END; ++i)
	{
		m_vRelativeAxis[i] = Vector3::Axis[i].TranslationNorm(matRot);
		m_vRelativeAxis[i].Normalize();
	}

	matRot.Rotation(m_vWorldRot);

	for (int i = 0; i < AXIS_END; ++i)
	{
		m_vWorldAxis[i] = Vector3::Axis[i].TranslationNorm(matRot);
		m_vWorldAxis[i].Normalize();
	}

	std::list<CTransform*>::iterator iter = m_ChildList.begin();
	std::list<CTransform*>::iterator iterEnd = m_ChildList.end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->InheritRot();
	}
}

void CTransform::AddRelativePos(const Vector3& v)
{
	m_vVelocity += v;

	m_vRelativePos += v;

	m_vWorldPos = m_vRelativePos;

	if (m_pParent)
	{
		Matrix mat;

		mat.Rotation(m_pParent->GetWorldRot());

		Vector3 vPos = m_pParent->GetWorldPos();

		memcpy_s(&mat._41, 16, &vPos, 12);

		m_vWorldPos = m_vRelativePos.TranslationCoor(mat);
	}

	m_bUpdatePos = true;

	std::list<CTransform*>::iterator iter = m_ChildList.begin();
	std::list<CTransform*>::iterator iterEnd = m_ChildList.end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->InheritPos();
	}
}

void CTransform::AddRelativePos(float x, float y, float z)
{
	m_vVelocity += Vector3(x,y,z);

	m_vRelativePos += Vector3(x, y, z);

	m_vWorldPos = m_vRelativePos;

	if (m_pParent)
	{
		Matrix mat;

		mat.Rotation(m_pParent->GetWorldRot());

		Vector3 vPos = m_pParent->GetWorldPos();

		memcpy_s(&mat._41, 16, &vPos, 12);

		m_vWorldPos = m_vRelativePos.TranslationCoor(mat);
	}

	m_bUpdatePos = true;

	std::list<CTransform*>::iterator iter = m_ChildList.begin();
	std::list<CTransform*>::iterator iterEnd = m_ChildList.end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->InheritPos();
	}
}

void CTransform::AddRelativePosX(float x)
{
	m_vVelocity.x += x;
	m_vRelativePos.x += x;
	m_vWorldPos = m_vRelativePos;

	if (m_pParent)
	{
		Matrix mat;

		mat.Rotation(m_pParent->GetWorldRot());

		Vector3 vPos = m_pParent->GetWorldPos();

		memcpy_s(&mat._41, 16, &vPos, 12);

		m_vWorldPos = m_vRelativePos.TranslationCoor(mat);
	}

	m_bUpdatePos = true;

	std::list<CTransform*>::iterator iter = m_ChildList.begin();
	std::list<CTransform*>::iterator iterEnd = m_ChildList.end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->InheritPos();
	}
}

void CTransform::AddRelativePosY(float y)
{
	m_vVelocity.y += y;
	m_vRelativePos.y += y;
	m_vWorldPos = m_vRelativePos;

	if (m_pParent)
	{
		Matrix mat;

		mat.Rotation(m_pParent->GetWorldRot());

		Vector3 vPos = m_pParent->GetWorldPos();

		memcpy_s(&mat._41, 16, &vPos, 12);

		m_vWorldPos = m_vRelativePos.TranslationCoor(mat);
	}

	m_bUpdatePos = true;

	std::list<CTransform*>::iterator iter = m_ChildList.begin();
	std::list<CTransform*>::iterator iterEnd = m_ChildList.end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->InheritPos();
	}
}

void CTransform::AddRelativePosZ(float z)
{
	m_vVelocity.z += z;
	m_vRelativePos.z += z;
	m_vWorldPos = m_vRelativePos;

	if (m_pParent)
	{
		Matrix mat;

		mat.Rotation(m_pParent->GetWorldRot());

		Vector3 vPos = m_pParent->GetWorldPos();

		memcpy_s(&mat._41, 16, &vPos, 12);

		m_vWorldPos = m_vRelativePos.TranslationCoor(mat);
	}

	m_bUpdatePos = true;

	std::list<CTransform*>::iterator iter = m_ChildList.begin();
	std::list<CTransform*>::iterator iterEnd = m_ChildList.end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->InheritPos();
	}
}

const Vector3& CTransform::GetRelativeScale() const
{
	return m_vRelativeScale;
}

const Vector3& CTransform::GetRelativeRot() const
{
	return m_vRelativeRot;
}

const Vector3& CTransform::GetRelativePos() const
{
	return m_vRelativePos;
}

const Vector3& CTransform::GetRelativeAxis(AXIS axis) const
{
	return m_vRelativeAxis[axis];
}

const Vector3& CTransform::GetWorldScale() const
{
	return m_vWorldScale;
}

const Vector3& CTransform::GetWorldRot() const
{
	return m_vWorldRot;
}

const Vector3& CTransform::GetWorldPos() const
{
	/*if (m_bBone)
	{
		Vector3 vPos = {};

		memcpy_s(&vPos.x, 12, &m_matWorld._41, 12);
		return vPos;
	}*/

	return m_vWorldPos;
}

const Vector3& CTransform::GetPivot() const
{
	return m_vPivot;
}

const Vector3& CTransform::GetWorldAxis(AXIS axis) const
{
	return m_vWorldAxis[axis];
}

const Vector3& CTransform::GetMeshSize() const
{
	return m_vMeshSize;
}

const Vector3 CTransform::GetBoneWorldPos() const
{
	Vector3 vBonePos = {};

	if (m_bBone)
	{
		memcpy_s(&vBonePos, 12, &static_cast<Matrix>(m_matWorld)[3][0], 12);

		return vBonePos;
	}

	return m_vWorldPos + vBonePos;
}

const Vector4& CTransform::GetWorldQRot() const
{
	return m_vWorldQuaternion;
}

const Vector3& CTransform::GetMeshPos() const
{
	return m_vMeshPos;
}

void CTransform::SetWorldScale(const Vector3& v)
{
	m_vVelocityScale += v - m_vWorldScale;

	m_vWorldScale = v;

	m_vRelativeScale = m_vWorldScale;

	if (m_pParent && m_bInheritScale)
		m_vRelativeScale = m_vWorldScale / m_pParent->GetWorldScale();

	m_bUpdateScale = true;

	std::list<CTransform*>::iterator iter = m_ChildList.begin();
	std::list<CTransform*>::iterator iterEnd = m_ChildList.end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->InheritScale();
	}
}

void CTransform::SetWorldScale(float x, float y, float z)
{
	m_vVelocityScale += Vector3(x, y, z) - m_vWorldScale;

	m_vWorldScale = Vector3(x, y, z);

	m_vRelativeScale = m_vWorldScale;

	if (m_pParent && m_bInheritScale)
		m_vRelativeScale = m_vWorldScale / m_pParent->GetWorldScale();

	m_bUpdateScale = true;

	std::list<CTransform*>::iterator iter = m_ChildList.begin();
	std::list<CTransform*>::iterator iterEnd = m_ChildList.end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->InheritScale();
	}
}

void CTransform::SetWorldRot(const Vector3& v)
{
	m_vVelocityRot += v - m_vWorldRot;

	m_vWorldRot = v;

	m_vRelativeRot = m_vWorldRot;

	if (m_pParent )
	{
		if (m_bInheritRotX)
			m_vRelativeRot.x = m_vWorldRot.x - m_pParent->GetWorldRot().x;

		if (m_bInheritRotY)
			m_vRelativeRot.y = m_vWorldRot.y - m_pParent->GetWorldRot().y;

		if (m_bInheritRotZ)
			m_vRelativeRot.z = m_vWorldRot.z - m_pParent->GetWorldRot().z;
	}		

	m_bUpdateRot = true;

	Matrix matRot;

	matRot.Rotation(m_vRelativeRot);

	for (int i = 0; i < AXIS_END; ++i)
	{
		m_vRelativeAxis[i] = Vector3::Axis[i].TranslationNorm(matRot);
		m_vRelativeAxis[i].Normalize();
	}

	matRot.Rotation(m_vWorldRot);

	for (int i = 0; i < AXIS_END; ++i)
	{
		m_vWorldAxis[i] = Vector3::Axis[i].TranslationNorm(matRot);
		m_vWorldAxis[i].Normalize();
	}

	std::list<CTransform*>::iterator iter = m_ChildList.begin();
	std::list<CTransform*>::iterator iterEnd = m_ChildList.end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->InheritRot();
	}
}

void CTransform::SetWorldRot(float x, float y, float z)
{
	m_vVelocityRot += Vector3(x,y,z) - m_vWorldRot;

	m_vWorldRot = Vector3(x, y, z);

	m_vRelativeRot = m_vWorldRot;

	if (m_pParent)
	{
		if (m_bInheritRotX)
			m_vRelativeRot.x = m_vWorldRot.x - m_pParent->GetWorldRot().x;

		if (m_bInheritRotY)
			m_vRelativeRot.y = m_vWorldRot.y - m_pParent->GetWorldRot().y;

		if (m_bInheritRotZ)
			m_vRelativeRot.z = m_vWorldRot.z - m_pParent->GetWorldRot().z;
	}

	m_bUpdateRot = true;

	Matrix matRot;

	matRot.Rotation(m_vRelativeRot);

	for (int i = 0; i < AXIS_END; ++i)
	{
		m_vRelativeAxis[i] = Vector3::Axis[i].TranslationNorm(matRot);
		m_vRelativeAxis[i].Normalize();
	}

	matRot.Rotation(m_vWorldRot);

	for (int i = 0; i < AXIS_END; ++i)
	{
		m_vWorldAxis[i] = Vector3::Axis[i].TranslationNorm(matRot);
		m_vWorldAxis[i].Normalize();
	}

	std::list<CTransform*>::iterator iter = m_ChildList.begin();
	std::list<CTransform*>::iterator iterEnd = m_ChildList.end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->InheritRot();
	}
}

void CTransform::SetWorldRotX(float x)
{
	m_vVelocityRot.x += x - m_vWorldRot.x;

	m_vWorldRot.x = x;

	m_vRelativeRot.x = m_vWorldRot.x;

	if (m_pParent)
	{
		if (m_bInheritRotX)
			m_vRelativeRot.x = m_vWorldRot.x - m_pParent->GetWorldRot().x;
	}

	m_bUpdateRot = true;

	Matrix matRot;

	matRot.Rotation(m_vRelativeRot);

	for (int i = 0; i < AXIS_END; ++i)
	{
		m_vRelativeAxis[i] = Vector3::Axis[i].TranslationNorm(matRot);
		m_vRelativeAxis[i].Normalize();
	}

	matRot.Rotation(m_vWorldRot);

	for (int i = 0; i < AXIS_END; ++i)
	{
		m_vWorldAxis[i] = Vector3::Axis[i].TranslationNorm(matRot);
		m_vWorldAxis[i].Normalize();
	}

	std::list<CTransform*>::iterator iter = m_ChildList.begin();
	std::list<CTransform*>::iterator iterEnd = m_ChildList.end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->InheritRot();
	}
}

void CTransform::SetWorldRotY(float y)
{
	m_vVelocityRot.y += y - m_vWorldRot.y;

	m_vWorldRot.y = y;

	m_vRelativeRot.y = m_vWorldRot.y;

	if (m_pParent)
	{
		if (m_bInheritRotY)
			m_vRelativeRot.y = m_vWorldRot.y - m_pParent->GetWorldRot().y;
	}

	m_bUpdateRot = true;

	Matrix matRot;

	matRot.Rotation(m_vRelativeRot);

	for (int i = 0; i < AXIS_END; ++i)
	{
		m_vRelativeAxis[i] = Vector3::Axis[i].TranslationNorm(matRot);
		m_vRelativeAxis[i].Normalize();
	}

	matRot.Rotation(m_vWorldRot);

	for (int i = 0; i < AXIS_END; ++i)
	{
		m_vWorldAxis[i] = Vector3::Axis[i].TranslationNorm(matRot);
		m_vWorldAxis[i].Normalize();
	}

	std::list<CTransform*>::iterator iter = m_ChildList.begin();
	std::list<CTransform*>::iterator iterEnd = m_ChildList.end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->InheritRot();
	}
}

void CTransform::SetWorldRotZ(float z)
{
	m_vVelocityRot.z += z - m_vWorldRot.z;

	m_vWorldRot.z = z;

	m_vRelativeRot.z = m_vWorldRot.z;

	if (m_pParent)
	{
		if (m_bInheritRotZ)
			m_vRelativeRot.z = m_vWorldRot.z - m_pParent->GetWorldRot().z;
	}

	m_bUpdateRot = true;

	Matrix matRot;

	matRot.Rotation(m_vRelativeRot);

	for (int i = 0; i < AXIS_END; ++i)
	{
		m_vRelativeAxis[i] = Vector3::Axis[i].TranslationNorm(matRot);
		m_vRelativeAxis[i].Normalize();
	}

	matRot.Rotation(m_vWorldRot);

	for (int i = 0; i < AXIS_END; ++i)
	{
		m_vWorldAxis[i] = Vector3::Axis[i].TranslationNorm(matRot);
		m_vWorldAxis[i].Normalize();
	}

	std::list<CTransform*>::iterator iter = m_ChildList.begin();
	std::list<CTransform*>::iterator iterEnd = m_ChildList.end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->InheritRot();
	}
}

void CTransform::SetWorldPos(const Vector3& v)
{
	m_vVelocity += v - m_vWorldPos;

	m_vWorldPos = v;

	m_vRelativePos = m_vWorldPos;

	if (m_pParent)
	{
		Matrix mat;

		mat.Rotation(m_pParent->GetWorldRot());

		Vector3 vPos = m_pParent->GetWorldPos();

		memcpy_s(&mat._41, 16, &vPos, 12);

		mat.Inverse();

		m_vRelativePos = m_vWorldPos.TranslationCoor(mat);
	}

	m_bUpdatePos = true;

	std::list<CTransform*>::iterator iter = m_ChildList.begin();
	std::list<CTransform*>::iterator iterEnd = m_ChildList.end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->InheritPos();
	}
}

void CTransform::SetWorldPos(float x, float y, float z)
{
	m_vVelocity += Vector3(x,y,z) - m_vWorldPos;

	m_vWorldPos = Vector3(x, y, z);

	m_vRelativePos = m_vWorldPos;

	if (m_pParent)
	{
		Matrix mat;

		mat.Rotation(m_pParent->GetWorldRot());

		Vector3 vPos = m_pParent->GetWorldPos();

		memcpy_s(&mat._41, 16, &vPos, 12);

		mat.Inverse();

		m_vRelativePos = m_vWorldPos.TranslationCoor(mat);
	}

	m_bUpdatePos = true;

	std::list<CTransform*>::iterator iter = m_ChildList.begin();
	std::list<CTransform*>::iterator iterEnd = m_ChildList.end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->InheritPos();
	}
}

void CTransform::SetWorldPosX(float x)
{
	m_vVelocity.x += x - m_vWorldPos.x;
	m_vWorldPos.x = x;

	m_vRelativePos.x = m_vWorldPos.x;

	if (m_pParent)
	{
		Matrix mat;

		mat.Rotation(m_pParent->GetWorldRot());

		Vector3 vPos = m_pParent->GetWorldPos();

		memcpy_s(&mat._41, 16, &vPos, 12);

		mat.Inverse();

		m_vRelativePos = m_vWorldPos.TranslationCoor(mat);
	}

	m_bUpdatePos = true;

	std::list<CTransform*>::iterator iter = m_ChildList.begin();
	std::list<CTransform*>::iterator iterEnd = m_ChildList.end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->InheritPos();
	}
}

void CTransform::SetWorldPosY(float y)
{
	m_vVelocity.y += y - m_vWorldPos.y;
	m_vWorldPos.y = y;

	m_vRelativePos.y = m_vWorldPos.y;

	if (m_pParent)
	{
		Matrix mat;

		mat.Rotation(m_pParent->GetWorldRot());

		Vector3 vPos = m_pParent->GetWorldPos();

		memcpy_s(&mat._41, 16, &vPos, 12);

		mat.Inverse();

		m_vRelativePos = m_vWorldPos.TranslationCoor(mat);
	}

	m_bUpdatePos = true;

	std::list<CTransform*>::iterator iter = m_ChildList.begin();
	std::list<CTransform*>::iterator iterEnd = m_ChildList.end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->InheritPos();
	}
}

void CTransform::SetWorldPosZ(float z)
{
	m_vVelocity.z += z - m_vWorldPos.z;
	m_vWorldPos.z = z;

	m_vRelativePos.z = m_vWorldPos.z;

	if (m_pParent)
	{
		Matrix mat;

		mat.Rotation(m_pParent->GetWorldRot());

		Vector3 vPos = m_pParent->GetWorldPos();

		memcpy_s(&mat._41, 16, &vPos, 12);

		mat.Inverse();

		m_vRelativePos = m_vWorldPos.TranslationCoor(mat);
	}

	m_bUpdatePos = true;

	std::list<CTransform*>::iterator iter = m_ChildList.begin();
	std::list<CTransform*>::iterator iterEnd = m_ChildList.end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->InheritPos();
	}
}

void CTransform::AddWorldScale(const Vector3& v)
{
	m_vVelocityScale += v;

	m_vWorldScale += v;

	m_vRelativeScale = m_vWorldScale;

	if (m_pParent && m_bInheritScale)
		m_vRelativeScale = m_vWorldScale / m_pParent->GetWorldScale();

	m_bUpdateScale = true;

	std::list<CTransform*>::iterator iter = m_ChildList.begin();
	std::list<CTransform*>::iterator iterEnd = m_ChildList.end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->InheritScale();
	}
}

void CTransform::AddWorldScale(float x, float y, float z)
{
	m_vVelocityScale += Vector3(x,y,z);

	m_vWorldScale += Vector3(x, y, z);

	m_vRelativeScale = m_vWorldScale;

	if (m_pParent && m_bInheritScale)
		m_vRelativeScale = m_vWorldScale / m_pParent->GetWorldScale();

	m_bUpdateScale = true;

	std::list<CTransform*>::iterator iter = m_ChildList.begin();
	std::list<CTransform*>::iterator iterEnd = m_ChildList.end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->InheritScale();
	}
}

void CTransform::AddWorldRot(const Vector3& v)
{
	m_vVelocityRot += v;

	m_vWorldRot += v;

	m_vRelativeRot = m_vWorldRot;

	if (m_pParent)
	{
		if (m_bInheritRotX)
			m_vRelativeRot.x = m_vWorldRot.x - m_pParent->GetWorldRot().x;

		if (m_bInheritRotY)
			m_vRelativeRot.y = m_vWorldRot.y - m_pParent->GetWorldRot().y;

		if (m_bInheritRotZ)
			m_vRelativeRot.z = m_vWorldRot.z - m_pParent->GetWorldRot().z;
	}

	m_bUpdateRot = true;

	Matrix matRot;

	matRot.Rotation(m_vRelativeRot);

	for (int i = 0; i < AXIS_END; ++i)
	{
		m_vRelativeAxis[i] = Vector3::Axis[i].TranslationNorm(matRot);
		m_vRelativeAxis[i].Normalize();
	}

	matRot.Rotation(m_vWorldRot);

	for (int i = 0; i < AXIS_END; ++i)
	{
		m_vWorldAxis[i] = Vector3::Axis[i].TranslationNorm(matRot);
		m_vWorldAxis[i].Normalize();
	}

	std::list<CTransform*>::iterator iter = m_ChildList.begin();
	std::list<CTransform*>::iterator iterEnd = m_ChildList.end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->InheritRot();
	}
}

void CTransform::AddWorldRot(float x, float y, float z)
{
	m_vVelocityRot += Vector3(x,y,z);

	m_vWorldRot += Vector3(x,y,z);

	m_vRelativeRot = m_vWorldRot;

	if (m_pParent)
	{
		if (m_bInheritRotX)
			m_vRelativeRot.x = m_vWorldRot.x - m_pParent->GetWorldRot().x;

		if (m_bInheritRotY)
			m_vRelativeRot.y = m_vWorldRot.y - m_pParent->GetWorldRot().y;

		if (m_bInheritRotZ)
			m_vRelativeRot.z = m_vWorldRot.z - m_pParent->GetWorldRot().z;
	}

	m_bUpdateRot = true;

	Matrix matRot;

	matRot.Rotation(m_vRelativeRot);

	for (int i = 0; i < AXIS_END; ++i)
	{
		m_vRelativeAxis[i] = Vector3::Axis[i].TranslationNorm(matRot);
		m_vRelativeAxis[i].Normalize();
	}

	matRot.Rotation(m_vWorldRot);

	for (int i = 0; i < AXIS_END; ++i)
	{
		m_vWorldAxis[i] = Vector3::Axis[i].TranslationNorm(matRot);
		m_vWorldAxis[i].Normalize();
	}

	std::list<CTransform*>::iterator iter = m_ChildList.begin();
	std::list<CTransform*>::iterator iterEnd = m_ChildList.end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->InheritRot();
	}
}

void CTransform::AddWorldRotX(float x)
{
	m_vVelocityRot.x += x;

	m_vWorldRot.x += x;

	m_vRelativeRot.x = m_vWorldRot.x;

	if (m_pParent)
	{
		if (m_bInheritRotX)
			m_vRelativeRot.x = m_vWorldRot.x - m_pParent->GetWorldRot().x;
	}

	if (m_vWorldRot.x > 360.f)
		m_vWorldRot.x -= 360.f;

	else if (	m_vWorldRot.x < -360.f)
				m_vWorldRot.x += 360.f;

	if (m_vRelativeRot.x > 360.f)
		m_vRelativeRot.x -= 360.f;

	else if (	m_vRelativeRot.x < -360.f)
				m_vRelativeRot.x += 360.f;

	m_bUpdateRot = true;

	Matrix matRot;

	matRot.Rotation(m_vRelativeRot);

	for (int i = 0; i < AXIS_END; ++i)
	{
		m_vRelativeAxis[i] = Vector3::Axis[i].TranslationNorm(matRot);
		m_vRelativeAxis[i].Normalize();
	}

	matRot.Rotation(m_vWorldRot);

	for (int i = 0; i < AXIS_END; ++i)
	{
		m_vWorldAxis[i] = Vector3::Axis[i].TranslationNorm(matRot);
		m_vWorldAxis[i].Normalize();
	}

	std::list<CTransform*>::iterator iter = m_ChildList.begin();
	std::list<CTransform*>::iterator iterEnd = m_ChildList.end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->InheritRot();
	}
}

void CTransform::AddWorldRotY(float y)
{
	m_vVelocityRot.y += y;

	m_vWorldRot.y += y;

	m_vRelativeRot.y = m_vWorldRot.y;

	if (m_pParent)
	{
		if (m_bInheritRotX)
			m_vRelativeRot.y = m_vWorldRot.y - m_pParent->GetWorldRot().y;
	}

	if (m_vWorldRot.y > 360.f)
		m_vWorldRot.y -= 360.f;

	else if (m_vWorldRot.y < -360.f)
		m_vWorldRot.y += 360.f;

	if (m_vRelativeRot.y > 360.f)
		m_vRelativeRot.y -= 360.f;

	else if (m_vRelativeRot.y < -360.f)
		m_vRelativeRot.y += 360.f;

	m_bUpdateRot = true;

	Matrix matRot;

	matRot.Rotation(m_vRelativeRot);

	for (int i = 0; i < AXIS_END; ++i)
	{
		m_vRelativeAxis[i] = Vector3::Axis[i].TranslationNorm(matRot);
		m_vRelativeAxis[i].Normalize();
	}

	matRot.Rotation(m_vWorldRot);

	for (int i = 0; i < AXIS_END; ++i)
	{
		m_vWorldAxis[i] = Vector3::Axis[i].TranslationNorm(matRot);
		m_vWorldAxis[i].Normalize();
	}

	std::list<CTransform*>::iterator iter = m_ChildList.begin();
	std::list<CTransform*>::iterator iterEnd = m_ChildList.end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->InheritRot();
	}
}

void CTransform::AddWorldRotZ(float z)
{
	m_vVelocityRot.z += z;

	m_vWorldRot.z += z;

	m_vRelativeRot.z = m_vWorldRot.z;

	if (m_pParent)
	{
		if (m_bInheritRotX)
			m_vRelativeRot.z = m_vWorldRot.z - m_pParent->GetWorldRot().z;
	}

	if (m_vWorldRot.z > 360.f)
		m_vWorldRot.z -= 360.f;

	else if (m_vWorldRot.z < -360.f)
		m_vWorldRot.z += 360.f;

	if (m_vRelativeRot.z > 360.f)
		m_vRelativeRot.z -= 360.f;

	else if (m_vRelativeRot.z < -360.f)
		m_vRelativeRot.z += 360.f;

	m_bUpdateRot = true;

	Matrix matRot;

	matRot.Rotation(m_vRelativeRot);

	for (int i = 0; i < AXIS_END; ++i)
	{
		m_vRelativeAxis[i] = Vector3::Axis[i].TranslationNorm(matRot);
		m_vRelativeAxis[i].Normalize();
	}

	matRot.Rotation(m_vWorldRot);

	for (int i = 0; i < AXIS_END; ++i)
	{
		m_vWorldAxis[i] = Vector3::Axis[i].TranslationNorm(matRot);
		m_vWorldAxis[i].Normalize();
	}

	std::list<CTransform*>::iterator iter = m_ChildList.begin();
	std::list<CTransform*>::iterator iterEnd = m_ChildList.end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->InheritRot();
	}
}

void CTransform::AddWorldRot(const Vector3& v, float fAngle)
{
	Matrix matRot;

	matRot.RotationAxis(v, fAngle);
}

void CTransform::AddWorldPos(const Vector3& v)
{
	m_vVelocity += v;

	m_vWorldPos += v;

	m_vRelativePos = m_vWorldPos;

	if (m_pParent)
	{
		Matrix mat;

		mat.Rotation(m_pParent->GetWorldRot());

		Vector3 vPos = m_pParent->GetWorldPos();

		memcpy_s(&mat._41, 16, &vPos, 12);

		mat.Inverse();

		m_vRelativePos = m_vWorldPos.TranslationCoor(mat);
	}

	m_bUpdatePos = true;

	std::list<CTransform*>::iterator iter = m_ChildList.begin();
	std::list<CTransform*>::iterator iterEnd = m_ChildList.end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->InheritPos();
	}
}

void CTransform::AddWorldPos(float x, float y, float z)
{
	m_vVelocity += Vector3(x,y,z);

	m_vWorldPos += Vector3(x,y,z);

	m_vRelativePos = m_vWorldPos;

	if (m_pParent)
	{
		Matrix mat;

		mat.Rotation(m_pParent->GetWorldRot());

		Vector3 vPos = m_pParent->GetWorldPos();

		memcpy_s(&mat._41, 16, &vPos, 12);

		mat.Inverse();

		m_vRelativePos = m_vWorldPos.TranslationCoor(mat);
	}

	m_bUpdatePos = true;

	std::list<CTransform*>::iterator iter = m_ChildList.begin();
	std::list<CTransform*>::iterator iterEnd = m_ChildList.end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->InheritPos();
	}
}

void CTransform::AddWorldPosX(float x)
{
	m_vVelocity.x += x;
	m_vWorldPos.x += x;

	m_vRelativePos = m_vWorldPos;

	if (m_pParent)
	{
		Matrix mat;

		mat.Rotation(m_pParent->GetWorldRot());

		Vector3 vPos = m_pParent->GetWorldPos();

		memcpy_s(&mat._41, 16, &vPos, 12);

		mat.Inverse();

		m_vRelativePos = m_vWorldPos.TranslationCoor(mat);
	}

	m_bUpdatePos = true;

	std::list<CTransform*>::iterator iter = m_ChildList.begin();
	std::list<CTransform*>::iterator iterEnd = m_ChildList.end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->InheritPos();
	}
}

void CTransform::AddWorldPosY(float y)
{
	m_vVelocity.y += y;
	m_vWorldPos.y += y;
	m_vRelativePos = m_vWorldPos;

	if (m_pParent)
	{
		Matrix mat;

		mat.Rotation(m_pParent->GetWorldRot());

		Vector3 vPos = m_pParent->GetWorldPos();

		memcpy_s(&mat._41, 16, &vPos, 12);

		mat.Inverse();

		m_vRelativePos = m_vWorldPos.TranslationCoor(mat);
	}

	m_bUpdatePos = true;

	std::list<CTransform*>::iterator iter = m_ChildList.begin();
	std::list<CTransform*>::iterator iterEnd = m_ChildList.end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->InheritPos();
	}
}

void CTransform::AddWorldPosZ(float z)
{
	m_vVelocity.z += z;
	m_vWorldPos.z += z;
	m_vRelativePos = m_vWorldPos;

	if (m_pParent)
	{
		Matrix mat;

		mat.Rotation(m_pParent->GetWorldRot());

		Vector3 vPos = m_pParent->GetWorldPos();

		memcpy_s(&mat._41, 16, &vPos, 12);

		mat.Inverse();

		m_vRelativePos = m_vWorldPos.TranslationCoor(mat);
	}

	m_bUpdatePos = true;

	std::list<CTransform*>::iterator iter = m_ChildList.begin();
	std::list<CTransform*>::iterator iterEnd = m_ChildList.end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->InheritPos();
	}
}

void CTransform::SetPivot(const Vector3& v)
{
	m_vPivot = v;
}

void CTransform::SetPivot(float x, float y, float z)
{
	SetPivot(Vector3(x,y,z));
}

void CTransform::SetMeshSize(const Vector3& v)
{
	m_vMeshSize = v;
}

void CTransform::LookAt(const Vector3& v)
{/*
	Vector3 vVec = v - m_vWorldPos;

	vVec.Normalize();

	Vector3 vAxis = vVec.Cross(m_vWorldAxis[AXIS_Z]);

	float fAngle = DirectX::XMConvertToDegrees(acosf(vVec.Dot(m_vWorldAxis[AXIS_Z])));


	if (vVec.x < 0.f)
		fAngle *= -1.f;

	if (m_vWorldRot.y > 360.f)
		m_vWorldRot.y -= 360.f;

	else if (m_vWorldRot.y < -360.f)
		m_vWorldRot.y += 360.f;

	AddWorldRotY(fAngle);*/

	Vector3 vPos = GetWorldPos();

	Vector2 vDir = Vector2(v.x - vPos.x, v.z - vPos.z);
	vDir.Normalize();

	Vector3 vLook = Vector3::Axis[AXIS_Z];

	Vector2 _vLook = Vector2(vLook.x, vLook.z);

	_vLook.Normalize();

	float fAngle = RadToDeg(acosf(vDir.Dot(_vLook)));

	if (vDir.x < 0.f)
		fAngle *= -1.f;

	SetWorldRotY(fAngle + 180.f);
}

void CTransform::SetQuaternionRotX(float x)
{
	m_bQuaternion = true;
	m_vWorldQuaternion =
		XMQuaternionRotationAxis(m_vWorldAxis[AXIS_X].Convert(), DegToRad(x));

	m_bUpdateRot = true;

	Matrix matRot = XMMatrixRotationQuaternion(m_vWorldQuaternion.Convert());

	for (int i = 0; i < AXIS_END; ++i)
	{
		m_vWorldAxis[i] = Vector3::Axis[i].TranslationNorm(matRot);
		m_vWorldAxis[i].Normalize();
	}

	std::list<CTransform*>::iterator iter = m_ChildList.begin();
	std::list<CTransform*>::iterator iterEnd = m_ChildList.end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->InheritRot();
	}
}

void CTransform::AddQuaternionRotX(float x)
{
	m_bQuaternion = true;
	m_vWorldQuaternion = XMQuaternionMultiply(
		m_vWorldQuaternion.Convert(),
		XMQuaternionRotationNormal(m_vWorldAxis[AXIS_X].Convert(), DegToRad(x)));

	m_bUpdateRot = true;

	Matrix matRot = XMMatrixRotationQuaternion(m_vWorldQuaternion.Convert());

	for (int i = 0; i < AXIS_END; ++i)
	{
		m_vWorldAxis[i] = Vector3::Axis[i].TranslationNorm(matRot);
		m_vWorldAxis[i].Normalize();
	}

	std::list<CTransform*>::iterator iter = m_ChildList.begin();
	std::list<CTransform*>::iterator iterEnd = m_ChildList.end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->InheritRot();
	}
}

void CTransform::SetQuaternionRotY(float y)
{
	m_bQuaternion = true;
	m_vWorldQuaternion = 
		XMQuaternionRotationAxis(m_vWorldAxis[AXIS_Y].Convert(), DegToRad(y));

	m_bUpdateRot = true;

	Matrix matRot = XMMatrixRotationQuaternion(m_vWorldQuaternion.Convert());

	for (int i = 0; i < AXIS_END; ++i)
	{
		m_vWorldAxis[i] = Vector3::Axis[i].TranslationNorm(matRot);
		m_vWorldAxis[i].Normalize();
	}

	std::list<CTransform*>::iterator iter = m_ChildList.begin();
	std::list<CTransform*>::iterator iterEnd = m_ChildList.end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->InheritRot();
	}
}

void CTransform::AddQuaternionRotY(float y)
{
	m_bQuaternion = true;
	m_vWorldQuaternion = XMQuaternionMultiply(
		m_vWorldQuaternion.Convert(), 
		XMQuaternionRotationNormal(m_vWorldAxis[AXIS_Y].Convert(), DegToRad(y)));

	m_bUpdateRot = true;

	Matrix matRot = XMMatrixRotationQuaternion(m_vWorldQuaternion.Convert());

	for (int i = 0; i < AXIS_END; ++i)
	{
		m_vWorldAxis[i] = Vector3::Axis[i].TranslationNorm(matRot);
		m_vWorldAxis[i].Normalize();
	}

	std::list<CTransform*>::iterator iter = m_ChildList.begin();
	std::list<CTransform*>::iterator iterEnd = m_ChildList.end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->InheritRot();
	}
}

void CTransform::SetQuaternionRotZ(float z)
{
	m_bQuaternion = true;
	m_vWorldQuaternion =
		XMQuaternionRotationAxis(m_vWorldAxis[AXIS_Z].Convert(), DegToRad(z));

	m_bUpdateRot = true;

	Matrix matRot = XMMatrixRotationQuaternion(m_vWorldQuaternion.Convert());

	for (int i = 0; i < AXIS_END; ++i)
	{
		m_vWorldAxis[i] = Vector3::Axis[i].TranslationNorm(matRot);
		m_vWorldAxis[i].Normalize();
	}

	std::list<CTransform*>::iterator iter = m_ChildList.begin();
	std::list<CTransform*>::iterator iterEnd = m_ChildList.end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->InheritRot();
	}
}

void CTransform::AddQuaternionRotZ(float z)
{
	m_bQuaternion = true;
	m_vWorldQuaternion = XMQuaternionMultiply(
		m_vWorldQuaternion.Convert(),
		XMQuaternionRotationNormal(m_vWorldAxis[AXIS_Z].Convert(), DegToRad(z)));

	m_bUpdateRot = true;

	Matrix matRot = XMMatrixRotationQuaternion(m_vWorldQuaternion.Convert());

	for (int i = 0; i < AXIS_END; ++i)
	{
		m_vWorldAxis[i] = Vector3::Axis[i].TranslationNorm(matRot);
		m_vWorldAxis[i].Normalize();
	}

	std::list<CTransform*>::iterator iter = m_ChildList.begin();
	std::list<CTransform*>::iterator iterEnd = m_ChildList.end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->InheritRot();
	}
}

void CTransform::AddQuaternionRotAxis(const Vector3& vAxis, float fAngle)
{
	m_bQuaternion = true;
	m_vWorldQuaternion = XMQuaternionMultiply(
		m_vWorldQuaternion.Convert(), 
		XMQuaternionRotationAxis(vAxis.Convert(), DegToRad(fAngle)));

	m_bUpdateRot = true;

	Matrix matRot = XMMatrixRotationQuaternion(m_vWorldQuaternion.Convert());

	for (int i = 0; i < AXIS_END; ++i)
	{
		m_vWorldAxis[i] = Vector3::Axis[i].TranslationNorm(matRot);
		m_vWorldAxis[i].Normalize();
	}

	std::list<CTransform*>::iterator iter = m_ChildList.begin();
	std::list<CTransform*>::iterator iterEnd = m_ChildList.end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->InheritRot();
	}
}

void CTransform::SetQuaternionRotAxis(const Vector3& vAxis, float fAngle)
{
	m_bQuaternion = true;

	m_vWorldQuaternion = XMQuaternionRotationAxis(vAxis.Convert(), DegToRad(fAngle));

	m_bUpdateRot = true;

	Matrix matRot = XMMatrixRotationQuaternion(m_vWorldQuaternion.Convert());

	for (int i = 0; i < AXIS_END; ++i)
	{
		m_vWorldAxis[i] = Vector3::Axis[i].TranslationNorm(matRot);
		m_vWorldAxis[i].Normalize();
	}

	std::list<CTransform*>::iterator iter = m_ChildList.begin();
	std::list<CTransform*>::iterator iterEnd = m_ChildList.end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->InheritRot();
	}
}

void CTransform::SetMeshPos(const Vector3& vPos)
{
	m_vMeshPos = vPos;
}

void CTransform::SetInheritScale(bool bInherit)
{
	m_bInheritScale = bInherit;
}
void CTransform::SetInheritRotX(bool bInherit)
{
	m_bInheritRotX = bInherit;
}

void CTransform::SetInheritRotY(bool bInherit)
{
	m_bInheritRotY = bInherit;
}

void CTransform::SetInheritRotZ(bool bInherit)
{
	m_bInheritRotZ = bInherit;
}

void CTransform::SetInheritPos(bool bInherit)
{
	m_bInheritPos = bInherit;
}

void CTransform::SetGravity(bool bGravity)
{
	m_bGravity = bGravity;
}

bool CTransform::IsInheritScale() const
{
	return m_bInheritScale;
}

bool CTransform::IsInheritRotX() const
{
	return m_bInheritRotX;
}

bool CTransform::IsInheritRotY() const
{
	return m_bInheritRotY;
}

bool CTransform::IsInheritRotZ() const
{
	return m_bInheritRotZ;
}

const Vector3& CTransform::GetVelocityScale()	const
{
	return m_vVelocityScale;
}

const Vector3& CTransform::GetVelocityRot()	const
{
	return m_vVelocityRot;
}

const Vector3& CTransform::GetVelocity()	const
{
	return m_vVelocity;
}

float CTransform::GetVelocityAmount()	const
{
	return m_vVelocity.Length();
}

const TransformCBuffer& CTransform::GetCBuffer() const
{
	return m_tCBuffer;
}

bool CTransform::IsQuaternion() const
{
	return m_bQuaternion;
}

void CTransform::AddSpeed(const Vector3& vSpeed)
{
	m_vSpeed += vSpeed;
}

void CTransform::SetSpeed(const Vector3& vSpeed)
{
	m_vSpeed = vSpeed;
}

const Vector3& CTransform::GetSpeed() const
{
	return m_vSpeed;
}

void CTransform::SetSpeedY(float fSpeedY)
{
	m_vSpeed.y = fSpeedY;
}

void CTransform::AddSpeedY(float fSpeedY)
{
	m_vSpeed.y += fSpeedY;
}
