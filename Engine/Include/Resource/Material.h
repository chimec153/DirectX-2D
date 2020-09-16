#pragma once

#include "../Ref.h"

class CMaterial :
	public CRef
{
	friend class CResourceManager;

private:
	CMaterial();
	CMaterial(const CMaterial& material);
	~CMaterial();

private:
	class CShader*		m_pShader;
	ShaderCBuffer		m_tCBuffer;

public:
	void SetShader(const std::string& strName);

public:
	bool Init();
	void Start();
	void Update(float fTime);
	void PostUpdate(float fTime);
	void Collision(float fTime);
	void PreRender(float fTime);
	void Render(float fTime);
	void PostRender(float fTime);
};

