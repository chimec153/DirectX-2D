#pragma once

#include "GameEngine.h"

struct BoundingSphere
{
	BoundingSphere() :
		Center(0.0f, 0.0f, 0.0f)
		, Radius(0.0f)
	{
	}
	Vector3 Center;
	float Radius;
};

class CShadowMap
{
public:
	CShadowMap(ID3D11Device* pDevice, UINT iWidth, UINT iHeight);
	~CShadowMap();

private:
	static CShadowMap* m_pMainShadow;

	ID3D11ShaderResourceView* GetDepthMapSRV();

	void BindDsvAndSetNullRenderTarget(ID3D11DeviceContext* pDC);

private:
	CShadowMap(const CShadowMap& rhs);
	CShadowMap& operator = (const CShadowMap& rhs);

private:
	UINT	m_iWidth;
	UINT	m_iHeight;

	ID3D11ShaderResourceView*	m_pDepthMapSRV;
	ID3D11DepthStencilView*		m_pDepthMapDSV;

	D3D11_VIEWPORT	m_tViewPort;

	BoundingSphere m_tSceneBounds;

	Matrix m_matLightView;
	Matrix m_matLightProj;
	Matrix m_matShadowTransform;

	ID3D11RenderTargetView* m_pRenderTargetView;

public:
	void SetMainShadow(CShadowMap* pShadow);
	void SetSceneBounds(const BoundingSphere& bounds);
	const BoundingSphere& GetSceneBounds()	const;

public:
	static bool Init();
	static void Update(float fTime);
	static void Render(float fTime);

private:
	static void BuildShadowTransform();
	static void BindDsvAndShdoawMap();
	static void DrawSceneToShadowMap();
};

