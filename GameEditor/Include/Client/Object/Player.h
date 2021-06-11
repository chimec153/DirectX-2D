#pragma once

#include "Object/GameObject.h"


class CPlayer :
	public CGameObject
{
public:
	CPlayer();
	CPlayer(const CPlayer& player);
	virtual ~CPlayer();

private:
	std::shared_ptr<CComponent>	m_pMesh;
	std::shared_ptr<CComponent>	m_pChildMesh;
	std::shared_ptr<CComponent>	m_pChildChildMesh;
	std::shared_ptr<CComponent>	m_pCam;
	float						m_fSpeed;
	bool						m_bPress;
	bool						m_bIdleEnable;
	std::shared_ptr<CComponent>	m_pBar;
	std::shared_ptr<CComponent>	m_pNav;
	std::stack<class CTile*>	m_stack;
	Vector2						m_vEndPos;
	std::shared_ptr<CComponent>	m_pPaper;

public:
	virtual bool Init();
	virtual void Start();
	virtual void Input(float fTime);
	virtual void Update(float fTime);
	virtual void PostUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void PreRender(float fTime);
	virtual void Render(float fTime);
	virtual void PostRender(float fTime);
	virtual std::shared_ptr<CGameObject> Clone();

private:
	void MoveSide(float fScale, float fTime);
	void MoveVertical(float fScale, float fTime);
	void Rotate(float fScale, float fTime);
	void FireCon(float fScale, float fTime);
	void Fire(float fTime);
	void Fire2(float fTime);
	void Fire3(float fTime);
	void Down(float fTime);
	void Up(float fTime);
	void FireEnd();
	virtual void CallNotify(const std::string& strTag);
	void Color(float fTime);
	void Paper(float fTime);
public:
	void Move(const Vector2& vPos);

public:
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);

public:
	void ColInit(class CCollider* pSrc, class CCollider* pDest, float fTime);
	void ColCon(class CCollider* pSrc, class CCollider* pDest, float fTime);
	void ColEnd(class CCollider* pSrc, class CCollider* pDest, float fTime);
};

