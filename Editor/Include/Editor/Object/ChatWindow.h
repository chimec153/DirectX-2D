#pragma once
#include "Object\GameObject.h"
class CChatWindow :
    public CGameObject
{
    friend class CScene;

private:
    CChatWindow();
    CChatWindow(const CChatWindow& ui);
    virtual ~CChatWindow();

public:
    virtual bool Init();
    virtual void Start();
    virtual void Update(float fTime);
    virtual void PostUpdate(float fTime);
    virtual void Collision(float fTime);
    virtual void PreRender(float fTime);
    virtual void Render(float fTime);
    virtual void PostRender(float fTime);
    virtual std::shared_ptr<class CGameObject> Clone();

public:
    virtual void Save(FILE* pFile);
    virtual void Load(FILE* pFile);
};

