#include "Light.h"
#include "../Resource/ShaderManager.h"
#include "../CameraManager.h"
#include "Camera.h"
#include "../LightManager.h"
#include "../UI/imgui/imgui.h"
#include "../Device.h"

CLight::CLight()	:
	m_tCBuffer()
{
	m_eSceneComponentType = SCENE_COMPONENT_TYPE::SCT_3D;
	m_iSceneComponentClassType = static_cast<int>(SCENECOMPONENT_CLASS_TYPE::LIGHT);

	m_tCBuffer.eType = LIGHT_TYPE::DIRECTIONAL;
	m_tCBuffer.vDif = Vector4::White;
	m_tCBuffer.vAmb = Vector4(0.2f, 0.2f, 0.2f, 1.f);
	m_tCBuffer.vSpe = Vector4::White;
	m_tCBuffer.vEmi = Vector4::White;
	m_tCBuffer.fAngleIn = cosf(PI_DIV4 / 2.f);
	m_tCBuffer.fAngleOut = cosf(PI_DIV2 / 3.f);
	m_tCBuffer.fRange = 10.f;
	m_tCBuffer.vAttn.x = 1.f;
	m_tCBuffer.vAttn.y = m_tCBuffer.fRange * 2.f;
	m_tCBuffer.vAttn.z = m_tCBuffer.fRange * m_tCBuffer.fRange;
	m_tCBuffer.fLength = 32.f;
}

CLight::CLight(const CLight& light)	:
	CSceneComponent(light)
	, m_tCBuffer(light.m_tCBuffer)
{
}

CLight::~CLight()
{
	CLightManager::DeleteLight(this);
}

const Matrix& CLight::GetView(int iIdx) const
{
	return m_matView[iIdx];
}

const Matrix& CLight::GetProj() const
{
	return m_matProj;
}

const Matrix& CLight::GetVP(int iIdx) const
{
	return m_matVP[iIdx];
}

void CLight::SetFrustum(int iWidth, int iHeight, float fNear, float fFar)
{
	m_matProj = XMMatrixOrthographicLH(static_cast<float>(iWidth), static_cast<float>(iHeight), fNear, fFar);
}

void CLight::SetDifColor(const Vector4& vDif)
{
	m_tCBuffer.vDif = vDif;
}

void CLight::SetDifColor(float r, float g, float b, float a)
{
	SetDifColor(Vector4(r, g, b, a));
}

void CLight::SetDifColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	SetDifColor(r / 255.f, g / 255.f, b / 255.f, a / 255.f);
}

void CLight::SetAmbColor(const Vector4& vAmb)
{
	m_tCBuffer.vAmb = vAmb;
}

void CLight::SetAmbColor(float r, float g, float b, float a)
{
	SetAmbColor(Vector4(r, g, b, a));
}

void CLight::SetAmbColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	SetAmbColor(r / 255.f, g / 255.f, b / 255.f, a / 255.f);
}

void CLight::SetSpcColor(const Vector4& vSpc)
{
	m_tCBuffer.vSpe = vSpc;
}

void CLight::SetSpcColor(float r, float g, float b, float a)
{
	SetSpcColor(Vector4(r, g, b, a));
}

void CLight::SetSpcColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	SetSpcColor(r / 255.f, g / 255.f, b / 255.f, a / 255.f);
}

void CLight::SetEmiColor(const Vector4& vEmi)
{
	m_tCBuffer.vEmi = vEmi;
}

void CLight::SetEmiColor(float r, float g, float b, float a)
{
	SetEmiColor(Vector4(r, g, b, a));
}

void CLight::SetEmiColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	SetEmiColor(r / 255.f, g / 255.f, b / 255.f, a / 255.f);
}

void CLight::SetRange(float fRange)
{
	m_tCBuffer.fRange = fRange;
	if (m_tCBuffer.eType == LIGHT_TYPE::POINT)
	{
		m_matProj = XMMatrixPerspectiveFovLH(DegToRad(90.f),
			1.f, 0.3f, m_tCBuffer.fRange);
	}
}

void CLight::SetInAngle(float fAngle)
{
	m_tCBuffer.fAngleIn = fAngle;
}

void CLight::SetOutAngle(float fAngle)
{
	m_tCBuffer.fAngleOut = fAngle;
}

void CLight::SetAttn(float c,float a,float b)
{
	m_tCBuffer.vAttn = Vector3(c,a,b);
}

void CLight::SetLightType(LIGHT_TYPE eType)
{
	m_tCBuffer.eType = eType;

	Resolution tRS = RESOLUTION;
	switch (m_tCBuffer.eType)
	{
	case LIGHT_TYPE::DIRECTIONAL:
	{

		m_matProj = XMMatrixOrthographicLH(static_cast<float>(tRS.iWidth), static_cast<float>(tRS.iHeight), 0.f, 5000.f);
	}
		break;
	case LIGHT_TYPE::POINT:
	{
		//m_matProj = XMMatrixPerspectiveFovLH(DegToRad(90.f), 
		//static_cast<float>(tRS.iWidth) / static_cast<float>(tRS.iHeight) ,0.3f, 5000.f);
		m_matProj = XMMatrixPerspectiveFovLH(DegToRad(90.f), 
		1.f ,0.3f, m_tCBuffer.fRange);
		break;
	}
		break;
	case LIGHT_TYPE::CAPSULE:
		break;
	case LIGHT_TYPE::PROJ_POINT:
		break;
	}
}

void CLight::SetShader()
{
	GET_SINGLE(CShaderManager)->UpdateCBuffer("Light", &m_tCBuffer);
}

bool CLight::Init()
{
	if (!CSceneComponent::Init())
		return false;

	Resolution tRS = RESOLUTION;

	//m_matProj = XMMatrixPerspectiveFovLH(DegToRad(90.f), 
		//static_cast<float>(tRS.iWidth) / static_cast<float>(tRS.iHeight) ,0.3f, 5000.f);
	m_matProj = XMMatrixOrthographicLH(static_cast<float>(tRS.iWidth), static_cast<float>(tRS.iHeight), 0.f, 5000.f);

	return true;
}

void CLight::Start()
{
	CSceneComponent::Start();

	GET_SINGLE(CLightManager)->AddLight(this);
}

void CLight::Update(float fTime)
{
	CSceneComponent::Update(fTime);

	Vector3 vPos = GetWorldPos();

	Vector3 vAxisX = GetWorldAxis(AXIS_X);
	Vector3 vAxisY = GetWorldAxis(AXIS_Y);
	Vector3 vAxisZ = GetWorldAxis(AXIS_Z);

	switch (m_tCBuffer.eType)
	{
	case LIGHT_TYPE::DIRECTIONAL:
	{
		CCamera* pCam = GET_SINGLE(CCameraManager)->GetMainCam();
		if (pCam)
		{
			m_tCBuffer.vDir = GetWorldAxis(AXIS_Z).TranslationNorm(pCam->GetView());
			m_tCBuffer.vDir.Normalize();
		}

		m_matView[0][0][0] = vAxisX.x;
		m_matView[0][1][0] = vAxisX.y;
		m_matView[0][2][0] = vAxisX.z;
		m_matView[0][0][1] = vAxisY.x;
		m_matView[0][1][1] = vAxisY.y;
		m_matView[0][2][1] = vAxisY.z;
		m_matView[0][0][2] = vAxisZ.x;
		m_matView[0][1][2] = vAxisZ.y;
		m_matView[0][2][2] = vAxisZ.z;
		m_matView[0][3][0] = -vPos.Dot(vAxisX);
		m_matView[0][3][1] = -vPos.Dot(vAxisY);
		m_matView[0][3][2] = -vPos.Dot(vAxisZ);
		m_matView[0][3][3] = 1.f;
		m_matVP[0] = m_matView[0];
		m_matVP[0] = m_matVP[0] * m_matProj;
		m_tCBuffer.matViewProj[0] = m_matVP[0];
		m_tCBuffer.matViewProj[0].Transpose();
	}
		break;
	case LIGHT_TYPE::POINT:
	{
		CCamera* pCam = GET_SINGLE(CCameraManager)->GetMainCam();
		if (pCam)
		{
			m_tCBuffer.vPos = vPos.TranslationCoor(pCam->GetView());
		}

		m_matView[0][0][0] = -vAxisZ.x;
		m_matView[0][1][0] = -vAxisZ.y;
		m_matView[0][2][0] = -vAxisZ.z;
		m_matView[0][0][1] = vAxisY.x;
		m_matView[0][1][1] = vAxisY.y;
		m_matView[0][2][1] = vAxisY.z;
		m_matView[0][0][2] = vAxisX.x;
		m_matView[0][1][2] = vAxisX.y;
		m_matView[0][2][2] = vAxisX.z;
		m_matView[0][3][0] = vPos.Dot(vAxisZ);
		m_matView[0][3][1] = -vPos.Dot(vAxisY);
		m_matView[0][3][2] = -vPos.Dot(vAxisX);
		m_matView[0][3][3] = 1.f;

		m_matView[1][0][0] = vAxisZ.x;
		m_matView[1][1][0] = vAxisZ.y;
		m_matView[1][2][0] = vAxisZ.z;
		m_matView[1][0][1] = vAxisY.x;
		m_matView[1][1][1] = vAxisY.y;
		m_matView[1][2][1] = vAxisY.z;
		m_matView[1][0][2] = -vAxisX.x;
		m_matView[1][1][2] = -vAxisX.y;
		m_matView[1][2][2] = -vAxisX.z;
		m_matView[1][3][0] = -vPos.Dot(vAxisZ);
		m_matView[1][3][1] = -vPos.Dot(vAxisY);
		m_matView[1][3][2] = vPos.Dot(vAxisX);
		m_matView[1][3][3] = 1.f;

		m_matView[2][0][0] = vAxisX.x;
		m_matView[2][1][0] = vAxisX.y;
		m_matView[2][2][0] = vAxisX.z;
		m_matView[2][0][1] = -vAxisZ.x;
		m_matView[2][1][1] = -vAxisZ.y;
		m_matView[2][2][1] = -vAxisZ.z;
		m_matView[2][0][2] = vAxisY.x;
		m_matView[2][1][2] = vAxisY.y;
		m_matView[2][2][2] = vAxisY.z;
		m_matView[2][3][0] = -vPos.Dot(vAxisX);
		m_matView[2][3][1] = vPos.Dot(vAxisZ);
		m_matView[2][3][2] = -vPos.Dot(vAxisY);
		m_matView[2][3][3] = 1.f;

		m_matView[3][0][0] = vAxisX.x;
		m_matView[3][1][0] = vAxisX.y;
		m_matView[3][2][0] = vAxisX.z;
		m_matView[3][0][1] = vAxisZ.x;
		m_matView[3][1][1] = vAxisZ.y;
		m_matView[3][2][1] = vAxisZ.z;
		m_matView[3][0][2] = -vAxisY.x;
		m_matView[3][1][2] = -vAxisY.y;
		m_matView[3][2][2] = -vAxisY.z;
		m_matView[3][3][0] = -vPos.Dot(vAxisX);
		m_matView[3][3][1] = -vPos.Dot(vAxisZ);
		m_matView[3][3][2] = vPos.Dot(vAxisY);
		m_matView[3][3][3] = 1.f;

		m_matView[4][0][0] = vAxisX.x;
		m_matView[4][1][0] = vAxisX.y;
		m_matView[4][2][0] = vAxisX.z;
		m_matView[4][0][1] = vAxisY.x;
		m_matView[4][1][1] = vAxisY.y;
		m_matView[4][2][1] = vAxisY.z;
		m_matView[4][0][2] = vAxisZ.x;
		m_matView[4][1][2] = vAxisZ.y;
		m_matView[4][2][2] = vAxisZ.z;
		m_matView[4][3][0] = -vPos.Dot(vAxisX);
		m_matView[4][3][1] = -vPos.Dot(vAxisY);
		m_matView[4][3][2] = -vPos.Dot(vAxisZ);
		m_matView[4][3][3] = 1.f;

		m_matView[5][0][0] = -vAxisX.x;
		m_matView[5][1][0] = -vAxisX.y;
		m_matView[5][2][0] = -vAxisX.z;
		m_matView[5][0][1] = vAxisY.x;
		m_matView[5][1][1] = vAxisY.y;
		m_matView[5][2][1] = vAxisY.z;
		m_matView[5][0][2] = -vAxisZ.x;
		m_matView[5][1][2] = -vAxisZ.y;
		m_matView[5][2][2] = -vAxisZ.z;
		m_matView[5][3][0] = vPos.Dot(vAxisX);
		m_matView[5][3][1] = -vPos.Dot(vAxisY);
		m_matView[5][3][2] = vPos.Dot(vAxisZ);
		m_matView[5][3][3] = 1.f;

		for (int i = 0;i < 6; i++)
		{
			m_matVP[i] = m_matView[i];
			m_matVP[i] = m_matVP[i] * m_matProj;
			m_tCBuffer.matViewProj[i] = m_matVP[i];
			m_tCBuffer.matViewProj[i].Transpose();
		}
	}
		break;
	case LIGHT_TYPE::SPOT:
	case LIGHT_TYPE::CAPSULE:
	{
		CCamera* pCam = GET_SINGLE(CCameraManager)->GetMainCam();
		m_tCBuffer.vDir = GetWorldAxis(AXIS_Z).TranslationNorm(pCam->GetView());
		m_tCBuffer.vDir.Normalize();
		m_tCBuffer.vPos = vPos.TranslationCoor(pCam->GetView());

		m_matView[0][0][0] = vAxisX.x;
		m_matView[0][1][0] = vAxisX.y;
		m_matView[0][2][0] = vAxisX.z;
		m_matView[0][0][1] = vAxisY.x;
		m_matView[0][1][1] = vAxisY.y;
		m_matView[0][2][1] = vAxisY.z;
		m_matView[0][0][2] = vAxisZ.x;
		m_matView[0][1][2] = vAxisZ.y;
		m_matView[0][2][2] = vAxisZ.z;
		m_matView[0][3][0] = -vPos.Dot(vAxisX);
		m_matView[0][3][1] = -vPos.Dot(vAxisY);
		m_matView[0][3][2] = -vPos.Dot(vAxisZ);
		m_matView[0][3][3] = 1.f;
		m_matVP[0] = m_matView[0];
		m_matVP[0] = m_matVP[0] * m_matProj;
		m_tCBuffer.matViewProj[0] = m_matVP[0];
		m_tCBuffer.matViewProj[0].Transpose();
	}
		break;
	case LIGHT_TYPE::PROJ_POINT:
	{
		Matrix matRot = GetMatRot();

		m_tCBuffer.matTransform = matRot;

		CCamera* pCam = GET_SINGLE(CCameraManager)->GetMainCam();
		m_tCBuffer.vDir = GetWorldAxis(AXIS_Z).TranslationNorm(pCam->GetView());
		m_tCBuffer.vDir.Normalize();
		m_tCBuffer.vPos = vPos.TranslationCoor(pCam->GetView());
	}
		break;
	}	

	//R*T  T^-1 * R'
	//	T^-1 =	1	0	0	0	R' =	ux	vx	wx	0
	//			0	1	0	0			uy	vy	wv	0
	//			0	0	1	0			uz	vz	wz	0
	//			-X	-Y	-X	1			0	0	0	1

	m_tCBuffer.vPerspectiveValues.x = m_matProj[2][2];
	m_tCBuffer.vPerspectiveValues.y = m_matProj[3][2];
}

void CLight::PostUpdate(float fTime)
{
	CSceneComponent::PostUpdate(fTime);
}

void CLight::Collision(float fTime)
{
	CSceneComponent::Collision(fTime);
}

void CLight::PreRender(float fTime)
{
	//CSceneComponent::PreRender(fTime);
}

void CLight::Render(float fTime)
{
	CSceneComponent::Render(fTime);

	SetShader();

	UINT iOffset = 0;

	CONTEXT->IASetInputLayout(nullptr);
	CONTEXT->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	CONTEXT->IASetVertexBuffers(0, 0, nullptr, 0, &iOffset);
	CONTEXT->IASetIndexBuffer(nullptr, DXGI_FORMAT_UNKNOWN, 0);
	CONTEXT->Draw(4, 0);
}

void CLight::PostRender(float fTime)
{
	CSceneComponent::PostRender(fTime);
}

std::shared_ptr<class CComponent> CLight::Clone()
{
	return std::shared_ptr<class CComponent>(new CLight(*this));
}

void CLight::Save(FILE* pFile)
{
	CSceneComponent::Save(pFile);

	fwrite(&m_tCBuffer, sizeof(LIGHTCBUFFER), 1, pFile);
}

void CLight::Load(FILE* pFile)
{
	CSceneComponent::Load(pFile);
	fread(&m_tCBuffer, sizeof(LIGHTCBUFFER), 1, pFile);

	GET_SINGLE(CLightManager)->AddLight(this);

}

void CLight::ShowWindow()
{
	CSceneComponent::ShowWindow();
	if (ImGui::Begin("Light"))
	{
		ImGui::Text(GetName().c_str());
		ImGui::ColorPicker4("Diffuse", &m_tCBuffer.vDif.x);
		ImGui::ColorPicker4("Ambient", &m_tCBuffer.vAmb.x);
		ImGui::ColorPicker4("Specular", &m_tCBuffer.vSpe.x);
		ImGui::ColorPicker4("Emissive", &m_tCBuffer.vEmi.x);
		ImGui::SliderInt("Type", reinterpret_cast<int*>(&m_tCBuffer.eType), static_cast<int>(LIGHT_TYPE::DIRECTIONAL), static_cast<int>(LIGHT_TYPE::END)-1 );
		Vector3 vRot = GetWorldRot();
		ImGui::SliderFloat3("Rotation", &vRot.x, -180.f, 180.f);
		SetWorldRot(vRot);
		Vector3 vPos = GetWorldPos();
		ImGui::InputFloat3("Position", &vPos.x);
		SetWorldPos(vPos);
		ImGui::SliderFloat("AngleIn", &m_tCBuffer.fAngleIn, 0.f, 1.f);
		ImGui::SliderFloat("AngleOut", &m_tCBuffer.fAngleOut, 0.f, 1.f);
		ImGui::InputFloat3("Attenuation", &m_tCBuffer.vAttn.x);
		ImGui::InputFloat("Range", &m_tCBuffer.fRange);
		ImGui::InputFloat("Length", &m_tCBuffer.fLength);
	}
	ImGui::End();
}
