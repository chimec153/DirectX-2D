#pragma once
#include "Editor/EditorFlags.h"
#include "GameEngine.h"

class CQuest
{
private:
	CQuest();
	~CQuest();

private:
	QUEST_STATE		m_eState;
	std::string		m_strName;
	std::wstring		m_strDesc;
	std::wstring		m_pTalk[static_cast<int>(QUEST_STATE::END)];

public:
	const std::string& GetName()	const
	{
		return m_strName;
	}

	const std::wstring& GetDesc()	const
	{
		return m_strDesc;
	}

	std::wstring GetTalk(QUEST_STATE eState = QUEST_STATE::NONE)	const
	{
		if (static_cast<int>(eState) >= static_cast<int>(QUEST_STATE::END) ||
			static_cast<int>(eState) < static_cast<int>(QUEST_STATE::NONE))
			return std::wstring();

		return m_pTalk[static_cast<int>(eState)];
	}
	QUEST_STATE GetState()	const
	{
		return m_eState;
	}

	void SetState(QUEST_STATE eState)
	{
		m_eState = eState;
	}

public:
	static std::unordered_map<std::string, CQuest*>	m_mapQuest;
	static CQuest* FindQuest(const std::string& strKey);
	static void Destroy();

public:
	static void LoadQuestInfo(const char* pFilePath, const std::string& strPathKey = DATA_PATH);
};

