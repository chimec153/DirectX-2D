#include "Animation2DNotify.h"

CAnimation2DNotify::CAnimation2DNotify()
{
}

CAnimation2DNotify::~CAnimation2DNotify()
{
}

void CAnimation2DNotify::CreateNotify(const std::string& strTag, int iFrame)
{
	SetTag(strTag);

	m_iFrame = iFrame;
}

void CAnimation2DNotify::Save(FILE* pFile)
{
	int iLength = (int)m_strTag.length();

	fwrite(&iLength, 4, 1, pFile);
	fwrite(m_strTag.c_str(), 1, iLength, pFile);
	fwrite(&m_iFrame, 4, 1, pFile);
}

void CAnimation2DNotify::Load(FILE* pFile)
{
	int iLength = 0;
	char strTag[256] = {};

	fread(&iLength, 4, 1, pFile);
	fread(strTag, 1, iLength, pFile);
	fread(&m_iFrame, 4, 1, pFile);
}