
struct PS_OUTPUT_SINGLE
{
	float4 vColor	:	SV_TARGET;
};

struct PS_OUTPUT_MULTI
{
	float4	LightDif	:	SV_TARGET;
	float4	LightSpc	:	SV_TARGET1;
	float4	LightEmv	:	SV_TARGET2;
};

struct PS_OUTPUT_TARGET
{
	float4 Albedo	:	SV_TARGET;
	float4 GBuffer1	:	SV_TARGET1;
	float4 GBuffer2	:	SV_TARGET2;
	float4 GBuffer3	:	SV_TARGET3;
	float4 GBuffer4	:	SV_TARGET4;
	float4 GBuffer5	:	SV_TARGET5;
};

struct VS_INPUT_3D
{
	float3	vPos	:	POSITION;
	float3	vNormal	:	NORMAL;
	float2	vUV	:	TEXCOORD;
	float3	vTangent	:	TANGENT;
	float3	vBinormal	:	BINORMAL;
	float4	vBlendWeight	:	BLENDWEIGHT;
	float4	vBlendIndex	:	BLENDINDICES;
};

struct VS_INPUT_INST_3D
{
	float3	vPos			:	POSITION;
	float3	vNormal			:	NORMAL;
	float2	vUV				:	TEXCOORD;
	float3	vTangent		:	TANGENT;
	float3	vBinormal		:	BINORMAL;
	float4	vBlendWeight	:	BLENDWEIGHT;
	float4	vBlendIndex		:	BLENDINDICES;
	matrix	matWVP			:	WVP;
	matrix	matWV			:	WV;
	matrix	matShadowWVP	:	SHADOW;
	matrix	matWorld		:	WORLD;
};

struct VS_INPUT_INST_COLLIDER_3D
{
	float3	vPos			:	POSITION;
	float3	vNormal			:	NORMAL;
	float2	vUV				:	TEXCOORD;
	float3	vTangent		:	TANGENT;
	float3	vBinormal		:	BINORMAL;
	float4	vBlendWeight	:	BLENDWEIGHT;
	float4	vBlendIndex		:	BLENDINDICES;
	matrix	matWVP			:	WVP;
	matrix	matWV			:	WV;
	matrix	matShadowWVP	:	SHADOW;
	float4	vColor			:	COLOR;
	matrix	matWorld		:	WORLD;
};

struct VS_OUTPUT_3D
{
	float4	vPos	:	SV_POSITION;
	float4	vProjPos	:	POSITION;
	float3	vPosW	:	POSITION1;
	float3	vPosV	:	POSITION2;
	float3	vNormal	:	NORMAL;
	float3	vTangent	:	TANGENT;
	float3	vBinormal	:	BINORMAL;
	float4	vBlendWeight	:	BLENDWEIGHT;
	float4	vBlendIndex	:	BLENDINDICES;
	float2	vUV	:	TEXCOORD;
	float4	vProjTex	:	TEXCOORD1;
	float4	vShadowPos	:	POSITION3;
	float3	vPosL		:	POSITION4;
};

struct VS_OUTPUT_3D_COLLIDER
{
	float4	vPos	:	SV_POSITION;
	float4	vProjPos	:	POSITION;
	float3	vPosW	:	POSITION1;
	float3	vPosV	:	POSITION2;
	float3	vNormal	:	NORMAL;
	float3	vTangent	:	TANGENT;
	float3	vBinormal	:	BINORMAL;
	float4	vBlendWeight	:	BLENDWEIGHT;
	float4	vBlendIndex	:	BLENDINDICES;
	float2	vUV	:	TEXCOORD;
	float4	vProjTex	:	TEXCOORD1;
	float4	vShadowPos	:	POSITION3;
	float3	vPosL		:	POSITION4;
	float4	vColor	:	COLOR;
};

cbuffer Transform : register(b0)
{
	matrix					g_matWorld;
	matrix					g_matView;
	matrix					g_matProj;
	matrix					g_matWV;
	matrix					g_matWVP;
	matrix					g_matWInvTrans;
	matrix					g_matTexTransform;
	matrix					g_matProjInv;
	matrix					g_matLightWorldViewProjTexture;
	matrix					g_matShadowViewProj;
	matrix					g_matViewInv;
	float3					g_vPivot;
	float					g_vE1;
	float3					g_vMeshSize;
	float					g_vE2;
};

cbuffer Material : register(b1)
{
	float4	g_vDif;
	float4	g_vAmb;
	float4	g_vSpe;
	float4	g_vEmi;
	int		g_iDifTex;
	int		g_iNrmTex;
	int		g_iSpecTex;
	int		g_iEmiTex;
	int		g_iBump;
	int		g_iAnimation;
	int		g_iDecal;
	float	g_vE3;
};

cbuffer Animation	:	register(b2)
{
	int		g_iImageType;
	float	g_fFrame;
	float2	g_vStart;
	float2	g_vEnd;
	float2	g_vSize;
};

cbuffer Fog	:	register(b3)
{
	float3 FogColor	:	packoffset(c0);
	float FogStartDepth : packoffset(c0.w);
	float3 FogHightlightColor :	packoffset(c1);
	float FogGlobalDensity : packoffset(c1.w);
	float3 FogSunDir	:	packoffset(c2);
	float FogHeightFallOff : packoffset(c2.w);
}

cbuffer Paper	:	register(b4)
{
	float	g_fFilter;
	float3	g_vOutColor;
	float3	g_vColor;
	float	g_fOutFilter;
	float3	g_vInColor;
	float	g_fMidFilter;
	float	g_fInFilter;
	int		g_iInverse;
	int		g_iPaperEnable;
}

cbuffer Distort	:	register(b5)
{
	float g_fWeight;
	float g_fSpeed;
	float g_fFrequency;
	float g_fED;
}

cbuffer Global : register(b6)
{
	float	g_fDeltaTime;
	float	g_fAccTime;
	float2	g_vNoiseSize;
	float	g_fSampleDist;
	float	g_fSampleStrength;
	float2	g_vBlurCenter;
}

cbuffer Light	:	register(b7)
{
	float4	g_vLightDif;
	float4	g_vLightAmb;
	float4	g_vLightSpc;
	float4	g_vLightEmi;
	int		g_iLightType;
	float3	g_vLightPos;
	float	g_vLightRange;
	float3	g_vLightDir;
	float	g_vLightAngleIn;
	float3	g_vLightAttn;
	float	g_vLightAngleOut;
	float	g_vLightLength;
	float2	g_vLightPerspectiveValues;
	matrix	g_matLightTransform;
	matrix	g_matLightViewProj[6];
}

cbuffer Particle	:	register(b8)
{
	float3	g_vWorldPos;
	int		g_iSpawn;
	float3	g_vAccel;
	float	g_fMaxTime;
	float4	g_vMaxColor;
	float4	g_vMinColor;
	float3	g_vMaxScale;
	float	g_fMinTime;
	float3	g_vMinScale;
	float	g_fAngleAccel;
	float3	g_vRange;
	bool	g_bMove;
	int		g_iMaxCount;
	float	g_fMaxSpeed;
	float	g_fMinSpeed;
	int		g_iParticleMaxFrame;
	float2	g_vParticleImageSize;
	int		g_iParticleCountX;
	int		g_iParticleCountY;
}

cbuffer BoneAnimation	:	register(b9)
{
	int		g_iAniFrameCount;
	int		g_iAniFrame;
	int		g_iAniNextFrame;
	float	g_fAniRatio;
	int		g_iAniBoneCount;
	int		g_iAniTransition;
	float	g_fAniTransitionRatio;
	int		g_iAniNextFrameCount;
	int		g_iInstIndex;
	float	g_fAniBlendRate;
	int		g_iAniBlendFrame;
	float	g_fAniBlendRatio;
	int		g_iAniBlendFrameCount;
}

cbuffer Bar	:	register(b10)
{
	int		g_iDir;
	float	g_fPrg;
	float	g_fMaxTessDistance;
	float	g_fMinTessDistance;
	float	g_fMinTessFactor;
	float	g_fMaxTessFactor;
}

cbuffer Terrain	:register(b11)
{
	float	g_fDetailLevelX;
	float	g_fDetailLevelY;
	int		g_iSplatCount;
	float	gMinDist;

	float	gMaxDist;
	float	gMinTess;
	float	gMaxTess;
	float	gTexelCellSpaceU;

	float	gTexelCellSpaceV;
	float	gWorldCellSpace;
	float2	g_EmptyT;

	float4	gWorldFrustumPlanes[6];
}

struct DirectionalLight
{
	float4 Ambient;
	float4 Diffuse;
	float4 Specular;
	float3 Direction;
	float pad;
};

struct PointLight
{
	float4 Ambient;
	float4 Diffuse;
	float4 Specular;

	float3 Position;
	float Range;

	float3 Att;
	float pad;
};

struct SpotLight
{
	float4 Ambient;
	float4 Diffuse;
	float4 Specular;
	float3 Position;
	float Range;

	float3 Direction;
	float Spot;

	float3 Att;
	float pad;
};
cbuffer Debug	:	register(b12)
{
	matrix	g_matDebugWVP;
}


SamplerState g_tPoint	:	register(s0);
SamplerState g_tLinear	:	register(s1);
SamplerState g_tAnisotropic	:	register(s2);
SamplerState g_tProjLinear	:	register(s3);
SamplerComparisonState g_tShadow	:	register(s4);

Texture2D g_tTex	:	register(t0);
Texture2D g_NrmTex	:	register(t1);
Texture2D g_SpcTex	:	register(t2);
Texture2D g_EmiTex	:	register(t3);

Texture2DArray	g_ArrayTex	:	register(t30);
Texture2DArray	g_ArrayNrmTex	:	register(t31);
Texture2DArray	g_ArraySpcTex	:	register(t32);
Texture2DArray	g_ArrayEmvTex	:	register(t33);

Texture2D g_ShadowMap	:	register(t40);

TextureCube gCubeMap	:	register(t50);
TextureCube gCubeMap2	:	register(t51);
TextureCube gProjLightTex	:	register(t52);

Texture2D g_NoiseTex	:	register(t98);
Texture2D g_BurnTex	:	register(t99);

Texture2D g_DistortNormal	:	register(t101);
Texture2D g_DistortFilter	:	register(t102);
Texture2D g_Scene	:	register(t103);
Texture2D g_Proj	:	register(t104);
Texture2D g_ShadowTexture	:	register(t105);
TextureCube<float> g_PointShadowMapTexture	:	register(t106);

#define ATLAS	0
#define ARRAY	1
#define TEXTUREARRAY 2

float2 ComputeUV(float2 vUV)
{
	if (g_iImageType != ATLAS)
		return vUV;

	float2 output = (float2)0.f;

	if (vUV.x > 0.f)
		output.x = g_vEnd.x / g_vSize.x;

	else
		output.x = g_vStart.x / g_vSize.x;

	if (vUV.y > 0.f)
		output.y = g_vEnd.y / g_vSize.y;

	else
		output.y = g_vStart.y / g_vSize.y;

	return output;
}

float2 GetUV(float2 vUV, float2 vSize, float2 vStart, float2 vEnd)
{
	float2 output = (float2)0.f;

	if (vUV.x > 0.f)
		output.x = vEnd.x / vSize.x;

	else
		output.x = vStart.x / vSize.x;

	if (vUV.y > 0.f)
		output.y = vEnd.y / vSize.y;

	else
		output.y = vStart.y / vSize.y;

	return output;
}

float4 PaperBurn(float4 vColor, float2 vUV)
{
	if (!g_iPaperEnable)
		return vColor;

	float4 vPaperColor = g_BurnTex.Sample(g_tPoint, vUV);

	float fFilter = vPaperColor.r + g_fFilter;

	if (fFilter >= g_fOutFilter)
	{
		clip(-1);
		vColor = float4(g_vOutColor, 0.f);
	}

	else if (fFilter > g_fMidFilter)
	{
		vColor = float4(g_vColor, vColor.w);
	}

	else if (fFilter > g_fInFilter)
	{
		vColor = float4(g_vInColor, vColor.w);
	}

	return vColor;
}

float ChangeSpecFactor(float fSpec)
{
	if (fSpec <= 0.1f)
	{
		return 0.f;
	}

	else if (fSpec <= 0.2f)
	{
		return 0.5f;
	}

	else
	{
		return 0.7f;
	}
}

float ChangeDiffuseFactor(float fDiff)
{
	if (fDiff <= 0.f)
	{
		return 0.1f;
	}

	else if (fDiff <= 0.1f)
	{
		return 0.5f;
	}

	else
	{
		return 1.0f;
	}
}

float3 ComputeBumpNormal(float3 vNormal, float3 vTangent, float3 vBinormal, float2 vUV)
{
	float3 vResult = vNormal;

	if (g_iBump)
	{
		float4 vNormalColor = g_NrmTex.Sample(g_tAnisotropic, vUV);

		vNormalColor.xyz = vNormalColor.xyz * 2.f - 1.f;

		float3x3 mat =
		{
			vTangent,
			vBinormal,
			vNormal
		};

		vResult = normalize(mul(vNormalColor.xyz, mat));
	}

	return vResult;
}

float3 ComputeBumpNormalOrtho(float3 vNormal, float3 vTangent, float3 vBinormal, float2 vUV)
{
	float3 vResult = vNormal;

	if (g_iBump)
	{
		float4 vNormalColor = g_NrmTex.Sample(g_tAnisotropic, vUV);

		vNormalColor.xyz = vNormalColor.xyz * 2.f - 1.f;

		float3x3 mat =
		{
			vTangent,
			vBinormal,
			vNormal
		};

		vResult = normalize(mul(vNormalColor.xyz, mat));
	}

	return vResult;
}
float3 ComputeBumpNormalArray(float3 vNormal, float3 vTangent, float3 vBinormal, 
	float2 vUV, int iCount, float pAlpha[8])
{
	float3 vResult = vNormal;

	if (g_iBump)
	{
		float4 vNormalColor = g_ArrayNrmTex.Sample(g_tAnisotropic, float3(vUV, 0));
		for (int i = 0; i < iCount - 1; i++)
		{
			vNormalColor = lerp(vNormalColor, g_ArrayNrmTex.Sample(g_tAnisotropic, float3(vUV, i+1)), pAlpha[i]);
		}

		float3 vConvert = vNormalColor.xyz * 2.f - 1.f;

		vConvert = normalize(vConvert);

		float3x3 mat =
		{
			vTangent,
			vBinormal,
			vNormal
		};

		vResult = normalize(mul(vConvert, mat));
	}

	return vResult;
}

#define DIR_LIGHT		0
#define POINT_LIGHT		1
#define SPOT_LIGHT		2
#define CAPSULE_LIGHT	3
#define PROJ_POINT_LIGHT	4

struct LightResult
{
	float4 vDif;
	float4 vAmb;
	float4 vSpc;
	float4 vEmi;
};

LightResult ComputeLight(float3 vPos, float3 vNormal, float3 vTangent,
	float3 vBinormal, float2 vUV)
{
	LightResult tResult = (LightResult)0.f;

	float fLight = 0.f;

	float d = 0.f;

	float3 vLightDir = (float3)0.f;

	float fIntensity = 1.f;

	float3 vView = -vPos;
	vView = normalize(vView);

	float4 vEmi = g_vEmi;

	if (g_iEmiTex)
	{
		vEmi = g_EmiTex.Sample(g_tAnisotropic, vUV);
	}
	tResult.vEmi = vEmi;

	if (g_iLightType == DIR_LIGHT)
	{
		vLightDir = -g_vLightDir;
		normalize(vLightDir);
	}

	else if (g_iLightType == POINT_LIGHT || g_iLightType == PROJ_POINT_LIGHT)
	{
		vLightDir = g_vLightPos - vPos;

		d = length(vLightDir);

		if (d > g_vLightRange)
			return tResult;

		fIntensity = 1.f / (dot(float3(1.f, d, d * d), g_vLightAttn));// float3(1.f, 2 / g_vLightRange, 1 / g_vLightRange / g_vLightRange)));

		vLightDir /= d;
	}

	else if (g_iLightType == SPOT_LIGHT)
	{
		vLightDir = -g_vLightDir;
		d = length(vLightDir);

		vLightDir /= d;

		float3 ToLight = g_vLightPos - vPos;
		ToLight = normalize(ToLight);

		float conAng = dot(vLightDir, ToLight);
		float conAtt = saturate((conAng - g_vLightAngleOut) / (-g_vLightAngleOut + g_vLightAngleIn));
		conAtt *= conAtt;

		float DistToLightNorm = 1.f - saturate(d / g_vLightRange);
		fIntensity = DistToLightNorm * DistToLightNorm * conAtt;
	}

	else if (g_iLightType == CAPSULE_LIGHT)
	{
		float3 ToCapsuleStart = vPos - g_vLightPos;
		float DistOnLine = dot(ToCapsuleStart, g_vLightDir) / g_vLightLength;
		DistOnLine = saturate(DistOnLine) * g_vLightLength;
		float3 PointOnLine = g_vLightPos + g_vLightDir * DistOnLine;
		vLightDir = PointOnLine - vPos;
		d = length(vLightDir);
		vLightDir /= d;

		float DistToLightNorm = 1.f - saturate(d / g_vLightRange);
		fIntensity = DistToLightNorm * DistToLightNorm;
	}

	float3 vViewNormal = ComputeBumpNormal(vNormal, vTangent, vBinormal, vUV);

	fLight = max(0.f, dot(vViewNormal, vLightDir));

	tResult.vDif = g_vLightDif * g_vDif * fLight * fIntensity;
	tResult.vAmb = g_vLightAmb * g_vAmb * fIntensity;

	float3 vReflect = dot(vViewNormal, vLightDir) * vViewNormal * 2.f - vLightDir;

	vReflect = normalize(vReflect);

	float4 vMtrlSpc = g_vSpe;

	if (g_iSpecTex)
	{
		vMtrlSpc = g_SpcTex.Sample(g_tAnisotropic, vUV);
	}
	vMtrlSpc.w = 1.f;

	tResult.vSpc = g_vLightSpc * vMtrlSpc * pow(max(0.f, dot(vView, vReflect)), g_vSpe.w) * fIntensity;

	return tResult;
}

LightResult ComputeLightEmvRClip(float3 vPos, float3 vNormal, float3 vTangent,
	float3 vBinormal, float2 vUV)
{
	LightResult tResult = (LightResult)0.f;
	float4 vEmi = g_vEmi;

	if (g_iEmiTex)
	{
		float4 vClip = g_EmiTex.Sample(g_tAnisotropic, vUV);

		clip(vClip.r - 0.15f);
	}

	tResult.vEmi = vEmi;


	float fLight = 0.f;

	float d = 0.f;

	float3 vLightDir = (float3)0.f;

	float fIntensity = 1.f;

	float3 vView = -vPos;
	vView = normalize(vView);

	if (g_iLightType == DIR_LIGHT)
	{
		vLightDir = -g_vLightDir;
		normalize(vLightDir);
	}

	else if (g_iLightType == POINT_LIGHT || g_iLightType == PROJ_POINT_LIGHT)
	{
		vLightDir = g_vLightPos - vPos;

		d = length(vLightDir);

		if (d > g_vLightRange)
			return tResult;

		fIntensity = 1.f / (dot(float3(1.f, d, d * d), g_vLightAttn));// float3(1.f, 2 / g_vLightRange, 1 / g_vLightRange / g_vLightRange)));

		vLightDir /= d;
	}

	else if (g_iLightType == SPOT_LIGHT)
	{
		vLightDir = -g_vLightDir;
		d = length(vLightDir);

		vLightDir /= d;

		float3 ToLight = g_vLightPos - vPos;
		ToLight = normalize(ToLight);

		float conAng = dot(vLightDir, ToLight);
		float conAtt = saturate((conAng - g_vLightAngleOut) / (-g_vLightAngleOut + g_vLightAngleIn));
		conAtt *= conAtt;

		float DistToLightNorm = 1.f - saturate(d / g_vLightRange);
		fIntensity = DistToLightNorm * DistToLightNorm * conAtt;
	}

	else if (g_iLightType == CAPSULE_LIGHT)
	{
		float3 ToCapsuleStart = vPos - g_vLightPos;
		float DistOnLine = dot(ToCapsuleStart, g_vLightDir) / g_vLightLength;
		DistOnLine = saturate(DistOnLine) * g_vLightLength;
		float3 PointOnLine = g_vLightPos + g_vLightDir * DistOnLine;
		vLightDir = PointOnLine - vPos;
		d = length(vLightDir);
		vLightDir /= d;

		float DistToLightNorm = 1.f - saturate(d / g_vLightRange);
		fIntensity = DistToLightNorm * DistToLightNorm;
	}

	float3 vViewNormal = ComputeBumpNormal(vNormal, vTangent, vBinormal, vUV);

	fLight = max(0.f, dot(vViewNormal, vLightDir));

	tResult.vDif = g_vLightDif * g_vDif * fLight * fIntensity;
	tResult.vAmb = g_vLightAmb * g_vAmb * fIntensity;

	float3 vReflect = dot(vViewNormal, vLightDir) * vViewNormal * 2.f - vLightDir;

	vReflect = normalize(vReflect);

	float4 vMtrlSpc = g_vSpe;

	if (g_iSpecTex)
	{
		vMtrlSpc = g_SpcTex.Sample(g_tAnisotropic, vUV);
	}
	vMtrlSpc.w = 1.f;

	tResult.vSpc = g_vLightSpc * vMtrlSpc * pow(max(0.f, dot(vView, vReflect)), g_vSpe.w) * fIntensity;

	return tResult;
}

float ConvertColor(float4 vColor)
{
	uint4 _vColor = 0;
	_vColor.x = (uint)(vColor.x * 255);
	_vColor.y = (uint)(vColor.y * 255);
	_vColor.z = (uint)(vColor.z * 255);
	_vColor.w = (uint)(vColor.w * 255);

	uint iColor = 0;

	iColor = _vColor.x;
	iColor = (iColor << 8) | _vColor.y;
	iColor = (iColor << 8) | _vColor.z;
	iColor = (iColor << 8) | _vColor.w;

	return asfloat(iColor);
}

float4 ConvertColor(float fColor)
{
	uint iColor = asuint(fColor);

	float4 vColor = 0.f;

	vColor.x = ((iColor >> 24) & 0xff) / 255.f;
	vColor.y = ((iColor >> 16) & 0xff) / 255.f;
	vColor.z = ((iColor >> 8) & 0xff) / 255.f;
	vColor.w = (iColor & 0xff) / 255.f;

	return vColor;
}

LightResult ComputeLightAcc(float3 vPos, float3 vNormal, float2 vUV,float4 vMtrlColor, float fSpecPower)
{
	LightResult tResult = (LightResult)0.f;

	float fLight = 0.f;

	float d = 0.f;

	float3 vLightDir = (float3)0.f;

	float fIntensity = 1.f;

	if (g_iLightType == DIR_LIGHT)
	{
		vLightDir = -g_vLightDir;
		vLightDir = normalize(vLightDir);
	}

	else if (g_iLightType == POINT_LIGHT || 
		g_iLightType == PROJ_POINT_LIGHT)
	{
		vLightDir = g_vLightPos - vPos;

		d = length(vLightDir);

		if (d > g_vLightRange)
			return tResult;

		fIntensity = 1 /  (dot(float3(1.f, d, d * d), g_vLightAttn));
			//float3(1.f, 2 / g_vLightRange, 1 / g_vLightRange / g_vLightRange)));

		vLightDir /= d;
	}

	else if (g_iLightType == SPOT_LIGHT)
	{
		vLightDir = -g_vLightDir;
		d = length(vLightDir);

		vLightDir /= d;

		float3 ToLight = g_vLightPos - vPos;
		ToLight = normalize(ToLight);

		float conAng = dot(vLightDir, ToLight);
		float conAtt = saturate((conAng - g_vLightAngleOut) / (-g_vLightAngleOut + g_vLightAngleIn));
		conAtt *= conAtt;

		float DistToLightNorm = 1.f - saturate(d / g_vLightRange);
		fIntensity = DistToLightNorm * DistToLightNorm * conAtt;
	}

	else if (g_iLightType == CAPSULE_LIGHT)
	{
		float3 ToCapsuleStart = vPos - g_vLightPos;
		float DistOnLine = dot(ToCapsuleStart, g_vLightDir) / g_vLightLength;
		DistOnLine = saturate(DistOnLine) * g_vLightLength;
		float3 PointOnLine = g_vLightPos + g_vLightDir * DistOnLine;
		vLightDir = PointOnLine - vPos;
		d = length(vLightDir);
		vLightDir /= d;

		float DistToLightNorm = 1.f - saturate(d / g_vLightRange);
		fIntensity = DistToLightNorm * DistToLightNorm;
	}

	fLight = max(0.f, dot(vNormal, vLightDir));

	tResult.vDif = g_vLightDif * ConvertColor(vMtrlColor.x) * fLight * fIntensity;
	tResult.vAmb = g_vLightAmb * ConvertColor(vMtrlColor.y) * fIntensity;

	float3 vView = -vPos;
	vView = normalize(vView);

	float3 vReflect = dot(vNormal, vLightDir) * vNormal * 2.f - vLightDir;

	vReflect = normalize(vReflect);

	float4 vMtrlSpc = ConvertColor(vMtrlColor.z);

	tResult.vSpc = g_vLightSpc* vMtrlSpc* pow(max(0.f, dot(vView, vReflect)), fSpecPower)* fIntensity;

	tResult.vSpc.r = saturate(tResult.vSpc.r);
	tResult.vSpc.g = saturate(tResult.vSpc.g);
	tResult.vSpc.b = saturate(tResult.vSpc.b);
	tResult.vSpc.a = saturate(tResult.vSpc.a);

	tResult.vEmi = g_vLightEmi * ConvertColor(vMtrlColor.w);

	return tResult;
}


static float gGaussianFilter5x5[25] =
{
	0.003765, 0.015019, 0.023792, 0.015019, 0.003765,
	0.015019, 0.059912, 0.094907, 0.059912, 0.015019,
	0.023792, 0.094907, 0.150342, 0.094907, 0.023792,
	0.015019, 0.059912, 0.094907, 0.059912, 0.015019,
	0.003765, 0.015019, 0.023792, 0.015019, 0.003765
};

float gaussian5x5Sample(int2 vUV, Texture2D gTex)
{
	float4 vOut = (float4)0.f;

	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			vOut += gTex[vUV + int2(i - 2, j - 2)] * gGaussianFilter5x5[j * 5 + i];
		}
	}

	return vOut.x;
}

float Rand(in float fRand)
{
	float2 vUV = float2(cos(fRand + g_fAccTime), g_fAccTime);

	vUV.y += sin(fRand + g_fAccTime);

	if (vUV.x > 0)
		vUV.x = frac(vUV.x);

	else
		vUV.x = frac(-vUV.x);
		

	if (vUV.y > 0)
		vUV.y = frac(vUV.y);

	else
		vUV.y = frac(-vUV.y);

	vUV *= g_vNoiseSize;

	return gaussian5x5Sample((int2)vUV, g_NoiseTex);
}

StructuredBuffer<matrix> g_matFinal	:	register(t110);

matrix GetBoneMatrix(int iIndex)
{
	return g_matFinal[iIndex];
}

struct Skinning
{
	float3 vPos;
	float3 vNormal;
	float3 vTangent;
	float3 vBinormal;
};

Skinning Skining(float3 vPos, float3 vNormal, float3 vTangent, float3 vBinormal, 
	float4 vBlendWeight, float4 vBlendIndex)
{
	Skinning tSkin = (Skinning)0.f;

	if (!g_iAnimation)
	{
		tSkin.vPos = vPos;
		tSkin.vNormal = vNormal;
		tSkin.vTangent = vTangent;
		tSkin.vBinormal = vBinormal;

		return tSkin;
	}

	for (int i = 0; i < 4; i++)
	{
		matrix matBone = GetBoneMatrix(vBlendIndex[i]);

		tSkin.vPos += (mul(float4(vPos, 1.f), matBone) * vBlendWeight[i]).xyz;
		tSkin.vNormal += (mul(float4(vNormal, 0.f), matBone) * vBlendWeight[i]).xyz;
		tSkin.vTangent += (mul(float4(vTangent, 0.f), matBone) * vBlendWeight[i]).xyz;
		tSkin.vBinormal += (mul(float4(vBinormal, 0.f), matBone) * vBlendWeight[i]).xyz;
	}

	tSkin.vNormal = normalize(tSkin.vNormal);
	tSkin.vTangent = normalize(tSkin.vTangent);
	tSkin.vBinormal = normalize(tSkin.vBinormal);

	return tSkin;
}

Skinning Skining(float3 vPos, float3 vNormal, float3 vTangent, 
	float3 vBinormal, float4 vBlendWeight, float4 vBlendIndex, uint iID)
{
	Skinning tSkin = (Skinning)0.f;

	if (!g_iAnimation)
	{
		tSkin.vPos = vPos;
		tSkin.vNormal = vNormal;
		tSkin.vTangent = vTangent;
		tSkin.vBinormal = vBinormal;

		return tSkin;
	}

	for (int i = 0; i < 4; i++)
	{
		matrix matBone = GetBoneMatrix(vBlendIndex[i] + iID * g_iAniBoneCount);

		tSkin.vPos += (mul(float4(vPos, 1.f), matBone) * vBlendWeight[i]).xyz;
		tSkin.vNormal += (mul(float4(vNormal, 0.f), matBone) * vBlendWeight[i]).xyz;
		tSkin.vTangent += (mul(float4(vTangent, 0.f), matBone) * vBlendWeight[i]).xyz;
		tSkin.vBinormal += (mul(float4(vBinormal, 0.f), matBone) * vBlendWeight[i]).xyz;
	}

	tSkin.vNormal = normalize(tSkin.vNormal);
	tSkin.vTangent = normalize(tSkin.vTangent);
	tSkin.vBinormal = normalize(tSkin.vBinormal);

	return tSkin;
}

static const float SMAP_SIZE = 2048.f;
static const float SMAP_DX = 1.f / SMAP_SIZE;

float CalcShadowFactor(SamplerComparisonState samShadow,
	Texture2D shadowMap,
	float4 shadowPosH)
{
	shadowPosH.xyz /= shadowPosH.w;
	float depth = shadowPosH.z;

	const float dx = SMAP_DX;

	float percentLit = 0.f;
	const float2 offsets[9] =
	{
		float2(-dx,-dx),float2(0.f, -dx),float2(dx,-dx),
		float2(-dx, 0.f), float2(0.f,0.f),float2(dx,0.f),
		float2(-dx,dx),float2(0.f,dx),float2(dx,dx)
	};

	[unroll]
	for (int i = 0; i < 9; i++)
	{
		percentLit += shadowMap.SampleCmpLevelZero(samShadow,
			shadowPosH.xy + offsets[i], depth).r;
	}

	return percentLit /= 9.f;
}
// ¸ÊºÎÅÍ Âï¾î
float3 ApplyFog(float3 originalColor, float eyePosY, float3 eyeToPixel)
{
	/*float pixelDist = length(eyeToPixel);
	float3 eyeToPixelNorm = eyeToPixel / pixelDist;

	float fogDist = max(pixelDist - FogStartDist, 0.f);

	float fogHeightDensityAtViewer = exp(-FogHeightFalloff * eyePosY);
	float fogDistInt = fogDist * fogHeightDensityAtViewer;

	float eyeToPixelY = eyeToPixel.y * (fogDist / pixelDist);
	float t = FogHeightFalloff * eyeToPixelY;
	const float thresholdT = 0.01f;
	float fogHeightInt = abs(t) > thresholdT ?
		(1.f - exp(-t)) / t : 1.f;

	float fogFinalFactor = exp(-FogGlobalDensity * fogDistInt * fogHeightInt);

	float sunHeightlightFactor = saturate(dot(eyeToPixelNorm, FogSunDir));

	sunHighlightFactor = pow(sunHighlightFactor, 8.0);
	float3 fogFinalColor = lerp(FogColor, FogHightlightColor,
		sunHighlightFactor);

	return lerp(fogFinalColor, originalColor, fogFinalFactor);*/
	return 0.f;
}

float4 DirLightPS(VS_OUTPUT_3D In) : SV_TARGET
{
	/*float2 uv = In.Position.xy; In.UV.xy;
	SURFACE_DATA gbd = UnpackGBuffer_Loc(int3(uv, 0));

	Material mat;
	MaterialFromGBuffer(gbd, mat);

	float2 cpPos = In.UV.xy * float2(2.0, -2.0) - float2(1.0, -1.0);
	float3 position = CalcWorldPos(cpPos, gbd.LinearDepth);

	float ao = AOTexture.Sample(LinearSampler, In.UV);

	float4 finalColor;
	finalColor.xyz = CalcAmbient(mat.normal, mat.diffuseColor.xyz) * ao;
	finalColor.xyz += CalcDirectional(position, mat);
	finalColor.w = 1.f;

	float3 eyeToPixel = position - EyePosition;
	finalColor.xyz = ApplyFog(finalColor.xyz, EyePosition.y,
		eyeToPixel);

	return finalColor;*/
	return 0.f;
}


float PointShadowPCF(float3 ToPixel)
{
	float3 ToPixelAbs = abs(ToPixel);
	float Z = max(ToPixelAbs.x, max(ToPixelAbs.y, ToPixelAbs.z));
	float Depth = (g_vLightPerspectiveValues.x * Z + g_vLightPerspectiveValues.y) /Z;
	//Depth = pow(Depth, 1000);
	float fPCF = g_PointShadowMapTexture.SampleCmpLevelZero(g_tShadow, ToPixel, Depth);

	//float fNearDepth = g_PointShadowMapTexture.Sample(g_tPoint, ToPixel);

	/*float fShadow = 1.f;

	if (Depth > fNearDepth)
	{
		fShadow = 0.f;
	}*/

	return fPCF;
}