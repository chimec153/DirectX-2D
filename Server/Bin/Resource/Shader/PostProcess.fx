#include "Share.fx"

Texture2D g_tFlareTexture	:	register(t0);

struct VS_OUTPUT
{
	float4	vPos	:	SV_POSITION;
	float2	vUV	:	TEXCOORD;
	float4	vColor	:	COLOR;
};

static const float4 g_vNullPos[6] =
{
	float4(-1.f, 1.f,0.f, 1.f),
	float4(1.f, 1.f,0.f, 1.f),
	float4(-1.f, -1.f,0.f, 1.f),
	float4(1.f, 1.f,0.f, 1.f),
	float4(1.f, -1.f,0.f, 1.f),
	float4(-1.f, -1.f,0.f, 1.f)
};

static const float2 g_vNullUV[6] =
{
	float2(0.f, 0.f),
	float2(1.f, 0.f),
	float2(0.f, 1.f),
	float2(1.f, 0.f),
	float2(1.f, 1.f),
	float2(0.f, 1.f)
};

struct FlareInfo
{
	float4	vFlarePos;
	float4	vScaleRotation;
	float4	vColor;
};

static const int	g_iMaxFlares = 8;

cbuffer Flare	:	register(b0)
{
	FlareInfo g_pFlares[g_iMaxFlares];
}

VS_OUTPUT VS(uint iId	:	SV_VertexID)
{
	VS_OUTPUT output = (VS_OUTPUT)0.f;

	output.vPos.x = dot(g_vNullPos[iId % 6].xy, g_pFlares[iId / 6].vScaleRotation.xy);
	output.vPos.y = dot(g_vNullPos[iId % 6].xy, g_pFlares[iId / 6].vScaleRotation.zw);
	output.vPos.xy += g_pFlares[iId / 6].vFlarePos.xy;
	output.vPos.zw = float2(0.f, 1.f);
	output.vUV = g_vNullUV[iId % 6];
	output.vColor = g_pFlares[iId/6].vColor;

	return output;
}

float4 PS(VS_OUTPUT input) : SV_TARGET
{
	float4 vColor = g_tFlareTexture.Sample(g_tLinear, input.vUV);

	if (vColor.r == 0.f &&
		vColor.g == 0.f &&
		vColor.b == 0.f )
		clip(-1);

	vColor.rgb *= vColor.rgb;

	return vColor * input.vColor;
}

