#pragma once
#include "UIControl.h"

enum class TEXT_ALIGN_VERTICAL
{
	TOP,
	MID,
	BOT
};

enum class TEXT_ALIGN_HORIZONAL
{
	LEFT,
	CEN,
	RIGHT
};

class CUFont :
	public CUIControl
{
	friend class CGameObject;

public:
	CUFont();
	CUFont(const CUFont& ui);
	virtual ~CUFont();

private:
	bool					m_bAlpha;
	bool					m_bShadowAlpha;
	bool					m_bShadow;
	Vector3					m_vOffset;
	Vector4					m_vColor;
	Vector4					m_vShaderColor;
	ID2D1SolidColorBrush*	m_pColor;
	ID2D1SolidColorBrush*	m_pShaderColor;
	TCHAR*					m_pText;
	int						m_iMaxCount;
	float					m_fSize;
	TCHAR*					m_pFont;
	ID2D1RenderTarget*		m_p2DTarget;
	IDWriteTextFormat*		m_pFormat;
	IDWriteTextLayout*		m_pLayout;
	D2D1_RECT_F				m_tRect;
	TEXT_ALIGN_VERTICAL		m_eAlignV;
	TEXT_ALIGN_HORIZONAL	m_eAlignH;
	int						m_iWidth;
	int						m_iHeight;
	float					m_fOpacity;
	float					m_fShadowOpacity;
	UINT					m_iColor;
	UINT					m_iShadowColor;
	bool					m_b3D;

public:
	void SetText(const TCHAR* pText);
	void SetText(const char* pText);
	void AddText(const TCHAR* pText);
	void SetColor(float r, float g, float b, float a);
	void SetColor(BYTE r, BYTE g, BYTE b, BYTE a);
	void SetColor(UINT iColor);
	void SetShadowColor(float r, float g, float b, float a);
	void SetShadowColor(BYTE r, BYTE g, BYTE b, BYTE a);
	void SetShadowColor(UINT iColor);
	void CreateLayout();
	void SetFormat(const std::string& strKey);
	void SetFormat(const std::string& strKey, const TCHAR* pText, int iWidth, int iHeight, float fSize, const TCHAR* pLocal);
	void SetRect(float l, float t, float r, float b);
	void SetAlignH(TEXT_ALIGN_HORIZONAL eAlign);
	void SetAlignV(TEXT_ALIGN_VERTICAL eAlign);
	void SetOpacity(float fOp);
	void SetShadowOpacity(float fOp);
	UINT ConvertRGBA(float r, float g, float b, float a);
	void SetShadowOffset(const Vector3& vOffset);
	void SetAlpha(bool bAlpha);
	void SetShadowAlpha(bool bAlpha);
	void SetSize(float fSize);
	void Set3D();

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

