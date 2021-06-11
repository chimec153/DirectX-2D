#pragma once
#include "Monster3D.h"

enum class CYCLOPS_NEAR_SKILL_TYPE
{
	ATTACK,
	HEAVY_ATTACK,
	END
};

enum class CYCLOPS_FAR_SKILL_TYPE
{
	LONG_ATTACK,
	LONG_MOVE,
	END
};

class CCyclops :
    public CMonster3D
{
public:
	CCyclops();
	CCyclops(const CCyclops& mon);
	virtual ~CCyclops() override;

private:
	float				m_fLongAtkTime;
	float				m_fLongAtkTimeLimit;
	bool				m_bLongAtk;
	class std::shared_ptr<CGameObject>	m_pBall;
	float				m_fCoolTime;
	float				m_fCoolTimeLimit;
	class std::shared_ptr<CGameObject>	m_pLeftHand;
	class std::shared_ptr<CGameObject>	m_pRightHand;
	float				m_fModeChangeTime;
	float				m_fModeChangeLimit;
	bool				m_bModeChange;
	bool				m_bLongMove;
	bool				m_bSequenceMode;

public:
	void SetSequenceMode(bool bMode)
	{
		m_bSequenceMode = bMode;
	}

public:
	virtual bool Init()	override;
	virtual void Start() override;
	virtual void Update(float fTime) override;
	virtual void PostUpdate(float fTime) override;
	virtual void Collision(float fTime) override;
	virtual void PreRender(float fTime) override;
	virtual void Render(float fTime) override;
	virtual void PostRender(float fTime) override;
	virtual std::shared_ptr<CGameObject> Clone();

public:
	virtual void Save(FILE* pFile) override;
	virtual void Load(FILE* pFile) override;

public:
	void Death(float fTime);
	void End(float fTime);
	void HeavyAttackEnd(float fTime);
	void HeavyAttackSound(float fTime);
	void LongAtk(float fTime);
	void LongAtkStart(float fTime);
	void HitGroundLeft(float fTime);
	void HitGroundRight(float fTime);
	void Rage(float fTime);
	void LongMoveNotify(float fTime);
	void ModeChangeBlur(float fTime);
	void ModeChangeBlurEnd(float fTime);
	void DamagedSound(float fTime);
	void AttackSound(float fTime);
	void IdleSound(float fTime);
	void WalkSound(float fTime);
	void LongMoveSound(float fTime);
	void LoneAttackSound(float fTime);
	void LongAtkBRockHold(float fTime);
	void LongAtk_B_Punch(float fTime);

public:
	void HeavyToAttack();
	void WalkToLongAtk();
	void LongAtkToWalk();
	void ToIdle();
	void ModeChangeStart();
	void LongMoveStart();
	void LongMoveEnd(float fTime);
	void ModeChangeBlurEnd();
	void ModeChangeEnd();
	void LongAtkBStart();

public:
	bool Transition();
	bool LongAtkTransition();
	bool ToDeath();
	bool IdleToLongMove();
	bool LongMoveToIdle();
	bool LongAttackA();
	bool LongAttackB();

public:
	virtual void Move(float fTime);
	virtual void Attack(float fTime);
	virtual void AttackAround(float fTime);
	virtual void HeavyAttack(float fTime);
	virtual void LongAttack(float fTime);
	virtual void Idle(float fTime);
	virtual void LongMove(float fTime);

public:
	virtual void ChangeSequenceAll(const char* pSequence);

public:
	void CollideInit(class CCollider* pSrc, class CCollider* pDest, float fTime);
	void CollideStay(class CCollider* pSrc, class CCollider* pDest, float fTime);
	void CollideEnd(class CCollider* pSrc, class CCollider* pDest, float fTime);

public:
	virtual void ShowWindow() override;
public:
	virtual void Attacked();

public:
	void MotionStart();
	void MotionEnd();

public:
	void HeavyAttackMove(float fAniTime, float fTime);
	void LongMove(float fAniTime, float fTime);
};

