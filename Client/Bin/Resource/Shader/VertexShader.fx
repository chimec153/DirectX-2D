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
	float4 vColor	:	COLOR;
};


VS_OUTPUT_2D GetOutputVS(VS_INPUT_2D input)
{
	VS_OUTPUT_2D output = (VS_OUTPUT_2D)0;

	float3 vPos = input.vPos - g_vPivot * g_vMeshSize;

	output.vPos = mul(float4(vPos, 1.f), g_matWVP);
	output.vUV = input.vUV;
	output.vColor = input.vColor;

	return output;
}

float4 GetOutputPS(VS_OUTPUT_2D input)	:	SV_TARGET
{
	PS_OUTPUT_SINGLE output = (PS_OUTPUT_SINGLE)0;

	output.vColor = g_tTex.Sample(g_tPoint, input.vUV) * g_vDif;

	return output.vColor;
}

VS_OUTPUT_2D TexVS(VS_INPUT_2D input)
{
	VS_OUTPUT_2D output = (VS_OUTPUT_2D)0;

	float3 vPos = input.vPos - g_vPivot * g_vMeshSize;

	output.vPos = mul(float4(vPos, 1.f), g_matWVP);
	output.vUV = input.vUV;
	output.vColor = input.vColor;

	return output;
}

PS_OUTPUT_SINGLE TexPS(VS_OUTPUT_2D input)
{
	PS_OUTPUT_SINGLE output = (PS_OUTPUT_SINGLE)0;

	output.vColor = g_tTex.Sample(g_tPoint, input.vUV) * g_vDif;

	return output;
}

VS_OUTPUT_2D Standard2DSpriteVS(VS_INPUT_2D input)
{
	VS_OUTPUT_2D output = (VS_OUTPUT_2D)0;

	float3 vPos = input.vPos - g_vPivot * g_vMeshSize;

	output.vPos = mul(float4(vPos, 1.f), g_matWVP);
	output.vUV = ComputeUV(input.vUV);
	output.vColor = input.vColor;

	return output;
}

PS_OUTPUT_SINGLE Standard2DSpritePS(VS_OUTPUT_2D input)
{
	PS_OUTPUT_SINGLE output = (PS_OUTPUT_SINGLE)0;
	if (g_iImageType != TEXTUREARRAY)
	{
		output.vColor = g_tTex.Sample(g_tPoint, input.vUV) * g_vDif;
	}
	else
	{
		output.vColor = g_ArrayTex.Sample(g_tPoint, float3(input.vUV, g_fFrame)) * g_vDif;
	}

	return output;
}
uniform float4 position	:	SV_POSITION;
struct VS_TEXTURED_LIGHTING_INPUT
{
	float3 position	:	POSITION;
	float3 normal	:	NORMAL;
	float2 texcoord0	:	TEXCOORD0;
	float2 texcoord1	:	TEXCOORD1;
};

float4 PSTexturedColor(float4 position	:	SV_POSITION,
	float2	texcoord0 : TEXCOORD0) : SV_Target
{
	float4 cColor = 0.f;//gtxtTexture.Sample(gSamplerState, texcoord0);
	return(cColor);
}

struct VSOut
{
	float4 pos	:	SV_Position;
	float4 color	:	color;
};

VSOut VS(uint nVertexID	:	SV_VertexID)
{
	VSOut output;
	if (nVertexID == 0) output.pos = float4(0.0, 0.5, 0.5, 1.0);
	else if (nVertexID == 2) output.pos = float4(0.5, -0.5, 0.5, 1.0);
	else if (nVertexID == 1) output.pos = float4(-0.5, -0.5, 0.5, 1.0);
	output.color = clamp(output.pos, 0, 1);
	return output;
}

PS_OUTPUT_TARGET PaperBurnPS(VS_OUTPUT_3D input)
{
	PS_OUTPUT_TARGET output = (PS_OUTPUT_TARGET)0.f;

	float4 vDif = g_tTex.Sample(g_tAnisotropic, input.vUV);

	vDif = PaperBurn(vDif, input.vUV);

	output.Albedo = vDif;
	output.GBuffer1.xyz = ComputeBumpNormal(input.vNormal, input.vTangent, input.vBinormal, input.vUV);
	output.GBuffer2.x = input.vProjPos.z / input.vProjPos.w;
	output.GBuffer2.w = input.vProjPos.w;
	output.GBuffer2.y = g_vSpe.w;
	output.GBuffer2.z = g_iDecal;
	output.GBuffer3.x = ConvertColor(g_vDif);
	output.GBuffer3.y = ConvertColor(g_vAmb);
	output.GBuffer4.xyz = input.vTangent;
	output.GBuffer5.xyz = input.vBinormal;

	float4 vSpc = g_vSpe;
	vSpc.w = 1.f;
	if (g_iSpecTex)
	{
		vSpc = g_SpcTex.Sample(g_tAnisotropic, input.vUV);
	}

	float4 vEmv = g_vEmi;
	if (g_iEmiTex)
	{
		vEmv = g_EmiTex.Sample(g_tAnisotropic, input.vUV);
	}
	output.GBuffer3.z = ConvertColor(vSpc);
	output.GBuffer3.w = ConvertColor(vEmv);

	return output;
}

static const float2 FullPos[4] =
{
	float2(-1.f, 1.f),
	float2(1.f, 1.f),
	float2(-1.f, -1.f),
	float2(1.f, -1.f)
};

static const float2 FullUV[4] =
{
	float2(0.f,0.f),
	float2(1.f,0.f),
	float2(0.f,1.f),
	float2(1.f,1.f)
};

struct VS_OUTPUT_FXAA
{
	float4 vPos	:	SV_Position;
	float2	vUV	:	TEXCOORD;
};

Texture2D g_tTexture	:	register(t20);

VS_OUTPUT_FXAA FullVS(uint iVertexID	:	SV_VertexID)
{
	VS_OUTPUT_FXAA output = (VS_OUTPUT_FXAA)0.f;

	output.vPos = float4(FullPos[iVertexID], 0.f, 1.f);
	output.vUV = FullUV[iVertexID];

	return output;
}

PS_OUTPUT_SINGLE FullPS(VS_OUTPUT_FXAA input)
{
	PS_OUTPUT_SINGLE output = (PS_OUTPUT_SINGLE)0.f;

	output.vColor = g_tTexture.Sample(g_tPoint, input.vUV);

	return output;
}

float hash(float2 p) 
{ 
	return frac(sin(dot(p, float2(41, 289))) * 45758.5453); 
}

static const float samples[10] = { -0.08f, -0.05f, -0.03f,-0.02f,-0.01f, 0.01f, 0.02f, 0.03f, 0.05f, 0.08f };

PS_OUTPUT_SINGLE FullRadialBlurPS(VS_OUTPUT_FXAA input)
{
	PS_OUTPUT_SINGLE output = (PS_OUTPUT_SINGLE)0.f;

	float2 dir = g_vBlurCenter - input.vUV;
	float dist = length(dir);
	float4 color = g_tTexture.Sample(g_tPoint, input.vUV);
	float4 sum = color;

	dir /= dist;

	float2 pos = (float2)0.f;

	for (int i = 0; i < 10; i++)
	{
		sum += g_tTexture.Sample(g_tPoint, input.vUV + (hash(input.vUV + frac(g_fAccTime)) * 2.f - 1.f) * dir * samples[i] * g_fSampleDist);
	}

	sum *= 1 / 11.f;
	float t = saturate(dist * g_fSampleStrength);

	output.vColor = lerp(color, sum, t);

	return output;
}

VS_OUTPUT_FXAA EquipVS(uint iVertexID	:	SV_VertexID)
{
	VS_OUTPUT_FXAA output = (VS_OUTPUT_FXAA)0.f;

	output.vPos = mul(float4(FullPos[iVertexID],0.f, 1.f), g_matWVP);
	output.vUV = FullUV[iVertexID];

	return output;
}

PS_OUTPUT_SINGLE EquipPS(VS_OUTPUT_FXAA input)
{
	PS_OUTPUT_SINGLE output = (PS_OUTPUT_SINGLE)0.f;

	output.vColor = g_tTexture.Sample(g_tPoint, input.vUV);

	return output;
}