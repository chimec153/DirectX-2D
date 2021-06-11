#include "Coaster.h"
#include "Component/SpriteComponent.h"
#include "Resource/Material.h"
#include "Resource/Texture.h"

CCoaster::CCoaster()	:
	m_pMesh(nullptr),
	m_fClif(0.f),
	m_fAngle(0.f),
	m_fDist(0.f),
	m_fLimitDist(600.f),
	m_iTrack(0)
{
}

CCoaster::CCoaster(const CCoaster& obj)	:
	CGameObject(obj)
{
	m_pMesh = FindComponentByType<CSpriteComponent>();
	m_fClif = obj.m_fClif;
	m_fAngle = obj.m_fAngle;
	m_fDist = obj.m_fDist;
	m_fLimitDist = obj.m_fLimitDist;
	m_iTrack = obj.m_iTrack;
}

CCoaster::~CCoaster()
{
}

void CCoaster::SetPt(const Vector3* v)
{
	memcpy(m_pPoint, v, sizeof(Vector3) * 6);
}

void CCoaster::SetDist(float fDist)
{
	m_fDist = fDist;
}

bool CCoaster::Init()
{
	if (!CGameObject::Init())
		return false;

	m_pMesh = CreateComponent<CSpriteComponent>("PTCT1");

	((CSpriteComponent*)m_pMesh.get())->AddAni2DInfo("0_0", "PTCT1_0_0");
	((CSpriteComponent*)m_pMesh.get())->AddAni2DInfo("15_0", "PTCT1_15_0");
	((CSpriteComponent*)m_pMesh.get())->AddAni2DInfo("30_0", "PTCT1_30_0");
	((CSpriteComponent*)m_pMesh.get())->AddAni2DInfo("45_0", "PTCT1_45_0");
	((CSpriteComponent*)m_pMesh.get())->AddAni2DInfo("60_0", "PTCT1_60_0");
	((CSpriteComponent*)m_pMesh.get())->AddAni2DInfo("75_0", "PTCT1_75_0");
	((CSpriteComponent*)m_pMesh.get())->AddAni2DInfo("90_0", "PTCT1_90_0");
	((CSpriteComponent*)m_pMesh.get())->AddAni2DInfo("105_0", "PTCT1_105_0");
	((CSpriteComponent*)m_pMesh.get())->AddAni2DInfo("120_0", "PTCT1_120_0");
	((CSpriteComponent*)m_pMesh.get())->AddAni2DInfo("135_0", "PTCT1_135_0");
	((CSpriteComponent*)m_pMesh.get())->AddAni2DInfo("150_0", "PTCT1_150_0");
	((CSpriteComponent*)m_pMesh.get())->AddAni2DInfo("165_0", "PTCT1_165_0");
	((CSpriteComponent*)m_pMesh.get())->AddAni2DInfo("180_0", "PTCT1_180_0");
	((CSpriteComponent*)m_pMesh.get())->AddAni2DInfo("195_0", "PTCT1_195_0");
	((CSpriteComponent*)m_pMesh.get())->AddAni2DInfo("210_0", "PTCT1_210_0");
	((CSpriteComponent*)m_pMesh.get())->AddAni2DInfo("225_0", "PTCT1_225_0");
	((CSpriteComponent*)m_pMesh.get())->AddAni2DInfo("240_0", "PTCT1_240_0");
	((CSpriteComponent*)m_pMesh.get())->AddAni2DInfo("255_0", "PTCT1_255_0");
	((CSpriteComponent*)m_pMesh.get())->AddAni2DInfo("270_0", "PTCT1_270_0");
	((CSpriteComponent*)m_pMesh.get())->AddAni2DInfo("285_0", "PTCT1_285_0");
	((CSpriteComponent*)m_pMesh.get())->AddAni2DInfo("300_0", "PTCT1_300_0");
	((CSpriteComponent*)m_pMesh.get())->AddAni2DInfo("315_0", "PTCT1_315_0");
	((CSpriteComponent*)m_pMesh.get())->AddAni2DInfo("330_0", "PTCT1_330_0");
	((CSpriteComponent*)m_pMesh.get())->AddAni2DInfo("345_0", "PTCT1_345_0");
	int t= 2;

	((CSpriteComponent*)m_pMesh.get())->SetPivot(0.5f, 0.5f, 0.f);

	Vector3 vPt[] =
	{
		Vector3(0.f, 0.f, 0.f),
		Vector3(200.f, 100.f, 0.f),
		Vector3(200.f, 200.f, 0.f),
		Vector3(100.f, 150.f, 0.f),
		Vector3(100.f, 50.f, 0.f),
		Vector3(300.f, 150.f, 0.f)
	};

	SetPt(vPt);

	SetRootComponent(m_pMesh);

	return true;
}

void CCoaster::Start()
{
	CGameObject::Start();
}

void CCoaster::Update(float fTime)
{
	CGameObject::Update(fTime);

	Vector3 vPos = {};

	m_fDist += fTime * 200.f;

	if (m_fLimitDist <= m_fDist)
	{
		m_fDist -= m_fLimitDist;

		//++m_iTrack;

		m_iTrack %= 4;

		switch (m_iTrack)
		{
		case 0:
		{
			Vector3 vPt[6]
			{
				Vector3(0.f, 0.f, 0.f),
				Vector3(200.f, 100.f, 0.f),
				Vector3(200.f, 200.f, 0.f),
				Vector3(100.f, 150.f, 0.f),
				Vector3(100.f, 50.f, 0.f),
				Vector3(300.f, 150.f, 0.f)
			};

			SetPt(vPt);
		}
			break;
		case 1:
		{
			Vector3 vPt[6]
			{
				Vector3(0.f, 0.f, 0.f),
				Vector3(200.f, 100.f, 0.f),
				Vector3(200.f, 200.f, 0.f),
				Vector3(100.f, 150.f, 0.f),
				Vector3(100.f, 50.f, 0.f),
				Vector3(300.f, 150.f, 0.f)
			};

			SetPt(vPt);
		}
			break;
		case 2:
		{
			Vector3 vPt[6]
			{
				Vector3(0.f, 0.f, 0.f),
				Vector3(200.f, 100.f, 0.f),
				Vector3(200.f, 200.f, 0.f),
				Vector3(100.f, 150.f, 0.f),
				Vector3(100.f, 50.f, 0.f),
				Vector3(300.f, 150.f, 0.f)
			};

			SetPt(vPt);
		}
			break;
		case 3:
		{
			Vector3 vPt[6]
			{
				Vector3(0.f, 0.f, 0.f),
				Vector3(200.f, 100.f, 0.f),
				Vector3(200.f, 200.f, 0.f),
				Vector3(100.f, 150.f, 0.f),
				Vector3(100.f, 50.f, 0.f),
				Vector3(300.f, 150.f, 0.f)
			};

			SetPt(vPt);
		}
			break;
		}
	}

	float fT = (m_fDist / m_fLimitDist) * 3.f + 3.f;

	int iStart = (int)fT;

	BSplineCurve(m_pPoint[iStart - 3], m_pPoint[iStart - 2], m_pPoint[iStart - 1], m_pPoint[iStart], fT - iStart, vPos);

	SetWorldPos(vPos);

	float fAngle = m_fDist / m_fLimitDist * 24.f;

	char strAngle[32] = {};

	sprintf_s(strAngle, "%d_0", (int)fAngle * 15);

	((CSpriteComponent*)m_pMesh.get())->ChangeClip(strAngle);
}

void CCoaster::PostUpdate(float fTime)
{
	CGameObject::PostUpdate(fTime);
}

void CCoaster::Collision(float fTime)
{
	CGameObject::Collision(fTime);
}

void CCoaster::PreRender(float fTime)
{
	CGameObject::PreRender(fTime);
}

void CCoaster::Render(float fTime)
{
	CGameObject::Render(fTime);
}

void CCoaster::PostRender(float fTime)
{
	CGameObject::PostRender(fTime);
}

std::shared_ptr<CGameObject> CCoaster::Clone()
{
	return std::shared_ptr<CGameObject>(new CCoaster(*this));
}

void CCoaster::Save(FILE* pFile)
{
	CGameObject::Save(pFile);
}

void CCoaster::Load(FILE* pFile)
{
	CGameObject::Load(pFile);
}

void CCoaster::BSplineCurve(const Vector3& pt1, const Vector3& pt2, const Vector3& pt3, const Vector3& pt4, const float fTime, Vector3& vResult)
{
	const float t2 = fTime * fTime;
	const float t3 = t2 * fTime;
	const float mt = 1.f - fTime;
	const float mt3 = mt * mt * mt;

	const float bi3 = mt3;
	const float bi2 = 3 * t3 - 6 * t2 + 4;
	const float bi1 = -3 * t3 + 3 * t2 + 3 * fTime + 1;
	const float bi = t3;

	vResult.x = pt1.x * bi3 +
		pt2.x * bi2 +
		pt3.x * bi1 +
		pt4.x * bi;

	vResult.x /= 6.f;

	vResult.y = pt1.y * bi3 +
		pt2.y * bi2 +
		pt3.y * bi1 +
		pt4.y * bi;

	vResult.y /= 6.f;
}
