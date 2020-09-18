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

	float3 vPos = input.vPos -vPivot * vMeshSize;

	output.vPos = mul(float4(vPos, 1.f), fmatWVP);
	output.vColor = input.vColor;

	return output;
}

PS_OUTPUT_SINGLE GetOutputPS(VS_OUTPUT_COLOR input)
{
	PS_OUTPUT_SINGLE output = (PS_OUTPUT_SINGLE)0;

	output.vColor = input.vColor * vDif;

	return output;
}
/*
-.5	-.5	0	1			-.284	.066	0		-.316 
.5	.5	0	1			-.233	-.252	0		.262
							0	0		.0003	0
							0	0		0		1

0.142 + 0.1265 = 0.269
-0.033 + 0.126 = 0.093
0
0.158-0.131+1 = 1.027
*/
