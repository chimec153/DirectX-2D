#include "Sound.h"
#include "../Sound/SoundManager.h"
#include "Camera.h"
#include "../CameraManager.h"
#include "../Device.h"

CSound::CSound()
{
	m_iSceneComponentClassType = (int)SCENECOMPONENT_CLASS_TYPE::SOUND;
}

CSound::CSound(const CSound& snd)	:
	CSceneComponent(snd)
{
}

CSound::~CSound()
{
}

void CSound::SetSound(const std::string& strKey)
{
	GET_SINGLE(CSoundManager)->FindSound(strKey);
}

void CSound::Play()
{
	m_pSystem->playSound(m_pSound, m_pGroup, false, &m_pChannel);
}

void CSound::Stop()
{
	
}

bool CSound::Init()
{
	if (!CSceneComponent::Init())
		return false;

	return true;
}

void CSound::Start()
{
	CSceneComponent::Start();
}

void CSound::Update(float fTime)
{
	CSceneComponent::Update(fTime);

	float fVol = 1.f;

	if (m_eSceneComponentType != SCENE_COMPONENT_TYPE::SCT_UI)
	{
		CCamera* pCam = GET_SINGLE(CCameraManager)->GetMainCam();

		Resolution tRS = RESOLUTION;

		Vector3 vCamPos = pCam->GetWorldPos();// -Vector3((float)tRS.iHeight, (float)tRS.iWidth, 0.f) * pCam->GetPivot();

		vCamPos.z = 0.f;

		Vector3 vPos = GetWorldPos();

		float fDst = (vCamPos - vPos).Length();

		if (fDst != 0.f)
			fVol = 50000.f / fDst / fDst;
	}

	GET_SINGLE(CSoundManager)->Vol("", fVol);
}

void CSound::PostUpdate(float fTime)
{
	CSceneComponent::PostUpdate(fTime);
}

void CSound::Collision(float fTime)
{
	CSceneComponent::Collision(fTime);
}

void CSound::PreRender(float fTime)
{
	CSceneComponent::PreRender(fTime);
}

void CSound::Render(float fTime)
{
	CSceneComponent::Render(fTime);
}

void CSound::PostRender(float fTime)
{
	CSceneComponent::PostRender(fTime);
}

std::shared_ptr<CComponent> CSound::Clone()
{
	return std::shared_ptr<CComponent>(new CSound(*this));
}

void CSound::Save(FILE* pFile)
{
	CSceneComponent::Save(pFile);

	int iLength = (int)m_strName.length();

	fwrite(&iLength, 4, 1, pFile);
	fwrite(m_strName.c_str(), 1, iLength, pFile);
}

void CSound::Load(FILE* pFile)
{
	CSceneComponent::Load(pFile);

	int iLength = 0;
	fread(&iLength, 4, 1, pFile);
	
	char strTag[256] = {};
	fread(strTag, 1, iLength, pFile);
}
