#pragma once
#include "Object\GameObject.h"
#include "../EditorFlags.h"
class CLootingWindow :
    public CGameObject
{
public:
    CLootingWindow();
    CLootingWindow(const CLootingWindow& ui);
    virtual ~CLootingWindow();

private:
    std::shared_ptr<class CGameObject>  m_pInven;
    class CItem*                        m_pLootItem[MAX_LOOTING];
    int                                 m_iCount;
    std::shared_ptr<class CComponent>   m_pSlot[MAX_LOOTING];
    std::function<void()>               m_LootingCallBack;

public:
    void CreateItem(class CItem* pItem);
    void Looting(int iSlotIndex);

    template <typename T>
    void SetLootingCallBack(T* pObj, void(T::*pFunc)())
    {
        m_LootingCallBack = std::bind(pFunc, pObj);
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
    void ButtonPress(class CUIButton* pButton);
};

