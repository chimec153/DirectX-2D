#pragma once

#include "Engine.h"

class CLightManager
{
	DECLARE_SINGLE(CLightManager)

private:
	static std::unordered_map<std::string, class CLight*>	m_mapLight;
	static class CLight* m_pMainLight;

public:
	void SetShader();

public:
	bool Init();
	void AddLight(class CLight*);
	static void DeleteLight(class CLight*);
	class CLight* FindLight(const std::string& strKey)	const;
	class CLight* GetMainLight()	const;
	void SetMainLight(class CLight* pLight);
	void SetMainLight(const std::string& strKey);

public:
	void Render(float fTime);

public:
	void ShowWindow();

};

