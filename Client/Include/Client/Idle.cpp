#include "Idle.h"
#include "Object/Monster3D.h"
#include "Fight.h"
#include "Walk.h"
#include "Resource/Animation.h"

CIdle::CIdle()
{
}

CIdle::CIdle(CGameObject* pObj) :
	CState(pObj)
{
}

CIdle::~CIdle()
{
}

void CIdle::Move(CGameObject* pObj, float fTime)
{
	ChangeState(pObj, new CWalk);
}

void CIdle::Attack(CGameObject* pObj, float fTime)
{
	ChangeState(pObj, new CFight);
}

void CIdle::AttackAround(CGameObject* pObj, float fTime)
{
	ChangeState(pObj, new CFight);
}

void CIdle::HeavyAttack(CGameObject* pObj, float fTime)
{
	ChangeState(pObj, new CFight);
}

void CIdle::Attacked(CGameObject* pObj, float fTime)
{
	ChangeState(pObj, new CFight);
}

void CIdle::Idle(CGameObject* pObj, float fTime)
{
	static_cast<CMonster3D*>(pObj)->ChangeSequenceAll("MonsterWait");
}

void CIdle::GiveItem(CGameObject* pObj, float fTime)
{
}

void CIdle::LongMove(CGameObject* pObj, float fTime)
{
	CState::LongMove(pObj, fTime);
}

void CIdle::Die(CGameObject* pObj, float fTime)
{
	CState::Die(pObj, fTime);
}

CIdle* CIdle::Clone()
{
	return new CIdle(*this);
}
