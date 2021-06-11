#include "Decal.h"
#include "../Resource/ShaderManager.h"
#include "../CameraManager.h"
#include "Camera.h"
#include "../UI/imgui/imgui.h"
#include "../Object/GameObject.h"

CDecal::CDecal()	:
	m_tCBuffer()
	, m_fFadeInTime(0.f)
	, m_fFadeIn(0.f)
	, m_bFadeInComplete(false)
	, m_fFadeOutTime(0.f)
	, m_fFadeOut(0.f)
	, m_bFadeOutComplete(false)
	, m_fDurationTime(0.f)
	, m_fDuration(0.f)
	, m_bDurationComplete(false)
	, m_fAlpha(0.f)
	, m_fRadius(0.f)
	, m_bRadiusMode(false)
{
	m_tCBuffer.fAlpha = 0.5f;
	m_eSceneComponentType = SCENE_COMPONENT_TYPE::SCT_3D;
	m_iSceneComponentClassType = static_cast<int>(SCENECOMPONENT_CLASS_TYPE::DECAL);
}

CDecal::CDecal(const CDecal& decal)	:
	CSceneComponent(decal)
	, m_tCBuffer(decal.m_tCBuffer)
	, m_fFadeInTime(decal.m_fFadeInTime)
	, m_fFadeIn(decal.m_fFadeIn)
	, m_bFadeInComplete(decal.m_bFadeInComplete)
	, m_fFadeOutTime(decal.m_fFadeOutTime)
	, m_fFadeOut(decal.m_fFadeOut)
	, m_bFadeOutComplete(decal.m_bFadeOutComplete)
	, m_fDurationTime(decal.m_fDurationTime)
	, m_fDuration(decal.m_fDuration)
	, m_bDurationComplete(decal.m_bDurationComplete)
	, m_fAlpha(decal.m_fAlpha)
	, m_fRadius(decal.m_fRadius)
	, m_bRadiusMode(decal.m_bRadiusMode)
{
}

CDecal::~CDecal()
{
}

void CDecal::SetAlpha(float fAlpha)
{
	m_tCBuffer.fAlpha = fAlpha;
	m_fAlpha = fAlpha;
}

void CDecal::SetFadeIn(float fTime)
{
	m_fFadeInTime = fTime;
	m_bFadeInComplete = false;
}

void CDecal::SetFadeOut(float fTime)
{
	m_fFadeOutTime = fTime;
	m_bFadeOutComplete = false;
}

void CDecal::SetDuration(float fTime)
{
	m_fDurationTime = fTime;
	m_bDurationComplete = false;
}

void CDecal::SetRadius(float fRadius)
{
	m_fRadius = fRadius;
}

void CDecal::SetRadiusMode(bool bMode)
{
	m_bRadiusMode = bMode;

	if (m_bRadiusMode)
	{
		SetShader("DecalRadius");
	}
	else
	{
		SetShader("Decal");
	}
}

bool CDecal::Init()
{
	if (!CSceneComponent::Init())
		return false;

	SetMesh("OBBMesh");
	SetMaterial("Color");
	SetShader("Decal");

	return true;
}

void CDecal::Start()
{
	CSceneComponent::Start();
}

void CDecal::Update(float fTime)
{
	CSceneComponent::Update(fTime);

	if (m_fFadeInTime > 0.f && !m_bFadeInComplete)
	{
		m_fFadeIn += fTime;

		if (m_fFadeIn >= m_fFadeInTime)
		{
			m_fFadeIn = 0.f;
			m_bFadeInComplete = true;
		}
		else
		{
			if (!m_bRadiusMode)
			{
				m_tCBuffer.fAlpha = m_fAlpha * m_fFadeIn / m_fFadeInTime;
			}
			else
			{
				m_tCBuffer.fRadius = m_fRadius* m_fFadeIn / m_fFadeInTime;
			}
		}
	}

	else if (m_fDurationTime > 0.f && !m_bDurationComplete)
	{
		m_fDuration += fTime;

		if (m_fDuration >= m_fDurationTime)
		{
			m_fDuration = 0.f;
			m_bDurationComplete = true;
		}

		if (!m_bRadiusMode)
		{
			m_tCBuffer.fAlpha = m_fAlpha;
		}
		else
		{
			m_tCBuffer.fRadius = m_fRadius;
		}
	}

	else if (m_fFadeOutTime > 0.f && !m_bFadeOutComplete)
	{
		m_fFadeOut += fTime;

		if (m_fFadeOut > m_fFadeOutTime)
		{
			m_fFadeOut = 0.f;
			m_bFadeOutComplete = true;

			if (m_pObj)
			{
				m_pObj->Destroy();
			}
		}

		if (m_bRadiusMode)
		{
			m_tCBuffer.fRadius = m_fRadius * (m_fFadeOutTime - m_fFadeOut) / m_fFadeOutTime;
		}
		else
		{
			m_tCBuffer.fAlpha = m_fAlpha * (m_fFadeOutTime - m_fFadeOut) / m_fFadeOutTime;
		}
	}
}

// Ä¸½¶ Ãæµ¹

void CDecal::PostUpdate(float fTime)
{
	CSceneComponent::PostUpdate(fTime);

	CCamera* pCam = GET_SINGLE(CCameraManager)->GetMainCam();

	Matrix matWorld = GetMatWorld();
	Matrix matView = pCam->GetView();

	Matrix matProj = pCam->GetProj();

	m_tCBuffer.matInvWVP = matWorld * matView * matProj;
	m_tCBuffer.matInvWVP.Inverse();
	m_tCBuffer.matInvWVP.Transpose();	
}

void CDecal::Collision(float fTime)
{
	CSceneComponent::PostUpdate(fTime);
}

void CDecal::PreRender(float fTime)
{
	CSceneComponent::PreRender(fTime);
}

void CDecal::Render(float fTime)
{
	GET_SINGLE(CShaderManager)->UpdateCBuffer("Decal", &m_tCBuffer);
	CSceneComponent::Render(fTime);

}

void CDecal::PostRender(float fTime)
{
	CSceneComponent::PostRender(fTime);
}

std::shared_ptr<CComponent> CDecal::Clone()
{
	return std::shared_ptr<CComponent>(new CDecal(*this));
}

void CDecal::Save(FILE* pFile)
{
	CSceneComponent::Save(pFile);

	fwrite(&m_tCBuffer, sizeof(DECALCBUFFER), 1, pFile);
	fwrite(&m_fFadeInTime, 4, 1, pFile);
	fwrite(&m_fFadeIn, 4, 1, pFile);
	fwrite(&m_bFadeInComplete, 1, 1, pFile);
	fwrite(&m_fFadeOutTime, 4, 1, pFile);
	fwrite(&m_fFadeOut, 4, 1, pFile);
	fwrite(&m_bFadeOutComplete, 1, 1, pFile);
	fwrite(&m_fDurationTime, 4, 1, pFile);
	fwrite(&m_fDuration, 4, 1, pFile);
	fwrite(&m_bDurationComplete, 1, 1, pFile);
	fwrite(&m_fAlpha, 4, 1, pFile);
}

void CDecal::Load(FILE * pFile)
{
	CSceneComponent::Load(pFile);

	fread(&m_tCBuffer, sizeof(DECALCBUFFER), 1, pFile);
	fread(&m_fFadeInTime, 4, 1, pFile);
	fread(&m_fFadeIn, 4, 1, pFile);
	fread(&m_bFadeInComplete, 1, 1, pFile);
	fread(&m_fFadeOutTime, 4, 1, pFile);
	fread(&m_fFadeOut, 4, 1, pFile);
	fread(&m_bFadeOutComplete, 1, 1, pFile);
	fread(&m_fDurationTime, 4, 1, pFile);
	fread(&m_fDuration, 4, 1, pFile);
	fread(&m_bDurationComplete, 1, 1, pFile);
	fread(&m_fAlpha, 4, 1, pFile);
}

void CDecal::ShowWindow()
{
	CSceneComponent::ShowWindow();

	std::string strName = GetName();

	if (strName == "")
	{
		strName = "None";
	}

	if (ImGui::Begin(strName.c_str()))
	{
		ImGui::Text("Decal");
		ImGui::SliderFloat("Fade-In", &m_fFadeIn, 0.f, m_fFadeInTime);
		ImGui::InputFloat("Fade-In Time", &m_fFadeInTime);
		ImGui::Checkbox("Fade-In Complete", &m_bFadeInComplete);
		ImGui::SliderFloat("Duration", &m_fDuration, 0.f, m_fDurationTime);
		ImGui::InputFloat("Duration Time", &m_fDurationTime);
		ImGui::Checkbox("Duration Complete", &m_bDurationComplete);
		ImGui::SliderFloat("Fade-Out", &m_fFadeOut, 0.f, m_fFadeOutTime);
		ImGui::InputFloat("Fade-Out Time", &m_fFadeOutTime);
		ImGui::Checkbox("Fade-Out Complete", &m_bFadeOutComplete);
		ImGui::InputFloat("Max Alpha", &m_fAlpha);
		ImGui::SliderFloat("Current Alpha", &m_tCBuffer.fAlpha, 0.f, m_fAlpha);
		ImGui::InputFloat("Max Radius", &m_fRadius);
		ImGui::SliderFloat("Current Radius", &m_tCBuffer.fRadius, 0.f, m_fRadius);
	}
	ImGui::End();
}
