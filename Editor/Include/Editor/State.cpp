#include "State.h"
#include "Object/Monster3D.h"
#include "Resource/Animation.h"
#include "Idle.h"
#include "Die.h"

CState::CState()	:
	m_pObj(nullptr)
{
}

CState::CState(CGameObject* pObj)	:
	m_pObj(pObj)
{
}

CState::~CState()
{
}


void CState::Move(CGameObject* pObj, float fTime)
{
	static_cast<CMonster3D*>(pObj)->ChangeSequenceAll("MonsterWalk");

	std::shared_ptr<CGameObject> pPlayer = static_cast<CMonster3D*>(pObj)->GetPlayer();
	if (!pPlayer)
	{
		float fMoveDist = fTime * static_cast<CMonster3D*>(pObj)->GetMoveSpeed();
		Vector3 vAxis = static_cast<CMonster3D*>(pObj)->GetWorldAxis(AXIS_X);
		static_cast<CMonster3D*>(pObj)->AddWorldPos(vAxis * fMoveDist);
		static_cast<CMonster3D*>(pObj)->AddDist(fMoveDist);
		float fDist = static_cast<CMonster3D*>(pObj)->GetDist();
		float fLimitDist = static_cast<CMonster3D*>(pObj)->GetLimitDist();

		if (fDist >= fLimitDist)
		{
			static_cast<CMonster3D*>(pObj)->AddDist(-fLimitDist);
			static_cast<CMonster3D*>(pObj)->AddWorldRotY(180.f);
		}
	}
	else
	{
		Vector3 vDir = pPlayer->GetBoneWorldPos() - pObj->GetWorldPos();
		vDir.Normalize();

		Vector3 vAxisX = Vector3::Axis[AXIS_X];

		float fAngle = RadToDeg(acosf(vAxisX.Dot(vDir)));

		if (vDir.z < 0.f)
			fAngle *= -1.f;

		pObj->SetWorldRotY(-fAngle);

		pObj->AddWorldPos(vDir * fTime * static_cast<CMonster3D*>(pObj)->GetMoveSpeed());
	}
}

void CState::Attack(CGameObject* pObj, float fTime)
{
}

void CState::AttackAround(CGameObject* pObj, float fTime)
{
}

void CState::HeavyAttack(CGameObject* pObj, float fTime)
{
}

void CState::Attacked(CGameObject* pObj, float fTime)
{
}

void CState::Idle(CGameObject* pObj, float fTime)
{
	static_cast<CMonster3D*>(pObj)->ChangeSequenceAll("MonsterWait");
}

void CState::GiveItem(CGameObject* pObj, float fTime)
{
}

void CState::LongAtk(CGameObject* pObj, float fTime)
{
}

void CState::LongMove(CGameObject* pObj, float fTime)
{
	std::shared_ptr<CGameObject> pPlayer = static_cast<CMonster3D*>(pObj)->GetPlayer();
	if (!pPlayer)
	{
		float fMoveDist = fTime * static_cast<CMonster3D*>(pObj)->GetMoveSpeed();
		Vector3 vAxis = static_cast<CMonster3D*>(pObj)->GetWorldAxis(AXIS_X);
		static_cast<CMonster3D*>(pObj)->AddWorldPos(vAxis * fMoveDist);
		static_cast<CMonster3D*>(pObj)->AddDist(fMoveDist);
		float fDist = static_cast<CMonster3D*>(pObj)->GetDist();
		float fLimitDist = static_cast<CMonster3D*>(pObj)->GetLimitDist();

		if (fDist >= fLimitDist)
		{
			static_cast<CMonster3D*>(pObj)->AddDist(-fLimitDist);
			static_cast<CMonster3D*>(pObj)->AddWorldRotY(180.f);
		}
	}
	else
	{
		Vector3 vDir = pPlayer->GetBoneWorldPos() - pObj->GetWorldPos();
		vDir.y = 0.f;
		vDir.Normalize();

		Vector3 vAxisX = Vector3::Axis[AXIS_X];

		float fAngle = RadToDeg(acosf(vAxisX.Dot(vDir)));

		if (vDir.z < 0.f)
			fAngle *= -1.f;

		pObj->SetWorldRotY(-fAngle);

		//float fDist = fTime * static_cast<CMonster3D*>(pObj)->GetSpeed() * 1.5f;
		//float fLimitDist = static_cast<CMonster3D*>(pObj)->GetLimitDist();

		//static_cast<CMonster3D*>(pObj)->AddDist(fDist);
		////pObj->AddWorldPos(vDir * fDist);
		//fDist = static_cast<CMonster3D*>(pObj)->GetDist();

		//if (fDist >= 294.63855f)
		//{
		//	static_cast<CMonster3D*>(pObj)->ChangeSequenceAll("longMove_End");
		//	static_cast<CMonster3D*>(pObj)->AddDist(-fLimitDist);
		//}
	}
}

void CState::Die(CGameObject* pObj, float fTime)
{
	ChangeState(pObj, new CDie);
	static_cast<CMonster3D*>(pObj)->GetAni()->ChangeSequence("MonsterDeath");
}

void CState::ChangeState(CGameObject* pObj, CState* pState)
{
	static_cast<CMonster3D*>(pObj)->ChangeState(pState);
}

CState* CState::Clone()
{
	return nullptr;
}
