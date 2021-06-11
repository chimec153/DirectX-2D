#pragma once
#include "Object\GameObject.h"
class CBr :
	public CGameObject
{
	friend class CScene;
	friend class CViewPort;

public:
	CBr();
	CBr(const CBr& obj);
	virtual ~CBr();

private:
	std::shared_ptr<CComponent> m_pImg;
	std::shared_ptr<CComponent>	m_pBr;
	int		m_iProgress;

public:
	void SetProgress(int iProgress);
	void AddProgress();

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

