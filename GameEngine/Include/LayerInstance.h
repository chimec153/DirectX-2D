#pragma once

#include "GameEngine.h"

class CLayerInstance
{
	friend class CRenderManager;

public:
	CLayerInstance();
	~CLayerInstance();

private:
	class CMesh* m_pMesh;
	std::shared_ptr<class CMaterial> m_pMaterial;
	PInstancingBuffer	m_pInstancingBuffer;
	int					m_iCount;
	bool				m_bAnimation;
	std::shared_ptr<class CComponent> m_pFont;
	std::shared_ptr<class CGameObject> m_pFontObj;
	class CLayer*		m_pLayer;
	std::vector<class CSceneComponent*>	m_vecCom;

public:
	void SetLayer(class CLayer* pLayer);
	void AddCom(class CSceneComponent* pCom);
	const std::vector<class CSceneComponent*>* GetComponents()	const;
	size_t GetComSize()	const;

public:
	bool Init(class CMesh* pMesh, std::shared_ptr<class CMaterial> pMtrl, int iSize, int iCount = 1000);
	void AddInstancingData(const void* pData);
	void Render();
	void Clear();
	void RenderFont(int idx);
};

