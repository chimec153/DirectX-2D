#pragma once
#include "SceneComponent.h"
class CLight :
	public CSceneComponent
{
	friend class CLightManager;

public:
	CLight();
	CLight(const CLight& light);
	virtual ~CLight();

private:
	LIGHTCBUFFER	m_tCBuffer;
	Matrix			m_matView[6];
	Matrix			m_matProj;
	Matrix			m_matVP[6];

public:
	const Matrix& GetView(int iIdx = 0)	const;
	const Matrix& GetProj()	const;
	const Matrix& GetVP(int iIdx = 0)	const;
	void SetFrustum(int iWidth, int iHeight, float fNear, float fFar);

public:
	void SetDifColor(const Vector4& vDif);
	void SetDifColor(float r, float g, float b, float a);
	void SetDifColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
	void SetAmbColor(const Vector4& vDif);
	void SetAmbColor(float r, float g, float b, float a);
	void SetAmbColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
	void SetSpcColor(const Vector4& vDif);
	void SetSpcColor(float r, float g, float b, float a);
	void SetSpcColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
	void SetEmiColor(const Vector4& vDif);
	void SetEmiColor(float r, float g, float b, float a);
	void SetEmiColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
	void SetRange(float fRange);
	void SetInAngle(float fAngle);
	void SetOutAngle(float fAngle);
	void SetAttn(float c,float a, float b);
	void SetLightType(LIGHT_TYPE eType);

public:
	void SetShader();

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
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);

public:
	virtual void ShowWindow();
};

