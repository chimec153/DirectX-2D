#pragma once
#include "UIControl.h"
class CUISprite :
	public CUIControl
{
	friend class CGameObject;

public:
	CUISprite();
	CUISprite(const CUISprite& ui);
	virtual ~CUISprite();

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

