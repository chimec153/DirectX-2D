#include "ClientButton.h"
#include "Component/ColliderRect.h"
#include "Component/Mesh2DComponent.h"
#include "Resource/ResourceManager.h"
#include "Resource/Material.h"
#include "Resource/Texture.h"
#include "Resource/Mesh2D.h"
#include "Input.h"
#include "Sound/SoundManager.h"
#include "Component/Sound.h"
#include "UI/UIButton.h"
#include "UI/UIImage.h"

CClientButton::CClientButton()	:
	m_pButton(nullptr),
	m_pImg(nullptr)
{
	m_vecTexture.resize((int)BUTTON_STATE::END);
	m_iObjType = (int)OBJECT_CLASS_TYPE::CLIENTBUTTON;
}

CClientButton::CClientButton(const CClientButton& btn)	:
	CUIObject(btn)
{
	m_pButton = std::shared_ptr<CComponent>(FindComponentByType<CUIButton>());
	m_pImg = std::shared_ptr<CComponent>(FindComponentByType<CUIImage>());
}

CClientButton::~CClientButton()
{
}

void CClientButton::SetTexture(BUTTON_STATE eState, std::shared_ptr<CTexture> pTex)
{
	((CUIButton*)m_pButton.get())->SetTexture(eState, pTex);
}

void CClientButton::SetTexture(BUTTON_STATE eState, const std::string& strTag)
{
	((CUIButton*)m_pButton.get())->SetTexture(eState, strTag);
}

void CClientButton::SetColor(BUTTON_STATE eState, const Vector4& vClr)
{
	((CUIButton*)m_pButton.get())->SetColor(eState, vClr);
}

void CClientButton::SetColor(BUTTON_STATE eState, BYTE r, BYTE g, BYTE b, BYTE a)
{
	((CUIButton*)m_pButton.get())->SetColor(eState, r/255.f,g / 255.f,b / 255.f,a / 255.f);
}

void CClientButton::SetColor(BUTTON_STATE eState, float r, float g, float b, float a)
{
	((CUIButton*)m_pButton.get())->SetColor(eState, r, g, b, a);
}

void CClientButton::SetSound(BUTTON_STATE eStat, SOUND_TYPE eType, const std::string& strKey)
{
}

void CClientButton::SetImgTexture(BUTTON_STATE eStat, std::shared_ptr<CTexture> pTex)
{
	m_vecTexture[(int)eStat] = pTex;

	((CUIImage*)m_pImg.get())->SetTexture(pTex);
}

void CClientButton::SetImgTexture(BUTTON_STATE eStat, const std::string& strKey)
{
	m_vecTexture[(int)eStat] = GET_SINGLE(CResourceManager)->FindTexture(strKey);

	((CUIImage*)m_pImg.get())->SetTexture(m_vecTexture[(int)eStat]);
}

void CClientButton::ChangeImg(float fTime)
{

	if (m_pImg)
	{
		BUTTON_STATE eStat = ((CUIButton*)m_pButton.get())->GetState();

		((CUIImage*)m_pImg.get())->SetTexture(m_vecTexture[(int)eStat]);
	}
}

void CClientButton::SetUIImage()
{
	m_pImg = CreateComponent<CUIImage>("Img");
	((CUIImage*)m_pImg.get())->SetInheritScale(false);

	((CUIButton*)m_pButton.get())->AddChild(m_pImg);
}

CUIButton* CClientButton::GetButton() const
{
	return ((CUIButton*)m_pButton.get());
}

bool CClientButton::Init()
{
	if (!CUIObject::Init())
		return false;

	m_pButton = CreateComponent<CUIButton>("UIButton");
	SetRootComponent(m_pButton);

	return true;
}

void CClientButton::Start()
{
	CUIObject::Start();

	m_pButton = FindComponentByType<CUIButton>("UIButton");

	((CUIButton*)m_pButton.get())->SetCallBack<CClientButton>(BUTTON_STATE::OFF, this, &CClientButton::ChangeImg);
	((CUIButton*)m_pButton.get())->SetCallBack<CClientButton>(BUTTON_STATE::ON, this, &CClientButton::ChangeImg);
	((CUIButton*)m_pButton.get())->SetCallBack<CClientButton>(BUTTON_STATE::PRESS, this, &CClientButton::ChangeImg);
	((CUIButton*)m_pButton.get())->SetCallBack<CClientButton>(BUTTON_STATE::DISABLE, this, &CClientButton::ChangeImg);
}

 void CClientButton::Update(float fTime)
{
	 CUIObject::Update(fTime);
}
 void CClientButton::PostUpdate(float fTime)
{
	 CUIObject::PostUpdate(fTime);

}
 void CClientButton::Collision(float fTime)
{
	 CUIObject::Collision(fTime);
 }
 void CClientButton::PreRender(float fTime)
 {
	 CUIObject::PreRender(fTime);
 }
 void CClientButton::Render(float fTime)
 {
	 CUIObject::Render(fTime);
 }
 void CClientButton::PostRender(float fTime)
 {
	 CUIObject::PostRender(fTime);
}

 std::shared_ptr<CGameObject> CClientButton::Clone()
{
	return std::shared_ptr<CGameObject>(new CClientButton(*this));
}

 void CClientButton::Save(FILE* pFile)
{
	 CUIObject::Save(pFile);

	 size_t iSz = m_vecTexture.size();

	 fwrite(&iSz, 4, 1, pFile);

	 for (size_t i = 0; i < iSz; ++i)
	 {
		 bool bTex = false;

		 if (m_vecTexture[i])
			 bTex = true;

		 fwrite(&bTex, 1, 1, pFile);

		 if(m_vecTexture[i])
			m_vecTexture[i]->Save(pFile);
	 }
}

 void CClientButton::Load(FILE* pFile)
{
	 CUIObject::Load(pFile);

	 m_pButton = FindComponentByType<CUIButton>();

	 ((CUIButton*)m_pButton.get())->SetCallBack<CClientButton>(BUTTON_STATE::OFF, this, &CClientButton::ChangeImg);
	 ((CUIButton*)m_pButton.get())->SetCallBack<CClientButton>(BUTTON_STATE::ON, this, &CClientButton::ChangeImg);
	 ((CUIButton*)m_pButton.get())->SetCallBack<CClientButton>(BUTTON_STATE::PRESS, this, &CClientButton::ChangeImg);
	 ((CUIButton*)m_pButton.get())->SetCallBack<CClientButton>(BUTTON_STATE::DISABLE, this, &CClientButton::ChangeImg);

	 m_pImg = FindComponentByType<CUIImage>();

	 size_t iSz = 0;

	 fread(&iSz, 4, 1, pFile);

	 for (size_t i = 0; i < iSz; ++i)
	 {
		 bool bTex = false;

		 fread(&bTex, 1, 1, pFile);

		 if (bTex)
		 {
			 m_vecTexture[i] = GET_SINGLE(CResourceManager)->LoadTexture(pFile);
		 }
	 }

}

 void CClientButton::ColBgn(CCollider* pSrc, CCollider* pDst, float fTime)
 {
 }

 void CClientButton::ColStay(CCollider* pSrc, CCollider* pDst, float fTime)
 {
 }

 void CClientButton::ColEnd(CCollider* pSrc, CCollider* pDst, float fTime)
 {
 }

 void CClientButton::SetClickCallBack(BUTTON_STATE eStat, void(*pFunc)(float))
 {
	 ((CUIButton*)m_pButton.get())->SetCallBack(eStat, pFunc);
 }
