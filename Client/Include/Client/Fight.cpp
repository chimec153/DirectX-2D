#include "Fight.h"
#include "Object/Monster3D.h"
#include "Resource/Animation.h"
#include "Rage.h"
#include "Die.h"

CFight::CFight()
{
}

CFight::CFight(CGameObject* pObj)	:
	CState(pObj)
{
}

CFight::~CFight()
{
}

void CFight::Move(CGameObject* pObj, float fTime)
{
	CState::Move(pObj, fTime);
}

void CFight::Attack(CGameObject* pObj, float fTime)
{
	static_cast<CMonster3D*>(pObj)->ChangeSequenceAll("MonsterAttack");
}

void CFight::AttackAround(CGameObject* pObj, float fTime)
{
	static_cast<CMonster3D*>(pObj)->ChangeSequenceAll("MonsterAttackAround");
}

void CFight::HeavyAttack(CGameObject* pObj, float fTime)
{
	static_cast<CMonster3D*>(pObj)->ChangeSequenceAll("MonsterHeavyAttack");
}

void CFight::Attacked(CGameObject* pObj, float fTime)
{
	int iHP = static_cast<CMonster3D*>(pObj)->GetHP();
	int iHPMax = static_cast<CMonster3D*>(pObj)->GetHPMax();
	

	if (iHP <= iHPMax * 0.3f)
	{
		ChangeState(pObj, new CRage(pObj));
	}

	else if (iHP <= 0)
	{
		ChangeState(pObj, new CDie(pObj));
			static_cast<CMonster3D*>(pObj)->GetAni()->ChangeSequence("MonsterDeath");
	}
}

void CFight::Idle(CGameObject* pObj, float fTime)
{
	std::shared_ptr<CGameObject> pPlayer = static_cast<CMonster3D*>(pObj)->GetPlayer();

	if (pPlayer)
	{
		Vector3 vDir = pPlayer->GetWorldPos() - pObj->GetWorldPos();
		vDir.Normalize();

		Vector3 vAxisX = Vector3::Axis[AXIS_X];

		float fAngle = RadToDeg(acosf(vAxisX.Dot(vDir)));

		if (vDir.z < 0.f)
			fAngle *= -1.f;

		pObj->SetWorldRotY(-fAngle);
	}

	static_cast<CMonster3D*>(pObj)->ChangeSequenceAll("MonsterWait");
}

void CFight::GiveItem(CGameObject* pObj, float fTime)
{
}

void CFight::LongAtk(CGameObject* pObj, float fTime)
{
	static_cast<CMonster3D*>(pObj)->GetAni()->ChangeSequence("longAtk01_Start");
}

void CFight::LongMove(CGameObject* pObj, float fTime)
{
	CState::LongMove(pObj, fTime);
}

void CFight::Die(CGameObject* pObj, float fTime)
{
	CState::Die(pObj, fTime);
}

CFight* CFight::Clone()
{
	return new CFight(*this);
}
