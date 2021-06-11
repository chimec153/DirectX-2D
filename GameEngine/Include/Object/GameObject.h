#pragma once

#include "../Ref.h"
#include "../InputObj.h"
#include "../Component/SceneComponent.h"

class CGameObject :
	public CRef
{
	friend class CScene;
	friend class CLayer;
	friend class CEngine;
	friend class CRenderManager;

public:
	CGameObject();
	CGameObject(const CGameObject& obj);
	virtual ~CGameObject();

protected:
	class CScene* m_pScene;
	class CLayer* m_pLayer;

protected:
	bool									m_bStart;
	std::shared_ptr<class CComponent>				m_pRootComponent;
	std::vector<std::shared_ptr<class CComponent>>	m_vecObjectComponent;
	int										m_iObjType;
	std::function<void(const std::string&)>	m_EditorDelete;

protected:
	class CInputObj* m_pInput;

public:
	bool IsStart()	const
	{
		return m_bStart;
	}

	int GetObjType()	const
	{
		return m_iObjType;
	}

	std::shared_ptr<CComponent> FindComponent(const std::string& strTag);
	template <typename T>
	std::shared_ptr<CComponent> FindComponentByType(const std::string& strKey = "")
	{
		size_t iSize = m_vecObjectComponent.size();

		for (size_t i = 0; i < iSize; i++)
		{
			if (typeid(T) == typeid(*m_vecObjectComponent[i].get()))
			{
				if (strKey == "" || strKey == m_vecObjectComponent[i]->GetName())
					return m_vecObjectComponent[i];
			}
		}

		if (typeid(T) == typeid(*m_pRootComponent.get()))
		{
			if (strKey == "" || strKey == m_pRootComponent->GetName())
				return m_pRootComponent;
		}

		return ((CSceneComponent*)m_pRootComponent.get())->FindComponentByType<T>(strKey);
	}
	void SetRootComponent(std::shared_ptr<CComponent>);
	const std::shared_ptr<class CComponent>& GetRootComponent()	const;
	virtual void SetScene(class CScene* pScene);
	virtual void SetLayer(class CLayer* pLayer);
	class CLayer* GetLayer()	const;
	const TransformCBuffer& GetCBuffer()	const;
	const std::vector<std::shared_ptr<CComponent>>* GetObjComponent()	const;
	void SetShadowAll(bool bShadow);

public:
	virtual bool Init();
	virtual void Start();
	virtual void Input(float fTime);
	virtual void Update(float fTime);
	virtual void PostUpdate(float fTime);
	virtual void Collision(float fTime);
	void CheckFrustum();
	virtual void PreRender(float fTime);
	virtual void Render(float fTime);
	virtual void PostRender(float fTime);
	virtual std::shared_ptr<CGameObject> Clone();
public:
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);

public:
	void SetInheritScale(bool bInherit);
	void SetInheritRotX(bool bInherit);
	void SetInheritRotY(bool bInherit);
	void SetInheritRotZ(bool bInherit);
	void SetInheritPos(bool bInherit);
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
	void SetRelativePosX(float d);
	void SetRelativePosY(float d);
	void SetRelativePosZ(float d);
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

public:
	void SetGravity(bool bGravity);
	void AddSpeed(const Vector3& vSpeed);
	void SetSpeed(const Vector3& vSpeed);
	const Vector3& GetSpeed()	const;
	void SetSpeedY(float fSpeedY);
	void AddSpeedY(float fSpeedY);

public:
	const Vector3& GetVelocityScale()			const;
	const Vector3& GetVelocityRot()			const;
	const Vector3& GetVelocity()				const;
	float GetVelocityAmount()			const;
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
	void AddWorldPos(const Vector3& v);
	void AddWorldPos(float x, float y, float z);
	void AddWorldPosX(float x);
	void AddWorldPosY(float y);
	void AddWorldPosZ(float z);
	void SetPivot(const Vector3& v);
	void SetPivot(float x, float y, float z);
	void SetMeshSize(const Vector3& v);

public:
	const Matrix& GetMatScale()	const;
	const Matrix& GetMatRot()	const;
	const Matrix& GetMatPos()	const;
	const Matrix& GetMatWorld()	const;

public:
	template <typename T>
	std::shared_ptr<CComponent> CreateComponent(const std::string& strName, class CLayer* pLayer = nullptr)
	{
		std::shared_ptr<CComponent> pComponent = std::shared_ptr<CComponent>(new T);

		pComponent->SetName(strName);
		pComponent->m_pScene = m_pScene;
		if (!pLayer)
		{
			pComponent->m_pLayer = m_pLayer;
		}
		else
		{
			pComponent->m_pLayer = pLayer;
		}
		pComponent->m_pObj = this;

		if (!pComponent->Init())
		{
			return nullptr;
		}

		if (pComponent->GetType() == COMPONENT_TYPE::CT_OBJECT)
		{
			m_vecObjectComponent.push_back(std::shared_ptr<CComponent>(pComponent));
		}

		return pComponent;
	}

public:
	void GetAllComponentName(std::vector<Hierarchy>& vecstrCom);
	void AddChild(std::shared_ptr<CComponent>);
	void SetUIEnableAll(bool bEnable);

public:
	void AddAxisBind(const std::string& strTag, void(*pFunc)(float, float));
	template <typename T>
	void AddAxisBind(const std::string& strTag, T* pObj, void(T::* pFunc)(float, float))
	{
		m_pInput->AddBindAxis<T>(strTag, pObj, pFunc);
	}

	void AddActionBind(const std::string& strTag, void(*pFunc)(float));
	template <typename T>
	void AddActionBind(const std::string& strTag, T* pObj, void(T::* pFunc)(float), KEY_TYPE eType)
	{
		m_pInput->AddBindAction<T>(strTag, pObj, pFunc, eType);
	}

public:
	virtual void CallNotify(const std::string& strTag);

public:
	virtual void ShowWindow();
};