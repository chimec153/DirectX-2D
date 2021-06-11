#pragma once
#include "Object/GameObject.h"

enum class BOSS_SCENE_TYPE
{
	NONE,
	FIND1,
	FIND2,
	BOSSFALL,
	BOSSLAND,
	END
};
class CSequence	:
	public CGameObject
{
public:
	CSequence();
	CSequence(const CSequence& seq);
	virtual ~CSequence();

private:
	std::shared_ptr<CComponent>	m_pCam;
	std::vector<Vector3>		m_vecPos;
	std::vector<Vector4>		m_vecRot;
	int							m_iMaxFrame;
	float						m_fSequenceTime;
	float						m_fMaxTime;
	bool						m_bPlay;
	class CGameObject*			m_pTarget;
	class CCamera*				m_pPrevCam;
	std::function<void()>		m_EndCallBack;
	std::function<void()>		m_StartCallBack;
	std::shared_ptr<CComponent>	m_pPlayerCam1;
	std::shared_ptr<CComponent>	m_pPlayerCam2;
	std::shared_ptr<CComponent>	m_pBossCam1;
	std::shared_ptr<CComponent>	m_pBossCam2;
	BOSS_SCENE_TYPE				m_eBossSceneType;
	float						m_fSceneTime;
	float						m_fSceneTimeMax;
	std::shared_ptr<CGameObject>	m_pPlayer;
	float						m_fIdleTime;
	float						m_fIdleTimeMax;

public:
	void SetTarget(class CGameObject* pObj);
	void AddPos(const Vector3& vPos);
	void SetMaxTime(float fMaxTime);
	template <typename T>
	void SetEndCallBack(T* pObj, void(T::* pFunc)())
	{
		m_EndCallBack = std::bind(pFunc, pObj);
	}
	template <typename T>
	void SetStartCallBack(T* pObj, void(T::* pFunc)())
	{
		m_StartCallBack = std::bind(pFunc, pObj);
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
	virtual void CollisionInit(class CCollider* pSrc, class CCollider* pDest, float fTime);
	virtual void CollisionStay(class CCollider* pSrc, class CCollider* pDest, float fTime);
	virtual void CollisionEnd(class CCollider* pSrc, class CCollider* pDest, float fTime);

public:
	virtual void ShowWindow() override;

};

