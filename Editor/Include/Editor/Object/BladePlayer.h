#pragma once
#include "Player3D.h"
class CBladePlayer :
    public CPlayer3D
{
public:
    CBladePlayer();
    CBladePlayer(const CBladePlayer& obj);
    virtual ~CBladePlayer();

private:
    std::shared_ptr<CComponent>   m_pLance;
    std::shared_ptr<CComponent>   m_pTop;
    std::shared_ptr<CComponent>   m_pBottom;
    std::vector<_tagVertexTrail>        m_vecVtx;
    float       m_fTrailTime;
    float       m_fTrailTimeLimit;
    int       m_iTrailCount;
    std::shared_ptr<CGameObject>  m_pTrail;
    float       m_fTrailStartAlpha;
    float       m_fTrailEndAlpha;
    int         m_iCombo;
    std::shared_ptr<CComponent>   m_pBladeBody;
    std::shared_ptr<CComponent>     m_pParticle;
    float       m_fComboTime;
    float       m_fComboTimeMax;
    float       m_fCharge;
    std::shared_ptr<CComponent>     m_pAlphaWeapon;
    std::shared_ptr<CComponent>     m_pChargeParticle;
    std::shared_ptr<CComponent>    m_pFaceOutLine;
    std::shared_ptr<CComponent>    m_pTailOutLine;
    std::shared_ptr<CComponent>    m_pHairOutLine;
    std::shared_ptr<CGameObject>    m_pHeadObj;
    std::shared_ptr<CGameObject>    m_pTailObj;

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
    void Trail(float,float fTime);
    void TrailEnd(float fTime);
    void TrailStart(float fTime);
    void ComboAdd(float fTime);
    void ComboEnd(float fTime);
    void SetTrailTimeLimit(float fLimit)
    {
        m_fTrailTimeLimit = fLimit;
    }
public:
    virtual void ShowWindow();

public:
    void CreateTrailMesh(int iCount);
    virtual void SetTransition();

public:
    void ColInit(class CCollider* pSrc, class CCollider* pDest, float fTime);
    void ColStay(class CCollider* pSrc, class CCollider* pDest, float fTime);
    void ColLast(class CCollider* pSrc, class CCollider* pDest, float fTime);

public:
    virtual void AttackEnd();
    void Skill1StartSound();
    void Skill1EndSound();

public:
    virtual void Skill1(float fTime);
    virtual void Skill1Down(float fTime);

public:
    virtual void SetOutLine(bool bLine);
public:
    virtual void SetLayer(class CLayer* pLayer) override;
    virtual void SetScene(class CScene* pScene) override;
    
};

