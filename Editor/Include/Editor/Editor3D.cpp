#include "Editor3D.h"
#include "Scene/SceneManager.h"
#include "Object/Player3D.h"
#include "Object/Monster3D.h"
#include "Object/Bullet3D.h"
#include "Object/Tree.h"
#include "Object/Rock.h"
#include "Object/Equip.h"
#include "Object/EquipButton.h"
#include "Object/InvenButton.h"
#include "Object/Inventory3D.h"
#include "Object/TargetUI.h"
#include "Object/UI.h"
#include "GameMode/MainMode.h"
#include "EditMode.h"
#include "Resource/ShaderManager.h"
#include "Resource/GraphicShader.h"
#include "../Quest.h"
#include "Item.h"
#include "Server/ClientSocket.h"
#include "Object/Char.h"
#include "StartGameMode.h"
#include "Server/User.h"
#include "UI/UFont.h"
#include "Object/UI.h"

DEFINITION_SINGLE(CEditor3D)

CEditor3D::CEditor3D()	:
	m_eState(EDITOR_STATE::WAIT) 
	, m_bChatMode(false)
{
	AllocConsole();
	FILE* pFile = nullptr;
	FILE* pstd = stdout;
	freopen_s(&pstd, "CONOUT$", "w", stdout);
}
CEditor3D::~CEditor3D()
{
	CItem::Destroy();
	CQuest::Destroy();
}

bool CEditor3D::Init()
{
	GET_SINGLE(CSceneManager)->SetEditorCreateObjFunc(this, &CEditor3D::CreateObj);
	GET_SINGLE(CSceneManager)->SetEditorCreateGameModeFunc(this, &CEditor3D::CreateGameMode);

	CQuest::LoadQuestInfo("Quests.csv");

	CClientSocket* pSocket = CClientSocket::GetInst();
	if (pSocket)
	{
		m_eState = EDITOR_STATE::CONNECT;
		pSocket->CClientSocket::SetPacketFunc<CEditor3D>(this, &CEditor3D::PacketFunc);
	}

	GET_SINGLE(CInput)->SetEnterFunc(this, &CEditor3D::EnterFunc);
	GET_SINGLE(CInput)->SetWordFunc(this, &CEditor3D::WordFunc);

	if (!LoadShader())
		return false;


	return true;
}

std::shared_ptr<CGameObject> CEditor3D::CreateObj(int iType)
{
	std::shared_ptr<CGameObject> pObj;

	switch (static_cast<EDITOR_OBJECT_TYPE>(iType))
	{
	case EDITOR_OBJECT_TYPE::NONE:
		pObj = std::shared_ptr<CGameObject>(new CGameObject);
		break;
	case EDITOR_OBJECT_TYPE::PLAYER:
		pObj = std::shared_ptr<CGameObject>(new CPlayer3D);
		break;
	case EDITOR_OBJECT_TYPE::BULLET:
		pObj = std::shared_ptr<CGameObject>(new CBullet3D);
		break;
	case EDITOR_OBJECT_TYPE::MONSTER:
		pObj = std::shared_ptr<CGameObject>(new CMonster3D);
		break;
	case EDITOR_OBJECT_TYPE::TREE:
		pObj = std::shared_ptr<CGameObject>(new CTree);
		break;
	case EDITOR_OBJECT_TYPE::ROCK:
		pObj = std::shared_ptr<CGameObject>(new CRock);
		break;
	case EDITOR_OBJECT_TYPE::EQUIP:
		pObj = std::shared_ptr<CGameObject>(new CEquip);
		break;
	case EDITOR_OBJECT_TYPE::EQUIPBUTTON:
		pObj = std::shared_ptr<CGameObject>(new CEquipButton);
		break;
	case EDITOR_OBJECT_TYPE::INVENTORYBUTTON:
		pObj = std::shared_ptr<CGameObject>(new CInvenButton);
		break;
	case EDITOR_OBJECT_TYPE::INVENTORY:
		pObj = std::shared_ptr<CGameObject>(new CInventory3D);
		break;
	case EDITOR_OBJECT_TYPE::TARGETUI:
		pObj = std::shared_ptr<CGameObject>(new CTargetUI);
		break;
	case EDITOR_OBJECT_TYPE::UI:
		pObj = std::shared_ptr<CGameObject>(new CUI);
		break;
	}

	return pObj;
}

CGameMode* CEditor3D::CreateGameMode(int iType)
{
	switch (static_cast<EDITOR_GAME_MODE_TYPE>(iType))
	{
	case EDITOR_GAME_MODE_TYPE::NONE:
	{
		//return new CGameMode;
	}
		break;
	case EDITOR_GAME_MODE_TYPE::EDIT:
	{
		return new CEditMode;
	}
		break;
	case EDITOR_GAME_MODE_TYPE::LOADING:
	{
	}
		break;
	case EDITOR_GAME_MODE_TYPE::START:
	{

	}
		break;
	case EDITOR_GAME_MODE_TYPE::MAIN:
	{
		return new CMainMode;
	}
		break;
	}

	return nullptr;
}

bool CEditor3D::LoadShader()
{
	std::shared_ptr<CShader> pParticleShader = GET_SINGLE(CShaderManager)->CreateShader<CGraphicShader>("ParticleBullet");
	CGraphicShader* _pParticleShader = (CGraphicShader*)pParticleShader.get();
	if (!_pParticleShader->LoadVertexShader("VS", TEXT("Particle.fx")))
		return false;
	if (!_pParticleShader->LoadGeometryShader("GS", TEXT("Particle.fx")))
		return false;
	if (!_pParticleShader->LoadPixelShader("BulletPS", TEXT("Particle.fx")))
		return false;

	_pParticleShader->AddInputDesc("POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0);
	_pParticleShader->AddInputDesc("TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 8, D3D11_INPUT_PER_VERTEX_DATA, 0);

	if (!_pParticleShader->CreateInputLayout())
		return false;

	pParticleShader = GET_SINGLE(CShaderManager)->CreateShader<CGraphicShader>("AlphaParticle");
	_pParticleShader = (CGraphicShader*)pParticleShader.get();
	if (!_pParticleShader->LoadVertexShader("VS", TEXT("Particle.fx")))
		return false;
	if (!_pParticleShader->LoadGeometryShader("GS", TEXT("Particle.fx")))
		return false;
	if (!_pParticleShader->LoadPixelShader("EmvAlphaPS", TEXT("Particle.fx")))
		return false;

	_pParticleShader->AddInputDesc("POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0);
	_pParticleShader->AddInputDesc("TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 8, D3D11_INPUT_PER_VERTEX_DATA, 0);

	if (!_pParticleShader->CreateInputLayout())
		return false;

	std::shared_ptr<CShader> pShader = GET_SINGLE(CShaderManager)->CreateShader<CGraphicShader>("ProjPoint");

	CGraphicShader* _pShader = static_cast<CGraphicShader*>(pShader.get());
	if (!_pShader->LoadVertexShader("ProjPointVS", TEXT("3D.fx")))
		return false;
	if (!_pShader->LoadPixelShader("ProjPointPS", TEXT("3D.fx")))
		return false;

	_pShader->AddInputDesc("POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0);
	_pShader->AddInputDesc("NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0);
	_pShader->AddInputDesc("TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 8, D3D11_INPUT_PER_VERTEX_DATA, 0);
	_pShader->AddInputDesc("TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0);
	_pShader->AddInputDesc("BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0);
	_pShader->AddInputDesc("BLENDWEIGHT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0);
	_pShader->AddInputDesc("BLENDINDICES", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0);

	if (!_pShader->CreateInputLayout())
		return false;

	pShader = GET_SINGLE(CShaderManager)->CreateShader<CGraphicShader>("EmvRClip");

	_pShader = static_cast<CGraphicShader*>(pShader.get());
	if (!_pShader->LoadVertexShader("StandardVS3D", TEXT("3D.fx")))
		return false;
	if (!_pShader->LoadPixelShader("EmvRClipPS", TEXT("3D.fx")))
		return false;

	_pShader->AddInputDesc("POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0);
	_pShader->AddInputDesc("NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0);
	_pShader->AddInputDesc("TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 8, D3D11_INPUT_PER_VERTEX_DATA, 0);
	_pShader->AddInputDesc("TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0);
	_pShader->AddInputDesc("BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0);
	_pShader->AddInputDesc("BLENDWEIGHT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0);
	_pShader->AddInputDesc("BLENDINDICES", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0);

	if (!_pShader->CreateInputLayout())
		return false;

	pShader = GET_SINGLE(CShaderManager)->CreateShader<CGraphicShader>("EmvGClip");

	_pShader = static_cast<CGraphicShader*>(pShader.get());
	if (!_pShader->LoadVertexShader("StandardVS3D", TEXT("3D.fx")))
		return false;
	if (!_pShader->LoadPixelShader("EmvGClipPS", TEXT("3D.fx")))
		return false;

	_pShader->AddInputDesc("POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0);
	_pShader->AddInputDesc("NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0);
	_pShader->AddInputDesc("TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 8, D3D11_INPUT_PER_VERTEX_DATA, 0);
	_pShader->AddInputDesc("TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0);
	_pShader->AddInputDesc("BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0);
	_pShader->AddInputDesc("BLENDWEIGHT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0);
	_pShader->AddInputDesc("BLENDINDICES", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0);

	if (!_pShader->CreateInputLayout())
		return false;

	pShader = GET_SINGLE(CShaderManager)->CreateShader<CGraphicShader>("CoolTimeShader");

	_pShader = static_cast<CGraphicShader*>(pShader.get());
	if (!_pShader->LoadVertexShader("CoolTimeVS", TEXT("CoolTime.fx")))
		return false;
	if (!_pShader->LoadPixelShader("CoolTimePS", TEXT("CoolTime.fx")))
		return false;

	_pShader->AddInputDesc("POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0);
	_pShader->AddInputDesc("TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 8, D3D11_INPUT_PER_VERTEX_DATA, 0);
	_pShader->AddInputDesc("COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0);

	if (!_pShader->CreateInputLayout())
		return false;


	std::shared_ptr<CShader> pTrail = GET_SINGLE(CShaderManager)->CreateShader<CGraphicShader>("Trail");

	if (!static_cast<CGraphicShader*>(pTrail.get())->LoadVertexShader("TrailVS", L"3D.fx"))
		return false;
	if (!static_cast<CGraphicShader*>(pTrail.get())->LoadPixelShader("TrailPS", L"3D.fx"))
		return false;

	pTrail->AddInputDesc("POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0);
	pTrail->AddInputDesc("TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 8, D3D11_INPUT_PER_VERTEX_DATA, 0);
	pTrail->AddInputDesc("ALPHA", 0, DXGI_FORMAT_R32_FLOAT, 0, 4, D3D11_INPUT_PER_VERTEX_DATA, 0);

	if (!pTrail->CreateInputLayout())
		return false;

	pTrail = GET_SINGLE(CShaderManager)->CreateShader<CGraphicShader>("TrailTexture");

	if (!static_cast<CGraphicShader*>(pTrail.get())->LoadVertexShader("TrailVS", L"3D.fx"))
		return false;
	if (!static_cast<CGraphicShader*>(pTrail.get())->LoadPixelShader("TrailTexturePS", L"3D.fx"))
		return false;

	pTrail->AddInputDesc("POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0);
	pTrail->AddInputDesc("TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 8, D3D11_INPUT_PER_VERTEX_DATA, 0);
	pTrail->AddInputDesc("ALPHA", 0, DXGI_FORMAT_R32_FLOAT, 0, 4, D3D11_INPUT_PER_VERTEX_DATA, 0);

	if (!pTrail->CreateInputLayout())
		return false;

	std::shared_ptr<CShader> pOcean = GET_SINGLE(CShaderManager)->CreateShader<CGraphicShader>("Ocean");

	if (!static_cast<CGraphicShader*>(pOcean.get())->LoadVertexShader("VS", L"Ocean.fx"))
		return false;
	if (!static_cast<CGraphicShader*>(pOcean.get())->LoadPixelShader("PS", L"Ocean.fx"))
		return false;

	pOcean->AddInputDesc("POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0);
	pOcean->AddInputDesc("NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0);
	pOcean->AddInputDesc("TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 8, D3D11_INPUT_PER_VERTEX_DATA, 0);
	pOcean->AddInputDesc("TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0);
	pOcean->AddInputDesc("BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0);
	pOcean->AddInputDesc("BLENDWEIGHT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0);
	pOcean->AddInputDesc("BLENDINDICES", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0);

	if (!pOcean->CreateInputLayout())
		return false;
	return true;
}

void CEditor3D::PacketFunc(const char* pPacket)
{
	PACKET_HEADER_TYPE eHeader= static_cast<PACKET_HEADER_TYPE>(pPacket[0]);

	const char* _pPacket = &pPacket[1];

	switch (eHeader)
	{
	case PACKET_HEADER_TYPE::CHAT:
	{
		CScene* pScene = GET_SINGLE(CSceneManager)->GetScene();

		std::shared_ptr<CGameObject> pObj = pScene->CreateObject<CChar>("Chat", pScene->FindLayer(UI_LAYER));

		TCHAR strMsg[MSG_SIZE] = {};

		memcpy_s(strMsg, MSG_SIZE, _pPacket, MSG_SIZE - 1);

		static_cast<CChar*>(pObj.get())->SetText(strMsg);
	}
		break;
	case PACKET_HEADER_TYPE::LOGIN:
	{
		CheckLogin(&pPacket[1]);
	}
		break;
	}

}

void CEditor3D::EnterFunc(const std::wstring& str)
{
	CClientSocket* pSocket = CClientSocket::GetInst();

	if (pSocket)
	{
		switch (m_eState)
		{
		case EDITOR_STATE::CONNECT:
		{
			if (m_strID == L"")
			{
				m_strID = str;
			}
			else
			{
				m_strPW = str;

				Login();
			}
		}
			break;
		case EDITOR_STATE::LOGIN:
		{
			CScene* pScene = GET_SINGLE(CSceneManager)->GetScene();

			CLayer* pLayer = pScene->FindLayer(UI_LAYER);

			std::shared_ptr<CGameObject> pUI = pLayer->FindObj("UI");

			if (!m_bChatMode)
			{
				m_bChatMode = true;

			}
			else
			{
				Chat(str);
				ChatFunc(L"");
				m_bChatMode = false;
			}

			if (pUI)
			{
				static_cast<CUI*>(pUI.get())->SetChatWindowSmall(m_bChatMode);
			}
		}
			break;
		}
	}
}

void CEditor3D::WordFunc(const std::wstring& str)
{
	CScene* pScene = GET_SINGLE(CSceneManager)->GetScene();

	CStartGameMode* pMode = static_cast<CStartGameMode*>(pScene->GetGameMode());

	if (m_strID == L"")
	{
		pMode->SetIDText(str.c_str());
	}
	else
	{
		pMode->AddPWText(L"*");
	}
}

void CEditor3D::ChatFunc(const std::wstring& str)
{
	if (!m_bChatMode)
	{
		return;
	}

	CScene* pScene = GET_SINGLE(CSceneManager)->GetScene();

	CLayer* pLayer = pScene->FindLayer(UI_LAYER);

	std::shared_ptr<CGameObject> pUI = pLayer->FindObj("UI");

	if (pUI)
	{
		static_cast<CUI*>(pUI.get())->SetCharText(str.c_str());
	}

}

void CEditor3D::Login()
{
	if (m_strID == L"")
	{
		return;
	}
	else if(m_strPW == L"")
	{
		m_strPW = GET_SINGLE(CInput)->GetWord();
	}

	TCHAR strID[MSG_SIZE] = {};
	wcscpy_s(strID, m_strID.c_str());
	TCHAR strPW[MSG_SIZE] = {};
	wcscpy_s(strPW, m_strPW.c_str());

	char pPacket[MSG_SIZE] = {};

	int idx = 0;

	pPacket[idx++] = static_cast<char>(EDITOR_HEADER::LOGIN);

	int iLength = lstrlen(strID);
	pPacket[idx++] = iLength;
	memcpy_s(&pPacket[idx], MSG_SIZE - idx, strID, iLength * 2);
	idx += iLength * 2;
	iLength = lstrlen(strPW);
	pPacket[idx++] = iLength;
	memcpy_s(&pPacket[idx], MSG_SIZE - idx, strPW, iLength * 2);

	CClientSocket* pSocket =  CClientSocket::GetInst();

	if (pSocket)
	{
		pSocket->SendPacket(pPacket);
	}
}

void CEditor3D::CheckLogin(const char* pPacket)
{
	if (pPacket[0])
	{
		CClientSocket* pSocket = CClientSocket::GetInst();

		if (pSocket)
		{
			CUser* pUser = pSocket->GetUser();

			pUser->SetState(USER_STATE::LOGIN);
			m_eState = EDITOR_STATE::LOGIN;

			SAFE_RELEASE(pUser);

			CScene* pScene = GET_SINGLE(CSceneManager)->GetScene();

			CStartGameMode* pMode = static_cast<CStartGameMode*>(pScene->GetGameMode());

			pMode->Login(0.f);

			GET_SINGLE(CInput)->SetWordFunc(this, &CEditor3D::ChatFunc);
		}
	}
	else
	{

	}
}

void CEditor3D::Chat(const std::wstring& str)
{
	std::wstring _str =L"[" + m_strID +L"]: " ;
	_str += str;
	
	char pPacket[MSG_SIZE] = {};
	int idx = 0;
	pPacket[idx++] = static_cast<char>(EDITOR_HEADER::CHAT);
	memcpy_s(&pPacket[idx], MSG_SIZE - 1, _str.c_str(), _str.length() * 2);

	CClientSocket* pSocket = CClientSocket::GetInst();

	if (pSocket)
	{
		pSocket->SendPacket(pPacket);
	}
}
