#pragma once
#include "UIControl.h"
class CTitleBar :
	public CUIControl
{
	friend class CGameObject;

public:
	CTitleBar();
	CTitleBar(const CTitleBar& com);
	virtual ~CTitleBar();

private:
	bool						m_bPush;
	Vector3						m_vStart;
	std::shared_ptr<CComponent>	m_pRC;
	std::shared_ptr<CComponent>	m_pTitle;

public:
	void SetTitle(const TCHAR* pText);
	void SetColor(float r, float g, float b, float a);
	void SetTextPivot(float x, float y, float z);
	void SetTextSize(float fSize);
	const std::shared_ptr<CComponent>& GetFont()	const;

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

