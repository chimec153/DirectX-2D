#pragma once
#include "NPC.h"
class CTati :
    public CNPC
{
public:
    CTati();
    CTati(const CTati& npc);
    virtual ~CTati();

private:
    class std::shared_ptr<class CComponent>     m_pCam;
    class CCamera* m_pPrevCam;
    

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
    virtual void NPCWork();
    virtual void Close();

public:
    virtual void NPCDialogEnd(bool bOk, QUEST_STATE eState);
};

