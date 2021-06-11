#pragma once

#define DIRECTINPUT_VERSION 0x0800

#include <DirectXMath.h>
#include <DirectXPackedVector.h>

#include "Flag.h"

using namespace DirectX;

#define PI		3.14159f
#define PI_DIV2	1.57080f
#define PI_DIV4	0.78540f

static float RadToDeg(float fRad)
{
	return fRad * 180.f / PI;
}

static float DegToRad(float fDeg)
{
	return fDeg * PI / 180.f;
}

