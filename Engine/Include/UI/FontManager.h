#pragma once

#include "../GameEngine.h"

class CFontManager
{
private:
	std::unordered_map<UINT, ID2D1SolidColorBrush*>		m_mapBrush;
	std::unordered_map<std::string, IDWriteTextFormat*>	m_mapFormat;
	IDWriteFactory*										m_pFactory;

public:
	bool Init();
	bool CreateBrush(float r, float g, float b, float a);
	bool CreateBrush(UINT iColor);
	bool CreateBrush(const Vector4& v);
	UINT CreateColorKey(float r, float g, float b, float a);
	bool CreateFormat(const std::string& strKey, const TCHAR* pFont, int iWidth, int iHeight, float fSize, const TCHAR* pLocal);
	IDWriteTextLayout* CreateLayout(const TCHAR* pText, IDWriteTextFormat* pFmt, int iWidth, int iHeight);

	IDWriteTextFormat* FindFormat(const std::string& strKey);
	ID2D1SolidColorBrush* FindBrush(float r, float g, float b, float a);

	DECLARE_SINGLE(CFontManager)
};

