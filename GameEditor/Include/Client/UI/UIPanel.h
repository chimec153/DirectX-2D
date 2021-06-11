#pragma once
#include "Object\GameObject.h"
class CUIPanel :
	public CGameObject
{
	friend class CScene;
	friend class CViewPort;

public:
	CUIPanel();
	CUIPanel(const CUIPanel& ui);
	virtual ~CUIPanel();

private:
	std::shared_ptr<CComponent> m_pCurPanel;
	
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

public:
	void Btn1(float fTime);
	void Btn2(float fTime);
	void Btn3(float fTime);
	void Close(float fTime);
};

