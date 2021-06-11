#pragma once
#include "Window.h"
class CRideWindow :
	public CWindow
{
	friend class CScene;
	friend class CLayer;

public:
	CRideWindow();
	CRideWindow(const CRideWindow& ui);
	virtual ~CRideWindow();

private:
	std::shared_ptr<class CComponent>		m_pTab;

public:
	virtual bool Init();
	virtual void Start();
	virtual void Update(float fTime);
	virtual void PostUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void PreRender(float fTime);
	virtual void Render(float fTime);
	virtual void PostRender(float fTime);
	virtual std::shared_ptr<class CGameObject> Clone();

public:
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);

public:
	void Play(class CChkBx* pBtn);
	class CUIButton* AddButton(int idx);
};

