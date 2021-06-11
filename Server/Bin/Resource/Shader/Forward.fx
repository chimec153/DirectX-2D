#include "Share.fx"

struct VS_IN
{
	float3 Pos	:	POSITION;
	float3 Norm	:	NORMAL;
	float2 UV	:	TEXCOORD;
};

struct VS_OUT
{
	float4 Pos	:	SV_POSITION;
	float2 UV	:	TEXCOORD;
	float3 Norm	:	TEXCOORD1;
};

cbuffer Hemiconstants	:	register(b13)
{
	float3 AmbientDown	:	packoffset(c0);
	float3 AmbientRange	:	packoffset(c1);
}

VS_OUT RenderSceneVS(VS_IN IN)
{
	VS_OUT Output;

	Output.Pos = mul(float4(IN.Pos,1.f), g_matWVP);

	Output.UV = IN.UV;

	Output.Norm = mul(IN.Norm, (float3x3)g_matWorld);

	return Output;
}

float3 CalcAmbient(float3 normal, float3 color)
{
	float up = normal.y * 0.5 + 0.5;

	float3 Ambient = AmbientDown + up * AmbientRange;

	return Ambient * color;
}

PS_OUTPUT_SINGLE PS(VS_OUT IN)
{
	PS_OUTPUT_SINGLE output = (PS_OUTPUT_SINGLE)0.f;

	float3 normal = normalize(IN.Norm);

	float4 color = float4(0.5f, 0.5f, 0.5f, 1.f);

	output.vColor = float4(color.rgb * color.rgb, color.a);
	output.vColor.rgb =  CalcAmbient(normal, color.rgb);

	return output;
}