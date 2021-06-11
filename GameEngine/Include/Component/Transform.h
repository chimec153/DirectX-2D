#pragma once

#include "../GameEngine.h"

class CTransform
{
	friend class CSceneComponent;

public:
	CTransform();
	CTransform(const CTransform& transform);
	~CTransform();

private:
	class CScene* m_pScene;
	class CSceneComponent* m_pOwner;
	CTransform* m_pParent;
	std::list<CTransform*>		m_ChildList;
	TransformCBuffer			m_tCBuffer;

private:
	Vector3						m_vVelocityScale;
	Vector3						m_vVelocityRot;
	Vector3						m_vVelocity;
	Vector3						m_vRelativeScale;
	Vector3						m_vRelativeRot;
	Vector3						m_vRelativePos;
	Vector3						m_vRelativeAxis[AXIS_END];
	bool						m_bInheritScale;
	bool						m_bInheritRotX;
	bool						m_bInheritRotY;
	bool						m_bInheritRotZ;
	bool						m_bInheritPos;
	bool						m_bUpdateScale;
	bool						m_bUpdateRot;
	bool						m_bUpdatePos;
	bool						m_bBone;
	bool						m_bQuaternion;
	Vector4						m_vRelativeQuaternion;
	bool						m_bGravity;
	Vector3						m_vSpeed;

public:
	void SetInheritScale(bool bInherit);
	void SetInheritRotX(bool bInherit);
	void SetInheritRotY(bool bInherit);
	void SetInheritRotZ(bool bInherit);
	void SetInheritPos(bool bInherit);

	bool IsInheritScale()	const;
	bool IsInheritRotX()	const;
	bool IsInheritRotY()	const;
	bool IsInheritRotZ()	const;

	const Vector3& GetVelocityScale()	const;
	const Vector3& GetVelocityRot()	const;
	const Vector3& GetVelocity()	const;
	float GetVelocityAmount()	const;
	const TransformCBuffer& GetCBuffer()	const;
	bool IsQuaternion()	const;
	void SetGravity(bool bGravity);
	void AddSpeed(const Vector3& vSpeed);
	void SetSpeed(const Vector3& vSpeed);
	const Vector3& GetSpeed()	const;
	void SetSpeedY(float fSpeedY);
	void AddSpeedY(float fSpeedY);

public:
	void InheritScale();
	void InheritRot();
	void InheritPos();
	void SetRelativeScale(const Vector3& v);
	void SetRelativeScale(float x, float y, float z);
	void SetRelativeRot(const Vector3& v);
	void SetRelativeRot(float x, float y, float z);
	void SetRelativeRotX(float x);
	void SetRelativeRotY(float y);
	void SetRelativeRotZ(float z);
	void SetRelativePos(const Vector3& v);
	void SetRelativePos(float x, float y, float z);
	void SetRelativePosX(float x);
	void SetRelativePosY(float y);
	void SetRelativePosZ(float z);
	void AddRelativeScale(const Vector3& v);
	void AddRelativeScale(float x, float y, float z);
	void AddRelativeRot(const Vector3& v);
	void AddRelativeRot(float x, float y, float z);
	void AddRelativeRotX(float x);
	void AddRelativeRotY(float y);
	void AddRelativeRotZ(float z);
	void AddRelativePos(const Vector3& v);
	void AddRelativePos(float x, float y, float z);
	void AddRelativePosX(float x);
	void AddRelativePosY(float y);
	void AddRelativePosZ(float z);

private:
	Vector3						m_vWorldScale;
	Vector3						m_vWorldRot;
	Vector3						m_vWorldPos;
	Vector3						m_vWorldAxis[AXIS_END];
	Vector3						m_vPivot;
	Vector3						m_vMeshSize;
	Vector4						m_vWorldQuaternion;
	Vector3						m_vMeshPos;

public:
	const Vector3& GetRelativeScale()			const;
	const Vector3& GetRelativeRot()			const;
	const Vector3& GetRelativePos()			const;
	const Vector3& GetRelativeAxis(AXIS axis)	const;
	const Vector3& GetWorldScale()				const;
	const Vector3& GetWorldRot()				const;
	const Vector3& GetWorldPos()				const;
	const Vector3& GetPivot()					const;
	const Vector3& GetWorldAxis(AXIS axis)		const;
	const Vector3& GetMeshSize()				const;
	const Vector3 GetBoneWorldPos()			const;
	const Vector4& GetWorldQRot()		const;
	const Vector3& GetMeshPos()			const;

public:
	void SetWorldScale(const Vector3& v);
	void SetWorldScale(float x, float y, float z);
	void SetWorldRot(const Vector3& v);
	void SetWorldRot(float x, float y, float z);
	void SetWorldRotX(float x);
	void SetWorldRotY(float y);
	void SetWorldRotZ(float z);
	void SetWorldPos(const Vector3& v);
	void SetWorldPos(float x, float y, float z);
	void SetWorldPosX(float x);
	void SetWorldPosY(float y);
	void SetWorldPosZ(float z);
	void AddWorldScale(const Vector3& v);
	void AddWorldScale(float x, float y, float z);
	void AddWorldRot(const Vector3& v);
	void AddWorldRot(float x, float y, float z);
	void AddWorldRotX(float x);
	void AddWorldRotY(float y);
	void AddWorldRotZ(float z);
	void AddWorldRot(const Vector3& v, float fAngle);
	void AddWorldPos(const Vector3& v);
	void AddWorldPos(float x, float y, float z);
	void AddWorldPosX(float x);
	void AddWorldPosY(float y);
	void AddWorldPosZ(float z);
	void SetPivot(const Vector3& v);
	void SetPivot(float x, float y, float z);
	void SetMeshSize(const Vector3& v);
	void LookAt(const Vector3& v);
	void SetQuaternionRotX(float x);
	void AddQuaternionRotX(float x);
	void SetQuaternionRotY(float y);
	void AddQuaternionRotY(float y);
	void SetQuaternionRotZ(float z);
	void AddQuaternionRotZ(float z);
	void AddQuaternionRotAxis(const Vector3& vAxis, float fAngle);
	void SetQuaternionRotAxis(const Vector3& vAxis, float fAngle);
	void SetMeshPos(const Vector3& vPos);

private:
	Matrix						m_matScale;
	Matrix						m_matRot;
	Matrix						m_matPos;
	Matrix						m_matWorld;
	Matrix						m_matBone;
	Matrix						m_matParent;

public:
	const Matrix& GetMatScale()	const;
	const Matrix& GetMatRot()	const;
	const Matrix& GetMatPos()	const;
	const Matrix& GetMatWorld()	const;
	const Matrix& GetMatBone()	const;
	const Matrix& GetMatWVP()	const;
	void SetBoneMatrix(const Matrix& mat);
	void DisableBoneMatrix();
	void SetWorldMatrix();
	void SetWorldMatrix(const Matrix& matWorld);
	void SetTexTransformMatrix(const Matrix& mat);
	void SetParentMatrix(const Matrix& mat);

public:
	void Start();
	void Update(float fTime);
	void PostUpdate(float fTime);
	void SetTransform();
	void SetTransformShadow();
	std::shared_ptr<CTransform> Clone();

public:
	void Save(FILE* pFile);
	void Load(FILE* pFile);

public:
	void ShowWindow();
};
