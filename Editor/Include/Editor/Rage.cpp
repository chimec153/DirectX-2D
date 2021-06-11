#include "Rage.h"
#include "Object/Monster3D.h"
#include "Resource/Animation.h"
#include "Die.h"
#include "Component/Renderer.h"
#include "Resource/Material.h"

CRage::CRage()
{
}

CRage::CRage(CGameObject* pObj)	:
	CState(pObj)
{
	if (pObj)
	{
		static_cast<CMonster3D*>(pObj)->ChangeSequenceAll("modeAlarm_Start");
	}
}

CRage::~CRage()
{
}

void CRage::Move(CGameObject* pObj, float fTime)
{
	CState::Move(pObj, fTime);
}

void CRage::Attack(CGameObject* pObj, float fTime)
{
	static_cast<CMonster3D*>(pObj)->GetAni()->ChangeSequence("MonsterAttack");
}

void CRage::AttackAround(CGameObject* pObj, float fTime)
{
	static_cast<CMonster3D*>(pObj)->GetAni()->ChangeSequence("MonsterAttackAround");
}

void CRage::HeavyAttack(CGameObject* pObj, float fTime)
{
	static_cast<CMonster3D*>(pObj)->GetAni()->ChangeSequence("MonsterHeavyAttack");
}

void CRage::Attacked(CGameObject* pObj, float fTime)
{
	int iHP = static_cast<CMonster3D*>(pObj)->GetHP();
	int iHPMax = static_cast<CMonster3D*>(pObj)->GetHPMax();
	if (iHP <= 0)
	{
		ChangeState(pObj, new CDie);
		static_cast<CMonster3D*>(pObj)->GetAni()->ChangeSequence("MonsterDeath");
	}
}

void CRage::GiveItem(CGameObject* pObj, float fTime)
{
}

void CRage::LongAtk(CGameObject* pObj, float fTime)
{
	static_cast<CMonster3D*>(pObj)->GetAni()->ChangeSequence("longAtk01_Start");
}

void CRage::LongMove(CGameObject* pObj, float fTime)
{
	CState::LongMove(pObj, fTime);
}

void CRage::Die(CGameObject* pObj, float fTime)
{
	CState::Die(pObj, fTime);
}

CRage* CRage::Clone()
{
	return new CRage(*this);
}
