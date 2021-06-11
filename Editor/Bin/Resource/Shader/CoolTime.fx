#include "Share.fx"

struct VS_INPUT_2D
{
	float3 vPos	:	POSITION;
	float2 vUV	:	TEXCOORD;
	float4 vColor	:	COLOR;
};

struct VS_OUTPUT_2D
{
	float4 vPos	:	SV_POSITION;
	float2 vUV	:	TEXCOORD;
	float2 vOriginUV	:	TEXCOORD1;
	float4 vColor	:	COLOR;
};

VS_OUTPUT_2D CoolTimeVS(VS_INPUT_2D input)
{
	VS_OUTPUT_2D output = (VS_OUTPUT_2D)0;

	float3 vPos = input.vPos - g_vPivot * g_vMeshSize;

	output.vPos = mul(float4(vPos, 1.f), g_matWVP);
	output.vUV = ComputeUV(input.vUV);
	output.vOriginUV = input.vUV;
	output.vColor = input.vColor;

	return output;
}

PS_OUTPUT_SINGLE CoolTimePS(VS_OUTPUT_2D input)
{
	PS_OUTPUT_SINGLE output = (PS_OUTPUT_SINGLE)0;

	output.vColor = g_tTex.Sample(g_tPoint, input.vUV) * g_vDif;

	float2 vUV = input.vOriginUV - 0.5f;

	float fAngle = atan2(vUV.x, vUV.y);

	if (fAngle + 3.141592f < g_fFrame * 3.141592f * 2.f)
	{
		output.vColor.xyz = saturate(output.vColor.xyz - 0.5f);
	}

	return output;
}