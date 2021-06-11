#include "Die.h"
#include "Object/Monster3D.h"
#include "Resource/Animation.h"

CDie::CDie()
{
}

CDie::CDie(CGameObject* pObj) :
	CState(pObj)
{
}

CDie::~CDie()
{
}

void CDie::Move(CGameObject* pObj, float fTime)
{
}

void CDie::Attack(CGameObject* pObj, float fTime)
{
}

void CDie::AttackAround(CGameObject* pObj, float fTime)
{
}

void CDie::HeavyAttack(CGameObject* pObj, float fTime)
{
}

void CDie::Attacked(CGameObject* pObj, float fTime)
{
	//static_cast<CMonster3D*>(pObj)->GetAni()->ChangeSequence("MonsterDeath");

}

void CDie::Idle(CGameObject* pObj, float fTime)
{
}

void CDie::GiveItem(CGameObject* pObj, float fTime)
{
}

void CDie::LongMove(CGameObject* pObj, float fTime)
{
}

void CDie::Die(CGameObject* pObj, float fTime)
{
}

CDie* CDie::Clone()
{
	return new CDie(*this);
}
