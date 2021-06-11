#include "MouseObj.h"
#include "../Component/ColliderPoint.h"
#include "../Component/SpriteComponent.h"
#include "UIObject.h"
#include "UIImage.h"
#include "../Component/ColliderRay.h"
#include "../CameraManager.h"
#include "../Component/Camera.h"
#include "../Input.h"
#include "../Device.h"

CMouseObj::CMouseObj()	:
	m_pPt(nullptr),
	m_pSprt(nullptr)
{
}

CMouseObj::CMouseObj(const CMouseObj& obj)	:
	CUIObject(obj)
{
	m_pPt = obj.m_pPt;

	if(obj.m_pSprt)
		m_pSprt = obj.m_pSprt->Clone();
}

CMouseObj::~CMouseObj()
{
}

std::shared_ptr<class CComponent> CMouseObj::GetCol() const
{
	return m_pPt;
}

bool CMouseObj::Init()
{
	if (!CUIObject::Init())
		return false;

	m_pSprt = CreateComponent<CSpriteComponent>("Mouse");

	((CSpriteComponent*)m_pSprt.get())->SetWorldScale(32.f, 31.f, 1.f);
	((CSpriteComponent*)m_pSprt.get())->SetPivot(0.f, 1.f, 0.f);
	((CSpriteComponent*)m_pSprt.get())->SetSceneComponentType(SCENE_COMPONENT_TYPE::SCT_UI);
	((CSpriteComponent*)m_pSprt.get())->AddAni2DInfo("Mouse", "Mouse");
	((CSpriteComponent*)m_pSprt.get())->AddRenderState(ALPHA_BLEND);
	((CSpriteComponent*)m_pSprt.get())->AddRenderState(NO_DEPTH);

	m_pPt = CreateComponent<CColliderRay>("MouseBody");

	Resolution tRS = RESOLUTION;

	SetRootComponent(m_pPt);
	((CColliderPoint*)m_pPt.get())->AddChild(m_pSprt);
	((CSpriteComponent*)m_pSprt.get())->AddRelativePos(-tRS.iWidth  / 2.f , -tRS.iHeight /2.f, 0.f);
	static_cast<CColliderRay*>(m_pPt.get())->SetMouse();


	return true;
}

void CMouseObj::Start()
{
	CUIObject::Start();
}

void CMouseObj::Update(float fTime)
{
	CUIObject::Update(fTime);

	CCamera* pCam = GET_SINGLE(CCameraManager)->GetMainCam();

	if (pCam)
	{
		Matrix matProj = pCam->GetProj();
		Matrix matView = pCam->GetView();

		Resolution tRS = RESOLUTION;

		Vector2 vMousePos = GET_SINGLE(CInput)->GetMousePos();

		float vx = (2.f * vMousePos.x / tRS.iWidth - 1.f) / matProj[0][0];
		float vy = (2.f * vMousePos.y / tRS.iHeight - 1.f) / matProj[1][1];
		matView.Inverse();

		Vector3 vViewDir = Vector3(vx, vy, 1.f);

		Vector3 vDir = vViewDir.TranslationNorm(matView);
		vDir.Normalize();
		static_cast<CColliderRay*>(m_pPt.get())->SetDir(vDir);
		static_cast<CColliderRay*>(m_pPt.get())->SetOrigin(pCam->GetWorldPos());
	}
}

void CMouseObj::PostUpdate(float fTime)
{
	CUIObject::PostUpdate(fTime);
}

void CMouseObj::Collision(float fTime)
{
	CUIObject::Collision(fTime);
}

void CMouseObj::PreRender(float fTime)
{
	CUIObject::PreRender(fTime);

	((CColliderPoint*)m_pPt.get())->EnableCol(true);
}

void CMouseObj::Render(float fTime)
{
	CUIObject::Render(fTime);
}

void CMouseObj::PostRender(float fTime)
{
	CUIObject::PostRender(fTime);
}

std::shared_ptr<CGameObject> CMouseObj::Clone()
{
	return std::shared_ptr<CGameObject>(new CMouseObj(*this));
}

void CMouseObj::Save(FILE* pFile)
{
	CUIObject::Save(pFile);
}

void CMouseObj::Load(FILE* pFile)
{
	CUIObject::Load(pFile);
}
