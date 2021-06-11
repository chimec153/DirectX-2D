#pragma once
#include "Editor.h"
class CState
{
public:
	CState();
	CState(class CGameObject* pObj);
	virtual ~CState();

private:
	class CGameObject* m_pObj;

public:
	void SetObj(class CGameObject* pObj)
	{
		m_pObj = pObj;
	}
	class CGameObject* GetObj()	const
	{
		return m_pObj;
	}

public:
	virtual void Move(class CGameObject* pObj,float fTime);
	virtual void Attack(class CGameObject* pObj,float fTime);
	virtual void AttackAround(class CGameObject* pObj,float fTime);
	virtual void HeavyAttack(class CGameObject* pObj,float fTime);
	virtual void Attacked(class CGameObject* pObj, float fTime);
	virtual void Idle(class CGameObject* pObj, float fTime);
	virtual void GiveItem(class CGameObject* pObj, float fTime);
	virtual void LongAtk(class CGameObject* pObj, float fTime);
	virtual void LongMove(class CGameObject* pObj, float fTime);
	virtual void Die(class CGameObject* pObj, float fTime);

protected:
	void ChangeState(class CGameObject* pObj, CState* pState);

public:
	virtual CState* Clone() = 0;
};

// °³³ä == Å¬·¡½º