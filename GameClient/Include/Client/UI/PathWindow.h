#pragma once
#include "Window.h"
class CPathWindow :
	public CWindow
{
	friend class CScene;

public:
	CPathWindow();
	CPathWindow(const CPathWindow& ui);
	virtual ~CPathWindow();

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
};

