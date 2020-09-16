#include "Share.fx"

struct VS_INPUT_COLOR
{
	float3 vPos	:	POSITION;
	float4 vColor	:	COLOR;
};

struct VS_OUTPUT_COLOR
{
	float4 vPos	:	SV_POSITION;
	float4 vColor	:	COLOR;
};

VS_OUTPUT_COLOR GetOutputVS(VS_INPUT_COLOR input)
{
	VS_OUTPUT_COLOR output = (VS_OUTPUT_COLOR)0;

	output.vPos = float4(input.vPos, 1.f);
	output.vColor = input.vColor;

	return output;
}

PS_OUTPUT_SINGLE GetOutputPS(VS_OUTPUT_COLOR input)
{
	PS_OUTPUT_SINGLE output = (PS_OUTPUT_SINGLE)0;

	output.vColor = input.vColor;

	return output;
}