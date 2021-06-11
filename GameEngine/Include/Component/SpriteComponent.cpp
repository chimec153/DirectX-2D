#include "SpriteComponent.h"
#include "../Resource/Material.h"
#include "../Resource/Mesh2D.h"
#include "../Scene/Scene.h"
#include "../Resource/ResourceManager.h"
#include "../Resource/Animation2DSequence.h"
#include "../Resource/Texture.h"
#include "../Resource/ShaderManager.h"
#include "../RenderManager.h"
#include "../UI/imgui/imgui.h"

CSpriteComponent::CSpriteComponent()	:
	m_pCurrent(nullptr),
	m_tCBuffer(),
	m_bAryTex(false),
	m_bStop(false)
{
	m_eSceneComponentType = SCENE_COMPONENT_TYPE::SCT_2D;
	m_iSceneComponentClassType = (int)SCENECOMPONENT_CLASS_TYPE::SCT_SPRITE;
}

CSpriteComponent::CSpriteComponent(const CSpriteComponent& com)	:
	CSceneComponent(com)
	, m_pCurrent(nullptr)
	, m_tCBuffer(com.m_tCBuffer)
{
	m_mapAni2D.clear();

	std::unordered_map<std::string, PAni2DInfo>::const_iterator iter = com.m_mapAni2D.begin();
	std::unordered_map<std::string, PAni2DInfo>::const_iterator iterEnd = com.m_mapAni2D.end();

	for (; iter != iterEnd; ++iter)
	{
		PAni2DInfo pInfo = new Ani2DInfo;

		*pInfo = *iter->second;

		if (pInfo->pSequence)
		{
			pInfo->pSequence = iter->second->pSequence->Clone();
			pInfo->pSequence->AddSprite(this);
		}

		if (pInfo->pSequence->GetName() == com.m_pCurrent->pSequence->GetName())
			m_pCurrent = pInfo;

		pInfo->pFunc = nullptr;

		m_mapAni2D.insert(std::make_pair(iter->first, pInfo));
	}

	m_bStop = com.m_bStop;
	m_bAryTex = com.m_bAryTex;
}

CSpriteComponent::~CSpriteComponent()
{
	std::unordered_map<std::string, PAni2DInfo>::iterator iter = m_mapAni2D.begin();
	std::unordered_map<std::string, PAni2DInfo>::iterator iterEnd = m_mapAni2D.end();

	for (; iter != iterEnd; ++iter)
	{
		if (iter->second->pSequence)
		{
			iter->second->pSequence->DeleteSprite(this);
		}

		SAFE_DELETE(iter->second);
	}
}

void CSpriteComponent::LED(bool bLED)
{
	m_tCBuffer.fFrame = 1.f - m_tCBuffer.fFrame;
}

int CSpriteComponent::GetFrame() const
{
	return m_pCurrent->iFrame;
}

PAni2DInfo CSpriteComponent::GetCurrentSeq() const
{
	return m_pCurrent;
}

bool CSpriteComponent::IsArrayTex() const
{
	return m_bAryTex;
}

bool CSpriteComponent::IsStopped() const
{
	return m_bStop;
}

const PAni2DInfo& CSpriteComponent::GetCurrentAni() const
{
	return m_pCurrent;
}

const FrameInfo& CSpriteComponent::GetFrameInfo() const
{
	return m_pCurrent->pSequence->GetFrame(m_pCurrent->iFrame);
}

void CSpriteComponent::Stop()
{
	m_bStop = true;
	m_pCurrent->iFrame = 0;
}

void CSpriteComponent::Play()
{
	m_bStop = false;
}

const std::unordered_map<std::string, PAni2DInfo>& CSpriteComponent::GetAniSequences() const
{
	return m_mapAni2D;
}

void CSpriteComponent::SetSize(const Vector2& vSize)
{
	m_tCBuffer.vSize = vSize;
}

void CSpriteComponent::SetStart(const Vector2& vSize)
{
	m_tCBuffer.vStart = vSize;
}

void CSpriteComponent::SetEnd(const Vector2& vSize)
{
	m_tCBuffer.vEnd = vSize;
}

void CSpriteComponent::SetImageType(IMAGE_TYPE eType)
{
	m_tCBuffer.eImageType = static_cast<int>(eType);
}

void CSpriteComponent::AddAni2DInfo(const std::string& strTag, const std::string& strSeq, float fPlayRate, bool bLoop)
{
	PAni2DInfo pInfo = FindAni2DInfo(strTag);

	if (pInfo)
		return;

	pInfo = new Ani2DInfo;

	pInfo->pSequence = GET_SINGLE(CResourceManager)->FindAni2DSequence(strSeq);

	if(pInfo->pSequence)
		pInfo->pSequence->AddSprite(this);

	pInfo->fPlayRate = fPlayRate;
	pInfo->bLoop = bLoop;

	if (!m_pCurrent)
		m_pCurrent = pInfo;

	m_mapAni2D.insert(std::make_pair(strTag, pInfo));
}

void CSpriteComponent::AddAni2DInfo(const std::string& strTag, std::shared_ptr<CAnimation2DSequence> pSequence, float fPlayRate, bool bLoop)
{
	PAni2DInfo pInfo = FindAni2DInfo(strTag);

	if (pInfo)
		return;

	pInfo = new Ani2DInfo;

	pInfo->pSequence = pSequence;

	if (pSequence)
	{
		pSequence->AddRef();
		pSequence->AddSprite(this);
	}

	pInfo->fPlayRate = fPlayRate;
	pInfo->bLoop = bLoop;

	if (!m_pCurrent)
		m_pCurrent = pInfo;

	m_mapAni2D.insert(std::make_pair(strTag, pInfo));
}

PAni2DInfo CSpriteComponent::FindAni2DInfo(const std::string& strTag)
{
	std::unordered_map<std::string, PAni2DInfo>::iterator iter = m_mapAni2D.find(strTag);

	if (iter == m_mapAni2D.end())
		return nullptr;

	return iter->second;
}

void CSpriteComponent::ChangeClip(const std::string& strTag)
{
	PAni2DInfo pInfo = FindAni2DInfo(strTag);

	if (!pInfo)
		return;

	if (pInfo == m_pCurrent)
		return;

	m_pCurrent = pInfo;

	m_pCurrent->fTime = 0.f;
	m_pCurrent->iFrame = 0;
}

void CSpriteComponent::AddNotify(const std::string& strTag, const std::string& strNot, int iFrame)
{
	PAni2DInfo pInfo = FindAni2DInfo(strTag);

	if (!pInfo)
		return;

	if(pInfo->pSequence)
		pInfo->pSequence->AddAni2DNotify(strNot, iFrame);
}

void CSpriteComponent::DeleteSprite(const std::string strTag)
{
	PAni2DInfo pSeq = FindAni2DInfo(strTag);

	if (!pSeq)
		return;

	if(pSeq->pSequence)
		pSeq->pSequence->DeleteSprite(this);
}

void CSpriteComponent::SetFrame(float fFrame)
{
	m_tCBuffer.fFrame = fFrame;
}

void CSpriteComponent::CallNotify(const std::string& strTag)
{
	m_pObj->CallNotify(strTag);
}

bool CSpriteComponent::Init()
{
	if (!CSceneComponent::Init())
		return false;

	class CMesh* pMesh = GET_SINGLE(CResourceManager)->GetDefaultMesh();

	SetMesh(pMesh);

	SAFE_RELEASE(pMesh);

	SetShader("Sprite2D");

	return true;
}

void CSpriteComponent::Start()
{
	CSceneComponent::Start();

	if(m_pCurrent)
		m_pCurrent->pSequence->Update(m_pCurrent->iFrame);
}

void CSpriteComponent::Update(float fTime)
{
	CSceneComponent::Update(fTime);

	if (m_pCurrent && !m_bStop)
	{
		m_pCurrent->fTime += m_pCurrent->fPlayRate * fTime;

		if (m_pCurrent->pSequence)
		{
			float fFrameTime = m_pCurrent->pSequence->m_fMaxTime / m_pCurrent->pSequence->m_iFrameMax;

			while (m_pCurrent->fTime >= fFrameTime)
			{
				m_pCurrent->fTime -= fFrameTime;

				++m_pCurrent->iFrame;

				if (m_pCurrent->iFrame >= m_pCurrent->pSequence->m_iFrameMax)
				{
					if (m_pCurrent->bLoop)
						m_pCurrent->iFrame = 0;

					else
						--m_pCurrent->iFrame;

					if (m_pCurrent->pFunc)
						m_pCurrent->pFunc();
				}

				m_pCurrent->pSequence->Update(m_pCurrent->iFrame);
			}
		}
	}
}

void CSpriteComponent::PostUpdate(float fTime)
{
	CSceneComponent::PostUpdate(fTime);
}

void CSpriteComponent::Collision(float fTime)
{
	CSceneComponent::Collision(fTime);
}

void CSpriteComponent::PreRender(float fTime)
{
	CSceneComponent::PreRender(fTime);

	if (m_bInstance)
		GET_SINGLE(CRenderManager)->AddAnim2DInstancing(this);

}

void CSpriteComponent::Render(float fTime)
{

	if (m_pCurrent)
	{
		int idx = 0;
		int iTex = 0;

		if (m_pCurrent->pSequence)
		{
			m_tCBuffer.eImageType = (int)m_pCurrent->pSequence->m_pTexture->GetType();

			if (m_tCBuffer.eImageType == (int)IMAGE_TYPE::IT_ATLAS)
			{
				idx = m_pCurrent->iFrame;

				m_tCBuffer.vStart = m_pCurrent->pSequence->m_vecFrame[idx].vStart;
				m_tCBuffer.vEnd = m_pCurrent->pSequence->m_vecFrame[idx].vEnd;

				m_tCBuffer.vSize.x = (float)m_pCurrent->pSequence->m_pTexture->GetWidth();
				m_tCBuffer.vSize.y = (float)m_pCurrent->pSequence->m_pTexture->GetHeight();
				SetWorldScale(m_tCBuffer.vEnd.x - m_tCBuffer.vStart.x, m_tCBuffer.vEnd.y - m_tCBuffer.vStart.y, 1.f);
				m_pCurrent->pSequence->m_pTexture->SetShader(0, (int)SHADER_CBUFFER_TYPE::CBUFFER_PIXEL, iTex);
			}

			else if(m_tCBuffer.eImageType == static_cast<int>(IMAGE_TYPE::IT_ARRAY))
			{
				iTex = m_pCurrent->iFrame;

				m_tCBuffer.fFrame = (float)m_pCurrent->iFrame;
				SetWorldScale((float)m_pCurrent->pSequence->m_pTexture->GetWidth(iTex), (float)m_pCurrent->pSequence->m_pTexture->GetHeight(iTex), 1.f);
				m_pCurrent->pSequence->m_pTexture->SetShader(0, (int)SHADER_CBUFFER_TYPE::CBUFFER_PIXEL, iTex);
			}
			else
			{
				iTex = m_pCurrent->iFrame;

				m_tCBuffer.fFrame = (float)m_pCurrent->iFrame;
				m_pCurrent->pSequence->m_pTexture->SetShader(30, (int)SHADER_CBUFFER_TYPE::CBUFFER_PIXEL);
			}
		}

	}
	GET_SINGLE(CShaderManager)->UpdateCBuffer("Sprite", &m_tCBuffer);

	CSceneComponent::Render(fTime);
}

void CSpriteComponent::PostRender(float fTime)
{
	CSceneComponent::PostRender(fTime);
}

std::shared_ptr<CComponent> CSpriteComponent::Clone()
{
	return std::shared_ptr<CComponent>(new CSpriteComponent(*this));
}

void CSpriteComponent::Save(FILE* pFile)
{
	CSceneComponent::Save(pFile);

	std::string strTag = m_pCurrent->pSequence->GetName();

	int iLength = (int)strTag.length();

	fwrite(&iLength, 4, 1, pFile);
	fwrite(strTag.c_str(), 1, iLength, pFile);

	size_t iSz = m_mapAni2D.size();

	fwrite(&iSz, sizeof(size_t), 1, pFile);
	
	std::unordered_map<std::string, PAni2DInfo>::iterator iter = m_mapAni2D.begin();
	std::unordered_map<std::string, PAni2DInfo>::iterator iterEnd = m_mapAni2D.end();

	for (; iter != iterEnd; ++iter)
	{
		int iLength = (int)iter->first.length();

		fwrite(&iLength, 4, 1, pFile);
		fwrite(iter->first.c_str(), 1, iLength, pFile);

		fwrite(&iter->second->bLoop, 1, 1, pFile);
		fwrite(&iter->second->fPlayRate, 4, 1, pFile);
		fwrite(&iter->second->fTime, 4, 1, pFile);
		fwrite(&iter->second->iFrame, 4, 1, pFile);

		iter->second->pSequence->Save(pFile);
	}

	fwrite(&m_tCBuffer, sizeof(AniCBuffer), 1, pFile);
	fwrite(&m_bAryTex, 1, 1, pFile);
	fwrite(&m_bStop, 1, 1, pFile);
}

void CSpriteComponent::Load(FILE* pFile)
{
	CSceneComponent::Load(pFile);

	int iLength = 0;
	char strTag[256] = {};

	fread(&iLength, 4, 1, pFile);
	if (iLength > 0)
	{
		fread(strTag, 1, iLength, pFile);
	}

	std::string pKey = strTag;

	size_t iSz = 0;

	fread(&iSz, sizeof(size_t), 1, pFile);

	for (size_t i = 0; i < iSz; ++i)
	{
		PAni2DInfo pInfo = new Ani2DInfo;

		int iLength = 0;
		char strTag[256] = {};

		fread(&iLength, 4, 1, pFile);
		if (iLength > 0)
		{
			fread(strTag, 1, iLength, pFile);
		}

		std::string strKey = strTag;

		fread(&pInfo->bLoop, 1, 1, pFile);
		fread(&pInfo->fPlayRate, 4, 1, pFile);
		fread(&pInfo->fTime, 4, 1, pFile);
		fread(&pInfo->iFrame, 4, 1, pFile);

		pInfo->pSequence = std::shared_ptr<CAnimation2DSequence>(new CAnimation2DSequence);
		pInfo->pSequence->Load(pFile);

		if (pKey == pInfo->pSequence->GetName())
			m_pCurrent = pInfo;

		m_mapAni2D.insert(std::make_pair(strKey, pInfo));
	}

	fread(&m_tCBuffer, sizeof(AniCBuffer), 1, pFile);
	fread(&m_bAryTex, 1, 1, pFile);
	fread(&m_bStop, 1, 1, pFile);
}

void CSpriteComponent::ShowWindow()
{
	CSceneComponent::ShowWindow();

	std::string strName = GetName();

	if (strName == "")
	{
		strName = "None";
	}

	if (ImGui::Begin(strName.c_str()))
	{
		ImGui::Text("SpriteComponent");
		if (ImGui::InputFloat2("Sprite Size", &m_tCBuffer.vSize.x))
		{
			SetSize(m_tCBuffer.vSize);
		}
		if (ImGui::InputFloat2("Sprite Start", &m_tCBuffer.vStart.x))
		{
			SetStart(m_tCBuffer.vStart);
		}
		if (ImGui::InputFloat2("Sprite End", &m_tCBuffer.vEnd.x))
		{
			SetEnd(m_tCBuffer.vEnd);
		}
		if (ImGui::SliderInt("Image Type", &m_tCBuffer.eImageType, static_cast<int>(IMAGE_TYPE::IT_ATLAS), static_cast<int>(IMAGE_TYPE::TEXTUREARRAY)))
		{
			SetImageType(static_cast<IMAGE_TYPE>(m_tCBuffer.eImageType));
		}
		ImGui::SliderFloat("Frame", &m_tCBuffer.fFrame, 0, 10);
	}
}
