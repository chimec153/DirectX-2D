#include "Share.fx"

struct VS_IN
{
	float4 vColor	:	COLOR;
	float3 vPos	:	POSITION;
};

struct VS_OUT
{
	float4 vPos	:	SV_POSITION;
	float4 vColor	:	COLOR;
};

VS_OUT VS(VS_IN input)
{
	VS_OUT output = (VS_OUT)0.f;

	//input.vPos.xy += 0.5f * sin(input.vPos.x) * sin(3.0f * g_fAccTime);
	//input.vPos.z *= 0.6f + 0.4f * sin(2.0f * g_fAccTime);

	output.vPos = mul(float4(input.vPos, 1.f), g_matWVP);
	output.vColor = input.vColor;

	return output;
}

PS_OUTPUT_SINGLE PS(VS_OUT input)	:	SV_TARGET
{
	PS_OUTPUT_SINGLE output = (PS_OUTPUT_SINGLE)0.f;

	output.vColor = input.vColor;

	return output;
}