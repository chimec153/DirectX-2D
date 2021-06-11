#pragma once

#include "../GameEngine.h"

class CAnimation2DNotify
{
	friend class CAnimation2DSequence;

private:
	CAnimation2DNotify();
	~CAnimation2DNotify();

private:
	std::string					m_strTag;
	int							m_iFrame;

public:
	void SetTag(const std::string& strTag)
	{
		m_strTag = strTag;
	}
	const std::string& GetTag()	const
	{
		return m_strTag;
	}

public:
	void CreateNotify(const std::string& strTag, int iFame);

public:
	void Save(FILE* pFile);
	void Load(FILE* pFile);
};
