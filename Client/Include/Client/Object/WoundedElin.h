#pragma once
#include "NPC.h"
class CWoundedElin :
    public CNPC
{
public:
    CWoundedElin();
    CWoundedElin(const CWoundedElin& npc);
    virtual ~CWoundedElin();

private:
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
    void Close();
    virtual void NPCDialogEnd(bool bOk, QUEST_STATE eState) override;
};

