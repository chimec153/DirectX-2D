#pragma once
#include "State.h"
class CRage :
    public CState
{
public:
	CRage();
	CRage(class CGameObject* pObj);
	virtual ~CRage();

public:
	virtual void Move(class CGameObject* pObj, float fTime);
	virtual void Attack(class CGameObject* pObj, float fTime);
	virtual void AttackAround(class CGameObject* pObj, float fTime);
	virtual void HeavyAttack(class CGameObject* pObj, float fTime);
	virtual void Attacked(class CGameObject* pObj, float fTime);
	virtual void GiveItem(class CGameObject* pObj, float fTime);
	virtual void LongAtk(class CGameObject* pObj, float fTime);
	virtual void LongMove(class CGameObject* pObj, float fTime);
	virtual void Die(class CGameObject* pObj, float fTime);

public:
	virtual CRage* Clone();
};

