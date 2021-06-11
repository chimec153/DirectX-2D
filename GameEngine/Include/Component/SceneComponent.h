#pragma once
#include "Component.h"

class CSceneComponent :
	public CComponent
{
	friend class CGameObject;

public:
	CSceneComponent();
	CSceneComponent(const CSceneComponent& com);
	virtual ~CSceneComponent();

protected:
	std::shared_ptr<class CTransform>			m_pTransform;
	SCENE_COMPONENT_TYPE				m_eSceneComponentType;
	CSceneComponent* m_pParent;
	std::list<std::shared_ptr<CComponent>>		m_ChildList;
	int									m_iSceneComponentClassType;
	int								m_iZOrder;

protected:
	bool				m_bInstance;
	bool				m_bPostProcess;
	std::vector<class CPostProcess*>	m_vecPostProcess;
	std::list<std::shared_ptr<CComponent>>	m_ObjComList;
	bool				m_bUIEnable;
	bool				m_bTreeOpen;
	bool				m_bInFrustum;
	bool				m_bFrustumCulling;
	class CRenderer*	m_pRenderer;
	bool				m_bCastShadow;

public:
	int GetZOrder()	const;
	void SetZOrder(int iZOrder);
	std::shared_ptr<class CTransform> GetTransform()	const;
	SCENE_COMPONENT_TYPE GetSceneComponentType()	const;
	void AddChild(const std::shared_ptr<CComponent>& pComponent);
	void DeleteChild(const std::shared_ptr<CComponent>& pComponent);
	const std::shared_ptr<CComponent> DeleteChild(class CSceneComponent* pComponent);
	void Detach(const std::shared_ptr<CComponent>& pComponent);
	bool IsUIEnabled()	const;
	void UIEnable(bool bEnable);
	void UIEnableAll(bool bEnable);
	std::shared_ptr<CComponent> FindComponent(const std::string& strTag);
	template <typename T>
	std::shared_ptr<CComponent> FindComponentByType(const std::string& strTag = "")
	{
		std::list<std::shared_ptr<class CComponent>>::iterator iter = m_ChildList.begin();
		std::list<std::shared_ptr<class CComponent>>::iterator iterEnd = m_ChildList.end();

		for(;iter!=iterEnd;++iter)
		{
			if (typeid(*(*iter).get()) == typeid(T))
			{
				if (strTag == "" || strTag == (*iter)->GetName())
					return *iter;
			}

			std::shared_ptr<CComponent> pCom = ((CSceneComponent*)(*iter).get())->FindComponentByType<T>();

			if (pCom)
				return pCom;
		}

		return nullptr;
	}
	int GetSceneComponentClassType()	const;
	void SetObj(class CGameObject* pObj);
	virtual void SetScene(class CScene* pScene);
	void SetLayer(class CLayer* pLayer);
	void SetSceneComponentType(SCENE_COMPONENT_TYPE eType);
	class CSceneComponent* GetParent()	const;
	const TransformCBuffer& GetCBuffer()	const;
	std::shared_ptr<CComponent> FindObjCom(const std::string& strKey)	const;
	bool IsFrustumIn()	const;
	class CRenderer* GetRenderer()	const;
	class CAnimation* GetAnimation()	const;
	void SetInstIndex(int iIndex);
	void SetInstancing(class CRenderInstancing* pInst);
	void SetShadow(bool bCast);
	bool IsShadow()	const;
	void SetShadowAll(bool bShadow);
	void SetFrustumCulling(bool bCull);

public:
	void SetMaterial(const std::shared_ptr<class CMaterial>& pMaterial, int iContainer = 0, int iSubset = 0);
	void SetMaterial(const std::string& strKey, int iContainer = 0, int iSubset = 0);
	std::shared_ptr<class CMaterial> GetMaterial(int iContainer = 0, int iSubset = 0)	const;
	class CMesh* GetMesh()	const;
	void SetMesh(const std::string& strName);
	void SetMesh(class CMesh* pShader);
	bool IsInstance()	const;
	virtual void SetInstance(bool bInst = true);
	bool IsPostProcess()	const;
	void SetShader(const std::string& strKey, int iContainer = 0, int iSubset = 0);
	void SetShader(const std::shared_ptr<class CShader>& pShader, int iContainer = 0, int iSubset = 0);
	std::shared_ptr<class CShader> GetShader(int iContainer = 0, int iSubset = 0);
	void AddRenderState(const std::string& strKey, int iContainer = 0, int iSubset = 0);
	void AddRenderState(class CRenderState* pState, int iContainer = 0, int iSubset = 0);
	class CRenderInstancing* GetInstancing()	const;
	int GetInstIndex()	const;
	void SetAnimation(class CAnimation* pAni);
	size_t GetMaterialSize()	const;
	size_t GetMaterialSubsetSize(int iContainer)	const;
	void ResizeMaterial(int iSize, int iContainer = 0);
	void ResizeShader(int iSize, int iContainer = 0);
	void ResizeRenderState(RENDER_STATE_TYPE eType, int iSize, int iContainer = 0);
	
	template <typename T>
	T* AddPostProcess(const std::string& strKey)
	{
		T* pProcess = new T;

		pProcess->SetName(strKey);

		if (!pProcess->Init())
		{
			SAFE_RELEASE(pProcess);
			return nullptr;
		}

		m_bPostProcess = true;

		m_vecPostProcess.push_back(pProcess);

		pProcess->AddRef();

		return pProcess;
	}

public:
	void SetTexture(LINK_TYPE eType, std::shared_ptr<class CTexture> pTexture,
		int iShaderType = (int)SHADER_CBUFFER_TYPE::CBUFFER_VERTEX | (int)SHADER_CBUFFER_TYPE::CBUFFER_PIXEL, int iRegister = 0, int iContainer = 0, int iSubSet = 0);
	void SetTexture(LINK_TYPE eType, const std::string& strTag,
		int iShaderType = (int)SHADER_CBUFFER_TYPE::CBUFFER_VERTEX | (int)SHADER_CBUFFER_TYPE::CBUFFER_PIXEL, int iRegister = 0, int iContainer = 0, int iSubSet = 0);
	void SetTexture(LINK_TYPE eType, const std::string& strTag, const TCHAR* pFileName, const std::string& strRootPath,
		int iShaderType = (int)SHADER_CBUFFER_TYPE::CBUFFER_VERTEX | (int)SHADER_CBUFFER_TYPE::CBUFFER_PIXEL, int iRegister = 0, int iContainer = 0, int iSubSet = 0);
	void SetTexture(LINK_TYPE eType, const std::string& strTag, const TCHAR* pFileName,
		int iShaderType = (int)SHADER_CBUFFER_TYPE::CBUFFER_VERTEX | (int)SHADER_CBUFFER_TYPE::CBUFFER_PIXEL, int iRegister = 0, int iContainer = 0, int iSubSet = 0);

public:
	template <typename T>
	std::shared_ptr<CComponent> AddObjComponent(const std::string& strKey)
	{
		std::shared_ptr<CComponent> pCom = FindObjCom(strKey);

		if (pCom.get())
			return nullptr;

		pCom = std::shared_ptr<CComponent>(new T);

		if (pCom->GetType() != COMPONENT_TYPE::CT_OBJECT)
		{
			return nullptr;
		}

		if (!pCom->Init())
		{
			return nullptr;
		}

		m_ObjComList.push_back(pCom);

		return pCom;
	}
	void AddObjComponent(const std::shared_ptr<CComponent>& pCom);

public:
	virtual bool Init();
	virtual void Start();
	virtual void Update(float fTime);
	virtual void PostUpdate(float fTime);
	virtual void Collision(float fTime);
	void CheckFrustum();
	virtual void PreRender(float fTime);
	virtual void RenderShadow(float fTime);
	virtual void Render(float fTime);
	virtual void PostRender(float fTime);
	virtual std::shared_ptr<CComponent> Clone();

public:
	void RenderObj(float fTime);

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
	void SetWorldMatrix();

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
	void SetMeshPos(const Vector3& vPos);

public:
	const Matrix& GetMatScale()	const;
	const Matrix& GetMatRot()	const;
	const Matrix& GetMatPos()	const;
	const Matrix& GetMatWorld()	const;

public:
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);

public:
	void GetAllComponentName(std::vector<Hierarchy>& vecHierarchy);
	void GetAllComponent(std::vector<CSceneComponent*>& vecSceneCom);

public:
	virtual bool ShowNode();
	virtual void ShowWindow();
};