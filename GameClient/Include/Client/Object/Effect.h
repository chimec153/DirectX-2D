#pragma once
#include "Object\GameObject.h"
class CEffect :
	public CGameObject
{
	friend class CScene;
	friend class CLayer;

public:
	CEffect();
	CEffect(const CEffect& eft);
	virtual ~CEffect();

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
	virtual void CallNotify(const std::string& strTag);

public:
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);
};

