#pragma once
#include "Object\GameObject.h"
class CMissionUI :
    public CGameObject
{
public:
    CMissionUI();
    CMissionUI(const CMissionUI& ui);
    virtual ~CMissionUI();

private:
    std::shared_ptr<CComponent> m_pSubWindow;
    int         m_iSelect;
    std::list<class CQuest*>    m_QuestList;
    int         m_iQuests;
    std::vector<std::shared_ptr<CComponent>> m_vecButtons;

public:
    void AddQuest(class CQuest* pQuest);
    class CQuest* FindQuest(int iIndex);
    void DeleteQuest(class CQuest* pQuest);

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
};

