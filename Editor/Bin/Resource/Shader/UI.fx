#include "Share.fx"

struct VS_INPUT_UI
{
	float3 vPos	:	POSITION;
	float2 vUV	:	TEXCOORD;
	float4 vColor	:	COLOR;
};

struct VS_OUTPUT_UI
{
	float4 vPos	:	SV_POSITION;
	float2 vUV	:	TEXCOORD;
	float2 vOriginUV	:	TEXCOORD1;
	float4 vColor	:	COLOR;
};

#define L	0
#define T	1
#define	R	2
#define	B	3

VS_OUTPUT_UI VS(VS_INPUT_UI input)
{
	VS_OUTPUT_UI output = (VS_OUTPUT_UI)0;

	float3 vPos = input.vPos - g_vPivot * g_vMeshSize;

	output.vPos = mul(float4(vPos, 1.f), g_matWVP);
	output.vUV = ComputeUV(input.vUV);
	output.vColor = input.vColor;

	return output;
}

PS_OUTPUT_SINGLE PS(VS_OUTPUT_UI input)
{
	PS_OUTPUT_SINGLE output = (PS_OUTPUT_SINGLE)0.f;

	output.vColor = g_tTex.Sample(g_tPoint, input.vUV) * g_vDif;

	return output;
}

PS_OUTPUT_SINGLE AlphaTexturePS(VS_OUTPUT_UI input)
{
	PS_OUTPUT_SINGLE output = (PS_OUTPUT_SINGLE)0.f;

	output.vColor = g_vDif;

	float fAlpha = g_tTex.Sample(g_tPoint, input.vUV).r;

	output.vColor.a = fAlpha;

	return output;
}

VS_OUTPUT_UI BarVS(VS_INPUT_UI input)
{
	VS_OUTPUT_UI output = (VS_OUTPUT_UI)0;

	float3 vPos = input.vPos - g_vPivot * g_vMeshSize;

	output.vPos = mul(float4(vPos, 1.f), g_matWVP);
	output.vUV = ComputeUV(input.vUV);
	output.vOriginUV = input.vUV;
	output.vColor = input.vColor;

	return output;
}

PS_OUTPUT_SINGLE BarPS(VS_OUTPUT_UI input)
{
	PS_OUTPUT_SINGLE output = (PS_OUTPUT_SINGLE)0.f;

	if (g_iDir == L)
	{
		if (input.vOriginUV.x < 1 - g_fPrg)
			clip(-1);
	}

	else if (g_iDir == T)
	{
		if (input.vOriginUV.y < 1 - g_fPrg)
			clip(-1);
	}

	else if (g_iDir == R)
	{
		if (input.vOriginUV.x > g_fPrg)
			clip(-1);
	}

	else if (g_iDir == B)
	{
		if (input.vOriginUV.y > g_fPrg)
			clip(-1);
	}

	output.vColor = g_tTex.Sample(g_tPoint, input.vUV)* g_vDif;

	return output;
}