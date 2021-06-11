#pragma once
#include "UIControl.h"
class CTab :
	public CUIControl
{
	friend class CGameObject;

public:
	CTab();
	CTab(const CTab& ui);
	virtual ~CTab();

private:
	std::vector<std::shared_ptr<CComponent>>		m_vecPanel;
	std::vector<std::shared_ptr<CComponent>>		m_vecButton;

public:
	std::shared_ptr<CComponent> GetPanel(int idx = 0);
	std::shared_ptr<CComponent> GetButton(int idx = 0);
	void AddTab();
	size_t GetSize()	const;
	void SetButtonSize(float x, float y);
	void SetTab();

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

public:
	void Tab(class CChkBx* pBtn);
	void UnTab(class CChkBx* pBtn);

};

