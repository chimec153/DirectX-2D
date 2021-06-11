#pragma once
#include "SceneComponent.h"

class CCollider :
	public CSceneComponent
{
	friend class CGameObject;

protected:
	CCollider();
	CCollider(const CCollider& col);
	virtual ~CCollider();

protected:
	COLLIDER_TYPE			m_eColType;
#ifdef _DEBUG
	class CMesh*		m_pDebugMesh;
	std::shared_ptr<class CMaterial>	m_pDebugMtrl;
	std::shared_ptr<class CShader>		m_pDebugShader;
	class CRenderState* m_pRasterizer;
	class CRenderState* m_pDepthStencilState;
#endif 		
	Vector3					m_vCross;
	bool					m_bUI;
	bool					m_b2D;
	Vector3					m_vMin;
	Vector3					m_vMax;
	bool					m_bCheck;
	bool					m_bEnableCol;
	std::list<int>			m_SectionList;
	std::list<int>			m_CurSecList;
	std::list<CCollider*>	m_PrevList;
	std::list<CCollider*>	m_CurList;
	PCollisionProfile		m_pProFile;
	std::vector<std::function<void(CCollider*, CCollider*, float)>>		m_vecCallBack[(int)COLLISION_STATE::CS_END];
	std::vector<std::function<void(CCollider*, const Vector2&, float)>>	m_vecMouseCallBack[(int)COLLISION_STATE::CS_END];
	bool					m_bMouse;
	std::string				m_strProFile;

public:
	COLLIDER_TYPE GetColType()	const;
	void SetCross(const Vector3& v);
	const Vector3& GetCrs()	const;
	bool IsUI()	const;
	bool Is2D()	const;
	const Vector3& GetMax()	const;
	const Vector3& GetMin()	const;
	void AddSection(int iSec);
	void Clear();
	void AddPrevCol(CCollider* pCol);
	bool HasPrevCol(CCollider* pCol);
	void AddCurCol(CCollider* pCol);
	bool HasCurCol(CCollider* pCol);
	void DeletePrevCol(CCollider* pCol);
	void Call(CCollider* pDest, float fTime, COLLISION_STATE eStat);
	void CallMouse(const Vector2& v, float fTime, COLLISION_STATE eStat);
	void SetProFile(const std::string& strTag);
	PCollisionProfile GetProFile()	const;
	bool IsCheck()	const;
	void Check();
	void PrevColSecCheck(float fTime);
	bool IsColEnable()	const;
	bool CheckCurSec(class CCollider* pCol);
	void SetUI();
	bool IsMouse()	const;
	void SetMouse();
	void EnableCol(bool bCol);
#ifdef _DEBUG
	class CMesh* GetDebugMesh()	const;
#endif

public:
	virtual bool Init();
	virtual void Start();
	virtual void Update(float fTime);
	virtual void PostUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void PreRender(float fTime);
	virtual void Render(float fTime);
	virtual void PostRender(float fTime);
	virtual std::shared_ptr<CComponent> Clone() = 0;

public:
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);

public:
	virtual bool Collision(class CCollider* pDest) = 0;

public:
	template <typename T>
	void SetCallBack(COLLISION_STATE eState, T* pObj, void(T::* pFunc)(CCollider*, CCollider*, float))
	{
		m_vecCallBack[(int)eState].push_back(std::bind(pFunc, pObj, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
	}

	template <typename T>
	void SetMouseCallBack(COLLISION_STATE eState, T* pObj, void(T::* pFunc)(CCollider*, const Vector2&, float))
	{
		m_vecMouseCallBack[(int)eState].push_back(std::bind(pFunc, pObj, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
	}

	virtual bool CollisionMouse(const Vector2& vMousePos) = 0;

};
