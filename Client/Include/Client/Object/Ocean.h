#pragma once
#include "Object\GameObject.h"

typedef struct _tagVertexOcean
{
    Vector3 vPos;
    Vector2 vUV;
    Vector3 vNormal;
    Vector3 vTangent;
    Vector3 vBinormal;
}VERTEXOCEAN, *PVERTEXOCEAN;

class COcean :
    public CGameObject
{
public:
    COcean();
    COcean(const COcean& obj);
    virtual ~COcean();

private:
    std::vector<Vector3>    m_vecPos;
    std::shared_ptr<CComponent> m_pRenderer;

public:
    void BuildWater(const std::string& strKey, int iWidth, int iHeight);

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
};

