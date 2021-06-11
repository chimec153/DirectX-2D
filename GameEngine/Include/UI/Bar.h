#pragma once
#include "UIControl.h"

enum class DIR
{
	L,
	T,
	R,
	B
};

class CBar :
	public CUIControl
{
	friend class CGameObject;

public:
	CBar();
	CBar(const CBar& ui);
	virtual ~CBar();

private:
	DIR			m_eDir;
	float		m_fPct;
	BARCBUFFER	m_tCBuffer;

public:
	void SetPrg(float f);
	void SetDir(DIR e);
	float GetPercent()	const;
	DIR GetDir()	const;

public:
	virtual bool Init();
	virtual void Start();
	virtual void Update(float fTime);
	virtual void PostUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void PreRender(float fTime);
	virtual void Render(float fTime);
	virtual void PostRender(float fTime);
	virtual std::shared_ptr<CComponent> Clone();

public:
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);

public:
	virtual void ShowWindow();
};

