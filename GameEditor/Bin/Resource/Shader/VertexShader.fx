#include "Share.fx"

struct VS_INPUT
{
	float3 vPos	:	POSITION;
	float4 vColor	:	COLOR;
};

struct VS_OUTPUT
{
	float4 vPos	:	SV_POSITION;
	float4 vColor	:	COLOR;
};

VS_OUTPUT GetOutputVS(VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT)0;

	output.vPos = float4(input.vPos, 1.f);
	output.vColor = input.vColor;

	return output;
}

PS_OUTPUT_COLOR GetOutputPS(VS_OUTPUT input)
{
	PS_OUTPUT_COLOR color = (PS_OUTPUT_COLOR)0;

	color.vColor = input.vColor;

	return color;
}