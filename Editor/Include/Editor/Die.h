#pragma once
#include "State.h"
class CDie :
    public CState
{
public:
	CDie();
	CDie(class CGameObject* pObj);
	~CDie();
public:
	virtual void Move(class CGameObject* pObj, float fTime);
	virtual void Attack(class CGameObject* pObj, float fTime);
	virtual void AttackAround(class CGameObject* pObj, float fTime);
	virtual void HeavyAttack(class CGameObject* pObj, float fTime);
	virtual void Attacked(class CGameObject* pObj, float fTime);
	virtual void Idle(class CGameObject* pObj, float fTime);
	virtual void GiveItem(class CGameObject* pObj, float fTime);
	virtual void LongMove(class CGameObject* pObj, float fTime);
	virtual void Die(class CGameObject* pObj, float fTime);

public:
	virtual CDie* Clone();
};

