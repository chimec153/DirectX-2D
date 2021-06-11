#pragma once

#include "UIControl.h"

class CUIImage :
	public CUIControl
{
	friend class CUIObject;

private:
	CUIImage();
	CUIImage(const CUIImage& ui);
	virtual ~CUIImage();

public:
	virtual bool Init();
	virtual void Start();
	virtual void Update(float fTime);
	virtual void PostUpdate(float fTime);
	virtual void PreRender();
	virtual void Render();
	virtual void PostRender();
	virtual CUIImage* Clone();

public:
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);
};

