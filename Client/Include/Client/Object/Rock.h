#pragma once
#include "Object\GameObject.h"
class CRock :
	public CGameObject
{
public:
	CRock();
	CRock(const CRock& rock);
	virtual ~CRock() override;

public:
	virtual bool Init() override;
	virtual void Start() override;
	virtual void Update(float fTime) override;
	virtual void PostUpdate(float fTime) override;
	virtual void Collision(float fTime) override;
	virtual void PreRender(float fTime) override;
	virtual void Render(float fTime) override;
	virtual void PostRender(float fTime) override;
	virtual std::shared_ptr<CGameObject> Clone() override;

public:
	virtual void Save(FILE* pFile) override;
	virtual void Load(FILE* pFile) override;
};
