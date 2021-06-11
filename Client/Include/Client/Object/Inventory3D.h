#pragma once
#include "Object\GameObject.h"
#include "../EditorFlags.h"
#define MAX_INVEN   80
#define INVEN_ROWS  10
#define INVEN_COLS  8

class CInventory3D :
    public CGameObject
{
public:
    CInventory3D();
    CInventory3D(const CInventory3D& inven);
    virtual ~CInventory3D() override;

private:
    class CItem* m_pItems[MAX_INVEN];
    std::shared_ptr<class CComponent>   m_pIcons[MAX_INVEN];
    int m_iSelect;
    std::shared_ptr<class CComponent>   m_pPanel;
    std::shared_ptr<class CComponent>   m_pMoneyFont;

public:
    class CItem* GetItemIndex(int iIndex) const
    {
        if (iIndex >= MAX_INVEN)
            return 0;

        return m_pItems[iIndex];
    }
    const std::shared_ptr<class CComponent> GetItemIcon(int iIdx)  const
    {
        if (iIdx >= MAX_INVEN)
            return nullptr;

        return m_pIcons[iIdx];
    }

    int GetSelect() const
    {
        return m_iSelect;
    }
    void DeleteItem(const std::string& strKey, int iCount = 1);
    void DeleteItem(int iIndex);
    bool AddItem(class CItem* pItem);
    bool AddItem(class CItem* pItem, const std::shared_ptr<CComponent>& pIcon);
    void SetSelect(int iSelect)
    {
        m_iSelect = iSelect;
    }
    bool HasItem(const std::string& strKey)  const;
    int FindItem(const std::string& strKey) const;

public:
    virtual bool Init() override;
    virtual void Start() override;
    virtual void Update(float fTime) override;
    virtual void PostUpdate(float fTime) override;
    virtual void Collision(float fTime) override;
    virtual void PreRender(float fTime) override;
    virtual void Render(float fTime) override;
    virtual void PostRender(float fTime) override;
    virtual std::shared_ptr<class CGameObject> Clone();

public:
    virtual void Save(FILE* pFile) override;
    virtual void Load(FILE* pFile) override;

private:
    int GetSlotIndex(const Vector2& vPos);
    const Vector2 GetSlotPos(int iIdx);
};

