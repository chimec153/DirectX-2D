#include "UI.h"
#include "Component/SpriteComponent.h"
#include "Resource/ResourceManager.h"
#include "Device.h"
#include "UI/UFont.h"
#include "Player3D.h"
#include "Resource/Material.h"
#include "Component/Mesh2DComponent.h"
#include "UI/UIImage.h"
#include "Scene/Scene.h"
#include "UI/Bar.h"
#include "UI/UIButton.h"
#include "Scene/SceneManager.h"
#include "../GameMode/Town.h"
#include "Sound/SoundManager.h"

CUI::CUI() :
	m_iValue(1)
	, m_iMax(1)
	, m_vScaleMax(512.f, 32.f)
	, m_pPlayer(nullptr)
	, m_fBloodAlpha(0.f)
	, m_fBackAlpha(0.f)
{
	m_iObjType = static_cast<int>(EDITOR_OBJECT_TYPE::UI);
}

CUI::CUI(const CUI& ui) :
	CGameObject(ui)
{
	*this = ui;
}

CUI::~CUI()
{
}

void CUI::AddValue(int iValue)
{
	m_iValue += iValue;

	if (m_iValue < 0)
		m_iValue = 0;

	else if (m_iValue > m_iMax)
		m_iValue = m_iMax;

	Vector2 vScale = m_vScaleMax;
	vScale.x = m_vScaleMax.x * ((float)m_iValue / m_iMax);

	static_cast<CSpriteComponent*>(m_pBar.get())->SetWorldScale(vScale.x, vScale.y, 0.f);
}

void CUI::SetValue(int iValue)
{
	m_iValue = iValue;

	if (m_iValue < 0)
		m_iValue = 0;

	else if (m_iValue > m_iMax)
		m_iValue = m_iMax;
}

void CUI::SetCastingBar(bool bOn)
{
	m_pCastingBack->SetEnable(bOn);
}

bool CUI::Init()
{
	GET_SINGLE(CResourceManager)->LoadTexture("icon_sheet_1301", L"IconSheet\\Texture2D\\icon_sheet_1301.dds");
	GET_SINGLE(CResourceManager)->LoadTexture("BLUIAimTargeting_I1", L"BLUIAimTargeting\\Texture2D\\BLUIAimTargeting_I1.dds");
	GET_SINGLE(CResourceManager)->LoadTexture("UI2_Worldmap_lock_gray_Img", L"UI_Share_Button\\Texture2D\\UI2_Worldmap_lock_gray_Img.dds");
	GET_SINGLE(CResourceManager)->LoadTexture("BLUIChat_IC", L"BLUIChat\\Texture2D\\BLUIChat_IC.dds");

	if (!CGameObject::Init())
		return false;

	CResourceManager::GetInst()->CreateAni2DSequence("BarBack", "BLUIPC_I9");

	CResourceManager::GetInst()->CreateAni2DSequence("HPBar", "BLUIPC_I22");

	CResourceManager::GetInst()->CreateAni2DSequence("MPBar", "BLUIPC_I22");

	CResourceManager::GetInst()->CreateAni2DSequence("ExpBar", "BLUIEXPBar_I3");

	CResourceManager::GetInst()->CreateAni2DSequence("ExpBarBack", "BLUIEXPBar_I3");

	CResourceManager::GetInst()->CreateAni2DSequence("Exp", "BLUIEXPBar_I3");

	CResourceManager::GetInst()->CreateAni2DSequence("GaugeBack", "BLUIPC_I9");

	CResourceManager::GetInst()->CreateAni2DSequence("CenterBack", "BLUIPC_I9");

	CResourceManager::GetInst()->CreateAni2DSequence("Wing", "BLUIPC_I9");

	CResourceManager::GetInst()->CreateAni2DSequence("SkillR", "icon_sheet_009");
	CResourceManager::GetInst()->CreateAni2DSequence("Skill1", "icon_sheet_009");
	CResourceManager::GetInst()->CreateAni2DSequence("Skill2", "icon_sheet_009");
	CResourceManager::GetInst()->CreateAni2DSequence("Skill3", "icon_sheet_009");
	CResourceManager::GetInst()->CreateAni2DSequence("Skill4", "icon_sheet_009");
	CResourceManager::GetInst()->CreateAni2DSequence("Skill5", "icon_sheet_009");
	CResourceManager::GetInst()->CreateAni2DSequence("Skill6", "icon_sheet_009");
	CResourceManager::GetInst()->CreateAni2DSequence("Skill7", "icon_sheet_009");
	CResourceManager::GetInst()->CreateAni2DSequence("Skill8", "icon_sheet_009");

	CResourceManager::GetInst()->CreateAni2DSequence("SkillSlot", "UI2_Slot_Lock_icon");
	//780, 4	860,102
	CResourceManager::GetInst()->AddFrame("GaugeBack", Vector2(0.f, 0.f), Vector2(773.f, 173.f));
	CResourceManager::GetInst()->AddFrame("CenterBack", Vector2(780.f, 4.f), Vector2(860.f, 102.f));
	CResourceManager::GetInst()->AddFrame("Wing", Vector2(772.f, 113.f), Vector2(853.f, 170.f));
	CResourceManager::GetInst()->AddFrame("BarBack", Vector2(240.f, 182.f), Vector2(674.f, 198.f));
	CResourceManager::GetInst()->AddFrame("HPBar", Vector2(361.f, 371.f), Vector2(723.f, 383.f));
	CResourceManager::GetInst()->AddFrame("HPBar", Vector2(361.f, 383.f), Vector2(723.f, 395.f));
	CResourceManager::GetInst()->AddFrame("HPBar", Vector2(361.f, 395.f), Vector2(723.f, 407.f));
	CResourceManager::GetInst()->AddFrame("HPBar", Vector2(361.f, 407.f), Vector2(723.f, 419.f));
	CResourceManager::GetInst()->AddFrame("MPBar", Vector2(0.f, 203.f), Vector2(360.f, 215.f));
	CResourceManager::GetInst()->AddFrame("MPBar", Vector2(0.f, 215.f), Vector2(360.f, 227.f));
	CResourceManager::GetInst()->AddFrame("MPBar", Vector2(0.f, 227.f), Vector2(360.f, 239.f));
	CResourceManager::GetInst()->AddFrame("MPBar", Vector2(0.f, 239.f), Vector2(360.f, 251.f));
	CResourceManager::GetInst()->AddFrame("SkillR", Vector2(576.f, 0.f), Vector2(624.f, 48.f));
	CResourceManager::GetInst()->AddFrame("Skill1", Vector2(192.f, 0.f), Vector2(240.f, 48.f));
	CResourceManager::GetInst()->AddFrame("Skill2", Vector2(480.f, 0.f), Vector2(528.f, 48.f));
	CResourceManager::GetInst()->AddFrame("Skill3", Vector2(816.f, 0.f), Vector2(864.f, 48.f));
	CResourceManager::GetInst()->AddFrame("Skill4", Vector2(576.f, 96.f), Vector2(624.f, 144.f));
	CResourceManager::GetInst()->AddFrame("Skill5", Vector2(192.f, 0.f), Vector2(240.f, 48.f));
	CResourceManager::GetInst()->AddFrame("Skill6", Vector2(480.f, 0.f), Vector2(528.f, 48.f));
	CResourceManager::GetInst()->AddFrame("Skill7", Vector2(816.f, 0.f), Vector2(864.f, 48.f));
	CResourceManager::GetInst()->AddFrame("Skill8", Vector2(576.f, 96.f), Vector2(624.f, 144.f));
	CResourceManager::GetInst()->AddFrame("ExpBarBack", Vector2(77.f, 0.f), Vector2(177.f, 13.f));
	CResourceManager::GetInst()->AddFrame("Exp", Vector2(41.f, 0.f), Vector2(77.f, 13.f));
	CResourceManager::GetInst()->AddFrame("ExpBar", Vector2(57.f, 5.f), Vector2(59.f, 8.f));
	CResourceManager::GetInst()->AddFrame("SkillSlot", Vector2(451.f, 819.f), Vector2(501.f, 869.f));

	Resolution tRS = RESOLUTION;
	m_pBackImage = CreateComponent<CSpriteComponent>("GaugeBack");
	CSpriteComponent* _pGaugeBack = static_cast<CSpriteComponent*>(m_pBackImage.get());

	_pGaugeBack->AddAni2DInfo("Idle", "GaugeBack");
	_pGaugeBack->SetWorldScale(772.f, 172.f, 1.f);
	_pGaugeBack->SetWorldPos(0.f, -400.f, 0.f);
	_pGaugeBack->SetPivot(0.5f, 0.5f, 0.f);
	_pGaugeBack->SetSceneComponentType(SCENE_COMPONENT_TYPE::SCT_UI);
	_pGaugeBack->SetZOrder(8);
	_pGaugeBack->SetInheritScale(false);
	SetRootComponent(m_pBackImage);

	std::shared_ptr<CComponent> pChatWindow = CreateComponent<CUIImage>("CharWindow");

	static_cast<CSceneComponent*>(m_pBackImage.get())->AddChild(pChatWindow);

	static_cast<CUIImage*>(pChatWindow.get())->SetWorldScale(469.f, 253.f, 1.f);
	static_cast<CUIImage*>(pChatWindow.get())->SetUV(Vector2(), Vector2(469.f, 253.f));
	static_cast<CUIImage*>(pChatWindow.get())->SetSize(Vector2(1024.f, 256.f));
	static_cast<CUIImage*>(pChatWindow.get())->SetTexture("BLUIChat_IC");
	static_cast<CUIImage*>(pChatWindow.get())->SetPivot(0.5f, 0.5f, 0.f);
	static_cast<CUIImage*>(pChatWindow.get())->SetRelativePos(-705.f, 100.f, 0.f);
	static_cast<CUIImage*>(pChatWindow.get())->SetZOrder(3);

	m_pChatWindowSmall = CreateComponent<CUIImage>("CharWindowSmall");
	m_pChatWindowSmall->SetEnable(false);

	static_cast<CSceneComponent*>(pChatWindow.get())->AddChild(m_pChatWindowSmall);

	static_cast<CUIImage*>(m_pChatWindowSmall.get())->SetWorldScale(470.f, 33.f, 1.f);
	static_cast<CUIImage*>(m_pChatWindowSmall.get())->SetUV(Vector2(469.f, 0.f), Vector2(939.f, 33.f));
	static_cast<CUIImage*>(m_pChatWindowSmall.get())->SetSize(Vector2(1024.f, 256.f));
	static_cast<CUIImage*>(m_pChatWindowSmall.get())->SetTexture("BLUIChat_IC");
	static_cast<CUIImage*>(m_pChatWindowSmall.get())->SetPivot(0.5f, 0.5f, 0.f);
	static_cast<CUIImage*>(m_pChatWindowSmall.get())->SetRelativePos(0.f, -145.f, 0.f);
	static_cast<CUIImage*>(m_pChatWindowSmall.get())->SetZOrder(3);

	std::shared_ptr<CComponent> pCenterBack = CreateComponent<CSpriteComponent>("CenterBack");
	CSpriteComponent* _pCenterBack = static_cast<CSpriteComponent*>(pCenterBack.get());

	_pCenterBack->AddAni2DInfo("Idle", "CenterBack");
	_pCenterBack->SetWorldScale(80.f, 98.f, 0.f);
	_pCenterBack->SetRelativePos(0.f, -20.f, 0.f);
	_pCenterBack->SetPivot(0.5f, 0.5f, 0.f);
	_pCenterBack->SetSceneComponentType(SCENE_COMPONENT_TYPE::SCT_UI);
	_pCenterBack->SetZOrder(7);
	_pCenterBack->SetInheritScale(false);
	_pGaugeBack->AddChild(pCenterBack);

	std::shared_ptr<CComponent> pWing = CreateComponent<CSpriteComponent>("Wing");
	CSpriteComponent* _pWing = static_cast<CSpriteComponent*>(pWing.get());

	_pWing->AddAni2DInfo("Idle", "Wing");
	_pWing->SetWorldScale(81.f, 57.f, 0.f);
	_pWing->SetRelativePos(0.f, 9.f, 0.f);
	_pWing->SetPivot(0.5f, 0.5f, 0.f);
	_pWing->SetSceneComponentType(SCENE_COMPONENT_TYPE::SCT_UI);
	_pWing->SetZOrder(6);
	_pWing->SetInheritScale(false);
	_pCenterBack->AddChild(pWing);

	std::shared_ptr<CComponent> pJobIcon = CreateComponent<CUIImage>("Jop Icon");
	CUIImage* _pJobIcon = static_cast<CUIImage*>(pJobIcon.get());

	_pJobIcon->SetTexture("icon_sheet_1301");
	_pJobIcon->SetWorldScale(48.f, 48.f, 1.f);
	_pJobIcon->SetRelativePos(0.f, 9.f, 0.f);
	_pJobIcon->SetPivot(0.5f, 0.5f, 0.f);
	_pJobIcon->SetSceneComponentType(SCENE_COMPONENT_TYPE::SCT_UI);
	_pJobIcon->SetZOrder(6);
	_pJobIcon->SetUV(Vector2(14.f * 48.f, 9.f * 48.f), Vector2(15.f * 48.f, 10.f * 48.f));
	_pJobIcon->SetSize(Vector2(960.f, 672.f));
	_pJobIcon->SetInheritScale(false);
	_pCenterBack->AddChild(pJobIcon);

	std::shared_ptr<CComponent> pBarBack = CreateComponent<CSpriteComponent>("BarBack");
	CSpriteComponent* _pBarBack = static_cast<CSpriteComponent*>(pBarBack.get());

	_pBarBack->AddAni2DInfo("Idle", "BarBack");
	_pBarBack->SetWorldScale(434.f, 16.f, 0.f);
	_pBarBack->SetRelativePos(-263.f, -5.f, 0.f);
	_pBarBack->SetPivot(0.5f, 0.5f, 0.f);
	_pBarBack->SetSceneComponentType(SCENE_COMPONENT_TYPE::SCT_UI);
	_pBarBack->SetZOrder(6);
	_pBarBack->SetInheritScale(false);
	_pGaugeBack->AddChild(pBarBack);

	m_pBar = CreateComponent<CSpriteComponent>("HPBar");
	CSpriteComponent* _pHPBar = static_cast<CSpriteComponent*>(m_pBar.get());

	_pHPBar->AddAni2DInfo("Idle", "HPBar");
	_pHPBar->SetWorldScale(362.f, 12.f, 0.f);
	_pHPBar->SetRelativePosX(181.f);
	_pHPBar->SetPivot(1.f, 0.5f, 0.f);
	_pHPBar->SetZOrder(5);
	_pHPBar->SetSceneComponentType(SCENE_COMPONENT_TYPE::SCT_UI);
	_pHPBar->SetInheritScale(false);

	_pBarBack->AddChild(m_pBar);

	std::shared_ptr<CComponent> pMPBarBack = CreateComponent<CSpriteComponent>("MPBarBack");
	CSpriteComponent* _pMPBarBack = static_cast<CSpriteComponent*>(pMPBarBack.get());

	_pMPBarBack->AddAni2DInfo("Idle", "BarBack");
	_pMPBarBack->SetWorldScale(434.f, 16.f, 0.f);
	_pMPBarBack->SetRelativePos(263.f, -5.f, 0.f);
	_pMPBarBack->SetPivot(0.5f, 0.5f, 0.f);
	_pMPBarBack->SetSceneComponentType(SCENE_COMPONENT_TYPE::SCT_UI);
	_pMPBarBack->SetZOrder(6);
	_pMPBarBack->SetInheritScale(false);
	_pGaugeBack->AddChild(pMPBarBack);

	m_pMPBar = CreateComponent<CSpriteComponent>("MPBar");
	CSpriteComponent* _pMPBar = static_cast<CSpriteComponent*>(m_pMPBar.get());

	_pMPBar->AddAni2DInfo("Idle", "MPBar");
	_pMPBar->SetWorldScale(362.f, 12.f, 0.f);
	_pMPBar->SetRelativePosX(-181.f);
	_pMPBar->SetPivot(0.f, 0.5f, 0.f);
	_pMPBar->SetZOrder(5);
	_pMPBar->SetInheritScale(false);
	_pMPBar->SetSceneComponentType(SCENE_COMPONENT_TYPE::SCT_UI);

	_pMPBarBack->AddChild(m_pMPBar);

	std::shared_ptr<CComponent> pExpBarBack = CreateComponent<CSpriteComponent>("ExpBarBack0");
	CSpriteComponent* _pExpBarBack = static_cast<CSpriteComponent*>(pExpBarBack.get());

	_pExpBarBack->AddAni2DInfo("Idle", "ExpBarBack");
	_pExpBarBack->SetWorldScale(434.f, 16.f, 0.f);
	_pExpBarBack->SetWorldPos(-tRS.iWidth / 2.f + 36.f, -340.f, 0.f);
	_pExpBarBack->SetPivot(0.f, 0.f, 0.f);
	_pExpBarBack->SetSceneComponentType(SCENE_COMPONENT_TYPE::SCT_UI);
	_pExpBarBack->SetZOrder(6);
	_pExpBarBack->SetInheritScale(false);
	_pGaugeBack->AddChild(pExpBarBack);

	for (int i = 0; i < 18; i++)
	{
		char strName[MAX_PATH] = {};

		sprintf_s(strName, "ExpBarBack%d",i+1);

		pExpBarBack = CreateComponent<CSpriteComponent>(strName);
		_pExpBarBack->AddChild(pExpBarBack);
		_pExpBarBack = static_cast<CSpriteComponent*>(pExpBarBack.get());

		_pExpBarBack->AddAni2DInfo("Idle", "ExpBarBack");
		_pExpBarBack->SetRelativePos(100.f, 0.f, 0.f);
		_pExpBarBack->SetPivot(0.f, 0.f, 0.f);
		_pExpBarBack->SetSceneComponentType(SCENE_COMPONENT_TYPE::SCT_UI);
		_pExpBarBack->SetZOrder(6);
	}

	std::shared_ptr<CComponent> pExp = CreateComponent<CSpriteComponent>("Exp");
	CSpriteComponent* _pExp = static_cast<CSpriteComponent*>(pExp.get());

	_pExp->AddAni2DInfo("Idle", "Exp");
	_pExp->SetWorldScale(434.f, 16.f, 0.f);
	_pExp->SetWorldPos(-tRS.iWidth / 2.f, -340.f, 0.f);
	_pExp->SetPivot(0.f, 0.f, 0.f);
	_pExp->SetSceneComponentType(SCENE_COMPONENT_TYPE::SCT_UI);
	_pExp->SetZOrder(6);
	_pGaugeBack->AddChild(pExp);

	m_pExpBar = CreateComponent<CSpriteComponent>("ExpBar");
	CSpriteComponent* _pExpBar = static_cast<CSpriteComponent*>(m_pExpBar.get());

	_pExpBar->AddAni2DInfo("Idle", "ExpBar");
	_pExpBar->SetRelativePos(0.f, 4.f, 0.f);
	_pExpBar->SetPivot(0.f, 0.f, 0.f);
	_pExpBar->SetZOrder(5);
	_pExpBar->SetSceneComponentType(SCENE_COMPONENT_TYPE::SCT_UI);

	_pExp->AddChild(m_pExpBar);

	std::vector<const char*> vecstr;

	vecstr.push_back("SkillR");
	vecstr.push_back("Skill1");
	vecstr.push_back("Skill2");
	vecstr.push_back("Skill3");
	vecstr.push_back("Skill4");
	vecstr.push_back("Skill5");
	vecstr.push_back("Skill6");
	vecstr.push_back("Skill7");
	vecstr.push_back("Skill8");

	for (int i = 0; i < static_cast<int>(SKILL_TYPE::_4); ++i)
	{
		std::shared_ptr<CComponent> pSkillSlot = CreateComponent<CSpriteComponent>("SkillSlot");
		CSpriteComponent* _pSkillSlot = static_cast<CSpriteComponent*>(pSkillSlot.get());

		_pSkillSlot->AddAni2DInfo("Idle", "SkillSlot");
		_pSkillSlot->SetRelativePos(-320.f + 52.f *i, -45.f, 0.f);
		_pSkillSlot->SetPivot(0.5f, 0.5f, 0.f);
		_pSkillSlot->SetSceneComponentType(SCENE_COMPONENT_TYPE::SCT_UI);
		_pSkillSlot->SetZOrder(4);
		_pGaugeBack->AddChild(pSkillSlot);

		m_pSkillIcon[i] = CreateComponent<CSpriteComponent>(vecstr[i]);
		CSpriteComponent* _pSkillIconR = static_cast<CSpriteComponent*>(m_pSkillIcon[i].get());

		_pSkillIconR->AddAni2DInfo("Idle", vecstr[i]);
		_pSkillIconR->SetWorldScale(48.f, 48.f, 0.f);
		_pSkillIconR->SetRelativePos(0.f, 0.f, 0.f);
		_pSkillIconR->SetPivot(0.5f, 0.5f, 0.f);
		_pSkillIconR->SetSceneComponentType(SCENE_COMPONENT_TYPE::SCT_UI);
		_pSkillIconR->SetZOrder(5);
		_pSkillIconR->SetShader("CoolTimeShader");
		_pSkillSlot->AddChild(m_pSkillIcon[i]);

	}


	m_pFont = CreateComponent<CUFont>("HPFont");

	TCHAR strHP[64] = {};

	swprintf_s(strHP, TEXT("%d / %d"), m_iValue, m_iMax);

	static_cast<CUFont*>(m_pFont.get())->SetText(strHP);
	static_cast<CUFont*>(m_pFont.get())->SetRect(-360.f, 2.f, 0.f, 0.f);
	static_cast<CUFont*>(m_pFont.get())->AddRelativePos(0.f, 0.f, 0.f);
	static_cast<CUFont*>(m_pFont.get())->SetAlignH(TEXT_ALIGN_HORIZONAL::RIGHT);
	static_cast<CUFont*>(m_pFont.get())->SetAlignV(TEXT_ALIGN_VERTICAL::MID);
	static_cast<CUFont*>(m_pFont.get())->SetSize(15.f);
	static_cast<CUFont*>(m_pFont.get())->SetShadowOffset(Vector3(1.f, -1.f, 0.f));
	static_cast<CUFont*>(m_pFont.get())->SetShadowColor(0.f, 0.f, 0.f, 1.f);

	_pHPBar->AddChild(m_pFont);

	m_pBloodHUD = CreateComponent<CMesh2DComponent>("BloodHUD");

	_pGaugeBack->AddChild(m_pBloodHUD);
	static_cast<CMesh2DComponent*>(m_pBloodHUD.get())->SetWorldScale(
		static_cast<float>(tRS.iWidth), static_cast<float>(tRS.iHeight), 0.f);
	static_cast<CMesh2DComponent*>(m_pBloodHUD.get())->SetPivot(0.5f, 0.5f, 0.f);
	static_cast<CMesh2DComponent*>(m_pBloodHUD.get())->SetWorldPos(0.f, 0.f, 0.f);
	static_cast<CMesh2DComponent*>(m_pBloodHUD.get())->SetSceneComponentType(
		SCENE_COMPONENT_TYPE::SCT_UI);
	static_cast<CMesh2DComponent*>(m_pBloodHUD.get())->SetInheritScale(false);
	static_cast<CMesh2DComponent*>(m_pBloodHUD.get())->SetTexture(LINK_TYPE::LT_DIF, "FX_Com_LowHP_TEX_KYH");
	std::shared_ptr<CMaterial> pMtrl = static_cast<CMesh2DComponent*>(m_pBloodHUD.get())->GetMaterial();

	if (pMtrl)
	{
		pMtrl->SetDiffuseColor(1.f, 1.f, 1.f, 0.f);
	}


	m_pBlinkHUD = CreateComponent<CMesh2DComponent>("BlinkHUD");

	_pGaugeBack->AddChild(m_pBlinkHUD);
	static_cast<CMesh2DComponent*>(m_pBlinkHUD.get())->SetWorldScale(
		static_cast<float>(tRS.iWidth), static_cast<float>(tRS.iHeight), 0.f);
	static_cast<CMesh2DComponent*>(m_pBlinkHUD.get())->SetPivot(0.5f, 0.5f, 0.f);
	static_cast<CMesh2DComponent*>(m_pBlinkHUD.get())->SetWorldPos(0.f, 0.f, 0.f);
	static_cast<CMesh2DComponent*>(m_pBlinkHUD.get())->SetSceneComponentType(
		SCENE_COMPONENT_TYPE::SCT_UI);
	static_cast<CMesh2DComponent*>(m_pBlinkHUD.get())->SetInheritScale(false);
	static_cast<CMesh2DComponent*>(m_pBlinkHUD.get())->SetTexture(LINK_TYPE::LT_DIF,
		"BlinkHUD", TEXT("FX_Com_Screen\\Texture2D\\FX_Com_LowerHP_Blink_TEX_KYH.dds"), TEXTURE_PATH);
	pMtrl = static_cast<CMesh2DComponent*>(m_pBlinkHUD.get())->GetMaterial();

	if (pMtrl)
	{
		pMtrl->SetDiffuseColor(1.f, 1.f, 1.f, 0.f);
	}


	m_pMsgHUD = CreateComponent<CMesh2DComponent>("MsgHUD");

	_pGaugeBack->AddChild(m_pMsgHUD);
	static_cast<CMesh2DComponent*>(m_pMsgHUD.get())->SetWorldScale(
		static_cast<float>(tRS.iWidth), static_cast<float>(tRS.iHeight), 0.f);
	static_cast<CMesh2DComponent*>(m_pMsgHUD.get())->SetPivot(0.5f, 0.5f, 0.f);
	static_cast<CMesh2DComponent*>(m_pMsgHUD.get())->SetWorldPos(0.f, 0.f, 0.f);
	static_cast<CMesh2DComponent*>(m_pMsgHUD.get())->SetSceneComponentType(
		SCENE_COMPONENT_TYPE::SCT_UI);
	static_cast<CMesh2DComponent*>(m_pMsgHUD.get())->SetInheritScale(false);
	static_cast<CMesh2DComponent*>(m_pMsgHUD.get())->SetTexture(LINK_TYPE::LT_DIF,
		"MsgHUD", TEXT("Msg.png"), TEXTURE_PATH);
	pMtrl = static_cast<CMesh2DComponent*>(m_pMsgHUD.get())->GetMaterial();

	if (pMtrl)
	{
		pMtrl->SetDiffuseColor(1.f, 1.f, 1.f, 0.f);
	}

	m_pDieButton = CreateComponent<CUIButton>("DieButton");
	m_pDieButton->SetEnable(false);

	static_cast<CMesh2DComponent*>(m_pMsgHUD.get())->AddChild(m_pDieButton);

	static_cast<CUIButton*>(m_pDieButton.get())->SetTexture(BUTTON_STATE::OFF, "UI2_Worldmap_lock_gray_Img");
	static_cast<CUIButton*>(m_pDieButton.get())->SetTexture(BUTTON_STATE::ON, "UI2_Worldmap_lock_gray_Img");
	static_cast<CUIButton*>(m_pDieButton.get())->SetTexture(BUTTON_STATE::PRESS, "UI2_Worldmap_lock_gray_Img");
	static_cast<CUIButton*>(m_pDieButton.get())->SetTexture(BUTTON_STATE::DISABLE, "UI2_Worldmap_lock_gray_Img");
	static_cast<CUIButton*>(m_pDieButton.get())->SetSize(Vector2(1024.f, 512.f));
	static_cast<CUIButton*>(m_pDieButton.get())->SetUV(Vector2(370.f, 318.f), Vector2(486.f, 342.f));
	static_cast<CUIButton*>(m_pDieButton.get())->SetPivot(0.5f, 0.5f, 0.f);
	static_cast<CUIButton*>(m_pDieButton.get())->SetCallBack(BUTTON_STATE::PRESS, this, &CUI::DieButtonCallBack);
	static_cast<CUIButton*>(m_pDieButton.get())->SetWorldPos(0.f, -200.f, 0.f);
	static_cast<CUIButton*>(m_pDieButton.get())->SetInheritScale(false);
	static_cast<CUIButton*>(m_pDieButton.get())->SetWorldScale(116.f, 24.f, 1.f);

	m_pCastingBack = CreateComponent<CUIImage>("CastingBarBack");
	m_pCastingBack->SetEnable(false);

	_pGaugeBack->AddChild(m_pCastingBack);

	static_cast<CUIImage*>(m_pCastingBack.get())->SetTexture("32_UI2_Waypoint_Manager_Start_icon");
	static_cast<CUIImage*>(m_pCastingBack.get())->SetUV(Vector2(552.f,841.f), Vector2(844.f,857.f));
	static_cast<CUIImage*>(m_pCastingBack.get())->SetSize(Vector2(1024.f, 1024.f));
	static_cast<CUIImage*>(m_pCastingBack.get())->SetPivot(0.5f, 0.5f, 0.f);
	static_cast<CUIImage*>(m_pCastingBack.get())->SetInheritScale(false);
	static_cast<CUIImage*>(m_pCastingBack.get())->SetWorldScale(292.f, 16.f, 1.f);
	static_cast<CUIImage*>(m_pCastingBack.get())->SetRelativePosY(225.f);
	
	m_pCastingBar = CreateComponent<CBar>("CastingBar");

	static_cast<CUIImage*>(m_pCastingBack.get())->AddChild(m_pCastingBar);

	static_cast<CBar*>(m_pCastingBar.get())->SetTexture("32_UI2_Waypoint_Manager_Start_icon");
	static_cast<CBar*>(m_pCastingBar.get())->SetUV(Vector2(274.f, 881.f), Vector2(514.f, 891.f));
	static_cast<CBar*>(m_pCastingBar.get())->SetSize(Vector2(1024.f, 1024.f));
	static_cast<CBar*>(m_pCastingBar.get())->SetPivot(0.5f, 0.5f, 0.f);
	static_cast<CBar*>(m_pCastingBar.get())->SetWorldScale(240.f, 10.f, 1.f);

	m_pCastingFont = CreateComponent<CUFont>("CastingFont");

	static_cast<CBar*>(m_pCastingBar.get())->AddChild(m_pCastingFont);
	
	static_cast<CUFont*>(m_pCastingFont.get())->SetRect(-120.f, 0.f, 120.f, 0.f);
	static_cast<CUFont*>(m_pCastingFont.get())->SetSize(12.f);
	static_cast<CUFont*>(m_pCastingFont.get())->SetAlignH(TEXT_ALIGN_HORIZONAL::RIGHT);
	static_cast<CUFont*>(m_pCastingFont.get())->SetShadowOffset(Vector3(1.f, -1.f, 0.f));
	static_cast<CUFont*>(m_pCastingFont.get())->SetShadowColor(0.f, 0.f, 0.f, 1.f);

	std::shared_ptr<CComponent> pAim = CreateComponent<CUIImage>("AimTarget");

	static_cast<CSceneComponent*>(m_pBackImage.get())->AddChild(pAim);

	static_cast<CUIImage*>(pAim.get())->SetInheritScale(false);
	static_cast<CUIImage*>(pAim.get())->SetWorldScale(32.f, 32.f, 1.f);
	static_cast<CUIImage*>(pAim.get())->SetWorldPos(0.f, 0.f, 0.f);
	static_cast<CUIImage*>(pAim.get())->SetPivot(0.5f, 0.5f, 0.f);
	static_cast<CUIImage*>(pAim.get())->SetUV(Vector2(0.f, 0.f), Vector2(32.f, 32.f));
	static_cast<CUIImage*>(pAim.get())->SetSize(Vector2(32.f,32.f));
	static_cast<CUIImage*>(pAim.get())->SetTexture("BLUIAimTargeting_I1");

	m_pChat = CreateComponent<CUFont>("Chat");

	static_cast<CSceneComponent*>(m_pBackImage.get())->AddChild(m_pChat);

	static_cast<CUFont*>(m_pChat.get())->SetText(L"");
	static_cast<CUFont*>(m_pChat.get())->SetRect(-250.f, 0.f, 250.f, 0.f);
	static_cast<CUFont*>(m_pChat.get())->SetAlignH(TEXT_ALIGN_HORIZONAL::LEFT);
	static_cast<CUFont*>(m_pChat.get())->SetWorldPos(-690.f, -440.f, 0.f);
	static_cast<CUFont*>(m_pChat.get())->SetSize(15.f);
	return true;
}

void CUI::Start()
{
	CGameObject::Start();

	CLayer* pLayer = m_pScene->FindLayer(ALBEDO_LAYER);

	if (pLayer)
	{
		std::shared_ptr<CGameObject> pPlayer = pLayer->FindObj("Player");

		if (pPlayer)
		{
			m_pPlayer = static_cast<CPlayer3D*>(pPlayer.get());
		}
	}
}

void CUI::Update(float fTime)
{
	CGameObject::Update(fTime);

	Resolution tRS = RESOLUTION;


	if (m_pPlayer)
	{
		int iHP = m_pPlayer->GetHP();
		int iHPMax = m_pPlayer->GetHPMax();

		int iExp = m_pPlayer->GetHP();
		int iExpMax = m_pPlayer->GetHPMax();

		TCHAR strHP[64] = {};

		swprintf_s(strHP, TEXT("%d / %d"), iHP, iHPMax);

		for (int i = static_cast<int>(SKILL_TYPE::R); 
			i < static_cast<int>(SKILL_TYPE::_4); 
			i++)
		{
			float fCoolTimeR = m_pPlayer->GetCoolTime(static_cast<SKILL_TYPE>(i));
			float fCoolTimeRMax = m_pPlayer->GetCoolTimeMax(static_cast<SKILL_TYPE>(i));

			if (fCoolTimeRMax == 0.f)
				continue;

			static_cast<CSpriteComponent*>(m_pSkillIcon[i -1].get())->SetFrame(fCoolTimeR / fCoolTimeRMax);
		}

		static_cast<CSceneComponent*>(m_pBar.get())->SetWorldScale(362.f * static_cast<float>(iHP) / iHPMax, 12.f, 0.f);
		static_cast<CSceneComponent*>(m_pExpBar.get())->SetWorldScale(tRS.iWidth / 100.f * static_cast<float>(iExp) / iExpMax, 6.f, 0.f);

		static_cast<CUFont*>(m_pFont.get())->SetText(strHP);


		if (iHP <= iHPMax * 0.3f)
		{
			if (iHP == 0)
			{
				if (!m_pDieButton->IsEnable())
				{
					m_pDieButton->SetEnable(true);
				}
			}

			m_fBloodAlpha += fTime;

			if (m_fBloodAlpha > 1.f)
				m_fBloodAlpha = 0.f;

			std::shared_ptr<CMaterial> pMtrl = static_cast<CMesh2DComponent*>(m_pBloodHUD.get())->GetMaterial();

			if (pMtrl)
			{
				pMtrl->SetDiffuseColor(1.f, 1.f, 1.f, m_fBloodAlpha);
			}
			if (iHP <= 0.f)
			{
				pMtrl = static_cast<CMesh2DComponent*>(m_pBlinkHUD.get())->GetMaterial();

				if (pMtrl)
				{
					pMtrl->SetDiffuseColor(1.f, 1.f, 1.f, 0.5f);
				}

				pMtrl = static_cast<CMesh2DComponent*>(m_pMsgHUD.get())->GetMaterial();

				if (pMtrl)
				{
					pMtrl->SetDiffuseColor(1.f, 1.f, 1.f, 1.f);
				}
			}
		}

		if (m_pCastingBack->IsEnable())
		{
			float fMax = m_pPlayer->GetCastMax();
			float fTime = m_pPlayer->GetCastTime();

			static_cast<CBar*>(m_pCastingBar.get())->SetPrg(fTime / fMax);

			TCHAR strPercent[MAX_PATH] = {};

			swprintf_s(strPercent, MAX_PATH, L"%.2f / %.2f", fTime, fMax);

			static_cast<CUFont*>(m_pCastingFont.get())->SetText(strPercent);
		}

		if (m_pPlayer->IsAttack())
		{
			m_fBackAlpha += fTime * 1.5f;

			if (m_fBackAlpha > 1.f)
				m_fBackAlpha = 1.f;

			static_cast<CSpriteComponent*>(m_pBackImage.get())->GetMaterial()->SetDiffuseColor(1.f, 1.f, 1.f, m_fBackAlpha);
		}
		else
		{
			m_fBackAlpha -= fTime * 1.5f;

			if (m_fBackAlpha < 0.f)
				m_fBackAlpha = 0.f;

			static_cast<CSpriteComponent*>(m_pBackImage.get())->GetMaterial()->SetDiffuseColor(1.f, 1.f, 1.f, m_fBackAlpha);
		}
	}

}

void CUI::PostUpdate(float fTime)
{
	CGameObject::PostUpdate(fTime);
}

void CUI::Collision(float fTime)
{
	CGameObject::Collision(fTime);
}

void CUI::PreRender(float fTime)
{
	CGameObject::PreRender(fTime);
}

void CUI::Render(float fTime)
{
	CGameObject::Render(fTime);
}

void CUI::PostRender(float fTime)
{
	CGameObject::PostRender(fTime);
}

std::shared_ptr<CGameObject> CUI::Clone()
{
	return std::shared_ptr<CGameObject>(new CUI(*this));
}

void CUI::Save(FILE* pFile)
{
	CGameObject::Save(pFile);
}

void CUI::Load(FILE* pFile)
{
	CGameObject::Load(pFile);
}

void CUI::DieButtonCallBack(float fTime)
{
	m_pPlayer->SetNameAndHPBar(false);
	m_pPlayer->AddHP(m_pPlayer->GetHPMax());
	m_pDieButton->SetEnable(false);	

	GET_SINGLE(CSoundManager)->Stop("NewBattle_02");
	GET_SINGLE(CSoundManager)->Stop("Amb_Forest_1");

	GET_SINGLE(CSceneManager)->CreateNextScene();

	GET_SINGLE(CSceneManager)->SetGameMode<CTown>(false);

	CScene* pNextScene = GET_SINGLE(CSceneManager)->GetNextScene();

	CLayer* pNextAlpha = pNextScene->FindLayer(ALPHA_LAYER);

	CLayer* pNextAlbedo = pNextScene->FindLayer(ALBEDO_LAYER);

	CLayer* pAlbedo = m_pScene->FindLayer(ALBEDO_LAYER);

	CLayer* pAlpha = m_pScene->FindLayer(ALPHA_LAYER);

	std::shared_ptr<CGameObject> pPlayer = pAlbedo->FindObj("Player");
	std::shared_ptr<CGameObject> pWeapon = pAlbedo->FindObj("Staff");
	std::shared_ptr<CGameObject> pRide = pAlpha->FindObj("BoneDragon");

	if (pRide)
	{
		pNextAlpha->AddObj(pRide);
	}
	pNextAlbedo->AddObj(pPlayer);
	pNextAlbedo->AddObj(pWeapon);

	CLayer* pNextUILayer = pNextScene->FindLayer(UI_LAYER);
	CLayer* pUILayer = m_pScene->FindLayer(UI_LAYER);

	std::shared_ptr<CGameObject> pUI = pUILayer->FindObj("UI");
	std::shared_ptr<CGameObject> pInventory = pUILayer->FindObj("Inventory");
	std::shared_ptr<CGameObject> pEquip = pUILayer->FindObj("Equip");
	std::shared_ptr<CGameObject> pButtonInven = pUILayer->FindObj("ButtonInven");
	std::shared_ptr<CGameObject> pButtonEquip = pUILayer->FindObj("ButtonEquip");
	std::shared_ptr<CGameObject> pMission = pUILayer->FindObj("Mission");

	pNextUILayer->AddObj(pUI);
	pNextUILayer->AddObj(pInventory);
	pNextUILayer->AddObj(pEquip);
	if (pButtonInven)
	{
		pNextUILayer->AddObj(pButtonInven);
	}
	if (pButtonEquip)
	{
		pNextUILayer->AddObj(pButtonEquip);
	}
	if (pMission)
	{
		pNextUILayer->AddObj(pMission);
	}

	pPlayer->SetWorldPos(3000.f, 0.f, 7000.f);
	static_cast<CPlayer3D*>(pPlayer.get())->SetTarget(nullptr);
	static_cast<CPlayer3D*>(pPlayer.get())->DamageEnd();
	static_cast<CPlayer3D*>(pPlayer.get())->ChangeSequenceAll("Idle");
	std::shared_ptr<CMaterial> pMtrl = static_cast<CMesh2DComponent*>(m_pBloodHUD.get())->GetMaterial();

	if (pMtrl)
	{
		pMtrl->SetDiffuseColor(1.f, 1.f, 1.f, 0.f);
	}
		pMtrl = static_cast<CMesh2DComponent*>(m_pBlinkHUD.get())->GetMaterial();

		if (pMtrl)
		{
			pMtrl->SetDiffuseColor(1.f, 1.f, 1.f, 0.f);
		}

		pMtrl = static_cast<CMesh2DComponent*>(m_pMsgHUD.get())->GetMaterial();

		if (pMtrl)
		{
			pMtrl->SetDiffuseColor(1.f, 1.f, 1.f, 0.f);
		}
}

void CUI::SetCharText(const TCHAR* pText)
{
	if (m_pChat)
	{
		static_cast<CUFont*>(m_pChat.get())->SetText(pText);
	}
}

void CUI::SetChatWindowSmall(bool bEnable)
{
	if (m_pChatWindowSmall)
	{
		m_pChatWindowSmall->SetEnable(bEnable);
	}
}
