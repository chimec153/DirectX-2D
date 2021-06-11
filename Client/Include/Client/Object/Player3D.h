#pragma once
#include "Object\GameObject.h"
#include "../Editor.h"


typedef struct _tagVertexTrail
{
	Vector3 vPos;
	Vector2 vUV;
	float   fAlpha;
	_tagVertexTrail() :
		fAlpha(0.f)
	{
	}
}VERTEXTRAIL, * PVERTEXTRAIL;

class CPlayer3D :
	public CGameObject
{
public:
	CPlayer3D();
	CPlayer3D(const CPlayer3D& obj);
	virtual ~CPlayer3D();

protected:
	std::shared_ptr<class CComponent>	m_pMesh;
	std::shared_ptr<class CComponent>	m_pCam;
	class CTerrain*					m_pTerrain;
	std::shared_ptr<class CGameObject>	m_pWeapon;
	float		m_fRot;
	class CAnimation*	m_pUpperAni;
	float	m_fX;
	float	m_fZ;
	float	m_fDist;
	bool	m_bAttack;
	float	m_fSpeed;
	bool	m_bAir;
	std::shared_ptr<class CComponent>	m_pArm;
	int		m_iHP;
	int		m_iHPMax;
	float	m_fRunTime;
	float	m_fAngle;
	bool	m_bAttacked;
	float	m_fDeadTime;
	std::shared_ptr<class CComponent>	m_pPaper;
	float	m_pCoolTime[static_cast<int>(SKILL_TYPE::END)];
	float	m_pCoolTimeMax[static_cast<int>(SKILL_TYPE::END)];
	int		m_iExp;
	int		m_iExpMax;
	float	m_fHeight;
	std::shared_ptr<class CComponent>	m_pUpperOutLine;
	std::shared_ptr<class CComponent>	m_pStaffOutLine;
	std::shared_ptr<CGameObject>		m_pRide;
	float	m_fY;
	int		m_iAttackMax;
	int		m_iAttackMin;
	int		m_iArmorMax;
	int		m_iArmorMin;
	int		m_iGold;
	bool	m_bEquipWindowOpened; 
	std::shared_ptr<CComponent> m_pEquipCam;
	class CGameObject*		m_pTarget;
	std::shared_ptr<CGameObject>	m_pTargetParticle;
	std::shared_ptr<CComponent>	m_pCameraRay;
	float	m_fDestDist;
	float	m_fCastMax;
	float	m_fCastTime;
	bool	m_bCast;
	std::shared_ptr<CComponent>	m_pHPBar;
	int		m_iLevel;
	SKILL_TYPE		m_eSkillType;
	std::shared_ptr<CComponent>	m_pLight;
	std::shared_ptr<CComponent>	m_pName;
	float		m_fMoveSpeed;
	bool		m_bSequenceMode;

public:
	void SetTerrain(class CTerrain* pTerrain)
	{
		m_pTerrain = pTerrain;
	}
	
	void AddHP(int iHP)
	{
		m_iHP += iHP;
	}
	int GetHPMax()	const
	{
		return m_iHPMax;
	}
	int GetHP()	const
	{
		return m_iHP;
	}
	int GetExp()	const
	{
		return m_iExp;
	}
	int GetExpMax()	const
	{
		return m_iExpMax;
	}
	float GetCoolTime(SKILL_TYPE eType)	const
	{
		return m_pCoolTime[static_cast<int>(eType)];
	}
	float GetCoolTimeMax(SKILL_TYPE eType)	const
	{
		return m_pCoolTimeMax[static_cast<int>(eType)];
	}
	float GetX()	const
	{
		return m_fX;
	}
	float GetZ()	const
	{
		return m_fZ;
	}
	float GetHeight()	const
	{
		return m_fHeight;
	}
	float GetY()	const
	{
		return m_fY;
	}
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
	void SetRide(const std::shared_ptr<CGameObject> pRide)
	{
		m_pRide = pRide;
	}
	void AddGold(int iGold)
	{
		m_iGold += iGold;
	}
	int GetGold()	const
	{
		return m_iGold;
	}

	const std::shared_ptr<CGameObject>& GetWeapon()	const
	{
		return m_pWeapon;
	}
	const std::shared_ptr<CComponent>& GetWeaponOutLine()	const
	{
		return m_pStaffOutLine;
	}
	void SetTarget(class CGameObject* pObj);
	class CAnimation* GetAni()	const
	{
		return m_pUpperAni;
	}
	const std::shared_ptr<CComponent>& GetMesh()	const
	{
		return m_pMesh;
	}
	const std::shared_ptr<CComponent>& GetOutLine()	const
	{
		return m_pUpperOutLine;
	}
	float GetCastMax()	const
	{
		return m_fCastMax;
	}
	float GetCastTime()	const
	{
		return m_fCastTime;
	}
	void SetNameAndHPBar(bool bEnable)
	{
		m_pName->SetEnable(bEnable);
		m_pHPBar->SetEnable(bEnable);
	}
	void SetSequenceMode(bool bOn)
	{
		m_bSequenceMode = bOn;
	}
	void SetRotY(float fRot)
	{
		m_fRot = fRot;
	}
	bool IsAttack()	const
	{
		return m_bAttack;
	}

public:
	virtual bool Init();
	virtual void Start();
	virtual void Update(float fTime);
	virtual void PostUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void PreRender(float fTime);
	virtual void Render(float fTime);
	virtual void PostRender(float fTime);
	virtual std::shared_ptr<CGameObject> Clone();

public:
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);

public:
	void ColInit(class CCollider* pSrc, class CCollider* pDest, float fTime);
	void ColStay(class CCollider* pSrc, class CCollider* pDest, float fTime);
	void ColLast(class CCollider* pSrc, class CCollider* pDest, float fTime);
public:
	virtual void ShowWindow() override;

public:
	void CameraMoveZ(float, float);
	void CameraMoveX(float, float);
	void CameraRotX(float, float);
	void CameraRotY(float, float);
	void CameraRotZ(float, float);
	void CamRotY(float, float);
	void Space(float, float);
	void MoveZ(float, float);
	void MoveX(float, float);

	void BoneRotX(float, float);
	void BoneRotY(float, float);

public:
	void Inventory(float fTime);
	void Equip(float fTime);
	void SpawnRide(float fTime);
	virtual void Skill1(float fTime);
	virtual void Skill1Down(float fTime);
	virtual void Skill2(float fTime);
	virtual void Skill3(float fTime);
	void Quest(float fTime);
	void RunSound(float fTime);

public:
	virtual void SetTransition();

public:
	void ChangeSequenceAll(const char* pSeq);

public:
	static bool TransitionFunc();
	bool AttackToIdleFunc();
	bool RunJumpLandToIdle();
	bool RunJumpLandToRunFR();
	bool RunJumpLandToRunFL();
	bool RunJumpLandToRunF();
	bool RunJumpLandToRunL();
	bool RunJumpLandToRunBR();
	bool RunJumpLandToRunBL();
	bool RunJumpLandToRunB();
	bool RunJumpLandToRunR();
	bool IdleToWalk();
	bool IdleToWalkBack();
	bool WalkToIdle();
	bool IdleToJump();
	bool IdleToSk14();
	bool JumpLandToIdle();
	bool JumpLoopToLand();
	bool WalkToRunF();
	bool WalkToRunFR();
	bool WalkToRunFL();
	bool WalkToRunL();
	bool WalkToRunB();
	bool WalkToRunBR();
	bool WalkToRunBL();
	bool WalkToRunR();
	bool IdleToDMG();
	bool IdleToDown();
	bool LoopToDeadBody();
	bool IdleToSk_28_Start();
	bool Sk_28_LoopToEnd();
	bool Sk_28ToWalk();

public:
	void Land();
	void DamageEnd();
	void DeadTimeStart();
	void DeadBody();
	virtual void CastFinish();
	void CastStart();
	virtual void CastCancel();

public:
	virtual void AttackEnd();
	void AttackEnd(float fTime);
public:
	virtual void AttackStart();
	virtual void sk08Start();

public:
	virtual void SetOutLine(bool bLine);

public:
	void SpawnRide();

public:
	virtual void SetLayer(class CLayer* pLayer) override;
	virtual void SetScene(class CScene* pScene) override;

public:
	void CalcDamage(int iAttackMax, int iAttackMin);

};

