#pragma once
#include "Object\GameObject.h"
#include "../EditorFlags.h"
class CNPC :
    public CGameObject
{
public:
    CNPC();
    CNPC(const CNPC& npc);
    virtual ~CNPC();

protected:
    class CAnimation* m_pAni;
    class std::shared_ptr<CComponent>   m_pCam;
    class std::shared_ptr<CComponent>   m_pQuest;
    std::shared_ptr<CGameObject>    m_pDialog;
    class std::shared_ptr<CComponent>   m_pName;

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
    virtual void Save(FILE* pFile);
    virtual void Load(FILE* pFile);

public:
    void CollisionInit(class CCollider* pSrc, class CCollider* pDest, float fTime);
    void CollisionStay(class CCollider* pSrc, class CCollider* pDest, float fTime);
    void CollisionEnd(class CCollider* pSrc, class CCollider* pDest, float fTime);

public:
    virtual void NPCWork();
    virtual void NPCDialogEnd(bool bOk, QUEST_STATE eState);


};

