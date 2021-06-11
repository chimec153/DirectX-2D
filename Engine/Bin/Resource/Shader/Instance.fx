#include "Share.fx"

struct VS_INST_INPUT
{
	float3	vPos	:	POSITION;
	float2	vUV		:	TEXCOORD;
	matrix	matWVP	:	WORLD;
	float3	vPivot	:	PIVOT;
	int		iFrame	:	FRAME;
	float4	vColor	:	COLOR;
	float3	vSize	:	SIZE;
};

struct VS_INST_OUTPUT
{
	float4	vPos	:	SV_POSITION;
	float2	vUV	:	TEXCOORD;
	float4	vColor	:	COLOR;
	int		iFrame	:	FRAME;
};

struct VS_INST_ANIM2D_INPUT
{
	float3	vPos	:	POSITION;
	float2	vUV		:	TEXCOORD;
	matrix	matWVP	:	WORLD;
	float3	vPivot	:	PIVOT;
	float3	vSize	:	SIZE;
	float2	vTexSize	:	TEXSIZE;
	float2	vStart	:	START;
	float2	vEnd	:	END;
};

struct VS_INST_ANIM2D_OUTPUT
{
	float4	vPos	:	SV_POSITION;
	float2	vUV	:	TEXCOORD;
};

struct VS_INST_COLLIDER_INPUT
{
	float3	vPos	:	POSITION;
	matrix	matWVP	:	WORLD;
	float4	vColor	:	COLOR;
	float3	vPivot	:	PIVOT;
	float3	vSize	:	SIZE;
};

struct VS_INST_COLLIDER_OUTPUT
{
	float4	vPos	:	SV_POSITION;
	float4	vColor	:	COLOR;
};

VS_INST_OUTPUT Inst_VS(VS_INST_INPUT input)
{
	VS_INST_OUTPUT output = (VS_INST_OUTPUT)0.f;

	float3 vPos = input.vPos - input.vPivot * input.vSize;

	output.vPos = mul(float4(vPos, 1.f), input.matWVP);
	output.vColor = input.vColor;
	output.vUV = input.vUV;
	output.iFrame = input.iFrame;

	return output;
}

PS_OUTPUT_SINGLE Inst_PS(VS_INST_OUTPUT input)
{
	PS_OUTPUT_SINGLE output = (PS_OUTPUT_SINGLE)0.f;

	output.vColor = g_ArrayTex.Sample(g_tPoint, float3(input.vUV, input.iFrame), 0.f) * input.vColor;

	return output;
}

VS_INST_ANIM2D_OUTPUT Inst_Anim2D_VS(VS_INST_ANIM2D_INPUT input)
{
	VS_INST_ANIM2D_OUTPUT output = (VS_INST_ANIM2D_OUTPUT)0.f;

	float3 vPos = input.vPos - input.vPivot * input.vSize;

	output.vPos = mul(float4(vPos, 1.f), input.matWVP);
	output.vUV = GetUV(input.vUV, input.vTexSize, input.vStart, input.vEnd);

	return output;
}

PS_OUTPUT_SINGLE Inst_Anim2D_PS(VS_INST_ANIM2D_OUTPUT input)
{
	PS_OUTPUT_SINGLE output = (PS_OUTPUT_SINGLE)0.f;

	output.vColor = g_tTex.Sample(g_tPoint, input.vUV);

	return output;
}

VS_INST_COLLIDER_OUTPUT Inst_Collider_VS(VS_INST_COLLIDER_INPUT input)
{
	VS_INST_COLLIDER_OUTPUT output = (VS_INST_COLLIDER_OUTPUT)0.f;

	float3 vPos = input.vPos - input.vPivot * input.vSize;

	output.vPos = mul(float4(vPos, 1.f), input.matWVP);
	output.vColor = input.vColor;

	return output;
}

PS_OUTPUT_SINGLE Inst_Collider_PS(VS_INST_COLLIDER_OUTPUT input)
{
	PS_OUTPUT_SINGLE output = (PS_OUTPUT_SINGLE)0.f;

	output.vColor = input.vColor;

	return output;
}