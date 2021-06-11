#pragma once
#include "Object\GameObject.h"
#include "../EditorFlags.h"
class CPortal :
    public CGameObject
{
public:
    CPortal();
    CPortal(const CPortal& obj);
    virtual ~CPortal();

private:
    EDITOR_GAME_MODE_TYPE   m_eGameModeType;

public:
    void SetGameModeType(EDITOR_GAME_MODE_TYPE eType)
    {
        m_eGameModeType = eType;
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
    void CollisionInit(class CCollider* pSrc, class CCollider* pDset, float fTime);
    void CollisionStay(class CCollider* pSrc, class CCollider* pDset, float fTime);
    void CollisionEnd(class CCollider* pSrc, class CCollider* pDset, float fTime);
};

