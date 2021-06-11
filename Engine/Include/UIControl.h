#pragma once
#include "Ref.h"
class CUIControl :
	public CRef
{
	friend class CUIManager;

protected:
	CUIControl();
	CUIControl(const CUIControl& ui);
	virtual ~CUIControl();

protected:
	bool						m_bStart;
	class CUIObject*			m_pObj;
	class CUIViewPort*			m_pViewPort;
	class CUITransform*			m_pTransform;
	CUIControl*					m_pParent;
	std::vector<CUIControl*>	m_vecChild;

public:
	class CUITransform* GetTransform()	const;
	void AddChild(CUIControl* pComponent);
	void DeleteChild(CUIControl* pComponent);
	void Detach(CUIControl* pComponent);
	CUIControl* FindComponent(const std::string& strTag);
	void SetObj(class CUIObject* pObj);

public:
	virtual bool Init();
	virtual void Start();
	virtual void Update(float fTime);
	virtual void PostUpdate(float fTime);
	virtual void PreRender();
	virtual void Render();
	virtual void PostRender();
	virtual CUIControl* Clone() = 0;

public:
	void SetInheritScale(bool bInherit);
	void SetInheritRotX(bool bInherit);
	void SetInheritRotY(bool bInherit);
	void SetInheritRotZ(bool bInherit);
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
	void AddRelativeScale(const Vector3& v);
	void AddRelativeScale(float x, float y, float z);
	void AddRelativeRot(const Vector3& v);
	void AddRelativeRot(float x, float y, float z);
	void AddRelativeRotX(float x);
	void AddRelativeRotY(float y);
	void AddRelativeRotZ(float z);
	void AddRelativePos(const Vector3& v);
	void AddRelativePos(float x, float y, float z);

public:
	Vector3 GetVelocityScale()			const;
	Vector3 GetVelocityRot()			const;
	Vector3 GetVelocity()				const;
	float GetVelocityAmount()			const;
	Vector3 GetRelativeScale()			const;
	Vector3 GetRelativeRot()			const;
	Vector3 GetRelativePos()			const;
	Vector3 GetRelativeAxis(AXIS axis)	const;
	Vector3 GetWorldScale()				const;
	Vector3 GetWorldRot()				const;
	Vector3 GetWorldPos()				const;
	Vector3 GetPivot()					const;
	Vector3 GetWorldAxis(AXIS axis)		const;

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
	void AddWorldScale(const Vector3& v);
	void AddWorldScale(float x, float y, float z);
	void AddWorldRot(const Vector3& v);
	void AddWorldRot(float x, float y, float z);
	void AddWorldRotX(float x);
	void AddWorldRotY(float y);
	void AddWorldRotZ(float z);
	void AddWorldPos(const Vector3& v);
	void AddWorldPos(float x, float y, float z);
	void SetPivot(const Vector3& v);
	void SetPivot(float x, float y, float z);
	void SetMeshSize(const Vector3& v);

public:
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);

public:
	void GetAllControlName(std::vector<Hierarchy>& vecHierarchy);
	void GetAllControl(std::vector<CUIControl*>& vecSceneCom);
};

