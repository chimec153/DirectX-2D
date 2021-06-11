#pragma once
#include "ObjectComponent.h"
class CNavigation :
	public CObjectComponent
{
	friend class CGameObject;

protected:
	CNavigation();
	CNavigation(const CNavigation& nav);
	virtual ~CNavigation();

public:
	virtual bool FindPath(const Vector3& vStart, const Vector3& vEnd) = 0;

public:
	virtual bool Init();
	virtual void Start();
	virtual void Update(float fTime);
	virtual void PostUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void PreRender(float fTime);
	virtual void Render(float fTime);
	virtual void PostRender(float fTime);
	virtual std::shared_ptr<CComponent> Clone();

public:
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);

};

