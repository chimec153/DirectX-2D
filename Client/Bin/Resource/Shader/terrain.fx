#include "Share.fx"

Texture2D	gHeightMap	:	register(t20);
Texture2DArray	gAlphaMapArray	:	register(t40);
SamplerState g_HeightSampler	:	register(s4);

struct VS_INPUT_T
{
	float3	vPos	:	POSITION;
	float3	vNormal	:	NORMAL;
	float2	vUV	:	TEXCOORD;
	float3	vTangent	:	TANGENT;
	float3	vBinormal	:	BINORMAL;
	float4	vBlendIndex	:	BLENDINDICES;
	float4	vBlendWeight	:	BLENDWEIGHT;
};

struct VS_OUTPUT_T
{
	float4	vPos	:	SV_POSITION;
	float4	vProjPos	:	POSITION;
	float3	vPosW	:	POSITION1;
	float2	vUV	:	TEXCOORD;
	float2	vTiledUV	:	TEXCOORD1;
	float3	vPosV	:	POSITION2;
	float4	vShadowProjPos	:	POSITION3;
	float3	vNormal	:	NORMAL;
	float3	vTangent	:	TANGENT;
	float3	vBinormal	:	BINORMAL;
};

VS_OUTPUT_T VS(VS_INPUT_T input)
{
	VS_OUTPUT_T output = (VS_OUTPUT_T)0.f;

	output.vUV = input.vUV;//mul(float4(input.vUV, 0.f, 1.f), g_matTexTransform).xy;
	output.vTiledUV = input.vUV*float2(g_fDetailLevelX, g_fDetailLevelY);
	float3 vPos = input.vPos;

	float2 vUV = float2(input.vUV.x, 1.f - input.vUV.y);

	vPos.y = gHeightMap[vUV * float2(512.f, 512.f)].r;

	output.vPos = mul(float4(vPos, 1.f), g_matWVP);

	output.vProjPos = output.vPos;
	output.vPosV = mul(float4(vPos, 1.f), g_matWV).xyz;
	output.vPosW = mul(float4(vPos, 1.f), g_matWorld).xyz;
	output.vNormal = normalize(mul(float4(input.vNormal, 0.f), g_matWV).xyz);
	output.vTangent = normalize(mul(float4(input.vTangent, 0.f), g_matWV).xyz);
	output.vBinormal = normalize(mul(float4(input.vBinormal, 0.f), g_matWV).xyz);
	output.vShadowProjPos = mul(float4(vPos, 1.f), g_matLightWorldViewProjTexture);

	return output;
}

PS_OUTPUT_TARGET PS(VS_OUTPUT_T input)
{
	PS_OUTPUT_TARGET output = (PS_OUTPUT_TARGET)0.f;

	float2 vUV = input.vTiledUV;// input.vUV* float2(g_fDetailLevelX, g_fDetailLevelY);

	float4 vDif = g_vDif;
	if (g_iDifTex)
	{
		vDif = g_tTex.Sample(g_tAnisotropic, vUV);
	}

	output.Albedo = vDif;
	output.GBuffer1.xyz = ComputeBumpNormal(input.vNormal, input.vTangent, input.vBinormal, vUV).xyz;
	output.GBuffer1.w = input.vShadowProjPos.z / input.vShadowProjPos.w;
	output.GBuffer2.x = input.vProjPos.z / input.vProjPos.w;
	output.GBuffer2.w = input.vProjPos.w;
	output.GBuffer2.y = g_vSpe.w;
	output.GBuffer2.z = g_iDecal;
	output.GBuffer3.x = ConvertColor(g_vDif);
	output.GBuffer3.y = ConvertColor(g_vAmb);

	float4 vSpc = g_vSpe;
	vSpc.w = 1.f;
	if (g_iSpecTex)
	{
		vSpc = g_SpcTex.Sample(g_tAnisotropic, vUV);
	}

	float4 vEmv = g_vEmi;
	if (g_iEmiTex)
	{
		vEmv = g_EmiTex.Sample(g_tAnisotropic, vUV);
	}
	output.GBuffer3.z = ConvertColor(vSpc);
	output.GBuffer3.w = ConvertColor(vEmv);

	return output;
}

PS_OUTPUT_TARGET ArrayPS(VS_OUTPUT_T input)
{
	PS_OUTPUT_TARGET output = (PS_OUTPUT_TARGET)0.f;

	float2 vUV = input.vUV*float2(g_fDetailLevelX, g_fDetailLevelY);

	float4 vDif = g_ArrayTex.Sample(g_tLinear, float3(vUV, 0));

	float4 vSpc = g_vSpe;
	if (g_iSpecTex)
	{
		vSpc = g_ArraySpcTex.Sample(g_tAnisotropic, float3(vUV, 0.f));
	}

	float pAlpha[8];
	float2 vAlphaUV = float2(input.vUV.x,1.f- input.vUV.y);

	for (int i = 0; i < g_iSplatCount-1; i++)
	{
		pAlpha[i] = gAlphaMapArray.Sample(g_tLinear, float3(vAlphaUV, i)).r;
		vDif = lerp(vDif, g_ArrayTex.Sample(g_tLinear, float3(vUV, i + 1)), pAlpha[i]);
		if (g_iSpecTex)
		{
			vSpc = lerp(vSpc, g_ArraySpcTex.Sample(g_tLinear, float3(vUV, i + 1)), pAlpha[i]);
		}
	}

	output.Albedo = vDif;

	float3 vNormal = ComputeBumpNormalArray(input.vNormal, input.vTangent, input.vBinormal, vUV, g_iSplatCount, pAlpha);

	output.GBuffer1.xyz = vNormal;
	float fDepth = input.vShadowProjPos.z / input.vShadowProjPos.w;
	output.GBuffer1.w = fDepth;
	output.GBuffer2.x = input.vProjPos.z / input.vProjPos.w;
	output.GBuffer2.w = input.vProjPos.w;
	output.GBuffer2.y = g_vSpe.w;
	output.GBuffer2.z = g_iDecal;
	output.GBuffer3.x = ConvertColor(g_vDif);
	output.GBuffer3.y = ConvertColor(g_vAmb);
	output.GBuffer4.xyz = input.vTangent;
	output.GBuffer5.xyz = input.vBinormal;

	float4 vEmv = g_vEmi;

	output.GBuffer3.z = ConvertColor(vSpc);
	output.GBuffer3.w = ConvertColor(vEmv);

	return output;
}

struct VertexIn
{
	float3 PosL	:	POSITION;
	float2 Tex	:	TEXCOORD;
	float2 BoundsY	:	TEXCOORD1;
};

struct VertexOut
{
	float3 PosW	:	POSITION;
	float2 Tex	:	TEXCOORD0;
	float2 BoundsY	:	TEXCOORD1;
};

struct VS_INPUT_TERRAIN
{
	float3	vPos	:	POSITION;
	float3	vNormal	:	NORMAL;
	float2	vUV	:	TEXCOORD;
	float3	vTangent	:	TANGENT;
	float3	vBinormal	:	BINORMAL;
	float4	vBlendIndex	:	BLENDINDICES;
	float4	vBlendWeight	:	BLENDWEIGHT;
	float2	vBoundsY	:	BOUND;
};

struct VS_OUTPUT_TERRAIN
{
	float3	vPos	:	POSITION;
	float4	vProjPos	:	POSITION1;
	float3	vPosW	:	POSITION2;
	float3	vPosV	:	POSITION3;
	float2	vUV	:	TEXCOORD;
	float2	vTiledUV	:	TEXCOORD1;
	float3	vNormal	:	NORMAL;
	float3	vTangent	:	TANGENT;
	float3	vBinormal	:	BINORMAL;
	float2	vBoundsY	:	BOUND;
};

VS_OUTPUT_TERRAIN TerrainVS(VS_INPUT_TERRAIN input)
{
	VS_OUTPUT_TERRAIN output = (VS_OUTPUT_TERRAIN)0.f;

	float3 vPos = input.vPos;

	float2 vUV = float2(input.vUV.x, 1.f - input.vUV.y);


	output.vPos = input.vPos;
	output.vPos.y = gHeightMap.SampleLevel(g_tPoint, vUV, 0).r;
	output.vUV = input.vUV;

	//output.vProjPos = output.vPos;
	output.vPosV = mul(float4(vPos, 1.f), g_matWV).xyz;
	output.vPosW = mul(float4(vPos, 1.f), g_matWorld).xyz;
	output.vNormal = normalize(mul(float4(input.vNormal, 0.f), g_matWV).xyz);
	output.vTangent = normalize(mul(float4(input.vTangent, 0.f), g_matWV).xyz);
	output.vBinormal = normalize(mul(float4(input.vBinormal, 0.f), g_matWV).xyz);

	return output;
}


float CalcTessFactor(float3 p)
{
	float3 vViewPos = mul(float4(p, 0.f), g_matView).xyz;

	float d = length(vViewPos);

	float s = saturate((d - gMinDist) / (gMaxDist - gMinDist));

	return pow(2, lerp(gMaxTess, gMinTess, s));
}

struct PatchTess
{
	float EdgeTess[4]	:	SV_TessFactor;
	float InsideTess[2]	:	SV_InsideTessFactor;
};

bool AabbBehindPlaneTest(float3 center, float3 extents, float4 plane)
{
	float3 n = abs(plane.xyz);

	float r = dot(extents, n);

	float s = dot(float4(center, 1.f), plane);

	return (s + r) < 0.f;
}

bool AabbOutsideFrustumTest(float3 center, float3 extents, float4 frustumPlanes[6])
{
	for (int i = 0; i < 6; i++)
	{
		if (AabbBehindPlaneTest(center, extents, frustumPlanes[i]))
		{
			return true;
		}
	}

	return false;
}

PatchTess ConstantHS(InputPatch<VS_OUTPUT_TERRAIN, 4> patch,
	uint patchID	:	SV_PrimitiveID)
{
	PatchTess pt;

	float minY = patch[0].vBoundsY.x;
	float maxY = patch[0].vBoundsY.y;

	float3 vMin = float3(patch[2].vPos.x, minY, patch[2].vPos.z);
	float3 vMax = float3(patch[1].vPos.x, maxY, patch[1].vPos.z);

	float3 boxCenter = 0.5f * (vMin + vMax);
	float3 boxExtents = 0.5f * (vMax - vMin);

	//if (AabbOutsideFrustumTest(boxCenter, boxExtents, gWorldFrustumPlanes))
	//{
	//	pt.EdgeTess[0] = 0.f;
	//	pt.EdgeTess[1] = 0.f;
	//	pt.EdgeTess[2] = 0.f;
	//	pt.EdgeTess[3] = 0.f;

	//	pt.InsideTess[0] = 0.f;
	//	pt.InsideTess[1] = 0.f;

	//	return pt;
	//}

	//else
	{
		float3 e0 = 0.5f * (patch[0].vPos + patch[2].vPos);
		float3 e1 = 0.5f * (patch[0].vPos + patch[1].vPos);
		float3 e2 = 0.5f * (patch[1].vPos + patch[3].vPos);
		float3 e3 = 0.5f * (patch[2].vPos + patch[3].vPos);
		float3 c = 0.25f * (patch[0].vPos + patch[1].vPos + patch[2].vPos + patch[3].vPos);

		pt.EdgeTess[0] = CalcTessFactor(e0);
		pt.EdgeTess[1] = CalcTessFactor(e1);
		pt.EdgeTess[2] = CalcTessFactor(e2);
		pt.EdgeTess[3] = CalcTessFactor(e3);

		pt.InsideTess[0] = CalcTessFactor(c);
		pt.InsideTess[1] = pt.InsideTess[0];

		return pt;
	}
}

struct HullOut
{
	float3	PosW	:	POSITION;
	float2	Tex	:	TEXCOORD;
	float3	vNormal	:	NORMAL;
	float3	vTangent	:	TANGENT;
	float3	vBinormal	:	BINORMAL;
};

struct DomainOut
{
	float4 PosH	:	SV_POSITION;
	float4 PosP	:	POSITION;
	float3 PosW	:	POSITION1;
	float3	vPosV	:	POSITION2;
	float2 Tex	:	TEXCOORD0;
	float2 TiledTex	:	TEXCOORD1;
	float3	vNormal	:	NORMAL;
	float3	vTangent	:	TANGENT;
	float3	vBinormal	:	BINORMAL;
};

[domain("quad")]
[patchconstantfunc("ConstantHS")]
[partitioning("fractional_even")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(4)]
[maxtessfactor(64.f)]
HullOut HS(InputPatch<VS_OUTPUT_TERRAIN,4> p,
	uint i	:	SV_OutputControlPointID,
	uint patchID	:	SV_PrimitiveID)
{
	HullOut output = (HullOut)0.f;

	output.PosW = p[i].vPos;
	output.Tex = p[i].vUV;
	output.vNormal = p[i].vNormal;
	output.vTangent = p[i].vTangent;
	output.vBinormal = p[i].vBinormal;

	return output;
}


[domain("quad")]
DomainOut DS(PatchTess patchTess,
	float2 uv	:	SV_DomainLocation,
	const OutputPatch<HullOut, 4> quad)
{
	DomainOut dout;

	dout.PosW = lerp(
		lerp(quad[0].PosW, quad[1].PosW, uv.x),
		lerp(quad[2].PosW, quad[3].PosW, uv.x),
		uv.y);

	dout.Tex = lerp(
		lerp(quad[0].Tex, quad[1].Tex, uv.x),
		lerp(quad[2].Tex, quad[3].Tex, uv.x),
		uv.y);

	dout.vNormal = lerp(
		lerp(quad[0].vNormal, quad[1].vNormal, uv.x),
		lerp(quad[2].vNormal, quad[3].vNormal, uv.x),
		uv.y);

	dout.vBinormal = lerp(
		lerp(quad[0].vBinormal, quad[1].vBinormal, uv.x),
		lerp(quad[2].vBinormal, quad[3].vBinormal, uv.x),
		uv.y);

	dout.vTangent = lerp(
		lerp(quad[0].vTangent, quad[1].vTangent, uv.x),
		lerp(quad[2].vTangent, quad[3].vTangent, uv.x),
		uv.y);

	dout.TiledTex = dout.Tex * float2(g_fDetailLevelX, g_fDetailLevelY);

	dout.PosW.y = gHeightMap.SampleLevel(g_tPoint, dout.Tex, 0).r;

	dout.PosH = mul(float4(dout.PosW, 1.f), g_matView); 
	dout.PosH = mul(dout.PosH, g_matProj);

	dout.PosP = dout.PosH;

	return dout;
}


PS_OUTPUT_TARGET TerrainPS(DomainOut input)
{
	float2 leftTex = input.Tex + float2(-gTexelCellSpaceU,0.f);
	float2 rightTex = input.Tex + float2(gTexelCellSpaceU, 0.f);
	float2 bottomTex = input.Tex + float2(0.f, gTexelCellSpaceV);
	float2 topTex = input.Tex + float2(0.f, -gTexelCellSpaceV);

	float leftY = gHeightMap.SampleLevel(g_tPoint, leftTex, 0).r;
	float rightY = gHeightMap.SampleLevel(g_tPoint, rightTex, 0).r;
	float bottomY = gHeightMap.SampleLevel(g_tPoint, bottomTex, 0).r;
	float topY = gHeightMap.SampleLevel(g_tPoint, topTex, 0).r;

	float3 tangent = normalize(
		float3(2.f * gWorldCellSpace, rightY - leftY, 0.f));
	float3 bitan = normalize(float3(0.f,bottomY-topY,-2.0f*gWorldCellSpace));
	float3 normalW = cross(tangent, bitan);

	PS_OUTPUT_TARGET output = (PS_OUTPUT_TARGET)0.f;

	float2 vUV = input.TiledTex;

	float4 vDif = g_ArrayTex.Sample(g_tLinear, float3(vUV, 0));

	float4 vSpc = g_vSpe;
	if (g_iSpecTex)
	{
		vSpc = g_ArraySpcTex.Sample(g_tAnisotropic, float3(vUV, 0.f));
	}

	float pAlpha[8];
	float2 vAlphaUV = float2(input.Tex.x, 1.f - input.Tex.y);

	for (int i = 0; i < g_iSplatCount - 1; i++)
	{
		pAlpha[i] = gAlphaMapArray.Sample(g_tLinear, float3(vAlphaUV, i)).r;
		vDif = lerp(vDif, g_ArrayTex.Sample(g_tLinear, float3(vUV, i + 1)), pAlpha[i]);
		if (g_iSpecTex)
		{
			vSpc = lerp(vSpc, g_ArraySpcTex.Sample(g_tLinear, float3(vUV, i + 1)), pAlpha[i]);
		}
	}

	output.Albedo = vDif;

	float3 vNormal = ComputeBumpNormalArray(normalW, tangent, bitan, vUV, g_iSplatCount, pAlpha);
	vNormal = mul(float4(vNormal, 0.f), g_matView).xyz;
	output.GBuffer1.xyz = vNormal;
	output.GBuffer2.x = input.PosP.z / input.PosP.w;
	output.GBuffer2.w = input.PosP.w;
	output.GBuffer2.y = g_vSpe.w;
	output.GBuffer2.z = g_iDecal;
	output.GBuffer3.x = ConvertColor(g_vDif);
	output.GBuffer3.y = ConvertColor(g_vAmb);
	output.GBuffer4.xyz = input.vTangent;
	output.GBuffer5.xyz = input.vBinormal;

	float4 vEmv = g_vEmi;

	output.GBuffer3.z = ConvertColor(vSpc);
	output.GBuffer3.w = ConvertColor(vEmv);

	return output;
}


struct Particle
{
	float4	vColor;
	float3	vPos;
	uint	iLive;
	float3	vSpeed;
	float	fTime;
	float3	vScale;
	float	fAngle;
	float	fAngleSpeed;
	float	fMaxTime;
	int		iNumber;
	float	E2;
};

struct ParticleShare
{
	int		iSpawnCount;
	float3	vMaxScale;
	int		iCurCount;
	float3	vMinScale;
	float4	vMaxColor;
	float4	vMinColor;
};