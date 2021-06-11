#include "Collision.h"
#include "ColliderRect.h"
#include "ColliderLine.h"
#include "ColliderCircle.h"
#include "ColliderOBB2D.h"
#include "ColliderPixel.h"
#include "ColliderPoint.h"
#include "../Input.h"
#include "ColliderSphere.h"
#include "ColliderRay.h"
#include "ColliderTerrain.h"
#include "ColliderOBB.h"
#include "ColliderCapsule.h"
#ifdef _DEBUG
#include "../Scene/Scene.h"
#include "../Scene/SceneManager.h"
#include "Renderer.h"
#include "../Resource/ResourceManager.h"
#include "../Resource/Material.h"
#endif
#include "../CameraManager.h"
#include "Camera.h"
#include "../Device.h"

bool CCollision::CollisionRectToRect(CColliderRect* pSrc, CColliderRect* pDest)
{
	Vector3 vCross;

	if (CollisionRectToRect(vCross, pSrc->GetInfo(), pDest->GetInfo()))
	{
		pSrc->SetCross(vCross);
		pDest->SetCross(vCross);

		return true;
	}

	return false;
}

bool CCollision::CollisionRectToRect(Vector3& vCross, const RectInfo& tSrc, const RectInfo& tDest)
{
	if (tSrc.fL > tDest.fR)
		return false;
	else if (tSrc.fR < tDest.fL)
		return false;
	else if (tSrc.fT < tDest.fB)
		return false;
	else if (tSrc.fB > tDest.fT)
		return false;

	float fLf = tSrc.fL < tDest.fL ? tDest.fL : tSrc.fL;
	float fRt = tSrc.fR < tDest.fR ? tDest.fR : tSrc.fR;
	float fTop = tSrc.fT < tDest.fT ? tDest.fT : tSrc.fT;
	float fBtm = tSrc.fB < tDest.fB ? tDest.fB : tSrc.fB;

	vCross.x = (fLf + fRt) / 2.f;
	vCross.y = (fTop + fBtm) / 2.f;

	return true;
}

bool CCollision::CollisionRectToCircle(CColliderRect* pSrc, CColliderCircle* pDest)
{
	Vector3 vCross;

	if (CollisionRectToCircle(vCross, ((CColliderRect*)pSrc)->GetInfo(), ((CColliderCircle*)pDest)->GetInfo()))
	{
		pSrc->SetCross(vCross);
		pDest->SetCross(vCross);

		return true;
	}

	return false;
}

bool CCollision::CollisionRectToCircle(Vector3& vCross, const RectInfo& tSrc, const SphereInfo& tDest)
{
	OBBInfo tInfo = {};

	tInfo.fLength[AXIS_X] = tSrc.fR - tSrc.fL;
	tInfo.fLength[AXIS_Y] = tSrc.fT - tSrc.fB;

	for (int i = 0; i < AXIS_END; ++i)
		tInfo.vAxis[i] = Vector3::Axis[i];

	tInfo.vPos = Vector3((tSrc.fR + tSrc.fL) / 2.f, (tSrc.fT + tSrc.fB) / 2.f, 0.f);

	return CollisionCircleToOBB2D(vCross, tDest, tInfo);
}

bool CCollision::CollisionRectToPoint(CColliderRect* pSrc, CColliderPoint* pDest)
{
	Vector3 vCross;

	Vector3 vPos = {};

	if (pSrc->IsUI())
		vPos = pDest->GetUIInfo();

	else
		vPos = pDest->GetInfo();

	if (CollisionRectToPoint(vCross, ((CColliderRect*)pSrc)->GetInfo(), vPos))//((CColliderPoint*)pDest)->GetInfo()))
	{
		pSrc->SetCross(vCross);
		pDest->SetCross(vCross);

		if (pDest->IsMouse())
			pDest->EnableCol(false);

		return true;
	}

	return false;
}

bool CCollision::CollisionRectToPoint(Vector3& vCross, const RectInfo& tSrc, const Vector3& tDest)
{
	if (tSrc.fR < tDest.x)
		return false;

	else if (tSrc.fL > tDest.x)
		return false;

	else if (tSrc.fT < tDest.y)
		return false;

	else if (tSrc.fB > tDest.y)
		return false;

	vCross = tDest;

	return true;
}

bool CCollision::CollisionRectToRay(CColliderRect* pSrc, CColliderRay* pDest)
{
	Vector3 vCross;

	if (CollisionRectToRay(vCross,pSrc->GetInfo(), pDest->GetInfo()))
	{
		pSrc->SetCross(vCross);
		pDest->SetCross(vCross);

		return true;
	}

	return false;
}

bool CCollision::CollisionRectToRay(Vector3& vCross, const RectInfo& tSrc, const RAYINFO& tDest)
{
	CCamera* pCam = GET_SINGLE(CCameraManager)->GetMainCam();

	Resolution tRS = RESOLUTION;

	Vector3 v0 = Vector3(tSrc.fL/tRS.iWidth*2.f, tSrc.fT/tRS.iHeight*2.f,0.f);
	Vector3 v1 = Vector3(tSrc.fR/tRS.iWidth*2.f, tSrc.fT/tRS.iHeight*2.f,0.f);
	Vector3 v2 = Vector3(tSrc.fL/tRS.iWidth*2.f, tSrc.fB/tRS.iHeight*2.f,0.f);
	Vector3 v3 = Vector3(tSrc.fR/tRS.iWidth*2.f, tSrc.fB/tRS.iHeight*2.f,0.f);

	Matrix matInvVP = pCam->GetVP();

	matInvVP.Inverse();

	v0 = v0.TranslationCoor(matInvVP);
	v1 = v1.TranslationCoor(matInvVP);
	v2 = v2.TranslationCoor(matInvVP);
	v3 = v3.TranslationCoor(matInvVP);

	if (CollisionRayToTriangle(vCross, tDest, v0, v1, v2))
	{
		return true;
	}

	else if (CollisionRayToTriangle(vCross, tDest, v1, v2, v3))
	{
		return true;
	}

	return false;
}

bool CCollision::CollisionOBB2DToRect(CColliderOBB2D* pSrc, CColliderRect* pDest)
{
	Vector3 vCross;

	if (CollisionOBB2DToRect(vCross, ((CColliderOBB2D*)pSrc)->GetInfo(), ((CColliderRect*)pDest)->GetInfo()))
	{
		pSrc->SetCross(vCross);
		pDest->SetCross(vCross);

		return true;
	}

	return false;
}

bool CCollision::CollisionOBB2DToRect(Vector3& vCross, const OBBInfo& tSrc, const RectInfo& tDest)
{
	OBBInfo tDst = {};

	tDst.fLength[AXIS_X] = (tDest.fR - tDest.fL) / 2.f;
	tDst.fLength[AXIS_Y] = (tDest.fT - tDest.fB) / 2.f;
	tDst.vPos[AXIS_X] = (tDest.fR + tDest.fL) / 2.f;
	tDst.vPos[AXIS_Y] = (tDest.fT + tDest.fB) / 2.f;

	return CollisionOBB2DToOBB2D(vCross, tSrc, tDst);
}

bool CCollision::CollisionOBB2DToOBB2D(CColliderOBB2D* pSrc, CColliderOBB2D* pDest)
{
	Vector3 vCross;

	if (CollisionOBB2DToOBB2D(vCross, ((CColliderOBB2D*)pSrc)->GetInfo(), ((CColliderOBB2D*)pDest)->GetInfo()))
	{
		pSrc->SetCross(vCross);
		pDest->SetCross(vCross);

		return true;
	}

	return false;
}

bool CCollision::CollisionOBB2DToOBB2D(Vector3& vCross, const OBBInfo& tSrc, const OBBInfo& tDest)
{
	Vector3 vDist = tSrc.vPos - tDest.vPos;

	for (int j = 0; j < AXIS_END; ++j)
	{
		float fProjDest = 0;

		for (int i = 0; i < AXIS_END; ++i)
			fProjDest += abs(tSrc.vAxis[j].Dot(tDest.vAxis[i] * tDest.fLength[i]));

		float fSrc = tSrc.fLength[j];

		float fDistProj = tSrc.vAxis[j].Dot(vDist);

		if (abs(fDistProj) > (fProjDest + fSrc) / 2.f)
			return false;

		float fProjSrc = 0;

		for (int i = 0; i < AXIS_END; ++i)
			fProjSrc += abs(tDest.vAxis[j].Dot(tSrc.vAxis[i] * tSrc.fLength[i]));

		float fDest = tDest.fLength[j];

		fDistProj = tDest.vAxis[j].Dot(vDist);

		if (abs(fDistProj) > (fProjSrc + fDest) / 2.f)
			return false;
	}

	vDist.Normalize();

	float fSrcDg = sqrtf(tSrc.fLength[AXIS_X] * tSrc.fLength[AXIS_X] + tSrc.fLength[AXIS_Y] * tSrc.fLength[AXIS_Y]);
	float fDstDg = sqrtf(tDest.fLength[AXIS_X] * tDest.fLength[AXIS_X] + tDest.fLength[AXIS_Y] * tDest.fLength[AXIS_Y]);

	vCross = tSrc.vPos + vDist * fSrcDg / (fSrcDg + fDstDg);

	return true;
}

bool CCollision::CollisionOBB2DToPoint(CColliderOBB2D* pSrc, CColliderPoint* pDest)
{
	Vector3 vCross;

	Vector3 vPos = {};

	if (pSrc->IsUI())
		vPos = pDest->GetUIInfo();

	else
		vPos = pDest->GetInfo();

	if (CollisionOBB2DToPoint(vCross, ((CColliderOBB2D*)pSrc)->GetInfo(), vPos))//pDest->GetInfo()))
	{
		pSrc->SetCross(vCross);
		pDest->SetCross(vCross);

		if (pDest->IsMouse())
			pDest->EnableCol(false);

		return true;
	}

	return false;
}

bool CCollision::CollisionOBB2DToPoint(Vector3& vCross, const OBBInfo& tSrc, const Vector3& tDest)
{
	Vector3 vDst = tSrc.vPos - tDest;

	for (int i = 0; i < AXIS_Z; ++i)
	{
		float fDot = abs(vDst.Dot(tSrc.vAxis[i]));

		if (fDot > tSrc.fLength[i])
			return false;
	}

	vCross = tDest;

	return true;
}

bool CCollision::CollisionCircleToCircle(CColliderCircle* pSrc, CColliderCircle* pDest)
{
	Vector3 vCross;

	if (CollisionCircleToCircle(vCross, ((CColliderCircle*)pSrc)->GetInfo(), ((CColliderCircle*)pDest)->GetInfo()))
	{
		pSrc->SetCross(vCross);
		pDest->SetCross(vCross);

		return true;
	}

	return false;
}

bool CCollision::CollisionCircleToCircle(Vector3& vCross, const SphereInfo& tSrc, const SphereInfo& tDest)
{
	Vector3 vDst = tSrc.vCenter - tDest.vCenter;
	float fDst = vDst.Length();

	if (tSrc.fRadius + tDest.fRadius > fDst)
	{
		vCross = tSrc.vCenter - vDst * tSrc.fRadius/(tSrc.fRadius + tDest.fRadius);

		return true;
	}	

	return false;
}

bool CCollision::CollisionCircleToPoint(CColliderCircle* pSrc, CColliderPoint* pDest)
{
	Vector3 vCross;

	Vector3 vPos = {};


	if (pSrc->IsUI())
		vPos = pDest->GetUIInfo();

	else
		vPos = pDest->GetInfo();

	if (CollisionCircleToPoint(vCross, ((CColliderCircle*)pSrc)->GetInfo(), vPos))
	{
		pSrc->SetCross(vCross);
		pDest->SetCross(vCross);

		if (pDest->IsMouse())
			pDest->EnableCol(false);

		return true;
	}

	return false;
}

bool CCollision::CollisionCircleToPoint(Vector3& vCross, const SphereInfo& tSrc, const Vector3& tDest)
{
	float fDst = (tSrc.vCenter - tDest).Length();

	if (fDst < tSrc.fRadius)
	{
		vCross = tDest;
		return true;
	}

	return false;
}

bool CCollision::CollisionCircleToOBB2D(CColliderCircle* pSrc, CColliderOBB2D* pDest)
{
	Vector3 vCross;

	if (CollisionCircleToOBB2D(vCross, ((CColliderCircle*)pSrc)->GetInfo(), ((CColliderOBB2D*)pDest)->GetInfo()))
	{
		pSrc->SetCross(vCross);
		pDest->SetCross(vCross);

		return true;
	}

	return false;
}

bool CCollision::CollisionCircleToOBB2D(Vector3& vCross, const SphereInfo& tSrc, const OBBInfo& tDest)
{
	Vector3 vDist = tSrc.vCenter - tDest.vPos;

	for (int i = 0; i < AXIS_Z; ++i)
	{
		float fDot = abs(vDist.Dot(tDest.vAxis[i]));

		if (fDot > tDest.fLength[i] / 2.f + tSrc.fRadius)
			return false;
	}

	float fLength = vDist.Length();

	vDist.Normalize();

	float fDot = 0.f;

	for (int i = 0; i < AXIS_Z; ++i)
		fDot += abs((tDest.vAxis[i] * tDest.fLength[i] / 2.f).Dot(vDist));

	if (fDot + tSrc.fRadius < fLength)
		return false;

	float fDst = sqrtf(tDest.fLength[AXIS_X] * tDest.fLength[AXIS_X] + tDest.fLength[AXIS_Y] * tDest.fLength[AXIS_Y]);

	vCross = tDest.vPos + vDist * fDst / (tSrc.fRadius + fDst);

	return true;
}

bool CCollision::CollisionLineToLine(CColliderLine* pSrc, CColliderLine* pDest)
{
	Vector3 vCross;

	if (CollisionLineToLine(vCross, ((CColliderLine*)pSrc)->GetInfo(), ((CColliderLine*)pDest)->GetInfo()))
	{
		pSrc->SetCross(vCross);
		pDest->SetCross(vCross);

		return true;
	}

	return false;
}

bool CCollision::CollisionLineToLine(Vector3& vCross, const LineInfo& tSrc, const LineInfo& tDest)
{
	Vector3 vFNrm = Vector3::Axis[AXIS_Z].Cross(Vector3(tSrc.vEnd - tSrc.vStart));

	float fFF = Vector3(tDest.vStart - tSrc.vStart).Dot(vFNrm);
	float fFB = Vector3(tDest.vEnd - tSrc.vStart).Dot(vFNrm);

	if (fFF * fFB >= 0.f)
		return false;

	Vector3 vSNrm = Vector3::Axis[AXIS_Z].Cross(Vector3(tDest.vEnd - tDest.vStart));

	float fSF = Vector3(tSrc.vStart - tDest.vStart).Dot(vSNrm);
	float fSB = Vector3(tSrc.vEnd - tDest.vStart).Dot(vSNrm);

	if (fSF * fSB >= 0.f)
		return false;

	Vector3 vA = tSrc.vStart;
	Vector3 vB = tSrc.vEnd - tSrc.vStart;
	vB.Normalize();

	Vector3 vC = tDest.vStart;
	Vector3 vD = tDest.vEnd - tDest.vStart;
	vD.Normalize();

	float p = ((vA - vC).Cross(vD) / vD.Cross(vB)).z;

	vCross = vA + p * vB;

	return true;
}

bool CCollision::CollisionLineToRect(CColliderLine* pSrc, CColliderRect* pDest)
{
	Vector3 vCross;

	if (CollisionLineToRect(vCross, ((CColliderLine*)pSrc)->GetInfo(), ((CColliderRect*)pDest)->GetInfo()))
	{
		pSrc->SetCross(vCross);
		pDest->SetCross(vCross);

		return true;
	}

	return false;
}

bool CCollision::CollisionLineToRect(Vector3& vCross, const LineInfo& tSrc, const RectInfo& tDest)
{
	OBBInfo tDst = {};

	tDst.fLength[AXIS_X] = (tDest.fR - tDest.fL) / 2.f;
	tDst.fLength[AXIS_Y] = (tDest.fT - tDest.fB) / 2.f;
	tDst.vPos[AXIS_X] = (tDest.fR + tDest.fL) / 2.f;
	tDst.vPos[AXIS_X] = (tDest.fT + tDest.fB) / 2.f;

	return CollisionLineToOBB2D(vCross, tSrc, tDst);
}

bool CCollision::CollisionLineToPoint(CColliderLine* pSrc, CColliderPoint* pDest)
{
	Vector3 vCross;

	Vector3 vPos = {};

	if (pSrc->IsUI())
		vPos = pDest->GetUIInfo();

	else
		vPos = pDest->GetInfo();

	if (CollisionLineToPoint(vCross, ((CColliderLine*)pSrc)->GetInfo(), vPos))//pDest->GetInfo()))
	{
		pSrc->SetCross(vCross);
		pDest->SetCross(vCross);

		if (pDest->IsMouse())
			pDest->EnableCol(false);

		return true;
	}

	return false;
}

bool CCollision::CollisionLineToPoint(Vector3& vCross, const LineInfo& tSrc, const Vector3& tDest)
{
	Vector3 vLine = Vector3(tSrc.vEnd.x - tSrc.vStart.x, tSrc.vEnd.y - tSrc.vStart.y, 0.f);

	Vector3 vAxis = Vector3::Axis[AXIS_Z];

	Vector3 vNrm = -vAxis.Cross(vLine);

	Vector3 vPt = Vector3(tDest.x - tSrc.vStart.x, tDest.y - tSrc.vStart.y, tDest.z - tSrc.vStart.z);

	if (vPt.Dot(vNrm) < 0.f)
		return false;

	return true;
}

bool CCollision::CollisionLineToCircle(CColliderLine* pSrc, CColliderCircle* pDest)
{
	Vector3 vCross;

	if (CollisionLineToCircle(vCross, ((CColliderLine*)pSrc)->GetInfo(), ((CColliderCircle*)pDest)->GetInfo()))
	{
		pSrc->SetCross(vCross);
		pDest->SetCross(vCross);

		return true;
	}

	return false;
}

bool CCollision::CollisionLineToCircle(Vector3& vCross, const LineInfo& tSrc, const SphereInfo& tDest)
{
	Vector3 vLn = tSrc.vEnd - tSrc.vStart;

	float fAng = DirectX::XMConvertToRadians(vLn.Angle(Vector3::Axis[AXIS_X]));

	if (vLn.y < 0.f)
		fAng *= -1.f;

	float fK = tanf(fAng);
	float fR = tDest.fRadius;
	float fA = tDest.vCenter.x;
	float fB = tDest.vCenter.y;
	float fM = tSrc.vStart.y - fK * tSrc.vStart.x;

	if (-(fA * fK - (fB - fM)) * (fA * fK - (fB - fM)) + (1 + fK * fK) * fR * fR < 0.f)
		return false;

	if ((tSrc.vStart - tDest.vCenter).Length() < tDest.fRadius)
	{
		vCross = tSrc.vStart;
		return true;
	}		

	else if ((tSrc.vEnd - tDest.vCenter).Length() < tDest.fRadius)
	{
		vCross = tSrc.vEnd;
		return true;
	}		

	Vector3 vS = tDest.vCenter - tSrc.vStart;

	if (vLn.Dot(vS) < 0.f)
		return false;

	Vector3 vE = tDest.vCenter - tSrc.vEnd;

	if ((-vLn).Dot(vE) < 0.f)
		return false;

	vLn.Normalize();

	Vector3 vNrm = vLn.Cross(Vector3::Axis[AXIS_Z]);

	float fDst = vS.Dot(vNrm);

	vCross = tDest.vCenter - vNrm * fDst;

	return true;
}

bool CCollision::CollisionLineToOBB2D(CColliderLine* pSrc, CColliderOBB2D* pDest)
{
	Vector3 vCross;

	if (CollisionLineToOBB2D(vCross, ((CColliderLine*)pSrc)->GetInfo(), ((CColliderOBB2D*)pDest)->GetInfo()))
	{
		pSrc->SetCross(vCross);
		pDest->SetCross(vCross);

		return true;
	}

	return false;
}

bool CCollision::CollisionLineToOBB2D(Vector3& vCross, const LineInfo& tSrc, const OBBInfo& tDest)
{
	for (int i = 0; i < AXIS_Z; ++i)
	{
		float o = tDest.vPos.Dot(tDest.vAxis[i]);
		float o1 = o + tDest.fLength[i];
		float o2 = o - tDest.fLength[i];

		float c1 = tSrc.vStart.Dot(tDest.vAxis[i]);
		float c2 = tSrc.vEnd.Dot(tDest.vAxis[i]);

		if (((c1 > o1 && c1 > o2) &&
			(c2 > o1 && c2 > o2)) ||
			((c1 < o1 && c1 < o2) &&
			(c2 < o1 && c2 < o2)))
			return false;
	}

	Vector3 vLn = tSrc.vEnd - tSrc.vStart;

	vLn.Normalize();

	Vector3 vNrm = Vector3::Axis[AXIS_Z].Cross(vLn);

	float fDst = abs((tSrc.vStart - tDest.vPos).Dot(vNrm));

	if (fDst > abs(vNrm.Dot(tDest.vAxis[AXIS_X] * tDest.fLength[AXIS_X])) +
		abs(vNrm.Dot(tDest.vAxis[AXIS_Y] * tDest.fLength[AXIS_Y])))
		return false;

	return true;
}

bool CCollision::CollisionPointToPoint(CColliderPoint* pSrc, CColliderPoint* pDst)
{
	Vector3 vCross;

	Vector3 vPos = {};

	if (pSrc->IsUI())
		vPos = pDst->GetUIInfo();

	else
		vPos = pDst->GetInfo();

	if (CollisionPointToPoint(vCross, ((CColliderPoint*)pSrc)->GetInfo(), vPos))//((CColliderPoint*)pDst)->GetInfo()))
	{
		pSrc->SetCross(vCross);
		pDst->SetCross(vCross);

		if (pDst->IsMouse())
			pDst->EnableCol(false);

		return true;
	}

	return false;
}

bool CCollision::CollisionPointToPoint(Vector3& vCrs, const Vector3& vSrc, const Vector3& vDst)
{
	vCrs = vSrc;

	return vSrc == vDst;
}

bool CCollision::CollisionSphereToSphere(CColliderSphere* pSrc, CColliderSphere* pDest)
{
	Vector3 vCross = {};

	if (CollisionSphereToSphere(vCross, pSrc->GetInfo(), pDest->GetInfo()))
	{
		pSrc->SetCross(vCross);
		pDest->SetCross(vCross);
		return true;
	}

	return false;
}

bool CCollision::CollisionSphereToSphere(Vector3& vCross, const SphereInfo& tSrc, 
	const SphereInfo& tDest)
{
	float fDist = (tSrc.vCenter - tDest.vCenter).Length();

	if (fDist < tSrc.fRadius + tDest.fRadius)
	{
		Vector3 vDir = tSrc.vCenter - tDest.vCenter;

		if (fDist != 0.f)
		{
			vDir /= fDist;

			vCross = tSrc.vCenter + vDir * tSrc.fRadius;
		}
		else
		{
			vCross = tSrc.vCenter;
		}

		return true;
	}

	return false;
}

bool CCollision::CollisionSphereToRay(CColliderSphere* pSrc, CColliderRay* pDest)
{
	Vector3 vCross = {};

	if (CollisionSphereToRay(vCross, pSrc->GetInfo(), pDest->GetInfo()))
	{
		pSrc->SetCross(vCross);
		pDest->SetCross(vCross);

		if (pDest->IsMouse())
		{
			pDest->EnableCol(false);
		}

		return true;
	}

	return false;
}

bool CCollision::CollisionSphereToRay(Vector3& vCross, const SphereInfo& tSrc, const RAYINFO& tDest)
{
	Vector3 m = tSrc.vCenter - tDest.vOrigin;

	float fLenght = m.Length();

	float mDotU = m.Dot(tDest.vDir);

	float D = mDotU * mDotU - (fLenght * fLenght - tSrc.fRadius * tSrc.fRadius);

	if (D >= 0)
	{
		float fSqrtD = sqrtf(D);

		float t1 = mDotU + fSqrtD;
		float t2 = mDotU - fSqrtD;

		if (t1 >= 0.f)
		{
			if (t2 >= 0.f)
			{
				if (t1 >= t2)
				{
					vCross = tDest.vOrigin + tDest.vDir * t2;
				}
				else
				{
					vCross = tDest.vOrigin + tDest.vDir * t1;
				}
			}
			else
			{
				vCross = tDest.vOrigin + tDest.vDir * t1;
			}
		}
		else if(t2 >= 0.f)
		{
			vCross = tDest.vOrigin + tDest.vDir * t2;
		}

		return true;
	}

	return false;
}

bool CCollision::CollisionSphereToTerrain(CColliderSphere* pSrc, CColliderTerrain* pDest)
{
	Vector3 vCross;

	if (CollisionSphereToTerrain(vCross, pSrc->GetInfo(), pDest->GetInfo()))
	{
		pSrc->SetCross(vCross);
		pDest->SetCross(vCross);

		return true;
	}
	return false;
}

bool CCollision::CollisionSphereToTerrain(Vector3& vCross, const SphereInfo& tSrc, const TERRAININFO& tDest)
{
	std::vector<Vector2> vecIndex;

	Vector3 vIndex = (tSrc.vCenter - tDest.vPos) /tDest.vScale;

	int x = static_cast<int>(vIndex.x);
	int z = static_cast<int>(vIndex.z);
	if (x >= 0 && z >= 0 && x < tDest.iWidth && z < tDest.iHeight)
	{
		int iIdx = z * tDest.iWidth + x;
		vCross = tDest.vecPos[iIdx] * tDest.vScale + tDest.vPos;
		return true;
	}

	/*int iEndX = x + static_cast<int>(tSrc.fRadius / tDest.vScale.x);
	int iEndZ = z + static_cast<int>(tSrc.fRadius / tDest.vScale.z);
	int iStartX = x - static_cast<int>(tSrc.fRadius / tDest.vScale.x);
	int iStartZ = z - static_cast<int>(tSrc.fRadius / tDest.vScale.z);

	iStartX = iStartX < 0 ? 0 : iStartX;
	iStartZ = iStartZ < 0 ? 0 : iStartZ;
	iEndX = iEndX >= tDest.iWidth ? tDest.iWidth - 1 : iEndX;
	iEndZ = iEndZ >= tDest.iHeight ? tDest.iHeight - 1 : iEndZ;

	for (int i = iStartX; i <= iEndX; i++)
	{
		for (int j = iStartZ; j <= iEndZ; j++)
		{
			Vector2 vTilePos = {};
			vTilePos.x = i * tDest.vScale.x + tDest.vPos.x;
			vTilePos.y = j * tDest.vScale.z + tDest.vPos.z;

			Vector2 CirclePos = Vector2(tSrc.vCenter.x, tSrc.vCenter.z);

			float fDist = (CirclePos - vTilePos).Length();

			if (fDist <= tSrc.fRadius)
			{
				vecIndex.push_back(Vector2(i, j));
			}
		}
	}

	size_t iSize = vecIndex.size();

	for (size_t i = 0; i < iSize; i++)
	{
		int iIdx = vecIndex[i].y * tDest.iWidth + vecIndex[i].x;
		Vector3 A = tDest.vecPos[iIdx] * tDest.vScale + tDest.vPos;
		Vector3 B = tDest.vecPos[iIdx + 1] * tDest.vScale + tDest.vPos;
		Vector3 C = tDest.vecPos[iIdx + tDest.iWidth] * tDest.vScale + tDest.vPos;
		Vector3 D = tDest.vecPos[iIdx + tDest.iWidth + 1] * tDest.vScale + tDest.vPos;
		Vector3 v0 = D - C;
		v0.Normalize();
		Vector3 v1 = B - C;
		v1.Normalize();
		Vector3 n = v0.Cross(v1);

		float d = -n.Dot(D);
		float k = n.Dot(tSrc.vCenter) + d;

		if (k > tSrc.fRadius)
		{
			vCross.x = tSrc.vCenter.x;
			vCross.y = A.y;
			vCross.z = tSrc.vCenter.z;
			return true;
		}

		v0 = A - B;
		v1 = C - B;

		v0.Normalize();
		v1.Normalize();

		n = v0.Cross(v1);

		d = -n.Dot(D);
		k = n.Dot(tSrc.vCenter) + d;

		if (k > tSrc.fRadius)
		{
			vCross.x = tSrc.vCenter.x;
			vCross.y = A.y;
			vCross.z = tSrc.vCenter.z;
			return true;
		}
	}*/

	return false;
}

bool CCollision::CollisionSphereToOBB(CColliderSphere* pSrc, CColliderOBB* pDest)
{
	Vector3 vCross;

	if (CollisionSphereToOBB(vCross, pSrc->GetInfo(), pDest->GetInfo()))
	{
		pSrc->SetCross(vCross);
		pDest->SetCross(vCross);

		return true;
	}
	return false;
}

bool CCollision::CollisionSphereToOBB(Vector3& vCross, const SphereInfo& tSrc, const OBBInfo& tDest)
{
	Vector3 vDist = tSrc.vCenter - tDest.vPos;

	float fDist = vDist.Length();

	if (fDist != 0.f)
	{
		vDist /= fDist;
	}

	float fDot = abs(vDist.Dot(tDest.vAxis[AXIS_X]) * tDest.fLength[AXIS_X]);

	fDot += abs(vDist.Dot(tDest.vAxis[AXIS_Y]) * tDest.fLength[AXIS_Y]);
	fDot += abs(vDist.Dot(tDest.vAxis[AXIS_Z]) * tDest.fLength[AXIS_Z]);

	fDot += tSrc.fRadius;

	if (fDist > fDot)
	{
		return false;
	}

	fDot = abs(vDist.Dot(tDest.vAxis[AXIS_X])) * fDist;

	if (fDot > tSrc.fRadius + tDest.fLength[AXIS_X])
	{
		return false;
	}
	fDot = abs(vDist.Dot(tDest.vAxis[AXIS_Y])) * fDist;

	if (fDot > tSrc.fRadius + tDest.fLength[AXIS_Y])
	{
		return false;
	}
	fDot = abs(vDist.Dot(tDest.vAxis[AXIS_Z])) * fDist;

	if (fDot > tSrc.fRadius + tDest.fLength[AXIS_Z])
	{
		return false;
	}

	vCross = tSrc.vCenter - vDist * fDist;

	return true;
}

bool CCollision::CollisionSphereToCapsule(CColliderSphere* pSrc, CColliderCapsule* pDest)
{
	return false;
}

bool CCollision::CollisionSphereToCapsule(Vector3& vCross, const SphereInfo& tSrc, const CAPSULEINFO& tDest)
{
	return false;
}

bool CCollision::CollisionRayToRay(CColliderRay* pSrc, CColliderRay* pDest)
{
	Vector3 vCross;

	if (CollisionRayToRay(vCross, pSrc->GetInfo(), pDest->GetInfo()))
	{
		pSrc->SetCross(vCross);
		pDest->SetCross(vCross);

		return true;
	}
	return false;
}

bool CCollision::CollisionRayToRay(Vector3& vCross, const RAYINFO& tSrc, const RAYINFO& tDest)
{
	Vector3 p1 = tSrc.vOrigin;
	Vector3 p2 = tSrc.vOrigin + tSrc.vDir * 50.f;

	Vector3 p3 = tDest.vOrigin;
	Vector3 p4 = tDest.vOrigin + tDest.vDir * 50.f;

	Vector3 L1 = p1 - p3;
	Vector3 L2 = p2 - p3;
	Vector3 L3 = p4 - p3;

	L1.Normalize();
	L2.Normalize();

	Vector3 vAxis = L1.Cross(L2);

	if (vAxis.Dot(L3) == 0.f)
	{
		Vector3 A = tSrc.vOrigin;
		Vector3 B = tSrc.vDir;
		Vector3 C = tDest.vOrigin;
		Vector3 D = tDest.vDir;

		float q = (C - A).Cross(B).z / B.Cross(D).z;

		vCross = tDest.vOrigin + tDest.vDir * q;

		return true;
	}

	return false;
}

bool CCollision::CollisionRayToTerrain(CColliderRay* pSrc, CColliderTerrain* pDest)
{
	Vector3 vCross;

	if (CollisionRayToTerrain(vCross, pSrc->GetInfo(), pDest->GetInfo()))
	{
		pSrc->SetCross(vCross);
		pDest->SetCross(vCross);

		return true;
	}
	return false;
}

bool CCollision::CollisionRayToTerrain(Vector3& vCross, const RAYINFO& tSrc, const TERRAININFO& tDest)
{
	Vector3 tStart = (tSrc.vOrigin - tDest.vPos)/tDest.vScale;
	Vector3 tEnd = (tSrc.vOrigin + tSrc.vDir * 5000.f - tDest.vPos) / tDest.vScale;

	Vector3 vDir = tSrc.vDir / tDest.vScale;
	/*if (tStart.z > tEnd.z)
	{
		Vector3 tTemp = tStart;
		tStart = tEnd;
		tEnd = tTemp;

		vDir *= -1.f;
	}*/

	std::vector<Vector2> vecIdx;

	if (tStart.x > 0.f && tStart.z > 0.f && tStart.x < tDest.iWidth - 1&& tStart.z < tDest.iHeight - 1)
	{
		vecIdx.push_back(Vector2(static_cast<float>(static_cast<int>(tStart.x)), static_cast<float>(static_cast<int>(tStart.z))));
	}

	float m = 0.f;

	if (abs(vDir.x) > abs(vDir.z))
	{
		m = abs(vDir.z) / abs(vDir.x);
	}
	else
	{
		m = abs(vDir.x) / abs(vDir.z);
	}

	/*if (vDir.z < vDir.x)
	{
		if (vDir.x * vDir.z < 0.f)
		{
			m = vDir.x / vDir.z;
		}

		else
		{
			m = vDir.z / vDir.x;
		}
	}
	else
	{
		if (vDir.x * vDir.z < 0.f)
		{
			m = vDir.z / vDir.x;
		}
		else
		{
			m = vDir.x / vDir.z;
		}
	}*/
	int iStep = 0;
	float z = tStart.z;
	float x = tStart.x;
	while (true)
	{
		if (tStart.z < tEnd.z)
		{
			if (vDir.z < vDir.x && vDir.x > 0.f)
			{
				if (x + 1 >= tDest.iWidth - 1 || z + 1 >= tDest.iHeight - 1)
					break;

				else if (x + 1 < 0 || z + 1 < 0)
				{
					z += m;
					x += 1.f;
					continue;
				}

				if (z + m >= 1.f)
				{
					vecIdx.push_back(Vector2(static_cast<float>(static_cast<int>(x + 1)),
						static_cast<float>(static_cast<int>(z))));
					vecIdx.push_back(Vector2(static_cast<float>(static_cast<int>(x + 1)),
						static_cast<float>(static_cast<int>(z + 1))));
				}
				else
				{
					vecIdx.push_back(Vector2(static_cast<float>(static_cast<int>(x + 1)),
						static_cast<float>(static_cast<int>(z))));
				}

				z += m;
				x += 1.f;
			}
			else if (vDir.z > vDir.x && vDir.x > 0.f)
			{
				if (x + 1 >= tDest.iWidth - 1 || z + 1 >= tDest.iHeight - 1)
					break;

				else if (x + 1 < 0 || z + 1 < 0)
				{
					z += 1.f;
					x += m;
					continue;
				}

				if (x + m >= 1.f)
				{
					vecIdx.push_back(Vector2(static_cast<float>(static_cast<int>(x)),
						static_cast<float>(static_cast<int>(z + 1))));
					vecIdx.push_back(Vector2(static_cast<float>(static_cast<int>(x + 1)),
						static_cast<float>(static_cast<int>(z + 1))));
				}
				else
				{
					vecIdx.push_back(Vector2(static_cast<float>(static_cast<int>(x)),
						static_cast<float>(static_cast<int>(z + 1))));
				}

				z += 1.f;
				x += m;
			}
			else if (vDir.z < -vDir.x && vDir.x < 0.f)
			{
				if (x - 1 < 0 || z + 1 >= tDest.iHeight - 1)
					break;

				else if (x - 1 >= tDest.iWidth - 1 || z - 1 < 0)
				{
					z -= m;
					x -= 1.f;
					continue;
				}

				if (z - m >= 1.f)
				{
					vecIdx.push_back(Vector2(static_cast<float>(static_cast<int>(x - 1)),
						static_cast<float>(static_cast<int>(z))));
					vecIdx.push_back(Vector2(static_cast<float>(static_cast<int>(x - 1)),
						static_cast<float>(static_cast<int>(z + 1))));
				}
				else
				{
					vecIdx.push_back(Vector2(static_cast<float>(static_cast<int>(x - 1)),
						static_cast<float>(static_cast<int>(z))));
				}

				z += m;
				x -= 1.f;
			}

			else
			{
				if (x + 1 < 0 || z + 1 >= tDest.iHeight - 1)
					break;

				else if (z + 1 < 0 || x >= tDest.iWidth-1)
				{
					z += 1.f;
					x += m;
					continue;
				}

				if (x - m >= 1.f)
				{
					vecIdx.push_back(Vector2(static_cast<float>(static_cast<int>(x)),
						static_cast<float>(static_cast<int>(z + 1))));
					vecIdx.push_back(Vector2(static_cast<float>(static_cast<int>(x - 1)),
						static_cast<float>(static_cast<int>(z + 1))));
				}
				else
				{
					vecIdx.push_back(Vector2(static_cast<float>(static_cast<int>(x)),
						static_cast<float>(static_cast<int>(z + 1))));
				}

				z += 1.f;
				x -= m;
			}
		}
		else
		{
			if (vDir.x > 0.f && -vDir.z < vDir.x)
			{
				if (x + 1 >= tDest.iWidth - 1 || z - 1 < 0)
					break;

				else if (x + 1 < 0 || z >= tDest.iHeight-1)
				{
					x += 1.f;
					z -= m;
					continue;
				}

				vecIdx.push_back(Vector2(static_cast<float>(static_cast<int>(x + 1)),
					static_cast<float>(static_cast<int>(z))));
				vecIdx.push_back(Vector2(static_cast<float>(static_cast<int>(x + 1)),
					static_cast<float>(static_cast<int>(z - 1))));

				x += 1.f;
				z -= m;
			}
			else if (vDir.x > 0.f  &&-vDir.z > vDir.x)
			{
				if (x + 1 >= tDest.iWidth || z - 1 < 0)
					break;

				else if (x + 1 < 0 || z - 1 >= tDest.iHeight-1)
				{
					x += m;
					z -= 1.f;
					continue;
				}

				vecIdx.push_back(Vector2(static_cast<float>(static_cast<int>(x)),
					static_cast<float>(static_cast<int>(z-1))));
				vecIdx.push_back(Vector2(static_cast<float>(static_cast<int>(x+1)),
					static_cast<float>(static_cast<int>(z - 1))));

				x += m;
				z -= 1.f;
			}
			else if (vDir.x < 0.f  &&-vDir.z > -vDir.x)
			{
				if (x - 1 < 0 || z - 1 < 0)
					break;

				else if (x >= tDest.iWidth - 1 || z - 1 >= tDest.iHeight-1)
				{
					x -= m;
					z -= 1.f;
					continue;
				}

				vecIdx.push_back(Vector2(static_cast<float>(static_cast<int>(x)),
					static_cast<float>(static_cast<int>(z - 1))));
				vecIdx.push_back(Vector2(static_cast<float>(static_cast<int>(x - 1)),
					static_cast<float>(static_cast<int>(z - 1))));

				x -= m;
				z -= 1.f;
			}
			else
			{
				if (x - 1 < 0 || z - 1 < 0)
					break;

				else if (x - 1 >= tDest.iWidth-1 || z >= tDest.iHeight-1)
				{
					x -= 1.f;
					z -= m;
					continue;
				}

				vecIdx.push_back(Vector2(static_cast<float>(static_cast<int>(x-1)),
					static_cast<float>(static_cast<int>(z))));
				vecIdx.push_back(Vector2(static_cast<float>(static_cast<int>(x - 1)),
					static_cast<float>(static_cast<int>(z - 1))));

				x -= 1.f;
				z -= m;
			}
		}
	}


	size_t iSize = vecIdx.size();
#ifdef _DEBUG
	bool bResult = false;
	unsigned int* pIdx = new unsigned int[iSize * 6];
	int iIdx = 0;
#endif

	for (size_t k = 0; k < iSize; k++)
	{
		int j = static_cast<int>(vecIdx[k].y);
		int i = static_cast<int>(vecIdx[k].x);

		int iIdx0 = (j + 1) * tDest.iWidth + i;
		int iIdx1 = (j + 1) * tDest.iWidth + i+1;
		int iIdx2 = j * tDest.iWidth + i + 1;

		Vector3 v0 = tDest.vecPos[iIdx0] * tDest.vScale + tDest.vPos;
		Vector3 v1 = tDest.vecPos[iIdx1] * tDest.vScale + tDest.vPos;
		Vector3 v2 = tDest.vecPos[iIdx2] * tDest.vScale + tDest.vPos;
#ifdef _DEBUG
		pIdx[iIdx++] = (j + 1) * tDest.iWidth + i;
		pIdx[iIdx++] = (j + 1) * tDest.iWidth + i + 1;
		pIdx[iIdx++] = j * tDest.iWidth + i + 1;
#endif

		Vector3 m0 = tSrc.vOrigin - v0;
		Vector3 e1 = v1 - v0;
		Vector3 e2 = v2 - v0;

		Vector3 mCrosse2 = m0.Cross(e2);
		Vector3 e1Crosse2 = e1.Cross(e2);

		float t = e1.Dot(mCrosse2) / tSrc.vDir.Dot(e1Crosse2);
		float u = tSrc.vDir.Dot(mCrosse2) / tSrc.vDir.Dot(e1Crosse2);
		float v = tSrc.vDir.Dot(e1.Cross(m0)) / tSrc.vDir.Dot(e1Crosse2);

		if (t >= 0.f && u >= 0.f && v >= 0.f && u + v <= 1.f)
		{
			vCross = tSrc.vOrigin + tSrc.vDir * t;
#ifdef _DEBUG
			bResult = true;
			break;
#else
			return true;
#endif
		}

		iIdx0 = (j + 1) * tDest.iWidth + i;
		iIdx1 = j * tDest.iWidth + i + 1;
		iIdx2 = j * tDest.iWidth + i;

		v0 = tDest.vecPos[iIdx0] * tDest.vScale + tDest.vPos;
		v1 = tDest.vecPos[iIdx1] * tDest.vScale + tDest.vPos;
		v2 = tDest.vecPos[iIdx2] * tDest.vScale + tDest.vPos;
#ifdef _DEBUG
		pIdx[iIdx++] = (j + 1) * tDest.iWidth + i;
		pIdx[iIdx++] = j * tDest.iWidth + i + 1;
		pIdx[iIdx++] = j * tDest.iWidth + i;
#endif

		m0 = tSrc.vOrigin - v0;
		e1 = v1 - v0;
		e2 = v2 - v0;

		mCrosse2 = m0.Cross(e2);
		e1Crosse2 = e1.Cross(e2);

		t = e1.Dot(mCrosse2) / tSrc.vDir.Dot(e1Crosse2);
		u = tSrc.vDir.Dot(mCrosse2) / tSrc.vDir.Dot(e1Crosse2);
		v = tSrc.vDir.Dot(e1.Cross(m0)) / tSrc.vDir.Dot(e1Crosse2);

		if (t >= 0.f && u >= 0.f && v >= 0.f && u + v <= 1.f)
		{
			vCross = tSrc.vOrigin + tSrc.vDir * t;
#ifdef _DEBUG
			bResult = true;
			break;
#else
			return true;
#endif
		}
	}

#ifdef _DEBUG
	delete[] pIdx;
	return bResult;
	if (iIdx > 3)
	{
		GET_SINGLE(CResourceManager)->DeleteMesh("DebugWire");

		VERTEX3D* pVtx = new VERTEX3D[tDest.vecPos.size()];

		memset(pVtx, 0, sizeof(VERTEX3D) * tDest.vecPos.size());

		for (int i = 0; i < tDest.vecPos.size(); ++i)
		{
			pVtx[i].vPos = tDest.vecPos[i];
		}

		GET_SINGLE(CResourceManager)->CreateMesh("DebugWire", MESH_TYPE::MT_STATIC_MESH, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, static_cast<int>(tDest.vecPos.size()), static_cast<int>(sizeof(VERTEX3D)),
			D3D11_USAGE_IMMUTABLE, pVtx, iIdx, 4, D3D11_USAGE_IMMUTABLE, DXGI_FORMAT_R32_UINT, pIdx);

		delete[] pVtx;

		CScene* pScene = GET_SINGLE(CSceneManager)->GetScene();

		std::shared_ptr<CGameObject> pDebugObj = pScene->FindLayer(COLLIDER_LAYER)->FindObj("Debug Obj");
		if (!pDebugObj)
		{
			pDebugObj = pScene->CreateObject<CGameObject>("Debug Obj", pScene->FindLayer(COLLIDER_LAYER));

			std::shared_ptr<CComponent> pCom = pDebugObj->CreateComponent<CSceneComponent>("MeshWire");

			pDebugObj->SetRootComponent(pCom);

			static_cast<CSceneComponent*>(pCom.get())->SetMesh("DebugWire");
			static_cast<CSceneComponent*>(pCom.get())->SetShader("DebugCollider");
			static_cast<CSceneComponent*>(pCom.get())->SetMaterial("Color");
			static_cast<CSceneComponent*>(pCom.get())->SetWorldPos(tDest.vPos + Vector3(0.f, 0.001f, 0.f));
			static_cast<CSceneComponent*>(pCom.get())->SetWorldScale(tDest.vScale);
			static_cast<CSceneComponent*>(pCom.get())->GetMaterial()->SetDiffuseColor(0.f, 0.f, 1.f, 1.f);
			static_cast<CSceneComponent*>(pCom.get())->AddRenderState(NO_DEPTH);
			static_cast<CSceneComponent*>(pCom.get())->AddRenderState(CULL_NONE);
		}
		else
		{
			std::shared_ptr<CComponent> pCom = pDebugObj->FindComponentByType<CRenderer>();

			static_cast<CSceneComponent*>(pCom.get())->SetMesh("DebugWire");
			static_cast<CSceneComponent*>(pCom.get())->SetShader("DebugCollider");
		}
	}
	delete[] pIdx;
	return bResult;
#endif

	return false;
}

bool CCollision::CollisionRayToOBB(CColliderRay* pSrc, CColliderOBB* pDest)
{
	Vector3 vCross;

	if (CollisionRayToOBB(vCross, pSrc->GetInfo(), pDest->GetInfo()))
	{
		pSrc->SetCross(vCross);
		pDest->SetCross(vCross);

		return true;
	}

	return false;
}

bool CCollision::CollisionRayToOBB(Vector3& vCross, const RAYINFO& tSrc, const OBBInfo& tDest)
{
	float fRadius = sqrtf(tDest.fLength[AXIS_X] * tDest.fLength[AXIS_X] + tDest.fLength[AXIS_Y] * tDest.fLength[AXIS_Y] + tDest.fLength[AXIS_Z] * tDest.fLength[AXIS_Z]);

	SphereInfo tInfo = {};

	tInfo.vCenter = tDest.vPos;
	tInfo.fRadius = fRadius;

	if (CollisionSphereToRay(vCross, tInfo, tSrc))
	{
		Vector3 vLBN = tDest.vPos - tDest.vAxis[AXIS_X] * tDest.fLength[AXIS_X] - tDest.vAxis[AXIS_Y] * tDest.fLength[AXIS_Y] - tDest.vAxis[AXIS_Z] * tDest.fLength[AXIS_Z];
		Vector3 vLBF = tDest.vPos - tDest.vAxis[AXIS_X] * tDest.fLength[AXIS_X] - tDest.vAxis[AXIS_Y] * tDest.fLength[AXIS_Y] + tDest.vAxis[AXIS_Z] * tDest.fLength[AXIS_Z];
		Vector3 vLTN = tDest.vPos - tDest.vAxis[AXIS_X] * tDest.fLength[AXIS_X] + tDest.vAxis[AXIS_Y] * tDest.fLength[AXIS_Y] - tDest.vAxis[AXIS_Z] * tDest.fLength[AXIS_Z];

		if (CollisionRayToTriangle(vCross, tSrc, vLBN, vLBF, vLTN))
		{
			return true;
		}

		Vector3 vLTF = tDest.vPos - tDest.vAxis[AXIS_X] * tDest.fLength[AXIS_X] + tDest.vAxis[AXIS_Y] * tDest.fLength[AXIS_Y] + tDest.vAxis[AXIS_Z] * tDest.fLength[AXIS_Z];
		if (CollisionRayToTriangle(vCross, tSrc, vLBF, vLTF, vLTN))
		{
			return true;
		}
		Vector3 vRBN = tDest.vPos + tDest.vAxis[AXIS_X] * tDest.fLength[AXIS_X] - tDest.vAxis[AXIS_Y] * tDest.fLength[AXIS_Y] - tDest.vAxis[AXIS_Z] * tDest.fLength[AXIS_Z];
		if (CollisionRayToTriangle(vCross, tSrc, vLBF, vLBN, vRBN))
		{
			return true;
		}
		if (CollisionRayToTriangle(vCross, tSrc, vLTN, vLBN, vRBN))
		{
			return true;
		}
		Vector3 vRBF = tDest.vPos + tDest.vAxis[AXIS_X] * tDest.fLength[AXIS_X] - tDest.vAxis[AXIS_Y] * tDest.fLength[AXIS_Y] + tDest.vAxis[AXIS_Z] * tDest.fLength[AXIS_Z];
		if (CollisionRayToTriangle(vCross, tSrc, vLBF, vRBF, vRBN))
		{
			return true;
		}
		if (CollisionRayToTriangle(vCross, tSrc, vLBF, vLTF, vRBF))
		{
			return true;
		}
		Vector3 vRTN = tDest.vPos + tDest.vAxis[AXIS_X] * tDest.fLength[AXIS_X] + tDest.vAxis[AXIS_Y] * tDest.fLength[AXIS_Y] - tDest.vAxis[AXIS_Z] * tDest.fLength[AXIS_Z];
		if (CollisionRayToTriangle(vCross, tSrc, vLTN, vRTN, vRBN))
		{
			return true;
		}
		if (CollisionRayToTriangle(vCross, tSrc, vRBF, vRTN, vRBN))
		{
			return true;
		}
		if (CollisionRayToTriangle(vCross, tSrc, vLTF, vRTN, vLTN))
		{
			return true;
		}
		Vector3 vRTF = tDest.vPos + tDest.vAxis[AXIS_X] * tDest.fLength[AXIS_X] + tDest.vAxis[AXIS_Y] * tDest.fLength[AXIS_Y] + tDest.vAxis[AXIS_Z] * tDest.fLength[AXIS_Z];
		if (CollisionRayToTriangle(vCross, tSrc, vLTF, vRTN, vRTF))
		{
			return true;
		}
		if (CollisionRayToTriangle(vCross, tSrc, vLTF, vRTF, vRBF))
		{
			return true;
		}
		if (CollisionRayToTriangle(vCross, tSrc, vRTN, vRTF, vRBF))
		{
			return true;
		}
	}

	return false;
}

bool CCollision::CollisionRayToCapsule(CColliderRay* pSrc, CColliderCapsule* pDest)
{
	Vector3 vCross;

	if (CollisionRayToCapsule(vCross, pSrc->GetInfo(), pDest->GetInfo()))
	{
		pSrc->SetCross(vCross);
		pDest->SetCross(vCross);

		return true;
	}

	return false;
}

bool CCollision::CollisionRayToCapsule(Vector3& vCross, const RAYINFO& tSrc, const CAPSULEINFO& tDest)
{
	SphereInfo tSphere1 = {};

	tSphere1.vCenter = tDest.vCenter;
	tSphere1.fRadius = tDest.fRadius;

	if (CollisionSphereToRay(vCross, tSphere1, tSrc))
	{
		return true;
	}

	SphereInfo tSphere2 = {};

	tSphere2.vCenter = tDest.vCenter + tDest.vDir * tDest.fHeight;
	tSphere2.fRadius = tDest.fRadius;

	if (CollisionSphereToRay(vCross, tSphere2, tSrc))
	{
		return true;
	}

	Vector3 vProjOrigin = {};
	Vector3 vProjDir = {};

	Vector3 vDist = tSrc.vOrigin - tDest.vCenter;

	Vector3 vAxis = vDist - vDist.Dot(tDest.vDir) * tDest.vDir;

	vProjOrigin = vAxis + tDest.vCenter;

	vProjDir = tSrc.vDir - tSrc.vDir.Dot(tDest.vDir) * tDest.vDir;

	vProjDir.Normalize();

	Vector3 m = vProjOrigin - tDest.vCenter;
	float fLength = m.Length();
	float fDot = m.Dot(tSrc.vDir);

	float D = fDot * fDot - fLength * fLength;

	if (D >= 0.f)
	{
		float fRoot = sqrtf(D);
		float t1 = -fDot + fRoot;
		float t2 = -fDot - fRoot;

		if (t1 < 0.f)
		{
			if (t2 >= 0.f)
			{
				vCross = vProjOrigin + vProjDir * t2;
			}
			else
			{
				return false;
			}
		}
		else
		{
			if (t1 > t2)
			{
				vCross = vProjOrigin + vProjDir * t2;
			}
			else
			{
				vCross = vProjOrigin + vProjDir * t1;
			}
		}
	}

	vDist = tSrc.vOrigin - tDest.vCenter;

	Vector3 vNormal = tSrc.vDir.Cross(tDest.vDir);

	vNormal.Normalize();

	LineInfo tInfo = {};

	tInfo.vStart = vDist - vDist.Dot(vNormal) * vNormal + tDest.vCenter;
	tInfo.vEnd = tInfo.vStart + (tSrc.vDir - tSrc.vDir.Dot(vNormal) * vNormal) * 5000.f;

	OBBInfo tOBB = {};
	tOBB.vAxis[AXIS_X] = vNormal.Cross(tDest.vDir);
	tOBB.vAxis[AXIS_Y] = tDest.vDir;
	tOBB.fLength[AXIS_X] = tDest.fHeight/2.f;
	tOBB.fLength[AXIS_Y] = tDest.fRadius;
	tOBB.vPos = tDest.vCenter;

	return CollisionLineToOBB2D(vCross, tInfo, tOBB);
}

bool CCollision::CollisionOBBToOBB(CColliderOBB* pSrc, CColliderOBB* pDest)
{
	Vector3 vCross;

	if (CollisionOBBToOBB(vCross, pSrc->GetInfo(), pDest->GetInfo()))
	{
		pSrc->SetCross(vCross);
		pDest->SetCross(vCross);

		return true;
	}

	return false;
}

bool CCollision::CollisionOBBToOBB(Vector3& vCross, const OBBInfo& tSrc, const OBBInfo& tDest)
{
	float fDot[AXIS_END][AXIS_END] = {};

	float fAbsDot[AXIS_END][AXIS_END] = {};
	float fDist[AXIS_END] = {};
	float r = 0.f, r1 = 0.f, r2 = 0.f;

	int i = 0;

	const float fCutoff = 0.999999f;
	bool bExistsParallelPair = false;

	Vector3 vDist = tSrc.vPos - tDest.vPos;

	for (int i = 0; i < AXIS_END; i++)
	{
		fDot[AXIS_X][i] = tSrc.vAxis[AXIS_X].Dot(tDest.vAxis[i]);
		fAbsDot[AXIS_X][i] = abs(fDot[AXIS_X][i]);
		if (fAbsDot[AXIS_X][i] > fCutoff)
		{
			bExistsParallelPair = true;
		}
	}

	fDist[AXIS_X] = vDist.Dot(tSrc.vAxis[AXIS_X]);
	r = abs(fDist[AXIS_X]);
	r1 = tSrc.fLength[AXIS_X];
	r2 = tDest.fLength[AXIS_X] * fAbsDot[AXIS_X][AXIS_X]
		+ tDest.fLength[AXIS_Y] * fAbsDot[AXIS_X][AXIS_Y]
		+ tDest.fLength[AXIS_Z] * fAbsDot[AXIS_X][AXIS_Z];

	if (r > r1 + r2)
		return false;

	for (int i = 0; i < AXIS_END; i++)
	{
		fDot[AXIS_Y][i] = tSrc.vAxis[AXIS_Y].Dot(tDest.vAxis[i]);
		fAbsDot[AXIS_Y][i] = abs(fDot[AXIS_Y][i]);
		if (fAbsDot[AXIS_Y][i] > fCutoff)
			bExistsParallelPair = true;
	}
	
	fDist[AXIS_Y] = vDist.Dot(tSrc.vAxis[AXIS_Y]);
	r = abs(fDist[AXIS_Y]);
	r1 = tSrc.fLength[AXIS_Y];
	r2 = tDest.fLength[AXIS_X] * fAbsDot[AXIS_Y][AXIS_X]
		+ tDest.fLength[AXIS_Y] * fAbsDot[AXIS_Y][AXIS_Y]
		+ tDest.fLength[AXIS_Z] * fAbsDot[AXIS_Y][AXIS_Z];
	if (r > r1 + r2)
		return false;

	for (int i = 0; i < AXIS_END; i++)
	{
		fDot[AXIS_Z][i] = tSrc.vAxis[AXIS_Z].Dot(tDest.vAxis[i]);
		fAbsDot[AXIS_Z][i] = abs(fDot[AXIS_Z][i]);
		if (fAbsDot[AXIS_Z][i] > fCutoff)
			bExistsParallelPair = true;
	}

	fDist[AXIS_Z] = vDist.Dot(tSrc.vAxis[AXIS_Z]);
	r = abs(fDist[AXIS_Z]);
	r1 = tSrc.fLength[AXIS_Z];
	r2 = tDest.fLength[AXIS_X] * fAbsDot[AXIS_Z][AXIS_X]
		+ tDest.fLength[AXIS_Y] * fAbsDot[AXIS_Z][AXIS_Y]
		+ tDest.fLength[AXIS_Z] * fAbsDot[AXIS_Z][AXIS_Z];

	if (r > r1 + r2)
		return false;

	r = abs(vDist.Dot(tDest.vAxis[AXIS_X]));
	r1 = tSrc.fLength[AXIS_X] * fAbsDot[AXIS_X][AXIS_X]
		+ tSrc.fLength[AXIS_Y] * fAbsDot[AXIS_Y][AXIS_X]
		+ tSrc.fLength[AXIS_Z] * fAbsDot[AXIS_Z][AXIS_X];
	r2 = tDest.fLength[AXIS_X];
	if (r > r1 + r2)
		return false;

	r = abs(vDist.Dot(tDest.vAxis[AXIS_Y]));
	r1 = tSrc.fLength[AXIS_X] * fAbsDot[AXIS_X][AXIS_Y]
		+ tSrc.fLength[AXIS_Y] * fAbsDot[AXIS_Y][AXIS_Y]
		+ tSrc.fLength[AXIS_Z] * fAbsDot[AXIS_Z][AXIS_Y];
	r2 = tDest.fLength[AXIS_Y];

	if (r > r1 + r2)
		return false;

	r = abs(vDist.Dot(tDest.vAxis[AXIS_Z]));
	r1 = tSrc.fLength[AXIS_X] * fAbsDot[AXIS_X][AXIS_Z]
		+ tSrc.fLength[AXIS_Y] * fAbsDot[AXIS_Y][AXIS_Z]
		+ tSrc.fLength[AXIS_Z] * fAbsDot[AXIS_Z][AXIS_Z];
	r2 = tDest.fLength[AXIS_Z];

	if (r > r1 + r2)
		return false;

	if (bExistsParallelPair)
		return true;

	r = abs(fDist[AXIS_Z] * fDot[AXIS_Y][AXIS_Z] - fDist[AXIS_Y] * fDot[AXIS_Z][AXIS_X]);
	r1 = tSrc.fLength[AXIS_Y] * fAbsDot[AXIS_Z][AXIS_X] + tSrc.fLength[AXIS_Z] * fAbsDot[AXIS_Y][AXIS_X];
	r2 = tDest.fLength[AXIS_Y] * fAbsDot[AXIS_X][AXIS_Z] + tDest.fLength[AXIS_Z] * fAbsDot[AXIS_X][AXIS_Y];

	if (r > r1 + r2)
		return false;

	r = abs(fDist[AXIS_Z] * fDot[AXIS_Y][AXIS_Y] - fDist[AXIS_Y] * fDot[AXIS_Z][AXIS_Y]);
	r1 = tSrc.fLength[AXIS_Y] * fAbsDot[AXIS_Z][AXIS_Y] + tSrc.fLength[AXIS_Z] * fAbsDot[AXIS_Y][AXIS_Y];
	r2 = tDest.fLength[AXIS_X] * fAbsDot[AXIS_X][AXIS_Z] + tDest.fLength[AXIS_Z] * fAbsDot[AXIS_X][AXIS_X];

	if (r > r1 + r2)
		return false;

	r = abs(fDist[AXIS_Z] * fDot[AXIS_Y][AXIS_Z] - fDist[AXIS_Y] * fDot[AXIS_Z][AXIS_Z]);
	r1 = tSrc.fLength[AXIS_Y] * fAbsDot[AXIS_Z][AXIS_Z] + tSrc.fLength[AXIS_Z] * fAbsDot[AXIS_Y][AXIS_Z];
	r2 = tDest.fLength[AXIS_X] * fAbsDot[AXIS_X][AXIS_Y] + tDest.fLength[AXIS_Y] * fAbsDot[AXIS_X][AXIS_X];

	if (r > r1 + r2)
		return false;

	r = abs(fDist[AXIS_X] * fDot[AXIS_Z][AXIS_X] - fDist[AXIS_Z] * fAbsDot[AXIS_X][AXIS_X]);
	r1 = tSrc.fLength[AXIS_X] * fAbsDot[AXIS_Z][AXIS_X] + tSrc.fLength[AXIS_Z] * fAbsDot[AXIS_X][AXIS_X];
	r2 = tDest.fLength[AXIS_Y] * fAbsDot[AXIS_Y][AXIS_Z] + tDest.fLength[AXIS_Z] * fAbsDot[AXIS_Y][AXIS_Y];

	if (r > r1 + r2)
		return false;

	r = abs(fDist[AXIS_X] * fDot[AXIS_Z][AXIS_Y] - fDist[AXIS_Z] * fDot[AXIS_X][AXIS_Y]);
	r1 = tSrc.fLength[AXIS_X] * fAbsDot[AXIS_Z][AXIS_Y] + tSrc.fLength[AXIS_Z] * fAbsDot[AXIS_X][AXIS_Y];
	r2 = tDest.fLength[AXIS_X] * fAbsDot[AXIS_Y][AXIS_Z] + tDest.fLength[AXIS_Z] * fAbsDot[AXIS_Y][AXIS_X];

	if (r > r1 + r2)
		return false;

	r = abs(fDist[AXIS_X] * fDot[AXIS_Z][AXIS_Z] - fDist[AXIS_Z] * fDot[AXIS_X][AXIS_Z]);
	r1 = tSrc.fLength[AXIS_X] * fAbsDot[AXIS_Z][AXIS_Z] + tSrc.fLength[AXIS_Z] * fAbsDot[AXIS_X][AXIS_Z];
	r2 = tDest.fLength[AXIS_X] * fAbsDot[AXIS_Y][AXIS_Y] + tDest.fLength[AXIS_Y] * fAbsDot[AXIS_Y][AXIS_X];

	if (r > r1 + r2)
		return false;

	r = abs(fDist[AXIS_Y] * fDot[AXIS_X][AXIS_X] - fDist[AXIS_X] * fDot[AXIS_Y][AXIS_X]);
	r1 = tSrc.fLength[AXIS_X] * fAbsDot[AXIS_Y][AXIS_X] + tSrc.fLength[AXIS_Y] * fAbsDot[AXIS_X][AXIS_X];
	r2 = tDest.fLength[AXIS_Y] * fAbsDot[AXIS_Z][AXIS_Z] + tDest.fLength[AXIS_Z] * fAbsDot[AXIS_Z][AXIS_Y];

	if (r > r1 + r2)
		return false;

	r = abs(fDist[AXIS_Y] * fDot[AXIS_X][AXIS_Y] - fDist[AXIS_X] * fDot[AXIS_Y][AXIS_Y]);
	r1 = tSrc.fLength[AXIS_X] * fAbsDot[AXIS_Y][AXIS_Y] + tSrc.fLength[AXIS_Y] * fAbsDot[AXIS_X][AXIS_Y];
	r2 = tDest.fLength[AXIS_X] * fAbsDot[AXIS_Z][AXIS_Z] + tDest.fLength[AXIS_Z] * fAbsDot[AXIS_Z][AXIS_X];

	if (r > r1 + r2)
		return false;

	r = abs(fDist[AXIS_Y] * fDot[AXIS_X][AXIS_Z] - fDist[AXIS_X] * fDot[AXIS_Y][AXIS_Z]);
	r1 = tSrc.fLength[AXIS_X] * fAbsDot[AXIS_Y][AXIS_Z] + tSrc.fLength[AXIS_Y] * fAbsDot[AXIS_X][AXIS_Z];
	r2 = tDest.fLength[AXIS_X] * fAbsDot[AXIS_Z][AXIS_Y] + tDest.fLength[AXIS_Y] * fAbsDot[AXIS_Z][AXIS_X];

	if (r > r1 + r2)
		return false;

	return true;
}

bool CCollision::CollisionCapsuleToCapsule(CColliderCapsule* pSrc, CColliderCapsule* pDest)
{
	CAPSULEINFO tSrcInfo = pSrc->GetInfo();
	CAPSULEINFO tDestInfo = pSrc->GetInfo();

	Vector3 vCross = {};

	if (CollisionCapsuleToCapsule(vCross, tSrcInfo, tDestInfo))
	{
		pSrc->SetCross(vCross);
		pDest->SetCross(vCross);
		return true;
	}

	return false;
}

bool CCollision::CollisionCapsuleToCapsule(Vector3& vCross, const CAPSULEINFO& tSrc, const CAPSULEINFO& tDest)
{
	Vector3 vDist = tSrc.vCenter - tDest.vCenter;

	float fDist = vDist.Length();

	float fSumRadius = tSrc.fRadius + tDest.fRadius;

	if (fDist > fSumRadius)
	{
		vCross = tSrc.vCenter + vDist / fDist * tSrc.fRadius;
		return true;
	}

	Vector3 vDestCentor2 = tDest.vCenter + tDest.fHeight * tDest.vDir;

	vDist = tSrc.vCenter - vDestCentor2;

	fDist = vDist.Length();

	if (fDist > fSumRadius)
	{
		vCross = tSrc.vCenter + vDist / fDist * tSrc.fRadius;
		return true;
	}

	Vector3 vSrcCentor2 = tSrc.vCenter + tSrc.fHeight * tSrc.vDir;

	vDist = tDest.vCenter - vSrcCentor2;

	fDist = vDist.Length();

	if (fDist > fSumRadius)
	{
		vCross = tSrc.vCenter + vDist / fDist * tSrc.fRadius;
		return true;
	}

	vDist = vSrcCentor2 - vDestCentor2;

	fDist = vDist.Length();

	if (fDist > fSumRadius)
	{
		vCross = tSrc.vCenter + vDist / fDist * tSrc.fRadius;
		return true;
	}
	
	Vector3 vSrcMid = (tSrc.vCenter + vSrcCentor2)/2.f;

	vDist = vSrcMid - tDest.vCenter;

	fDist = vDist.Length();

	vDist /= fDist;

	Vector3 vAxis = vDist - vDist.Dot(tSrc.vDir) * tSrc.vDir;

	vAxis.Normalize();

	float fDot = abs(vDist.Dot(vAxis * tSrc.fRadius)) + abs(vDist.Dot(tSrc.vDir * tSrc.fHeight / 2.f));

	if (fDot + tDest.fRadius < fDist)
	{
		return false;
	}

	vDist = vSrcMid - vDestCentor2;

	fDist = vDist.Length();

	vDist /= fDist;

	vAxis = vDist - vDist.Dot(tSrc.vDir) * tSrc.vDir;

	vAxis.Normalize();

	fDot = abs(vDist.Dot(vAxis * tSrc.fRadius)) + abs(vDist.Dot(tSrc.vDir * tSrc.fHeight / 2.f));

	if (fDot + tDest.fRadius < fDist)
	{
		return false;
	}

	Vector3 vDestMid = (tDest.vCenter + vDestCentor2) / 2.f;

	vDist = vDestMid - tSrc.vCenter;

	fDist = vDist.Length();

	vDist /= fDist;

	vAxis = vDist - vDist.Dot(tDest.vDir) * tDest.vDir;

	vAxis.Normalize();

	fDot = abs(vDist.Dot(vAxis * tDest.fRadius)) + abs(vDist.Dot(tDest.vDir * tDest.fHeight / 2.f));

	if (fDot + tSrc.fRadius < fDist)
	{
		return false;
	}
	
	vDist = vDestMid - vSrcCentor2;

	fDist = vDist.Length();

	vDist /= fDist;

	vAxis = vDist - vDist.Dot(tDest.vDir) * tDest.vDir;

	vAxis.Normalize();

	fDot = abs(vDist.Dot(vAxis * tDest.fRadius)) + abs(vDist.Dot(tDest.vDir * tDest.fHeight / 2.f));

	if (fDot + tSrc.fRadius < fDist)
	{
		return false;
	}

	vCross = vSrcCentor2 + vDist * tSrc.fRadius;

	return true;
}

bool CCollision::CollisionRayToTriangle(Vector3& vCross, const RAYINFO& tSrc, const Vector3& v0, const Vector3& v1, const Vector3& v2)
{
	Vector3 m = tSrc.vOrigin - v0;
	Vector3 e1 = v1 - v0;
	Vector3 e2 = v2 - v0;

	Vector3 mCrosse2 = m.Cross(e2);
	float dDote1Crosse2 = tSrc.vDir.Dot(e1.Cross(e2));

	float t = e1.Dot(mCrosse2) / dDote1Crosse2;
	float u = tSrc.vDir.Dot(mCrosse2) / dDote1Crosse2;
	float v = tSrc.vDir.Dot(e1.Cross(m)) / dDote1Crosse2;

	if (t >= 0.f && u >= 0.f && v >= 0.f && u + v <= 1.f)
	{
		vCross = tSrc.vOrigin + tSrc.vDir * t;

		return true;
	}

	return false;
}

bool CCollision::CollisionPixelToRect(CColliderPixel* pSrc, CColliderRect* pDest)
{
	Vector3 vCross;

	if (CollisionPixelToRect(vCross, ((CColliderPixel*)pSrc)->GetInfo(), ((CColliderRect*)pDest)->GetInfo()))
	{
		pSrc->SetCross(vCross);
		pDest->SetCross(vCross);

		return true;
	}

	return false;
}

bool CCollision::CollisionPixelToRect(Vector3& vCross, const PixelInfo& tSrc, const RectInfo& tDest)
{
	if (tSrc.tRt.fL > tDest.fR)
		return false;

	else if (tSrc.tRt.fR < tDest.fL)
		return false;

	else if (tSrc.tRt.fB > tDest.fT)
		return false;

	else if (tSrc.tRt.fT < tDest.fB)
		return false;

	int iStartX, iEndX, iStartY, iEndY;

	iStartX = (int)(tSrc.tRt.fL < tDest.fL ? tDest.fL : tSrc.tRt.fL);
	iEndX = (int)(tSrc.tRt.fR > tDest.fR ? tDest.fR : tSrc.tRt.fR);
	iStartY = (int)(tSrc.tRt.fB < tDest.fB ? tDest.fB : tSrc.tRt.fB);
	iEndY = (int)(tSrc.tRt.fT > tDest.fT ? tDest.fT : tSrc.tRt.fT);

	iStartX -= (int)(tSrc.tRt.fL);
	iEndX -= (int)(tSrc.tRt.fL);
	iStartY -= (int)(tSrc.tRt.fB);
	iEndY -= (int)(tSrc.tRt.fB);

	for (size_t y = iStartY; y <= iEndY; ++y)
	{
		for (size_t x = iStartX; x <= iEndX; ++x)
		{
			size_t idx = y * tSrc.iWidth + x;

			switch (tSrc.eType)
			{
			case PIXEL_COLLISION_TYPE::COLOR_IGNORE:
				if (tSrc.pPixel[idx].r == tSrc.tColor.r &&
					tSrc.pPixel[idx].g == tSrc.tColor.g &&
					tSrc.pPixel[idx].b == tSrc.tColor.b)
					continue;

				vCross.x = x + tSrc.tRt.fL;
				vCross.y = y + tSrc.tRt.fB;

				return true;
			case PIXEL_COLLISION_TYPE::LESS_ALPHA_IGNORE:
				if (tSrc.pPixel[idx].a <= tSrc.tColor.a)
					continue;

				vCross.x = x + tSrc.tRt.fL;
				vCross.y = y + tSrc.tRt.fB;

				return true;
			case PIXEL_COLLISION_TYPE::COLOR_CONFIRM:
				if (tSrc.pPixel[idx].r != tSrc.tCheckColor.r||
					tSrc.pPixel[idx].g != tSrc.tCheckColor.g ||
					tSrc.pPixel[idx].b != tSrc.tCheckColor.b)
					continue;

				vCross.x = x + tSrc.tRt.fL;
				vCross.y = y + tSrc.tRt.fB;

				return true;
			}
		}
	}

	return false;
}

bool CCollision::CollisionPixelToSphere2D(CColliderPixel* pSrc, CColliderCircle* pDest)
{
	Vector3 vCross;

	if (CollisionPixelToSphere2D(vCross, ((CColliderPixel*)pSrc)->GetInfo(), ((CColliderCircle*)pDest)->GetInfo()))
	{
		pSrc->SetCross(vCross);
		pDest->SetCross(vCross);

		return true;
	}

	return false;
}

bool CCollision::CollisionPixelToSphere2D(Vector3& vCross, const PixelInfo& tSrc, const SphereInfo& tDest)
{
	RectInfo tInfo = {};

	tInfo.fL = tDest.vCenter.x - tDest.fRadius;
	tInfo.fR = tDest.vCenter.x + tDest.fRadius;
	tInfo.fB = tDest.vCenter.y - tDest.fRadius;
	tInfo.fT = tDest.vCenter.y + tDest.fRadius;

	if (tSrc.tRt.fL > tInfo.fR)
		return false;

	else if (tSrc.tRt.fR < tInfo.fL)
		return false;

	else if (tSrc.tRt.fB > tInfo.fT)
		return false;

	else if (tSrc.tRt.fT < tInfo.fB)
		return false;

	int iStartX, iEndX, iStartY, iEndY;

	iStartX = (int)(tSrc.tRt.fL < tInfo.fL ? tInfo.fL : tSrc.tRt.fL);
	iEndX = (int)(tSrc.tRt.fR > tInfo.fR ? tInfo.fR : tSrc.tRt.fR);
	iStartY = (int)(tSrc.tRt.fB < tInfo.fB ? tInfo.fB : tSrc.tRt.fB);
	iEndY = (int)(tSrc.tRt.fT > tInfo.fT ? tInfo.fT : tSrc.tRt.fT);

	iStartX -= (int)(tSrc.tRt.fL);
	iEndX -= (int)(tSrc.tRt.fL);
	iStartY -= (int)(tSrc.tRt.fB);
	iEndY -= (int)(tSrc.tRt.fB);

	for (size_t y = iStartY; y <= iEndY; ++y)
	{
		for (size_t x = iStartX; x <= iEndX; ++x)
		{
			size_t idx = y * tSrc.iWidth + x;

			Vector3 vPt = {};

			vPt.x = x + tSrc.tRt.fL;
			vPt.y = y + tSrc.tRt.fB;

			if (!CollisionCircleToPoint(vCross, tDest, vPt))
				continue;

			switch (tSrc.eType)
			{
			case PIXEL_COLLISION_TYPE::COLOR_IGNORE:
				if (tSrc.pPixel[idx].r == tSrc.tColor.r &&
					tSrc.pPixel[idx].g == tSrc.tColor.g &&
					tSrc.pPixel[idx].b == tSrc.tColor.b)
					continue;

				vCross.x = x + tSrc.tRt.fL;
				vCross.y = y + tSrc.tRt.fB;

				return true;
			case PIXEL_COLLISION_TYPE::LESS_ALPHA_IGNORE:
				if (tSrc.pPixel[idx].a <= tSrc.tColor.a)
					continue;

				vCross.x = x + tSrc.tRt.fL;
				vCross.y = y + tSrc.tRt.fB;

				return true;
			case PIXEL_COLLISION_TYPE::COLOR_CONFIRM:
				if (tSrc.pPixel[idx].r != tSrc.tCheckColor.r ||
					tSrc.pPixel[idx].g != tSrc.tCheckColor.g ||
					tSrc.pPixel[idx].b != tSrc.tCheckColor.b)
					continue;

				vCross.x = x + tSrc.tRt.fL;
				vCross.y = y + tSrc.tRt.fB;

				return true;
			}
		}
	}

	return false;
}

bool CCollision::CollisionPixelToOBB2D(CColliderPixel* pSrc, CColliderOBB2D* pDest)
{
	Vector3 vCross;

	if (CollisionPixelToOBB2D(vCross, pDest->GetMax(), pDest->GetMin(),
		((CColliderPixel*)pSrc)->GetInfo(), ((CColliderOBB2D*)pDest)->GetInfo()))
	{
		pSrc->SetCross(vCross);
		pDest->SetCross(vCross);

		return true;
	}

	return false;
}

bool CCollision::CollisionPixelToOBB2D(Vector3& vCross,
	const Vector3& vMax, const Vector3& vMin, const PixelInfo& tSrc, const OBBInfo& tDest)
{
	RectInfo tInfo = {};

	tInfo.fL = vMin.x;
	tInfo.fB = vMin.y;
	tInfo.fR = vMax.x;
	tInfo.fT = vMax.y;

	if (tSrc.tRt.fL > tInfo.fR)
		return false;

	else if (tSrc.tRt.fR < tInfo.fL)
		return false;

	else if (tSrc.tRt.fB > tInfo.fT)
		return false;

	else if (tSrc.tRt.fT < tInfo.fB)
		return false;

	int iStartX, iEndX, iStartY, iEndY;

	iStartX = (int)(tSrc.tRt.fL < tInfo.fL ? tInfo.fL : tSrc.tRt.fL);
	iEndX = (int)(tSrc.tRt.fR > tInfo.fR ? tInfo.fR : tSrc.tRt.fR);
	iStartY = (int)(tSrc.tRt.fB < tInfo.fB ? tInfo.fB : tSrc.tRt.fB);
	iEndY = (int)(tSrc.tRt.fT > tInfo.fT ? tInfo.fT : tSrc.tRt.fT);

	iStartX -= (int)(tSrc.tRt.fL);
	iEndX -= (int)(tSrc.tRt.fL);
	iStartY -= (int)(tSrc.tRt.fB);
	iEndY -= (int)(tSrc.tRt.fB);

	for (size_t y = iStartY; y <= iEndY; ++y)
	{
		for (size_t x = iStartX; x <= iEndX; ++x)
		{
			size_t idx = y * tSrc.iWidth+ x;

			bool bCol = false;

			Vector3 vPt(x + tSrc.tRt.fL, y + tSrc.tRt.fB, 0.f);

			switch (tSrc.eType)
			{
			case PIXEL_COLLISION_TYPE::COLOR_IGNORE:
				if (tSrc.pPixel[idx].r == tSrc.tColor.r &&
					tSrc.pPixel[idx].g == tSrc.tColor.g &&
					tSrc.pPixel[idx].b == tSrc.tColor.b)
					continue;

				bCol =  true;
				break;
			case PIXEL_COLLISION_TYPE::LESS_ALPHA_IGNORE:
				if (tSrc.pPixel[idx].a <= tSrc.tColor.a)
					continue;

				bCol = true;
				break;
			case PIXEL_COLLISION_TYPE::COLOR_CONFIRM:
				if (tSrc.pPixel[idx].r != tSrc.tCheckColor.r ||
					tSrc.pPixel[idx].g != tSrc.tCheckColor.g ||
					tSrc.pPixel[idx].b != tSrc.tCheckColor.b)
					continue;

				bCol = true;
				break;
			}

			if (bCol && CollisionOBB2DToPoint(vCross, tDest, vPt))
				return true;

			vCross.x = x + tSrc.tRt.fL;
			vCross.y = y + tSrc.tRt.fB;
		}
	}

	return false;
}

bool CCollision::CollisionPixelToPoint(CColliderPixel* pSrc, CColliderPoint* pDest)
{
	Vector3 vCross;

	Vector3 vPos = {};

	if (pSrc->IsUI())
		vPos = pDest->GetUIInfo();

	else
		vPos = pDest->GetInfo();

	if (CollisionPixelToPoint(vCross, ((CColliderPixel*)pSrc)->GetInfo(), vPos))//((CColliderPoint*)pDest)->GetInfo()))
	{
		pSrc->SetCross(vCross);
		pDest->SetCross(vCross);

		if (pDest->IsMouse())
			pDest->EnableCol(false);

		return true;
	}

	return false;
}

bool CCollision::CollisionPixelToPoint(Vector3& vCross, const PixelInfo& tSrc, const Vector3& tDest)
{
	int idx = (int)(tDest.y - tSrc.tRt.fB) * (int)tSrc.iWidth + (int)(tDest.x - tSrc.tRt.fL);

	switch (tSrc.eType)
	{
	case PIXEL_COLLISION_TYPE::COLOR_IGNORE:
		if (tSrc.pPixel[idx].r == tSrc.tColor.r &&
			tSrc.pPixel[idx].g == tSrc.tColor.g &&
			tSrc.pPixel[idx].b == tSrc.tColor.b)
			return false;
	case PIXEL_COLLISION_TYPE::LESS_ALPHA_IGNORE:
		if (tSrc.pPixel[idx].a <= tSrc.tColor.a)
			return false;
	case PIXEL_COLLISION_TYPE::COLOR_CONFIRM:
		if (tSrc.pPixel[idx].r != tSrc.tCheckColor.r ||
			tSrc.pPixel[idx].g != tSrc.tCheckColor.g ||
			tSrc.pPixel[idx].b != tSrc.tCheckColor.b)
			return false;
	}

	vCross = tDest;

	return true;
}

bool CCollision::CollisionPixelToLine(CColliderPixel* pSrc, CColliderLine* pDest)
{
	return false;
}

bool CCollision::CollisionPixelToLine(Vector3& vCross, const PixelInfo& tSrc, const LineInfo& tDest)
{
	return false;
}
