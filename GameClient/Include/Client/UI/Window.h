#pragma once
#include "Object\GameObject.h"
class CWindow :
	public CGameObject
{
	friend class CScene;
	friend class CLayer;

public:
	CWindow();
	CWindow(const CWindow& ui);
	virtual ~CWindow();

protected:
	std::shared_ptr<class CComponent>	m_pMainPanel;
	std::shared_ptr<class CComponent>	m_pCloseButton;
	std::shared_ptr<class CComponent>	m_pTitleBar;

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
	void CloseWindow(float fTime);
	void OpenWindow(float fTime);
};

