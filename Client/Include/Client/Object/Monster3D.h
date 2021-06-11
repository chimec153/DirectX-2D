#pragma once
#include "Object\GameObject.h"
class CMonster3D :
	public CGameObject
{
	friend class CScene;
	friend class CState;

public:
	CMonster3D();
	CMonster3D(const CMonster3D& mon);
	virtual ~CMonster3D() override;

protected:
	std::shared_ptr<class CGameObject>	m_pPlayer;
	float		m_fTime;
	class CTerrain*	m_pTerrain;
	int			m_iHP;	
	class CAnimation * m_pAni;
	std::shared_ptr<class CComponent>	m_pPaper;
	bool		m_bFight;
	float		m_fDist;
	float		m_fLimitDist;
	bool		m_bDie;
	int			m_iHPMax;
	float		m_fSpeed;
	class CState* m_pState;
	int			m_iAttackMax;
	int			m_iAttackMin;
	int			m_iArmorMax;
	int			m_iArmorMin;
	bool		m_bRooted;
	char**		m_pLootItems;
	int			m_iLootCount;
	int			m_iGold;
	std::shared_ptr<CComponent>	m_pHPBar;
	std::shared_ptr<CComponent>	m_pName;
	bool		m_bDamaged;
	bool		m_bAir;

public:
	int GetHP()	const
	{
		return m_iHP;
	}
	int GetHPMax()	const
	{
		return m_iHPMax;
	}

	void SetPlayer(std::shared_ptr<CGameObject>& pPlayer)
	{
		m_pPlayer = pPlayer;
	}
	std::shared_ptr<class CGameObject> GetPlayer()	const
	{
		return m_pPlayer;
	}
	bool IsDie()	const
	{
		return m_bDie;
	}
	float GetDist()	const
	{
		return m_fDist;
	}
	float GetLimitDist()	const
	{
		return m_fLimitDist;
	}
	void AddDist(float fDist)
	{
		m_fDist += fDist;
	}
	class CAnimation* GetAni()	const
	{
		return m_pAni;
	}
	void ChangeState(class CState* pState);
	virtual void ChangeSequenceAll(const char* pSequence);
	int GetAttackMax()	const
	{
		return m_iAttackMax;
	}
	int GetAttackMin()	const
	{
		return m_iAttackMin;
	}
	int GetArmorMax()	const
	{
		return m_iArmorMax;
	}
	int GetArmorMin()	const
	{
		return m_iArmorMin;
	}
	void AddLootItem(const char* pItem);
	void Damaged(bool bDMG)
	{
		m_bDamaged = bDMG;
	}
	float GetMoveSpeed()	const
	{
		return m_fSpeed;
	}
	bool IsAir()	const
	{
		return m_bAir;
	}
	void SetAir(bool bAir)
	{
		m_bAir = bAir;
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
	void CollisionInit(class CCollider* pSrc, class CCollider* pDest, float fTime);
	void CollisionStay(class CCollider* pSrc, class CCollider* pDest, float fTime);
	void CollisionLast(class CCollider* pSrc, class CCollider* pDest, float fTime);

public:
	virtual void Move(float fTime);
	virtual void Attack(float fTime);
	virtual void AttackAround(float fTime);
	virtual void HeavyAttack(float fTime);
	virtual void LongAttack(float fTime);
	virtual void Idle(float fTime);
	virtual void LongMove(float fTime);

public:
	virtual void Attacked();

public:
	void LootingCallBack();

public:
	void CalcDamage(int iAttackMax = -1, int iAttackMin = -1);

public:
	bool DamagedTransitionFunc();

public:
	void DamagedEnd();


};

