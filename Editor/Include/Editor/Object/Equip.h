#pragma once
#include "Object\GameObject.h"
#include "../EditorFlags.h"

class CEquip :
    public CGameObject
{
public:
    CEquip();
    CEquip(const CEquip& ui);
    virtual ~CEquip() override;

private:
    std::shared_ptr<class CComponent>   m_pPanel;
    std::shared_ptr<class CComponent>   m_pEquipIcons[static_cast<int>(EQUIP_TYPE::END)];
    class CItem*                        m_ppItems[static_cast<int>(EQUIP_TYPE::END)];
    std::shared_ptr<class CComponent>   m_pEquipSlots[static_cast<int>(EQUIP_TYPE::END)];
    int                                 m_iSelect;
    std::shared_ptr<class CGameObject>  m_pPlayer;

public:
    int GetSelect() const
    {
        return m_iSelect;
    }
    void SetSelect(int iSelect)
    {
        m_iSelect = iSelect;
    }
    const std::shared_ptr<class CComponent> GetItemIcon(int iType)  const
    {
        if (iType >= static_cast<int>(EQUIP_TYPE::END))
            return nullptr;

        return m_pEquipIcons[iType];
    }
    class CItem* GetItemIndex(int iType)
    {
        return m_ppItems[iType];
    }
    void DeleteIcon(int iType);
    void SetItem(int iType);

public:
    virtual bool Init() override;
    virtual void Start() override;
    virtual void Update(float fTime) override;
    virtual void PostUpdate(float fTime) override;
    virtual void Collision(float fTime) override;
    virtual void PreRender(float fTime) override;
    virtual void Render(float fTime) override;
    virtual void PostRender(float fTime) override;
    virtual std::shared_ptr<CGameObject> Clone();

public:
    virtual void Save(FILE* pFile) override;
    virtual void Load(FILE* pFile) override;

public:
    void ColInit(class CCollider* pSrc, class CCollider* pDest, float fTime);
    void ColStay(class CCollider* pSrc, class CCollider* pDest, float fTime);
    void ColEnd(class CCollider* pSrc, class CCollider* pDest, float fTime);
};

