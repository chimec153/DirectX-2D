#pragma once

#define DIRECTINPUT_HEADER_VERSION  0x0800

#include <Windows.h>
#include <list>
#include <vector>
#include <unordered_map>
#include <crtdbg.h>
#include <functional>
#include <string>
#include <math.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <typeinfo>
#include <dinput.h>	//	게임 패드로도 가능
#include <algorithm>

#include "Macro.h"
#include "Matrix.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dinput8.lib")

typedef struct _tagResolution
{
	int		iWidth;
	int		iHeight;
}Resolution, * PResolution;

typedef struct _tagVertexColor
{
	Vector3 vPos;
	Vector4	vColor;
}VertexColor, *PVertexColor;