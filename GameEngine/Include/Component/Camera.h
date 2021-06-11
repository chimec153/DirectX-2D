#pragma once

#include "SceneComponent.h"

enum class PLANE_TYPE
{
	PT_LEFT,
	PT_RIGHT,
	PT_TOP,
	PT_BOTTOM,
	PT_NEAR,
	PT_FAR,
	PT_END
};

class CCamera :
	public CSceneComponent
{
	friend class CCameraManager;
	friend class CGameObject;

public:
	CCamera();
	CCamera(const CCamera& camera);
	virtual ~CCamera();

protected:
	Matrix				m_matView;
	Matrix				m_matProj;
	float				m_fAngle;
	float				m_fDist;
	CAMERA_TYPE			m_eType;
	class CSceneComponent* m_pTarget;
	Matrix				m_matVP;
	Vector4				m_Plane[static_cast<int>(PLANE_TYPE::PT_END)];
	Vector4				m_vProjPoint[8];
	Vector3				m_vTargetOffset;
	bool				m_bPosTarget;
	Vector3				m_vTargetPos;
	bool				m_bShake;
	Vector3				m_vShakeDistMax;
	float				m_fShakeTime;
	float				m_fShakeTimeMax;
	float				m_fShakePower;
	float				m_fShakeMass;
	float				m_fFrequency;
	float				m_fShakeDist;

public:
	const Matrix& GetView()	const;
	const Matrix& GetProj()	const;
	const Matrix& GetVP()	const;
	void SetCameraType(CAMERA_TYPE eType);
	CAMERA_TYPE GetCameraType()	const;
	void SetAngle(float fAngle);
	void SetDist(float fDist);
	float GetAngle()	const;
	float GetDist()	const;
	void SetTarget(class CSceneComponent* pCom);
	void LookAt(Vector3 vCenter, Vector3 vTarget, Vector3 vUp);
	void SetLens(float fAngle, float fRatio, float fNear, float fFar);
	const Vector4* GetPlanes()	const;
	void SetTargetOffset(const Vector3& vOffset);
	const Vector3& GetTargetOffset()	const;
	void SetPositionTargetMode(bool bTarget);
	void SetPositionTarget(const Vector3& vPos);
	void SetShake(bool bShake);
	void SetShakePower(float fPower);
	void SetShakeMass(float fMass);
	void SetFrequency(float fFrequency);

public:
	virtual bool Init();
	virtual void Start();
	virtual void Update(float fTime);
	virtual void PostUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void PreRender(float fTime);
	virtual void Render(float fTime);
	virtual void PostRender(float fTime);
	virtual std::shared_ptr<CComponent> Clone();

public:
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);

public:
	virtual void ShowWindow();

public:
	bool FrustumInPoint(const Vector3& vPos);
	bool FrustumInSphere(const Vector3& vPos, float fRadius);
};