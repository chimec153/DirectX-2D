#include "Share.fx"

struct VS_DEBUG_IN
{
	float3	vPos	:	POSITION;
	float2	vUV	:	TEXCOORD;
};

struct VS_DEBUG_OUT
{
	float4	vPos	:	SV_POSITION;
	float2	vUV	:	TEXCOORD;
};

VS_DEBUG_OUT DebugVS(VS_DEBUG_IN input)
{
	VS_DEBUG_OUT output = (VS_DEBUG_OUT)0.f;

	output.vPos = mul(float4(input.vPos, 1.f), g_matDebugWVP);
	output.vUV = input.vUV;

	return output;
}

PS_OUTPUT_SINGLE DebugPS(VS_DEBUG_OUT input)
{
	PS_OUTPUT_SINGLE output = (PS_OUTPUT_SINGLE)0.f;

	output.vColor = g_tTex.Sample(g_tPoint, input.vUV);

	return output;
}