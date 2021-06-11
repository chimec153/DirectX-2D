#pragma once
#include "Object\GameObject.h"
class CDialog :
    public CGameObject
{
public:
    CDialog();
    CDialog(const CDialog& obj);
    virtual ~CDialog();
    // 소프트 파티클

private:
    std::vector<const TCHAR*>    m_vecText;
    std::shared_ptr<CComponent> m_pOkButton;
    std::shared_ptr<CComponent> m_pCancelButton;
    int                         m_iPage;
    int                         m_iMaxPage;
    class CGameObject* m_pNPC;
    std::shared_ptr<CComponent> m_pName;
    class CQuest*           m_pQuest;

public:
    void AddPage(const TCHAR* pText);
    void ClearPage();
    void SetNPC(class CGameObject* pNPC)
    {
        m_pNPC = pNPC;
    }
    void SetNameText(const TCHAR* pName);
    void SetQuest(const std::string& strKey);
    void Open();
    void Close();
    class CQuest* GetQuest()    const
    {
        return m_pQuest;
    }

public:
    virtual bool Init();
    virtual void Start();
    virtual void Update(float fTime);
    virtual void PostUpdate(float fTime);
    virtual void Collision(float fTime);
    virtual void PreRender(float fTime);
    virtual void Render(float fTime);
    virtual void PostRender(float fTime);
    virtual std::shared_ptr<CGameObject> Clone();

public:
    virtual void Save(FILE* pFile);
    virtual void Load(FILE* pFile);

public:
    void ButtonCallBack(class CUIButton* pButton);

private:
    void SetPage(int iPage);

};

