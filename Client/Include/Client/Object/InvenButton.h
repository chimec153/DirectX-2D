#pragma once
#include "Object\GameObject.h"
class CInvenButton :
    public CGameObject
{
public:
    CInvenButton();
    CInvenButton(const CInvenButton& ui);
    virtual ~CInvenButton();

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
    void ButtonPress(float fTime);
};

