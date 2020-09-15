#pragma once

#include <DirectXMath.h>
#include <DirectXPackedVector.h>

#include "Flag.h"

using namespace DirectX;

#define PI	3.14159f

static float RadToDeg(float fRad)
{
	return fRad * 180.f / PI;
}

static float DegToRad(float fDeg)
{
	return fDeg * PI / 180.f;
}

