#pragma once
#include "Object\GameObject.h"
class CCheckBox :
	public CGameObject
{
	friend class CScene;
	friend class CViewPort;

public:
	CCheckBox();
	CCheckBox(const CCheckBox& ui);
	virtual ~CCheckBox();

private:
	std::shared_ptr<CComponent>	m_pBox;
	std::shared_ptr<CComponent>	m_pRC;
	std::shared_ptr<CComponent>	m_pText;

public:
	void SetText(const TCHAR *pText);

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

