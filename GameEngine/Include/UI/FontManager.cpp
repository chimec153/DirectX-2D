#include "FontManager.h"
#include "../Device.h"

DEFINITION_SINGLE(CFontManager)

CFontManager::CFontManager()	:
	m_pFactory(nullptr)
{

}

CFontManager::~CFontManager()
{
	m_pFactory->Release();

	SAFE_RELEASE_MAP(m_mapBrush);
	SAFE_RELEASE_MAP(m_mapFormat);
}

bool CFontManager::Init()
{
	if (FAILED(DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), (IUnknown * *)& m_pFactory)))
		return false;

	CreateBrush(Vector4::White);

	CreateFormat("Normal", TEXT("±Ã¼­"), 200, 5, 20, TEXT("ko"));

	return true;
}

bool CFontManager::CreateBrush(float r, float g, float b, float a)
{
	UINT iKey = CreateColorKey(r, g, b, a);

	ID2D1SolidColorBrush* pBrush = FindBrush(r,g,b,a);

	if (pBrush)
		return false;

	if (FAILED(TARGET2D->CreateSolidColorBrush(D2D1_COLOR_F(D2D1::ColorF(r, g, b, a)), &pBrush)))
		return false;

	m_mapBrush.insert(std::make_pair(iKey, pBrush));

	return true;
}

bool CFontManager::CreateBrush(UINT iColor)
{
	float r, g, b, a;

	b = (iColor & 0x000000ff) / 255.f;
	g = ((iColor >> 8) & 0x000000ff) / 255.f;
	r = ((iColor >> 16) & 0x000000ff) / 255.f;
	a = ((iColor >> 24) & 0x000000ff) / 255.f;

	return CreateBrush(r, g, b, a);
}

bool CFontManager::CreateBrush(const Vector4& v)
{
	return 	CreateBrush(v.x, v.y, v.z, v.w);
}

UINT CFontManager::CreateColorKey(float r, float g, float b, float a)
{
	UINT iColor = 0;
	BYTE R, G, B, A;

	R = BYTE(r * 255);
	G = BYTE(g * 255);
	B = BYTE(b * 255);
	A = BYTE(a * 255);

	iColor = A;

	iColor = (iColor << 8) | R;
	iColor = (iColor << 8) | G;
	iColor = (iColor << 8) | B;

	return iColor;
}

bool CFontManager::CreateFormat(const std::string& strKey, const TCHAR* pFont, int iWeight, int iStretch, float fSize, const TCHAR* pLocal)
{
	IDWriteTextFormat* pFormat = FindFormat(strKey);

	if (pFormat)
		return false;

	pFormat = nullptr;

	if (FAILED(m_pFactory->CreateTextFormat(pFont, nullptr, (DWRITE_FONT_WEIGHT)iWeight, DWRITE_FONT_STYLE_NORMAL, (DWRITE_FONT_STRETCH)iStretch, fSize, pLocal, &pFormat)))
		return false;

	m_mapFormat.insert(std::make_pair(strKey, pFormat));

	return true;
}

IDWriteTextLayout* CFontManager::CreateLayout(const TCHAR* pText, IDWriteTextFormat* pFmt, int iWidth, int iHeight)
{
	IDWriteTextLayout* pLayout = nullptr;

	if (FAILED(m_pFactory->CreateTextLayout(pText, (int)lstrlen(pText), pFmt, (float)iWidth, (float)iHeight, &pLayout)))
		return nullptr;

	return pLayout;
}

IDWriteTextFormat* CFontManager::FindFormat(const std::string& strKey)
{
	std::unordered_map<std::string, IDWriteTextFormat*>::iterator iter = m_mapFormat.find(strKey);

	if (iter == m_mapFormat.end())
		return nullptr;

	return iter->second;
}

ID2D1SolidColorBrush* CFontManager::FindBrush(float r, float g, float b, float a)
{
	UINT iKey = CreateColorKey(r, g, b, a);

	std::unordered_map<UINT, ID2D1SolidColorBrush*>::iterator iter = m_mapBrush.find(iKey);

	if (iter == m_mapBrush.end())
		return nullptr;

	return iter->second;
}
