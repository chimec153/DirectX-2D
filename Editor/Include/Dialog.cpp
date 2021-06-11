#include "Dialog.h"
#include "UI/UIButton.h"
#include "UI/Panel.h"
#include "UI/TitleBar.h"
#include "UI/UFont.h"
#include "Editor/Object/NPC.h"
#include "Resource/ResourceManager.h"
#include "UI/UIImage.h"
#include "Quest.h"
#include "Editor/Object/MissionUI.h"
#include "Scene/Scene.h"
#include "Sound/SoundManager.h"

CDialog::CDialog()  :
    m_iPage(0)
    , m_iMaxPage(1)
    , m_pNPC(nullptr)
    , m_pQuest(nullptr)
{
}

CDialog::CDialog(const CDialog& obj)    :
    CGameObject(obj)
    , m_iPage(obj.m_iPage)
    , m_iMaxPage(obj.m_iMaxPage)
    , m_pNPC(obj.m_pNPC)
    , m_pQuest(obj.m_pQuest)
{
}

CDialog::~CDialog()
{
}

void CDialog::AddPage(const TCHAR* pText)
{
    m_iMaxPage++;

    m_vecText.push_back(pText);

    SetPage(m_iPage);
}

void CDialog::ClearPage()
{
    m_vecText.clear();
    m_iMaxPage = 0;
}

void CDialog::SetNameText(const TCHAR* pName)
{
    if (m_pName)
    {
        static_cast<CUFont*>(m_pName.get())->SetText(pName);
    }
}

void CDialog::SetQuest(const std::string& strKey)
{
    m_pQuest = CQuest::FindQuest(strKey);
}

void CDialog::Open()
{
    SetEnable(true);
    SetPage(m_iPage);

    std::shared_ptr<CGameObject> pUI = m_pScene->FindLayer(UI_LAYER)->FindObj("UI");

    if (pUI)
    {
        pUI->SetEnable(false);
    }
    pUI = m_pScene->FindLayer(UI_LAYER)->FindObj("ButtonInven");

    if (pUI)
    {
        pUI->SetEnable(false);
    }

    pUI = m_pScene->FindLayer(UI_LAYER)->FindObj("ButtonEquip");

    if (pUI)
    {
        pUI->SetEnable(false);
    }
}

void CDialog::Close()
{
    SetEnable(false);

    std::shared_ptr<CGameObject> pUI = m_pScene->FindLayer(UI_LAYER)->FindObj("UI");

    if (pUI)
    {
        pUI->SetEnable(true);
    }
    pUI = m_pScene->FindLayer(UI_LAYER)->FindObj("ButtonInven");

    if (pUI)
    {
        pUI->SetEnable(true);
    }

    pUI = m_pScene->FindLayer(UI_LAYER)->FindObj("ButtonEquip");

    if (pUI)
    {
        pUI->SetEnable(true);
    }
}

bool CDialog::Init()
{
    GET_SINGLE(CSoundManager)->LoadSound("Effect", "Button_Normal", false,
        "Button\\Button_Normal.ogg");

    if(!CGameObject::Init())
        return false;

    GET_SINGLE(CResourceManager)->LoadTexture("BLUITalk_I52", L"BLUITalk\\Texture2D\\BLUITalk_I52.dds");
    GET_SINGLE(CResourceManager)->LoadTexture("BLUITalk_I15", L"BLUITalk\\Texture2D\\BLUITalk_I15.dds");
    GET_SINGLE(CResourceManager)->LoadTexture("BLUITalkDecoration_I1", L"BLUITalkDecoration\\Texture2D\\BLUITalkDecoration_I1.dds");

    std::shared_ptr<CComponent> pPanel = CreateComponent<CPanel>("Panel");

    SetRootComponent(pPanel);

    static_cast<CPanel*>(pPanel.get())->SetTexture("BLUITalk_I52");
    static_cast<CPanel*>(pPanel.get())->SetUV(Vector2(0.f, 0.f), Vector2(1921.f, 401.f));
    static_cast<CPanel*>(pPanel.get())->SetSize(Vector2(2048.f, 512.f));
    static_cast<CPanel*>(pPanel.get())->SetWorldScale(1920.f, 400.f, 1.f);
    static_cast<CPanel*>(pPanel.get())->SetPivot(0.5f, 0.5f, 0.f);
    static_cast<CPanel*>(pPanel.get())->SetWorldPos(0.f, -340.f, 0.f);
    static_cast<CPanel*>(pPanel.get())->SetZOrder(7);

    std::shared_ptr<CComponent> pImage = CreateComponent<CUIImage>("Deco");

    static_cast<CSceneComponent*>(pPanel.get())->AddChild(pImage);

    static_cast<CUIImage*>(pImage.get())->SetTexture("BLUITalkDecoration_I1");
    static_cast<CUIImage*>(pImage.get())->SetUV(Vector2(0.f, 0.f), Vector2(1921.f, 201.f));
    static_cast<CUIImage*>(pImage.get())->SetSize(Vector2(2048.f, 256.f));
    static_cast<CUIImage*>(pImage.get())->SetPivot(0.5f, 0.5f, 0.f);
    static_cast<CUIImage*>(pImage.get())->SetWorldScale(1920.f, 200.f, 1.f);
    static_cast<CUIImage*>(pImage.get())->SetWorldPosY(440.f);
    static_cast<CUIImage*>(pImage.get())->SetInheritScale(false);

    m_pName = CreateComponent<CUFont>("Name");

    static_cast<CPanel*>(pPanel.get())->AddChild(m_pName);

    static_cast<CUFont*>(m_pName.get())->SetRect(-500.f, 0.f, 500.f, 0.f);
    static_cast<CUFont*>(m_pName.get())->SetAlignH(TEXT_ALIGN_HORIZONAL::LEFT);
    static_cast<CUFont*>(m_pName.get())->SetColor(0.f, 0.4f, 0.f, 1.f);
    static_cast<CUFont*>(m_pName.get())->SetRelativePosY(-40.f);

    std::shared_ptr<CComponent> pText = CreateComponent<CUFont>("Text");

    static_cast<CPanel*>(pPanel.get())->AddChild(pText);

    static_cast<CUFont*>(pText.get())->SetAlignH(TEXT_ALIGN_HORIZONAL::LEFT);
    static_cast<CUFont*>(pText.get())->SetRect(-500.f, 0.f, 500.f, 0.f);
    static_cast<CUFont*>(pText.get())->SetRelativePosY(-80.f);

    //direct11 fire

    m_pOkButton = CreateComponent<CUIButton>("OKButton");

    static_cast<CSceneComponent*>(pPanel.get())->AddChild(m_pOkButton);

    static_cast<CUIButton*>(m_pOkButton.get())->SetTexture(BUTTON_STATE::OFF, "BLUITalk_I15");
    static_cast<CUIButton*>(m_pOkButton.get())->SetTexture(BUTTON_STATE::ON, "BLUITalk_I15");
    static_cast<CUIButton*>(m_pOkButton.get())->SetTexture(BUTTON_STATE::PRESS, "BLUITalk_I15");
    static_cast<CUIButton*>(m_pOkButton.get())->SetTexture(BUTTON_STATE::DISABLE, "BLUITalk_I15");
    static_cast<CUIButton*>(m_pOkButton.get())->SetUV(Vector2(396.f, 5.f), Vector2(964.f, 39.f));
    static_cast<CUIButton*>(m_pOkButton.get())->SetSize(Vector2(1024.f, 128.f));
    static_cast<CUIButton*>(m_pOkButton.get())->SetPivot(0.5f, 0.5f, 0.f);
    static_cast<CUIButton*>(m_pOkButton.get())->SetButtonCallBack(BUTTON_STATE::PRESS, this, &CDialog::ButtonCallBack);
    static_cast<CUIButton*>(m_pOkButton.get())->SetWorldScale(568.f, 34.f, 1.f);
    static_cast<CUIButton*>(m_pOkButton.get())->SetRelativePos(500.f, 50.f, 0.f);
    static_cast<CUIButton*>(m_pOkButton.get())->SetEnable(false);
    static_cast<CUIButton*>(pImage.get())->SetInheritScale(false);

    std::shared_ptr<CComponent> pOkFont = CreateComponent<CUFont>("OkFont");

    static_cast<CUIButton*>(m_pOkButton.get())->AddChild(pOkFont);

    static_cast<CUFont*>(pOkFont.get())->SetText(L"수락한다.");
    static_cast<CUFont*>(pOkFont.get())->SetAlignH(TEXT_ALIGN_HORIZONAL::LEFT);
    static_cast<CUFont*>(pOkFont.get())->SetRect(-200.f, 0.f, 200.f, 0.f);
    static_cast<CUFont*>(pOkFont.get())->SetShadowColor(0.f, 0.f, 0.f, 1.f);
    static_cast<CUFont*>(pOkFont.get())->SetShadowOffset(Vector3(1.f, -1.f, 0.f));
    static_cast<CUFont*>(pOkFont.get())->SetColor(0.75f, 0.75f, 0.75f, 1.f);

    m_pCancelButton = CreateComponent<CUIButton>("CancelButton");

    static_cast<CSceneComponent*>(pPanel.get())->AddChild(m_pCancelButton);


    static_cast<CUIButton*>(m_pCancelButton.get())->SetTexture(BUTTON_STATE::OFF, "BLUITalk_I15");
    static_cast<CUIButton*>(m_pCancelButton.get())->SetTexture(BUTTON_STATE::ON, "BLUITalk_I15");
    static_cast<CUIButton*>(m_pCancelButton.get())->SetTexture(BUTTON_STATE::PRESS, "BLUITalk_I15");
    static_cast<CUIButton*>(m_pCancelButton.get())->SetTexture(BUTTON_STATE::DISABLE, "BLUITalk_I15");
    static_cast<CUIButton*>(m_pCancelButton.get())->SetUV(Vector2(396.f, 5.f), Vector2(964.f, 39.f));
    static_cast<CUIButton*>(m_pCancelButton.get())->SetSize(Vector2(1024.f, 128.f));
    static_cast<CUIButton*>(m_pCancelButton.get())->SetPivot(0.5f, 0.5f, 0.f);
    static_cast<CUIButton*>(m_pCancelButton.get())->SetButtonCallBack(BUTTON_STATE::PRESS, this, &CDialog::ButtonCallBack);
    static_cast<CUIButton*>(m_pCancelButton.get())->SetWorldScale(568.f, 34.f, 1.f);
    static_cast<CUIButton*>(m_pCancelButton.get())->SetRelativePos(500.f, 11.f, 0.f);
    static_cast<CUIButton*>(m_pCancelButton.get())->SetEnable(false);
    static_cast<CUIButton*>(pImage.get())->SetInheritScale(false);

    std::shared_ptr<CComponent> pDenayFont = CreateComponent<CUFont>("DenayFont");

    static_cast<CUIButton*>(m_pCancelButton.get())->AddChild(pDenayFont);

    static_cast<CUFont*>(pDenayFont.get())->SetText(L"거절한다.");
    static_cast<CUFont*>(pDenayFont.get())->SetAlignH(TEXT_ALIGN_HORIZONAL::LEFT);
    static_cast<CUFont*>(pDenayFont.get())->SetRect(-200.f, 0.f, 200.f, 0.f);
    static_cast<CUFont*>(pDenayFont.get())->SetShadowColor(0.f, 0.f, 0.f, 1.f);
    static_cast<CUFont*>(pDenayFont.get())->SetShadowOffset(Vector3(1.f, -1.f, 0.f));
    static_cast<CUFont*>(pDenayFont.get())->SetColor(0.75f, 0.75f, 0.75f, 1.f);

    SetPage(m_iPage);

    return true;
}

void CDialog::Start()
{
    CGameObject::Start();
}

void CDialog::Update(float fTime)
{
    CGameObject::Update(fTime);

    if (m_iMaxPage)
    {
        if (GET_SINGLE(CInput)->IsUp(MOUSE_TYPE::LEFT))
        {
            m_iPage = (m_iPage + 1) % m_iMaxPage;

            SetPage(m_iPage);
        }
        else if (GET_SINGLE(CInput)->IsUp(MOUSE_TYPE::RIGHT))
        {
            m_iPage = (m_iPage + m_iMaxPage - 1) % m_iMaxPage;
            SetPage(m_iPage);
        }
    }
}

void CDialog::PostUpdate(float fTime)
{
    CGameObject::PostUpdate(fTime);
}

void CDialog::Collision(float fTime)
{
    CGameObject::Collision(fTime);
}

void CDialog::PreRender(float fTime)
{
    CGameObject::PreRender(fTime);
}

void CDialog::Render(float fTime)
{
    CGameObject::Render(fTime);
}

void CDialog::PostRender(float fTime)
{
    CGameObject::PostRender(fTime);
}

std::shared_ptr<CGameObject> CDialog::Clone()
{
    return std::shared_ptr<CGameObject>(new CDialog(*this));
}

void CDialog::Save(FILE* pFile)
{
    CGameObject::Save(pFile);
}

void CDialog::Load(FILE* pFile)
{
    CGameObject::Load(pFile);
}

void CDialog::ButtonCallBack(CUIButton* pButton)
{
    std::string strName = pButton->GetName();

    QUEST_STATE eState = QUEST_STATE::NONE;
    GET_SINGLE(CSoundManager)->Play("Button_Normal");
    if (m_pQuest)
    {
        eState = m_pQuest->GetState();
    }
    if (strName == "OKButton")
    {
        QUEST_STATE ePrevState = eState;
        switch (eState)
        {
        case QUEST_STATE::NONE:
        {
            m_pQuest->SetState(QUEST_STATE::ACCEPTED);

            std::shared_ptr<CGameObject> pMission = m_pScene->FindLayer(UI_LAYER)->FindObj("Mission");

            if (pMission)
            {
                static_cast<CMissionUI*>(pMission.get())->AddQuest(m_pQuest);
            }
        }
        break;
        case QUEST_STATE::ACCEPTED:
            break;
        case QUEST_STATE::QUALIFIED:
        {
            m_pQuest->SetState(QUEST_STATE::FINISHED);

            std::shared_ptr<CGameObject> pMission = m_pScene->FindLayer(UI_LAYER)->FindObj("Mission");

            if (pMission)
            {
                static_cast<CMissionUI*>(pMission.get())->DeleteQuest(m_pQuest);
            }
        }
        break;
        case QUEST_STATE::FINISHED:
        {
        }
            break;
        }
        if (m_pNPC)
        {
            static_cast<CNPC*>(m_pNPC)->NPCDialogEnd(true, ePrevState);
            m_iPage = 0;
        }

    }
    else if (strName == "CancelButton")
    {
        if (m_pNPC)
        {
            static_cast<CNPC*>(m_pNPC)->NPCDialogEnd(false, m_pQuest->GetState());
            m_iPage = 0;
        }
    }
    Close();

    SetPage(m_iPage);
}

void CDialog::SetPage(int iPage)
{
    m_iPage = iPage;

    std::shared_ptr<CComponent> pCom = FindComponent("Text");

    if (m_pQuest)
    {
        static_cast<CUFont*>(pCom.get())->SetText(m_pQuest->GetTalk(m_pQuest->GetState()).c_str());
    }

    if (m_iPage == m_iMaxPage - 1)
    {
        m_pOkButton->SetEnable(true);
        m_pCancelButton->SetEnable(true);
    }
    else
    {
        m_pOkButton->SetEnable(false);
        m_pCancelButton->SetEnable(false);
    }
}
