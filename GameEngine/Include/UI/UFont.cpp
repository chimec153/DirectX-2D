#include "UFont.h"
#include "FontManager.h"
#include "../Device.h"
#include "../Component/Camera.h"
#include "../CameraManager.h"

CUFont::CUFont() :
	m_bAlpha(false),
	m_bShadowAlpha(false),
	m_vOffset(),
	m_iMaxCount(256),
	m_fSize(20.f),
	m_p2DTarget(nullptr),
	m_pLayout(nullptr),
	m_tRect(),
	m_eAlignV(TEXT_ALIGN_VERTICAL::MID),
	m_eAlignH(TEXT_ALIGN_HORIZONAL::CEN),
	m_iWidth(100),
	m_iHeight(100),
	m_fOpacity(1.f),
	m_fShadowOpacity(1.f),
	m_pFormat(nullptr),
	m_b3D(false)
{
	m_pText = new TCHAR[m_iMaxCount];
	m_pFont = new TCHAR[64];

	memset(m_pText, 0, sizeof(TCHAR) * m_iMaxCount);
	memset(m_pFont, 0, sizeof(TCHAR) * 64);

	lstrcpy(m_pText, TEXT("Text"));
	lstrcpy(m_pFont, TEXT("±Ã¼­Ã¼"));

	SetColor(1.f, 1.f, 1.f, 1.f);
	SetShadowColor(0.5f, 0.5f, 0.5f, 0.5f);

	m_pFormat = GET_SINGLE(CFontManager)->FindFormat("Normal");

	CreateLayout();

	m_iSceneComponentClassType = (int)SCENECOMPONENT_CLASS_TYPE::UI_FONT;
	m_eSceneComponentType = SCENE_COMPONENT_TYPE::SCT_UI;
}

CUFont::CUFont(const CUFont& ui)	:
	CUIControl(ui)
	, m_b3D(ui.m_b3D)
{
	m_bAlpha = ui.m_bAlpha;
	m_bShadowAlpha = ui.m_bShadowAlpha;
	m_bShadow = ui.m_bShadow;
	m_vOffset = ui.m_vOffset;
	m_vColor = ui.m_vColor;
	m_vShaderColor = ui.m_vShaderColor;
	m_pColor = ui.m_pColor;
	m_pShaderColor = ui.m_pShaderColor;
	m_iMaxCount = ui.m_iMaxCount;
	m_fSize = ui.m_fSize;
	m_p2DTarget = ui.m_p2DTarget;

	m_pText = new TCHAR[m_iMaxCount];

	memset(m_pText, 0, sizeof(TCHAR) * m_iMaxCount);

	lstrcpy(m_pText, ui.m_pText);

	m_pFont = new TCHAR[64];

	memset(m_pFont, 0, sizeof(TCHAR) * 64);

	lstrcpy(m_pFont, ui.m_pFont);

	m_pFormat = ui.m_pFormat;
	m_tRect = ui.m_tRect;
	m_eAlignV = ui.m_eAlignV;
	m_eAlignH = ui.m_eAlignH;
	m_iWidth = ui.m_iWidth;
	m_iHeight = ui.m_iHeight;
	m_fOpacity = ui.m_fOpacity;
	m_fShadowOpacity = ui.m_fShadowOpacity;
	m_iColor = ui.m_iColor;
	m_iShadowColor = ui.m_iShadowColor;

	m_pLayout = nullptr;

	CreateLayout();
}

CUFont::~CUFont()
{
	SAFE_DELETE_ARRAY(m_pText);
	SAFE_DELETE_ARRAY(m_pFont);
}

bool CUFont::Init()
{
	if (!CUIControl::Init())
		return false;

	m_p2DTarget = TARGET2D;
	m_pFormat = GET_SINGLE(CFontManager)->FindFormat("Normal");

	return true;
}

void CUFont::Start()
{
	CUIControl::Start();
}

void CUFont::Update(float fTime)
{
	CUIControl::Update(fTime);
}

void CUFont::PostUpdate(float fTime)
{
	CUIControl::PostUpdate(fTime);
}

void CUFont::Collision(float fTime)
{
	CUIControl::Collision(fTime);
}

void CUFont::PreRender(float fTime)
{
	CUIControl::PreRender(fTime);
}

void CUFont::Render(float fTime)
{
	m_p2DTarget->BeginDraw();

	Vector3	vPos = GetWorldPos();

	Resolution tRS = RESOLUTION;

	CCamera* pCam = GET_SINGLE(CCameraManager)->GetMainCam();

	if (pCam)
	{
		if (!m_b3D)
		{
			vPos += Vector3(m_tRect.left, 0.f, 0.f);
			 //vPos -= pCam->GetWorldPos() - pCam->GetPivot() * Vector3((float)tRS.iWidth, (float)tRS.iHeight, 0.f);
			vPos += Vector3((float)tRS.iWidth/2.f, (float)tRS.iHeight / 2.f, 0.f);
		}
		else
		{
			Matrix matVP = pCam->GetVP();

			Vector3 vProjPos = {};

			vProjPos = vPos.TranslationCoor(matVP);

			vProjPos.x += 1.f;
			vProjPos.y += 1.f;

			vProjPos.x *= static_cast<float>(tRS.iWidth)/2.f;
			vProjPos.y *= static_cast<float>(tRS.iHeight) / 2.f;

			//SetSize(20.f / vProjPos.z);

			memcpy_s(&vPos, 12, &vProjPos, 12);

			vPos += Vector3(m_tRect.left, 0.f, 0.f);
		}
	}

	float fHeight = m_tRect.bottom - m_tRect.top;

	float fRH = (float)tRS.iHeight;

	if (m_bShadow)
	{
		Vector3 vShadowPos = vPos + m_vOffset;

		vShadowPos.y = fRH - vShadowPos.y - fHeight;

		if (m_bShadowAlpha)
			m_pShaderColor->SetOpacity(m_fShadowOpacity);

		else
			m_pShaderColor->SetOpacity(1.f);

		m_pShaderColor->SetColor(D2D1_COLOR_F(D2D1::ColorF(m_iShadowColor)));

		m_p2DTarget->DrawTextLayout(D2D1_POINT_2F(D2D1::Point2F(vShadowPos.x, vShadowPos.y)), m_pLayout, m_pShaderColor, D2D1_DRAW_TEXT_OPTIONS_NONE);
	}

	vPos.y = fRH - vPos.y - fHeight;

	if (m_bAlpha)
		m_pColor->SetOpacity(m_fOpacity);

	else
		m_pColor->SetOpacity(1.f);

	m_pColor->SetColor(D2D1_COLOR_F(D2D1::ColorF(m_iColor)));

	m_p2DTarget->DrawTextLayout(D2D1_POINT_2F(D2D1::Point2F(vPos.x, vPos.y)), m_pLayout, m_pColor, D2D1_DRAW_TEXT_OPTIONS_NONE);

	m_p2DTarget->EndDraw();
}

void CUFont::PostRender(float fTime)
{
	CUIControl::PostRender(fTime);
}

std::shared_ptr<CComponent> CUFont::Clone()
{
	return std::shared_ptr<CComponent>(new CUFont(*this));
}

void CUFont::Save(FILE* pFile)
{
	CUIControl::Save(pFile);

	fwrite(&m_bAlpha, 1, 1, pFile);
	fwrite(&m_bShadowAlpha, 1, 1, pFile);
	fwrite(&m_bShadow, 1, 1, pFile);
	fwrite(&m_vOffset, sizeof(Vector3), 1, pFile);
	fwrite(&m_vColor, sizeof(Vector4), 1, pFile);
	fwrite(&m_vShaderColor, sizeof(Vector4), 1, pFile);
	fwrite(&m_iMaxCount, 4, 1, pFile);
	fwrite(m_pText, sizeof(TCHAR), m_iMaxCount, pFile);
	fwrite(&m_fSize, 4, 1, pFile);
	fwrite(m_pFont, sizeof(TCHAR), 64, pFile);
	fwrite(&m_tRect, sizeof(D2D1_RECT_F), 1, pFile);
	fwrite(&m_eAlignV, 4, 1, pFile);
	fwrite(&m_eAlignH, 4, 1, pFile);
	fwrite(&m_iWidth, 4, 1, pFile);
	fwrite(&m_iHeight, 4, 1, pFile);
	fwrite(&m_fOpacity, 4, 1, pFile);
	fwrite(&m_fShadowOpacity, 4, 1, pFile);
	fwrite(&m_iColor, 4, 1, pFile);
	fwrite(&m_iShadowColor, 4, 1, pFile);
}

void CUFont::Load(FILE* pFile)
{
	CUIControl::Load(pFile);

	SAFE_DELETE_ARRAY(m_pText);

	m_p2DTarget = TARGET2D;

	fread(&m_bAlpha, 1, 1, pFile);
	fread(&m_bShadowAlpha, 1, 1, pFile);
	fread(&m_bShadow, 1, 1, pFile);
	fread(&m_vOffset, sizeof(Vector3), 1, pFile);
	fread(&m_vColor, sizeof(Vector4), 1, pFile);
	fread(&m_vShaderColor, sizeof(Vector4), 1, pFile);
	fread(&m_iMaxCount, 4, 1, pFile);

	m_pText = new TCHAR[m_iMaxCount];
	memset(m_pText, 0, sizeof(TCHAR) * m_iMaxCount);

	fread(m_pText, sizeof(TCHAR), m_iMaxCount, pFile);
	fread(&m_fSize, 4, 1, pFile);

	memset(m_pFont, 0, sizeof(TCHAR) * 64);

	fread(m_pFont, sizeof(TCHAR), 64, pFile);
	fread(&m_tRect, sizeof(D2D1_RECT_F), 1, pFile);
	fread(&m_eAlignV, 4, 1, pFile);
	fread(&m_eAlignH, 4, 1, pFile);
	fread(&m_iWidth, 4, 1, pFile);
	fread(&m_iHeight, 4, 1, pFile);
	fread(&m_fOpacity, 4, 1, pFile);
	fread(&m_fShadowOpacity, 4, 1, pFile);
	fread(&m_iColor, 4, 1, pFile);
	fread(&m_iShadowColor, 4, 1, pFile);

	SetColor(m_iColor);
	SetShadowAlpha(m_iShadowColor);

	m_pFormat = GET_SINGLE(CFontManager)->FindFormat("Normal");

	CreateLayout();
}

void CUFont::ShowWindow()
{
	CUIControl::ShowWindow();

	std::string strName = GetName();

	if (strName == "")
	{
		strName = "None";
	}

	if (ImGui::Begin(strName.c_str()))
	{
		ImGui::Text("UFont");
		ImGui::Checkbox("Alpha", &m_bAlpha);
		ImGui::Checkbox("Shadow", &m_bShadow);
		ImGui::Checkbox("ShadowAlpha", &m_bShadowAlpha);
		ImGui::InputFloat3("Offset", &m_vOffset.x);
		char strText[MAX_PATH] = {};
		static char strInput[MAX_PATH] = {};
#ifdef UNICODE
		WideCharToMultiByte(CP_ACP, 0, m_pText, -1, strText, lstrlen(m_pText), NULL, NULL);
#else
		strcpy_s(strText, MAX_PATH, m_pText);
#endif
		ImGui::InputText(strText, strInput,MAX_PATH);
		if (ImGui::Button("Change Text"))
		{
			TCHAR pInput[MAX_PATH] = {};
#ifdef UNICODE
			MultiByteToWideChar(CP_ACP, 0, strInput, -1, pInput, static_cast<int>(strlen(strInput)));
#else
			strcpy_s(pInput, MAX_PATH, strInput);
#endif
			SetText(pInput);
		}

		ImGui::Text("MaxCount %d", m_iMaxCount);
		if (ImGui::InputFloat("Size", &m_fSize))
		{
			SetSize(m_fSize);
		}
		
		char strFont[MAX_PATH] = {};
		static char strInputFont[MAX_PATH] = {};
#ifdef UNICODE
		WideCharToMultiByte(CP_ACP, 0, m_pFont, -1, strFont, lstrlen(m_pFont), NULL, NULL);
#else
		strcpy_s(strFont, MAX_PATH, m_pFont);
#endif

		ImGui::Text("Font: %s", strFont);
		ImGui::InputText(strFont, strInputFont, MAX_PATH);
		if (ImGui::Button("Change Font"))
		{
#ifdef UNICODE
			MultiByteToWideChar(CP_ACP, 0, strInputFont, -1, m_pFont,static_cast<int>( strlen(strInputFont)));
#else
			strcpy_s(m_pFont, MAX_PATH, strInputFont);
#endif
		}

		static char strFormat[MAX_PATH] = {};

		ImGui::InputText("Format: ", strFormat,MAX_PATH);
		if (ImGui::Button("Change Format"))
		{
			SetFormat(strFormat, m_pText, m_iWidth, m_iHeight, m_fSize, L"ko");
		}

		ImGui::InputFloat4("Rect left, top, right, bottom", &m_tRect.left);

		if (ImGui::Button("Set Rect"))
		{
			SetRect(m_tRect.left, m_tRect.top, m_tRect.right, m_tRect.bottom);
		}

		if (ImGui::SliderInt("Align Vertical", reinterpret_cast<int*>(&m_eAlignV),
			static_cast<int>(TEXT_ALIGN_VERTICAL::TOP), static_cast<int>(TEXT_ALIGN_VERTICAL::BOT)))
		{
			SetAlignV(m_eAlignV);
		}

		if (ImGui::SliderInt("Align Horizontal", reinterpret_cast<int*>(&m_eAlignH),
			static_cast<int>(TEXT_ALIGN_HORIZONAL::LEFT), static_cast<int>(TEXT_ALIGN_HORIZONAL::RIGHT)))
		{
			SetAlignH(m_eAlignH);
		}

		ImGui::InputInt("Width", &m_iWidth);
		ImGui::InputInt("Height", &m_iHeight);
		if (ImGui::InputFloat("Opacity", &m_fOpacity))
		{
			SetOpacity(m_fOpacity);
		}
		if (ImGui::InputFloat("Shadow Opacity", &m_fShadowOpacity))
		{
			SetShadowOpacity(m_fShadowOpacity);
		}
		ImGui::Checkbox("3D Mode", &m_b3D);
		if (ImGui::ColorPicker4("Color", &m_vColor.x))
		{
			SetColor(m_vColor.x, m_vColor.y, m_vColor.z, m_vColor.w);
		}
		if(ImGui::ColorPicker4("ShadowColor", &m_vShaderColor.x))
		{
			SetShadowColor(m_vShaderColor.x, m_vShaderColor.y, m_vShaderColor.z, m_vShaderColor.w);
		}

	}
	ImGui::End();
}

void CUFont::SetText(const TCHAR* pText)
{
	int iLength = lstrlen(pText);

	if (m_iMaxCount < iLength)
	{
		size_t iTotalLength = iLength + 1;
		TCHAR* pNT = new TCHAR[iTotalLength];

		memset(pNT, 0, sizeof(TCHAR) * iTotalLength);

		lstrcpy(pNT, m_pText);

		SAFE_DELETE_ARRAY(m_pText);

		m_pText = pNT;
	}

	lstrcpy(m_pText, pText);

	CreateLayout();
}

void CUFont::SetText(const char* pText)
{
	TCHAR strText[MAX_PATH] = {};

#ifdef UNICODE
	MultiByteToWideChar(CP_ACP, 0, pText, -1, strText, static_cast<int>(strlen(pText)));
#else
	strcpy_s(strText, MAX_PATH, pText);
#endif

	SetText(strText);
}

void CUFont::AddText(const TCHAR* pText)
{
	int iLength = lstrlen(pText);

	int iCur = lstrlen(m_pText);

	if (m_iMaxCount < iLength + iCur)
	{
		m_iMaxCount *= 2;

		TCHAR* pNT = new TCHAR[m_iMaxCount];

		memset(pNT, 0, sizeof(TCHAR) * m_iMaxCount);

		lstrcpy(pNT, m_pText);

		SAFE_DELETE_ARRAY(m_pText);

		m_pText = pNT;
	}

	lstrcat(m_pText, pText);

	CreateLayout();
}

void CUFont::SetColor(float r, float g, float b, float a)
{
	m_iColor = ConvertRGBA(r,g,b,a);

	m_vColor = Vector4(r, g, b, a);

	GET_SINGLE(CFontManager)->CreateBrush(r, g, b, a);

	m_pColor = GET_SINGLE(CFontManager)->FindBrush(r, g, b, a);
}

void CUFont::SetColor(BYTE r, BYTE g, BYTE b, BYTE a)
{
	float fR = r / 255.f;
	float fG = g / 255.f;
	float fB = b / 255.f;
	float fA = a / 255.f;

	SetColor(fR, fG, fB, fA);
}

void CUFont::SetColor(UINT iColor)
{
	float r, g, b, a;

	r = (iColor & 0x000000ff) / 255.f;
	g = ((iColor >> 8) & 0x000000ff) / 255.f;
	b = ((iColor >> 16) & 0x000000ff) / 255.f;
	a = ((iColor >> 24) & 0x000000ff) / 255.f;

	SetColor(r, g, b, a);
}

void CUFont::SetShadowColor(float r, float g, float b, float a)
{
	m_bShadow = true;

	m_iShadowColor = ConvertRGBA(r, g, b, a);

	m_vShaderColor = Vector4(r, g, b, a);

	GET_SINGLE(CFontManager)->CreateBrush(r, g, b, a);

	m_pShaderColor = GET_SINGLE(CFontManager)->FindBrush(r, g, b, a);
}

void CUFont::SetShadowColor(BYTE r, BYTE g, BYTE b, BYTE a)
{
	float fR = r / 255.f;
	float fG = g / 255.f;
	float fB = b / 255.f;
	float fA = a / 255.f;

	SetShadowColor(fR, fG, fB, fA);
}

void CUFont::SetShadowColor(UINT iColor)
{
	float r, g, b, a;

	r = (iColor & 0x000000ff) / 255.f;
	g = ((iColor >> 8) & 0x000000ff) / 255.f;
	b = ((iColor >> 16) & 0x000000ff) / 255.f;
	a = ((iColor >> 24) & 0x000000ff) / 255.f;

	SetShadowColor(r, g, b, a);
}

void CUFont::CreateLayout()
{
	if (!m_pFormat)
		return;

	SAFE_RELEASE(m_pLayout);

	m_pLayout = GET_SINGLE(CFontManager)->CreateLayout(m_pText, m_pFormat, m_iWidth, m_iHeight);

	DWRITE_TEXT_RANGE tRange = {};

	tRange.startPosition = 0;
	tRange.length = lstrlen(m_pText);

	if(m_pLayout)
	m_pLayout->SetFontSize(m_fSize, tRange);
	SetAlignH(m_eAlignH);
	SetAlignV(m_eAlignV);
}

void CUFont::SetFormat(const std::string& strKey)
{
	m_pFormat = GET_SINGLE(CFontManager)->FindFormat(strKey);
}

void CUFont::SetFormat(const std::string& strKey, const TCHAR* pText, int iWidth, int iHeight, float fSize, const TCHAR* pLocal)
{
	m_iHeight = iHeight;
	m_iWidth = iWidth;

	GET_SINGLE(CFontManager)->CreateFormat(strKey, m_pFont, iWidth, iHeight, fSize, pLocal);

	SetFormat(strKey);

	CreateLayout();
}

void CUFont::SetRect(float l, float t, float r, float b)
{
	float fDelta = t - b;

	if (fDelta <0.f)
		return;

	float fHeight = (float)RESOLUTION.iHeight;

	m_tRect = D2D1::RectF(l, fHeight - t, r, fHeight - b);

	m_iHeight = static_cast<int>(fDelta);

	m_iWidth = (int)(r - l);

	CreateLayout();
}

void CUFont::SetAlignH(TEXT_ALIGN_HORIZONAL eAlign)
{
	m_eAlignH = eAlign;

	if (m_pLayout)
	{
		switch (m_eAlignH)
		{
		case TEXT_ALIGN_HORIZONAL::LEFT:
			m_pLayout->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
			break;
		case TEXT_ALIGN_HORIZONAL::CEN:
			m_pLayout->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
			break;
		case TEXT_ALIGN_HORIZONAL::RIGHT:
			m_pLayout->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_TRAILING);
			break;
		}
	}
}

void CUFont::SetAlignV(TEXT_ALIGN_VERTICAL eAlign)
{
	m_eAlignV = eAlign;
	if (m_pLayout)
	{
		switch (m_eAlignV)
		{
		case TEXT_ALIGN_VERTICAL::TOP:
			m_pLayout->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
			break;
		case TEXT_ALIGN_VERTICAL::MID:
			m_pLayout->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
			break;
		case TEXT_ALIGN_VERTICAL::BOT:
			m_pLayout->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_FAR);
			break;
		}
	}
}

void CUFont::SetOpacity(float fOp)
{
	m_fOpacity = fOp;
}

void CUFont::SetShadowOpacity(float fOp)
{
	m_fShadowOpacity = fOp;
}

UINT CUFont::ConvertRGBA(float r, float g, float b, float a)
{
	UINT iColor = (UINT)(a * 255.f);

	iColor = (iColor << 8) | (BYTE)(r * 255.f);
	iColor = (iColor << 8) | (BYTE)(g * 255.f);
	iColor = (iColor << 8) | (BYTE)(b * 255.f);

	return iColor;
}

void CUFont::SetShadowOffset(const Vector3& vOffset)
{
	m_vOffset = vOffset;
}

void CUFont::SetAlpha(bool bAlpha)
{
	m_bAlpha = bAlpha;
}

void CUFont::SetShadowAlpha(bool bAlpha)
{
	m_bShadowAlpha = bAlpha;
}

void CUFont::SetSize(float fSize)
{
	m_fSize = fSize;

	CreateLayout();
}

void CUFont::Set3D()
{
	m_b3D = true;
}
