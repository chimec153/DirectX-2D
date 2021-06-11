#pragma once
#include "Object\GameObject.h"
class CDungeonUI :
    public CGameObject
{
public:
    CDungeonUI();
    CDungeonUI(const CDungeonUI& ui);
    virtual ~CDungeonUI();

private:
    std::shared_ptr<CComponent> m_pSubWindow;

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

