#pragma once

#include "../GameEngine.h"

class CCollision
{
public:
	static bool CollisionPixelToRect(class CColliderPixel* pSrc,
		class CColliderRect* pDest);
	static bool CollisionPixelToRect(Vector3& vCross,
		const PixelInfo& tSrc, const RectInfo& pDest);
	static bool CollisionPixelToSphere2D(class CColliderPixel* pSrc,
		class CColliderCircle* pDest);
	static bool CollisionPixelToSphere2D(Vector3& vCross,
		const PixelInfo& tSrc, const SphereInfo& pDest);
	static bool CollisionPixelToOBB2D(class CColliderPixel* pSrc,
		class CColliderOBB2D* pDest);
	static bool CollisionPixelToOBB2D(Vector3& vCross,
		const Vector3& vMax, const Vector3& vMin,
		const PixelInfo& tSrc, const OBBInfo& pDest);
	static bool CollisionPixelToPoint(class CColliderPixel* pSrc,
		class CColliderPoint* pDest);
	static bool CollisionPixelToPoint(Vector3& vCross,
		const PixelInfo& tSrc, const Vector3& pDest);
	static bool CollisionPixelToLine(class CColliderPixel* pSrc,
		class CColliderLine* pDest);
	static bool CollisionPixelToLine(Vector3& vCross,
		const PixelInfo& tSrc, const LineInfo& pDest);

public:
	static bool CollisionRectToRect(class CColliderRect* pSrc,
		class CColliderRect* pDest);
	static bool CollisionRectToRect(Vector3& vCross,
		const RectInfo& tSrc, const RectInfo& tDest);
	static bool CollisionRectToCircle(class CColliderRect* pSrc,
		class CColliderCircle* pDest);
	static bool CollisionRectToCircle(Vector3& vCross,
		const RectInfo& tSrc, const SphereInfo& tDest);
	static bool CollisionRectToPoint(class CColliderRect* pSrc,
		class CColliderPoint* pDest);
	static bool CollisionRectToPoint(Vector3& vCross,
		const RectInfo& tSrc, const Vector3& tDest);
	static bool CollisionRectToRay(class CColliderRect* pSrc,
		class CColliderRay* pDest);
	static bool CollisionRectToRay(Vector3& vCross,
		const RectInfo& tSrc, const RAYINFO& tDest);

public:
	static bool CollisionOBB2DToRect(class CColliderOBB2D* pSrc,
		class CColliderRect* pDest);
	static bool CollisionOBB2DToRect(Vector3& vCross,
		const OBBInfo& tSrc, const RectInfo& tDest);
	static bool CollisionOBB2DToOBB2D(class CColliderOBB2D* pSrc,
		class CColliderOBB2D* pDest);
	static bool CollisionOBB2DToOBB2D(Vector3& vCross,
		const OBBInfo& tSrc, const OBBInfo& tDest);
	static bool CollisionOBB2DToPoint(class CColliderOBB2D* pSrc,
		class CColliderPoint* pDest);
	static bool CollisionOBB2DToPoint(Vector3& vCross,
		const OBBInfo& tSrc, const Vector3& tDest);

public:
	static bool CollisionCircleToCircle(class CColliderCircle* pSrc,
		class CColliderCircle* pDest);
	static bool CollisionCircleToCircle(Vector3& vCross,
		const SphereInfo& tSrc, const SphereInfo& tDest);
	static bool CollisionCircleToPoint(class CColliderCircle* pSrc,
		class CColliderPoint* pDest);
	static bool CollisionCircleToPoint(Vector3& vCross,
		const SphereInfo& tSrc, const Vector3& tDest);
	static bool CollisionCircleToOBB2D(class CColliderCircle* pSrc,
		class CColliderOBB2D* pDest);
	static bool CollisionCircleToOBB2D(Vector3& vCross,
		const SphereInfo& tSrc, const OBBInfo& tDest);

public:
	static bool CollisionLineToLine(class CColliderLine* pSrc,
		class CColliderLine* pDest);
	static bool CollisionLineToLine(Vector3& vCross,
		const LineInfo& tSrc, const LineInfo& tDest);
	static bool CollisionLineToRect(class CColliderLine* pSrc,
		class CColliderRect* pDest);
	static bool CollisionLineToRect(Vector3& vCross,
		const LineInfo& tSrc, const RectInfo& tDest);
	static bool CollisionLineToPoint(class CColliderLine* pSrc,
		class CColliderPoint* pDest);
	static bool CollisionLineToPoint(Vector3& vCross,
		const LineInfo& tSrc, const Vector3& tDest);
	static bool CollisionLineToCircle(class CColliderLine* pSrc,
		class CColliderCircle* pDest);
	static bool CollisionLineToCircle(Vector3& vCross,
		const LineInfo& tSrc, const SphereInfo& tDest);
	static bool CollisionLineToOBB2D(class CColliderLine* pSrc,
		class CColliderOBB2D* pDest);
	static bool CollisionLineToOBB2D(Vector3& vCross,
		const LineInfo& tSrc, const OBBInfo& tDest);

public:
	static bool CollisionPointToPoint(class CColliderPoint* pSrc,
		class CColliderPoint* pDst);
	static bool CollisionPointToPoint(Vector3& vCrs,
		const Vector3& vSrc, const Vector3& vDst);

public:
	static bool CollisionSphereToSphere(class CColliderSphere* pSrc,
		class CColliderSphere* pDest);
	static bool CollisionSphereToSphere(Vector3& vCross,
		const SphereInfo& tSrc, const SphereInfo& tDest);
	static bool CollisionSphereToRay(class CColliderSphere* pSrc,
		class CColliderRay* pDest);
	static bool CollisionSphereToRay(Vector3& vCross,
		const SphereInfo& tSrc, const RAYINFO& tDest);
	static bool CollisionSphereToTerrain(class CColliderSphere* pSrc,
		class CColliderTerrain* pDest);
	static bool CollisionSphereToTerrain(Vector3& vCross,
		const SphereInfo& tSrc, const TERRAININFO& tDest);
	static bool CollisionSphereToOBB(class CColliderSphere* pSrc,
		class CColliderOBB* pDest);
	static bool CollisionSphereToOBB(Vector3& vCross,
		const SphereInfo& tSrc, const OBBInfo& tDest);
	static bool CollisionSphereToCapsule(class CColliderSphere* pSrc,
		class CColliderCapsule* pDest);
	static bool CollisionSphereToCapsule(Vector3& vCross,
		const SphereInfo& tSrc, const CAPSULEINFO& tDest);

public:
	static bool CollisionRayToRay(class CColliderRay* pSrc,
		class CColliderRay* pDest);
	static bool CollisionRayToRay(Vector3& vCross,
		const RAYINFO& tSrc, const RAYINFO& tDest);
	static bool CollisionRayToTerrain(class CColliderRay* pSrc,
		class CColliderTerrain* pDest);
	static bool CollisionRayToTerrain(Vector3& vCross,
		const RAYINFO& tSrc, const TERRAININFO& tDest);
	static bool CollisionRayToOBB(class CColliderRay* pSrc,
		class CColliderOBB* pDest);
	static bool CollisionRayToOBB(Vector3& vCross,
		const RAYINFO& tSrc, const OBBInfo& tDest);
	static bool CollisionRayToCapsule(class CColliderRay* pSrc,
		class CColliderCapsule* pDest);
	static bool CollisionRayToCapsule(Vector3& vCross,
		const RAYINFO& tSrc, const CAPSULEINFO& tDest);

public:
	static bool CollisionOBBToOBB(class CColliderOBB* pSrc, 
		class CColliderOBB* pDest);
	static bool CollisionOBBToOBB(Vector3& vCross, 
		const OBBInfo& tSrc, const OBBInfo& tDest);

public:
	static bool CollisionCapsuleToCapsule(class CColliderCapsule* pSrc,
		class CColliderCapsule* pDest);
	static bool CollisionCapsuleToCapsule(Vector3& vCross,
		const CAPSULEINFO& tSrc, const CAPSULEINFO& tDest);

private:
	static bool CollisionRayToTriangle(Vector3& vCross, const RAYINFO& tSrc, 
		const Vector3& v0, const Vector3& v1, const Vector3& v2);

};