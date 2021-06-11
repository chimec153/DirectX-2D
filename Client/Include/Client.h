#pragma once

#include "Engine.h"
#include "resource.h"
#include <fstream>
//
//struct DirectionalLight
//{
//	DirectionalLight() { ZeroMemory(this, sizeof(this)); }
//	XMFLOAT4 Ambient;
//	XMFLOAT4 Diffuse;
//	XMFLOAT4 Specular;
//	XMFLOAT3 Direction;
//	float Pad;
//};
//
//struct PointLight
//{
//	PointLight() { ZeroMemory(this, sizeof(this)); }
//	XMFLOAT4 Ambient;
//	XMFLOAT4 Diffuse;
//	XMFLOAT4 Specular;
//
//	XMFLOAT3 Position;
//	float Range;
//	XMFLOAT3 Att;
//	float Pad;
//};
//
//struct SpotLight
//{
//	SpotLight() { ZeroMemory(this, sizeof(this)); }
//
//	XMFLOAT4 Ambient;
//	XMFLOAT4 Diffuse;
//	XMFLOAT4 Specular;
//
//	XMFLOAT3 Position;
//	float Range;
//
//	XMFLOAT3 Direction;
//	float Spot;
//
//	XMFLOAT3 Att;
//	float Pad;
//};
#ifdef _WIN64
#ifdef _DEBUG
#pragma comment(lib, "GameEngine_Debug.lib")
#else
#pragma comment(lib, "GameEngine.lib")
#endif
#else
#ifdef _DEBUG
#pragma comment(lib, "GameEngine32_Debug.lib")
#else
#pragma comment(lib, "GameEngine32.lib")
#endif
#endif