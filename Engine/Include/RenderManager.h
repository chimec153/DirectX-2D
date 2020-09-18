#pragma once

#include "GameEngine.h"

class CRenderManager
{
private:
	std::vector<class CSceneComponent*>		m_vecComponent;
	std::vector<class CSceneComponent*>		m_vec2DComponent;
	std::vector<class CSceneComponent*>		m_vecUIComponent;

public:
	bool Init();
	void Render(float fTime);
	void Clear();
	bool AddComponent(class CSceneComponent* pComponent);

private:
	void Render2D(float fTime);
	void Render3D(float fTime);
	void RenderUI(float fTime);

	DECLARE_SINGLE(CRenderManager)
};

