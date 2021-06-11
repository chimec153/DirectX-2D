#pragma once
#include "NPC.h"
#include "../EditorFlags.h"
class CIseulenia :
    public CNPC
{
public:
    CIseulenia();
    CIseulenia(const CIseulenia& npc);
    virtual ~CIseulenia();

private:
    class std::shared_ptr<CGameObject>  m_pDungeonUI;
    class std::shared_ptr<class CComponent>      m_pCam;
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
    virtual void NPCWork()  override;

public:
    void Teleport(DUNGEON_TYPE eType);
};

