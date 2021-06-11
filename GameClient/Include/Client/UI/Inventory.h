#pragma once
#include "Object\GameObject.h"
class CInventory :
	public CGameObject
{
	friend class CScene;
	friend class CUIViewPort;

public:
	CInventory();
	CInventory(const CInventory& obj);
	virtual ~CInventory();

private:
	class CTitleBar*	m_pTitle;
	class CUIButton*	m_pCloseBtn;
	class CPanel*		m_pMainPnl;
	char*				m_pBlock;

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
	void Close(float fTime);
	void ColEnter(class CCollider* pSrc, class CCollider* pDst, float fTime);
	void ColStay(class CCollider* pSrc, class CCollider* pDst, float fTime);
	void ColEnd(class CCollider* pSrc, class CCollider* pDst, float fTime);
};

