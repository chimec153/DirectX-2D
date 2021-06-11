#pragma once

#include "../GameEngine.h"

typedef struct _tagMatrices
{
	Matrix	matWVP;
	Matrix	matWV;
	Matrix	matShadowWVP;
	Matrix	matWorld;
}MATRICES, *PMATRICES;

#ifdef _DEBUG
typedef struct _tagDebugMatrix
{
	Matrix	matWVP;
	Matrix	matWV;
	Matrix	matShadowWVP;
	Vector4 vColor;
	Matrix	matWorld;
}DEBUGMATRIX, *PDEBUGMATRIX;
#endif

class CRenderInstancing
{
public:
	CRenderInstancing();
	~CRenderInstancing();

private:
	std::string							m_strName;
	std::list<class CSceneComponent*>	m_RenderList;
	int									m_iCount;
	class CMesh*		m_pMesh;
	PInstancingBuffer					m_pInstanceBuffer;
	std::shared_ptr<class CShader>		m_pInstanceShader;
	std::shared_ptr<class CShader>		m_pInstanceShadow;
	PSTRUCTUREDBUFFER					m_pBoneBuffer;
	int									m_iBoneInstancingCount;
	int									m_iBoneCount;
#ifdef _DEBUG
	class CRenderState*					m_pDepth;
	class CRenderState*					m_pRasterizer;
#endif

public:
	int GetCount()	const;
	void AddComponent(class CSceneComponent* pCom);
	void DeleteComponent(class CSceneComponent* pCom);
	class CMesh* GetMesh()	const;
	void SetInstancingCount(int iCount);
	int GetInstancingCount()	const;
	PSTRUCTUREDBUFFER GetBoneBuffer()	const;
	bool CreateBoneBuffer();
	void SetShader(const std::string& strKey);
#ifdef _DEBUG
	void SetDepthState(class CRenderState* pState);
	void SetRasterizer(class CRenderState* pState);
#endif

public:
	bool CreateInstBuffer(int iCount, int iSize);

public:
	bool Init(class CMesh* pMesh, int iBoneCount);
	void Render(float fTime);
	void RenderShadow(float fTime);
};

