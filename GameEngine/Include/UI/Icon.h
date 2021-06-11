#pragma once
#include "UIControl.h"
class CIcon :
	public CUIControl
{
	friend class CGameObject;

public:
	CIcon();
	CIcon(const CIcon& ui);
	virtual ~CIcon();

private:
	std::shared_ptr<CComponent>	m_pRC;
	bool					m_bPush;
	Vector3					m_vPos;

public:
	bool IsPush()	const;
	void SetPos(const Vector3& vPos);

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

public:
	void Cancle();
};

