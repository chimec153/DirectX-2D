#pragma once
#include "Editor.h"
class CEditor3D
{
private:
	EDITOR_STATE	m_eState;
	std::wstring	m_strID;
	std::wstring	m_strPW;
	std::wstring	m_strChat;
	bool			m_bChatMode;

public:
	bool Init();

public:
	std::shared_ptr<class CGameObject> CreateObj(int iType);
	class CGameMode* CreateGameMode(int iType);
	bool LoadShader();

public:
	void PacketFunc(const char* pPacket);
	void EnterFunc(const std::wstring& str);
	void WordFunc(const std::wstring& str);
	void ChatFunc(const std::wstring& str);

public:
	void Login();
	void CheckLogin(const char* pPacket);
	void Chat(const std::wstring& str);

	DECLARE_SINGLE(CEditor3D)
};

