#pragma once
#include "UIControl.h"
class CUIList :
	public CUIControl
{
	friend class CGameObject;

public:
	CUIList();
	CUIList(const CUIList& ui);
	virtual ~CUIList();

private:
	std::list<const TCHAR*>		m_strList;
	std::list<std::shared_ptr<CComponent>>	m_FontList;
	std::shared_ptr<CComponent>				m_pSlider;
	std::shared_ptr<CComponent>				m_pPanel;
	Vector2									m_tExtent;
	std::shared_ptr<CComponent>				m_pRC;
	int										m_iSel;
	std::shared_ptr<CComponent>				m_pSel;

public:
	void AddString(const std::string& str);
	void SetExtent(float x, float y);

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
	void ColEnter(class CCollider* pSrc, class CCollider* pDst, float fTime);
	void ColStay(class CCollider* pSrc, class CCollider* pDst, float fTime);
	void ColEnd(class CCollider* pSrc, class CCollider* pDst, float fTime);

};

