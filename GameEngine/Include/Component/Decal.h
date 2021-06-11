#pragma once
#include "SceneComponent.h"
class CDecal :
	public CSceneComponent
{
public:
	CDecal();
	CDecal(const CDecal& decal);
	virtual ~CDecal() override;

private:
	DECALCBUFFER	m_tCBuffer;
	float			m_fFadeInTime;
	float			m_fFadeIn;
	bool			m_bFadeInComplete;
	float			m_fFadeOutTime;
	float			m_fFadeOut;
	bool			m_bFadeOutComplete;
	float			m_fDurationTime;
	float			m_fDuration;
	bool			m_bDurationComplete;
	float			m_fAlpha;
	float			m_fRadius;
	bool			m_bRadiusMode;

public:
	void SetAlpha(float fAlpha);
	void SetFadeIn(float fTime);
	void SetFadeOut(float fTime);
	void SetDuration(float fTime);
	void SetRadius(float fRadius);
	void SetRadiusMode(bool bMode);

public:
	virtual bool Init() override;
	virtual void Start() override;
	virtual void Update(float fTime) override;
	virtual void PostUpdate(float fTime) override;
	virtual void Collision(float fTime) override;
	virtual void PreRender(float fTime) override;
	virtual void Render(float fTime) override;
	virtual void PostRender(float fTime) override;
	virtual std::shared_ptr<CComponent> Clone() override;

public:
	virtual void Save(FILE* pFile) override;
	virtual void Load(FILE* pFile) override;

public:
	virtual void ShowWindow();
};

