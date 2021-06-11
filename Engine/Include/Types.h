#pragma once
#define WIN32_LEAN_AND_MEAN
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
#include <iostream>
#include "Resource/DirectXTex.h"
#include <dwrite.h>
#include <d2d1.h>
#include <queue>
#include <stack>
#include <mutex>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <mswsock.h>
#include "UI/imgui/imgui.h"
#include "UI/imgui/ImGuiFileDialog.h"
#include "sqlite3.h"

#include "Macro.h"
#include "Matrix.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "wsock32.lib")
#pragma comment(lib, "sqlite3.lib")

#ifdef _WIN64

#pragma comment(lib, "fmod64_vc.lib")

#ifdef _DEBUG
#pragma comment(lib, "DirectXTex_Debug.lib")
#pragma comment(lib, "libfbxsdk-md64_Debug.lib")
#else
#pragma comment(lib, "DirectXTex.lib")
#pragma comment(lib, "libfbxsdk-md64.lib")

#endif

#else

#pragma comment(lib, "fmod_vc.lib")

#ifdef _DEBUG
#pragma comment(lib, "DirectXTex32_Debug.lib")
#pragma comment(lib, "libfbxsdk-md_Debug.lib")
#else
#pragma comment(lib, "DirectXTex32.lib")
#pragma comment(lib, "libfbxsdk-md.lib")
#endif

#endif

typedef struct _tagVertexBuffer
{
	ID3D11Buffer* pBuffer;
	int					iSize;
	int					iCount;
	D3D11_USAGE			iUsage;
	void* pData;

	_tagVertexBuffer() :
		pBuffer(nullptr),
		iSize(0),
		iCount(0),
		iUsage(D3D11_USAGE_DEFAULT),
		pData(nullptr)
	{
	}
	~_tagVertexBuffer()
	{
	}
}Vertex, * PVertex;

typedef struct _tagIndexBuffer
{
	ID3D11Buffer* pBuffer;
	int							iSize;
	int							iCount;
	D3D11_USAGE					eUsage;
	DXGI_FORMAT					eFmt;
	void* pData;
	_tagIndexBuffer() :
		pBuffer(nullptr)
		, iSize(0)
		, iCount(0)
		, eUsage(D3D11_USAGE_DEFAULT)
		, eFmt(DXGI_FORMAT_UNKNOWN)
		, pData(nullptr)
	{

	}
	~_tagIndexBuffer()
	{
	}

}Index, * PIndex;

typedef struct _tagMeshContainer
{
	Vertex							tVtx;
	std::vector<Index>				vecIdx;
	std::vector<std::shared_ptr<class CMaterial>>	vecMaterial;
	std::vector<std::shared_ptr<class CShader>>		vecShader;
	D3D11_PRIMITIVE_TOPOLOGY		eTopology;
}MESHCONTAINER, * PMESHCONTAINER;

typedef struct _tagResolution
{
	int		iWidth;
	int		iHeight;
}Resolution, * PResolution;

typedef struct _tagVertex2D
{
	Vector3 vPos;
	Vector2 vUV;
	Vector4	vColor;
}Vertex2D, * PVertex2D;

typedef struct _tagVertex3D
{
	Vector3 vPos;
	Vector3 vNormal;
	Vector2 vUV;
	Vector3 vTangent;
	Vector3 vBinormal;
	Vector4 vWeight;
	Vector4 vWeightFactor;
}VERTEX3D, *PVERTEX3D;

typedef struct _tagBasic
{
	Vector3	vPos;
	Vector2	vUV;
}BASIC, *PBASIC;

typedef struct _tagTransformCBuffer
{
	Matrix		matWorld;
	Matrix		matView;
	Matrix		matProj;
	Matrix		matWV;
	Matrix		matWVP;
	Matrix		matWorldInvTrans;
	Matrix		matTexTransform;
	Matrix		matProjInv;
	Matrix		matLightWVPT;
	Matrix		matShadowViewProj;
	Matrix		matViewInv;
	Vector3		vPivot;
	float		fEmpty1;
	Vector3		vMeshSize;
	float		fEmpty2;
}TransformCBuffer,*PTransformCBuffer;

typedef struct _tagShaderCBuffer
{
	Vector4	vDif;
	Vector4 vEmb;
	Vector4 vSpec;
	Vector4 vEmi;
	int		iDifTex;
	int		iNrmTex;
	int		iSpecTex;
	int		iEmiTex;
	int		iBump;
	int		iAnimation;
	int		iDecal;
	float	vEmpty;
}ShaderCBuffer, *PShaderCBuffer;

typedef struct _tagAniCBuffer
{
	int		eImageType;
	float	fFrame;
	Vector2	vStart;
	Vector2	vEnd;
	Vector2	vSize;
}AniCBuffer, *PAniCBuffer;

typedef struct _tagPaperBurnCBuffer
{
	float		fFilter;
	Vector3		vOutColor;
	Vector3		vColor;
	float		fOutFilter;
	Vector3		vInColor;
	float		fMidFilter;
	float		fInFilter;
	int			iReverse;
	int			iEnable;
	float		fEmpty;
}PaperBurnCBuffer, *PPaperBurnCBuffer;

typedef struct _tagLightCBuffer
{
	Vector4	vDif;
	Vector4	vAmb;
	Vector4 vSpe;
	Vector4 vEmi;
	LIGHT_TYPE eType;
	Vector3	vPos;
	float	fRange;
	Vector3	vDir;
	float	fAngleIn;
	Vector3	vAttn;
	float	fAngleOut;
	float	fLength;
	Vector2	vPerspectiveValues;
	Matrix	matTransform;
	Matrix	matViewProj[6];
}LIGHTCBUFFER,*PLIGHTCBUFFER;

typedef struct _tagCBuffer
{
	ID3D11Buffer*	pBuffer;
	int				iSize;
	int				iRegister;
	int				iType;
}CBuffer,*PCBuffer;

typedef struct _tagDistortCBuffer
{
	float	fWeight;
	float	fSpeed;
	float	fFrequency;
	float	fE;
	_tagDistortCBuffer() :
		fWeight(0.f)
		, fSpeed(0.f)
		, fFrequency(0.f)
		, fE(0.f)
	{
	}
}DISTORTCBUFFER, * PDISTORTCBUFFER;

typedef struct alignas(16) _tagGlobalCBuffer
{
	float	fDeltaTime;
	float	fAccTime;
	Vector2 vNoiseSize;
	float	fSampleDist;
	float	fSampleStrength;
	Vector2	vBlurCenter;
}GLOBALCBUFFER, *PGLOBALCBUFFER;

typedef struct __declspec(align(16)) _tagParticleCBuffer
{
	Vector3 vPos;
	int		iSpawn;
	Vector3 vAccel;
	float	fMaxTime;
	Vector4 vMaxColor;
	Vector4 vMinColor;
	Vector3 vMaxScale;
	float	fMinTime;
	Vector3	vMinScale;
	float	fAngleAccel;
	Vector3	vRange;
	int		bMove;
	int		iMaxCount;
	float	fMaxSpeed;
	float	fMinSpeed;
	int		iMaxFrame;
	Vector2	vImageSize;
	int		iCountX;
	int		iCountY;
}PARTICLECBUFFER, *PPARTICLECBUFFER;

typedef struct _tagParticleSingle
{
	Vector4			vColor;
	Vector3			vPos;
	unsigned int	iLive;
	Vector3			vSpeed;
	float			fTime;
	Vector3			vScale;
	float			fAngle;
	float			fAngleSpeed;
	float			fMaxTime;
	int				iNumber;
	int				iFrame;
}PARTICLE, *PPARTICLE;

typedef struct _tagParticleShare
{
	int		iSpawnCount;
	Vector3 vMaxScale;
	int		iCurCount;
	Vector3 vMinScale;
	Vector4 vMaxColor;
	Vector4 vMinColor;
	int		iMaxFrame;
	Vector2	vImageSize;
	int		iCountX;
	int		iCountY;
}PARTICLESHARE,*PPARTICLESHARE;

typedef struct _declspec(align(16)) _tagAnimationCBuffer
{
	int		iFrameCount;
	int		iFrame;
	int		iNextFrame;
	float	fRatio;
	int		iBoneCount;
	int		iTransition;
	float	fTransitionRatio;
	int		iNextFrameCount;
	int		iInstanceIndex;
	float	fBlendRate;
	int		iBlendFrame;
	float	fBlendRatio;
	int		iBlendFrameCount;
}ANIMATIONCBUFFER, *PANIMATIONCBUFFER;

typedef struct alignas(16) _tagTerrainCBuffer
{
	float	fDetailX;
	float	fDetailY;
	int		iCount;
	float	fMinDist;

	float	fMaxDist;
	float	gMinTess;
	float	gMaxTess;
	float	gTexelCellSpaceU;

	float	gTexelCellSpaceV;
	float	gWorldCellSpace;
	Vector2	g_EmptyT;

	Vector4	gWorldFrustumPlanes[6];
}TERRAINCBUFFER, *PTERRAINCBUFFER;

typedef struct alignas(16) _tagDecalCBuffer
{
	Matrix	matInvWVP;
	float	fAlpha;
	float	fRadius;
}DECALCBUFFER, *PDECALCBUFFER;

typedef struct _tagHierarchy
{
	std::string strTag;
	std::string strParentTag;
}Hierarchy,*PHierarchy;

typedef struct _tagRectInfo
{
	float fL;
	float fR;
	float fT;
	float fB;

	_tagRectInfo() :
		fL(0.f),
		fR(0.f),
		fT(0.f),
		fB(0.f)
	{
	}

	const _tagRectInfo operator-(const _tagRectInfo& tRC)
	{
		_tagRectInfo _tRC;

		_tRC.fL = fL - tRC.fL;
		_tRC.fR = fR - tRC.fR;
		_tRC.fT = fT - tRC.fT;
		_tRC.fB = fB - tRC.fB;

		return _tRC;
	}

}RectInfo, *PRectInfo;

typedef struct _tagLineInfo
{
	Vector3		vStart;
	Vector3		vEnd;
}LineInfo, *PLineInfo;

typedef struct _tagRayInfo
{
	Vector3		vOrigin;
	Vector3		vDir;
}RAYINFO, *PRAYINFO;

typedef struct _tagSphereInfo
{
	Vector3		vCenter;
	float		fRadius;

	_tagSphereInfo() :
		fRadius(0.f)
	{
	}
}SphereInfo, *PSphereInfo;

typedef struct _tagTerrainInfo
{
	std::vector<Vector3>	vecPos;
	int	iWidth;
	int iHeight;
	Vector3 vScale;
	Vector3 vPos;
}TERRAININFO, *PTERRAININFO;

typedef struct _tagOBBInfo
{
	Vector3 vPos;
	Vector3	vAxis[AXIS_END];
	float fLength[AXIS_END];

	_tagOBBInfo()	:
		vPos()
	{
		for (int i = 0; i < AXIS_END; ++i)
			vAxis[i] = Vector3::Axis[i];

		memset(fLength, 0, 4 * AXIS_END);
	}
}OBBInfo, *POBBInfo;

typedef struct _tagCapsuleInfo
{
	float	fRadius;
	float	fHeight;
	Vector3	vCenter;
	Vector3	vDir;
}CAPSULEINFO, *PCAPSULEINFO;

typedef struct _tagColorInfo
{
	unsigned char r;
	unsigned char g;
	unsigned char b;
	unsigned char a;

	_tagColorInfo() :
		r(0),
		g(0),
		b(0),
		a(0)
	{
	}
}ColorInfo, * PColorInfo;

typedef struct _tagPixelInfo
{
	PColorInfo				pPixel;
	size_t					iWidth;
	size_t					iHeight;
	int						iSize;
	PIXEL_COLLISION_TYPE	eType;
	ColorInfo				tColor;
	ColorInfo				tCheckColor;
	RectInfo				tRt;

	_tagPixelInfo() :
		pPixel(nullptr),
		iWidth(0),
		iHeight(0),
		iSize(4),
		eType(PIXEL_COLLISION_TYPE::COLOR_IGNORE),
		tRt()
	{
		memset(&tColor, 0, sizeof(ColorInfo));
		memset(&tCheckColor, 0, sizeof(ColorInfo));
	}

	~_tagPixelInfo()
	{
		delete[] pPixel;
	}
}PixelInfo, *PPixelInfo;

typedef struct _tagCollisionChannel
{
	std::string				strTag;
	COLLISION_CHANNEL		eChannel;
	COLLISION_CHANNEL_TYPE	eType;
}CollisionChannel, * PCollisionChannel;

typedef struct _tagCollisionProfile
{
	std::string						strTag;
	COLLISION_CHANNEL				eChannel;
	std::vector<CollisionChannel>	vecChannel;
}CollisionProfile, * PCollisionProfile;

typedef struct _tagInstancingBuffer
{
	ID3D11Buffer*	pBuffer;
	void*			pData;
	int				iCount;
	int				iSize;
	D3D11_USAGE		eUsg;

	_tagInstancingBuffer() :
		pBuffer(nullptr),
		pData(nullptr),
		iCount(0),
		iSize(0),
		eUsg(D3D11_USAGE_DEFAULT)
	{

	}
}InstancingBuffer, *PInstancingBuffer;

typedef struct _tagInstancingData
{
	Matrix	matWVP;
	Vector3	vPivot;
	int		iFrame;
	Vector4	vColor;
	Vector3 vMeshSize;
}INSTANCINGDATA, * PINSTANCINGDATA;

typedef struct _tagInstancingAnim2D
{
	Matrix	matWVP;
	Vector3	vPivot;
	Vector3 vMeshSize;
	Vector2 vTexSize;
	Vector2 vStart;
	Vector2 vEnd;
}INSTANCINGANIM2D, * PINSTANCINGANIM2D;

typedef struct _tagInstancingCollider
{
	Matrix	matWVP;
	Vector4 vColor;
	Vector3	vPivot;
	Vector3 vMeshSize;
}INSTANCINGCOLLIDER, * PINSTANCINGCOLLIDER;

typedef struct _tagParticle
{
	Vector3 InitialPos;
	Vector3 InitialVel;
	Vector2 Size;
	float Age;
	unsigned int Type;
}Particle, *PParticle;

typedef struct _tagBarCBuffer
{
	int		iDir;
	float	fPrg;
	Vector2	v;
}BARCBUFFER, *PBARCBUFFER;

typedef struct _tagFrameInfo
{
	Vector2	vStart;
	Vector2	vEnd;
}FrameInfo, * PFrameInfo;

struct deleter
{
	void operator()(IUnknown* pSRV)
	{
		if (pSRV)
		{
			pSRV->Release();

			pSRV = nullptr;
		}
			
	}
};

typedef struct _tagHemiConstants
{
	Vector3	vAmbientDown;
	Vector3	vAmbientRange;
}HEMICONSTANTS, *PHEMICONSTANTS;

typedef struct _tagStructuredBuffer
{
	ID3D11Buffer*				pBuffer;
	ID3D11ShaderResourceView*	pSRV;
	ID3D11UnorderedAccessView*	pUAV;
	int							iCount;
	int							iSize;
	bool						bDynamic;
	int							iShaderType;
	D3D11_BUFFER_DESC			tDesc;

	_tagStructuredBuffer()	:
		pBuffer(nullptr)
		, pSRV(nullptr)
		, pUAV(nullptr)
		, iCount(0)
		, iSize(0)
		, bDynamic(false)
		, iShaderType(0)
		, tDesc()
	{
	}
	~_tagStructuredBuffer()
	{
		SAFE_RELEASE(pBuffer);
		SAFE_RELEASE(pSRV);
		SAFE_RELEASE(pUAV);
	}
}STRUCTUREDBUFFER, *PSTRUCTUREDBUFFER;

typedef struct _tagAnimationKeyFrame
{
	Vector4	vPos;
	Vector4 vScale;
	Vector4 vRot;
}ANIMATIONKEYFRAME, *PANIMATIONKEYFRAME;

typedef struct _tagOverPart
{
	OVERLAPPED		tOverlapped;
	int				iCount;
	DWORD			iRecvCount;
	DWORD			iFlag;
	WORK_TYPE		eType;
	WSABUF			tBuf;
	SOCKET			hSocket;
	SOCKADDR_IN		tAddr;
	char*			pPacket;
	class CUser* pUser;
}OVERPART, * POVERPART;