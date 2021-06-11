#pragma once

#include "Engine.h"

typedef struct _tagFlare
{
	float	fOffset;
	float	fScale;
	Vector4	vColor;
	_tagFlare() :
		fOffset(0.f)
		,fScale(0.f)
		, vColor()
	{

	}
}FLARE, *PFLARE;

typedef struct _tagFlareCBuffer
{
	Vector4 vPos;
	Vector4	vScaleRotation;
	Vector4	vColor;
	_tagFlareCBuffer()	:
		vPos()
		, vScaleRotation()
		, vColor()
	{

	}
}FLARECBUFFER, *PFLARECBUFFER;

class CLensFlares
{
private:
	std::vector<FLARECBUFFER>	m_vecCBuffers;
	std::vector<FLARE>			m_vecFlares;
	std::shared_ptr<class CShader> m_pLensShader;
	ID3D11Buffer*	m_pBuffer;
	std::shared_ptr<class CTexture>	m_pFlareTexture;
	std::shared_ptr<class CTexture>	m_pCoronaTexture;
	std::shared_ptr<class CTexture>	m_pSunTexture;
	bool			m_bInScreen;
	class CRenderState* m_pAlphaBlend;
	class CRenderState* m_pNoDepth;
	bool			m_bStart;
	Vector3			m_vSunPos;

public:
	void SetSunPos(const Vector3& vPos);

public:
	bool Init();
	void Start();
	void Update(float fTime);
	void Render(float fTime);
	void ShowWindow();

	DECLARE_SINGLE(CLensFlares)
};

