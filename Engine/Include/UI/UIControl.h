#pragma once

#include "../Component/SceneComponent.h"
#include "../Resource/Texture.h"

class CUIControl :
	public CSceneComponent
{
	friend class CGameObject;

protected:
	CUIControl();
	CUIControl(const CUIControl& ui);
	virtual ~CUIControl();

protected:
	std::shared_ptr<CTexture>	m_pTexture;
	AniCBuffer			m_tCBuffer;

public:
	bool SetTexture(std::shared_ptr<CTexture> pTex);
	bool SetTexture(const std::string& strTag);
	void SetUV(const Vector2& vStart, const Vector2& vEnd);
	void SetSize(const Vector2& vSize);

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
	virtual void ShowWindow() override;
};

