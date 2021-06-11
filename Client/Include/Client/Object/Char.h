#pragma once
#include "Object\GameObject.h"
class CChar :
    public CGameObject
{
public:
    CChar();
    CChar(const CChar& obj);
    virtual ~CChar();

private:
    std::shared_ptr<CComponent> m_pFont;

public:
    void SetText(const TCHAR* pText);

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

private:
    static std::unordered_map<class CChar*, size_t>   m_mapChat;
};

