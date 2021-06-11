#pragma once
#include "ObjectComponent.h"
class CPaperBurn :
	public CObjectComponent
{
	friend class CGameObject;

public:
	CPaperBurn();
	CPaperBurn(const CPaperBurn& com);
	virtual ~CPaperBurn();

private:
	PaperBurnCBuffer	m_tCBuffer;
	std::shared_ptr<class CTexture> m_pPaperTex;
	bool			m_bBurn;
	float			m_fScale;
	std::vector<std::function<void(float)>>	m_vecEndCallBack;
	bool			m_bLoop;

public:
	void AddEndCallBack(void(*pFunc)(float));
	template <typename T>
	void AddEndCallBack(T* pObj, void(T::* pFunc)(float))
	{
		m_vecEndCallBack.push_back(std::bind(pFunc, pObj, std::placeholders::_1));
	}
	void SetLoop(bool bLoop);
	void SetBurnTexture(const std::string& strKey);
	void SetFilter(float fFilter);

public:
	void ToggleBurn();
	void SetScale(float fScale);
	void SetOutFilter(float fFilter);
	void SetMidFilter(float fFilter);
	void SetInFilter(float fFilter);
	void SetOutColor(const Vector3& vColor);
	void SetColor(const Vector3& vColor);
	void SetInColor(const Vector3& vColor);
	void SetOutColor(float r, float g,float b);
	void SetColor(float r, float g,float b);
	void SetInColor(float r, float g,float b);

public:
	virtual bool Init();
	virtual void Start();
	virtual void Update(float fTime);
	virtual void PostUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void PreRender(float fTime);
	virtual void Render(float fTime);
	virtual void PostRender(float fTime);
	virtual std::shared_ptr<class CComponent> Clone();

public:
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);

public:
	virtual void ShowWindow();
};

