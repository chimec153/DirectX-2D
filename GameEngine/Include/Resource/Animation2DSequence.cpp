#include "Animation2DSequence.h"
#include "Texture.h"
#include "ResourceManager.h"
#include "Animation2DNotify.h"
#include "../Component/SpriteComponent.h"
#include "../PathManager.h"


CAnimation2DSequence::CAnimation2DSequence()	:
	m_pTexture(nullptr),
	m_fPlayRate(1.f),
	m_fMaxTime(1.f),
	m_iFrameMax(0)
{
}

CAnimation2DSequence::CAnimation2DSequence(const CAnimation2DSequence& pSeq)	:
	CRef(pSeq)
{
	m_pTexture = pSeq.m_pTexture;

	m_fPlayRate = pSeq.m_fPlayRate;
	m_fMaxTime = pSeq.m_fMaxTime;
	m_iFrameMax = pSeq.m_iFrameMax;

	size_t iSz = pSeq.m_vecFrame.size();

	for (size_t i = 0; i < iSz; ++i)
	{
		FrameInfo tInfo = {};

		tInfo = pSeq.m_vecFrame[i];

		m_vecFrame.push_back(tInfo);
	}

	std::list<CAnimation2DNotify*>::const_iterator iter = pSeq.m_NotifyList.begin();
	std::list<CAnimation2DNotify*>::const_iterator iterEnd = pSeq.m_NotifyList.end();

	for (; iter != iterEnd; ++iter)
	{
		CAnimation2DNotify* pNot = new CAnimation2DNotify;

		*pNot = *(*iter);

		m_NotifyList.push_back(pNot);
	}
}

CAnimation2DSequence::~CAnimation2DSequence()
{
	SAFE_DELETE_VECLIST(m_NotifyList);
}

void CAnimation2DSequence::AddSprite(CSpriteComponent* pSprite)
{
	m_SpriteList.push_back(pSprite);
}

void CAnimation2DSequence::DeleteSprite(CSpriteComponent* pSprite)
{
	std::list<CSpriteComponent*>::iterator iter = m_SpriteList.begin();
	std::list<CSpriteComponent*>::iterator iterEnd = m_SpriteList.end();

	for (; iter != iterEnd; ++iter)
	{
		if ((*iter) == pSprite)
		{
			m_SpriteList.erase(iter);
			break;
		}	
	}
}

float CAnimation2DSequence::GetPlayRate() const
{
	return m_fPlayRate;
}

float CAnimation2DSequence::GetMaxTime() const
{
	return m_fMaxTime;
}

std::shared_ptr<CTexture> CAnimation2DSequence::GetTexture() const
{
	return m_pTexture;
}

const FrameInfo& CAnimation2DSequence::GetFrame(int idx) const
{
	return m_vecFrame[idx];
}

int CAnimation2DSequence::GetFrameCount() const
{
	return (int)m_vecFrame.size();
}

bool CAnimation2DSequence::Init(std::shared_ptr<CTexture> pTex, float fPlayRate, float fMaxTime)
{
	m_pTexture = pTex;

	if (m_pTexture)
	{
		m_iFrameMax = (int)m_pTexture.get()->GetSize();
	}

	m_fPlayRate = fPlayRate;
	m_fMaxTime = fMaxTime;

	return true;
}

bool CAnimation2DSequence::Init(const std::string& strTex, float fPlayRate, float fMaxTime)
{
	m_pTexture = GET_SINGLE(CResourceManager)->FindTexture(strTex);

	if (!m_pTexture)
		return false;

	m_iFrameMax = (int)m_pTexture.get()->GetSize();

	m_fPlayRate = fPlayRate;
	m_fMaxTime = fMaxTime;

	return true;
}

void CAnimation2DSequence::Init(const TCHAR* pFileName, const std::string& strPathName)
{
	TCHAR strFullPath[MAX_PATH] = {};

	const TCHAR* pPath = GET_SINGLE(CPathManager)->FindPath(strPathName);

	if (pPath)
		lstrcpy(strFullPath, pPath);

	lstrcat(strFullPath, pFileName);

	LoadFromFullPath(strFullPath);
}

void CAnimation2DSequence::LoadFromFullPath(const TCHAR* pFileName)
{
	char pFullPath[MAX_PATH] = {};

	WideCharToMultiByte(CP_ACP, 0, pFileName, -1, pFullPath, lstrlen(pFileName), NULL, NULL);

	FILE* pFile = nullptr;

	fopen_s(&pFile, pFullPath, "rb");

	if (pFile)
	{
		Load(pFile);

		fclose(pFile);
	}
}

void CAnimation2DSequence::SaveFromFullPath(const TCHAR* pFileName)
{
	char pFullPath[MAX_PATH] = {};

	WideCharToMultiByte(CP_ACP, 0, pFileName, -1, pFullPath, lstrlen(pFileName), NULL, NULL);

	FILE* pFile = nullptr;

	fopen_s(&pFile, pFullPath, "wb");

	if (pFile)
	{
		Save(pFile);

		fclose(pFile);
	}
}

void CAnimation2DSequence::AddFrame(Vector2 vStart, Vector2 vEnd)
{
	FrameInfo tInfo;

	tInfo.vStart = vStart;
	tInfo.vEnd = vEnd;

	m_vecFrame.push_back(tInfo);

	m_iFrameMax = (int)m_vecFrame.size();
}

void CAnimation2DSequence::SetFrames()
{/*
	size_t iSz = m_pTexture->GetSize();

	for (size_t i = 0; i < iSz; ++i)
	{
		PResourceInfo pInfo = m_pTexture->GetInfo(i);

		size_t iCnt = pInfo->pImage->GetImageCount();
		
		for (size_t j = 0; j < iCnt; ++j)
		{
			pInfo->pImage->GetImages()[j].

			AddFrame();
		}
	}*/
}

void CAnimation2DSequence::AddAni2DNotify(const std::string& strTag, int iFrame)
{
	CAnimation2DNotify* pNotify = new CAnimation2DNotify;

	pNotify->CreateNotify(strTag, iFrame);

	m_NotifyList.push_back(pNotify);
}

void CAnimation2DSequence::Update(int iFrame)
{
	std::list<CAnimation2DNotify*>::iterator iter = m_NotifyList.begin();
	std::list<CAnimation2DNotify*>::iterator iterEnd = m_NotifyList.end();

	for (; iter != iterEnd; ++iter)
	{
		if ((*iter)->m_iFrame == iFrame)
		{
			std::list<CSpriteComponent*>::iterator iter1 = m_SpriteList.begin();
			std::list<CSpriteComponent*>::iterator iter1End = m_SpriteList.end();

			for (; iter1 != iter1End; ++iter1)
			{
				(*iter1)->CallNotify((*iter)->GetTag());
			}
		}
	}
}

std::shared_ptr<CAnimation2DSequence> CAnimation2DSequence::Clone()
{
	return std::shared_ptr<CAnimation2DSequence>(new CAnimation2DSequence(*this));
}

void CAnimation2DSequence::Save(FILE* pFile)
{
	CRef::Save(pFile);

	bool bTex = false;

	if (m_pTexture)
		bTex = true;

	fwrite(&bTex, 1, 1, pFile);

	if (m_pTexture)
		m_pTexture->Save(pFile);

	fwrite(&m_fPlayRate, 4, 1, pFile);
	fwrite(&m_fMaxTime, 4, 1, pFile);
	fwrite(&m_iFrameMax, 4, 1, pFile);

	int iFrameSize = (int)m_vecFrame.size();

	fwrite(&iFrameSize, 4, 1, pFile);

	if(iFrameSize > 0)
		fwrite(&m_vecFrame[0], sizeof(FrameInfo), iFrameSize, pFile);

	int iSize = (int)m_NotifyList.size();

	fwrite(&iSize, 4, 1, pFile);

	std::list<CAnimation2DNotify*>::iterator iter = m_NotifyList.begin();
	std::list<CAnimation2DNotify*>::iterator iterEnd = m_NotifyList.end();

	for (; iter != iterEnd; ++iter)
		(*iter)->Save(pFile);
}

void CAnimation2DSequence::Load(FILE* pFile)
{
	CRef::Load(pFile);

	bool bTex = false;

	fread(&bTex, 1, 1, pFile);

	if (bTex)
		m_pTexture = GET_SINGLE(CResourceManager)->LoadTexture(pFile);

	fread(&m_fPlayRate, 4, 1, pFile);
	fread(&m_fMaxTime, 4, 1, pFile);
	fread(&m_iFrameMax, 4, 1, pFile);

	int iFrmSize = 0;

	fread(&iFrmSize, 4, 1, pFile);

	if (iFrmSize > 0)
	{
		m_vecFrame.resize(iFrmSize);

		fread(&m_vecFrame[0], sizeof(FrameInfo), iFrmSize, pFile);
	}

	int iSize = 0;

	fread(&iSize, 4, 1, pFile);

	for (int i=0;i<iSize;++i)
	{
		CAnimation2DNotify* pNot = new CAnimation2DNotify;

		pNot->Load(pFile);

		m_NotifyList.push_back(pNot);
	}
		
}
