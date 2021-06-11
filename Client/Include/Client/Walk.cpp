#include "Walk.h"
#include "Idle.h"
#include "Fight.h"
#include "Scene/Scene.h"
#include "Object/Monster3D.h"

CWalk::CWalk()
{
}

CWalk::CWalk(CGameObject* pObj) :
	CState(pObj)
{
}

CWalk::~CWalk()
{
}
void CWalk::Move(CGameObject* pObj, float fTime)
{
	CState::Move(pObj, fTime);
}

void CWalk::Attack(CGameObject* pObj, float fTime)
{
	ChangeState(pObj, new CFight);
}

void CWalk::AttackAround(CGameObject* pObj, float fTime)
{
}

void CWalk::HeavyAttack(CGameObject* pObj, float fTime)
{
}

void CWalk::Attacked(CGameObject* pObj, float fTime)
{
	ChangeState(pObj, new CFight);
}

void CWalk::Idle(CGameObject* pObj, float fTime)
{
	static_cast<CMonster3D*>(pObj)->ChangeSequenceAll("MonsterWait");
}

void CWalk::GiveItem(CGameObject* pObj, float fTime)
{
}

void CWalk::LongMove(CGameObject* pObj, float fTime)
{
	CState::LongMove(pObj, fTime);
}

void CWalk::Die(CGameObject* pObj, float fTime)
{
	CState::Die(pObj, fTime);
}

CWalk* CWalk::Clone()
{
	return new CWalk(*this);
}
