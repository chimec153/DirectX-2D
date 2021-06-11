#include "Share.fx"


struct VS_OUTPUT_SHADOW
{
	float4	vPos	:	SV_POSITION;
	float4	vProjPos	:	POSITION;
};

VS_OUTPUT_SHADOW VS(VS_INPUT_3D input)
{
	VS_OUTPUT_SHADOW output = (VS_OUTPUT_SHADOW)0.f;

	Skinning tSkin = Skining(input.vPos, input.vNormal, input.vTangent, input.vBinormal, input.vBlendWeight, input.vBlendIndex);

	output.vPos = mul(float4(tSkin.vPos, 1.f), g_matLightWorldViewProjTexture);
	output.vProjPos = output.vPos;

	return output;
}

VS_OUTPUT_SHADOW InstVS(VS_INPUT_INST_3D input,
	uint iId	:	SV_InstanceID)
{
	VS_OUTPUT_SHADOW output = (VS_OUTPUT_SHADOW)0.f;

	Skinning tSkin = Skining(input.vPos, input.vNormal, input.vTangent, input.vBinormal,
		input.vBlendWeight, input.vBlendIndex, iId);

	output.vPos = mul(float4(tSkin.vPos, 1.f), input.matShadowWVP);
	output.vProjPos = output.vPos;

	return output;
}

PS_OUTPUT_SINGLE PS(VS_OUTPUT_SHADOW input)
{
	PS_OUTPUT_SINGLE output = (PS_OUTPUT_SINGLE)0.f;

	output.vColor = input.vProjPos.z / input.vProjPos.w;

	return output;
}

struct VS_CUBEMAP_IN
{
	float3 vPos	:	POSITION;
};

struct GS_CUBEMAP_IN
{
	float4 vPos	:	SV_POSITION;
};

GS_CUBEMAP_IN VS_CubeMap(VS_INPUT_3D input)
{
	GS_CUBEMAP_IN output = (GS_CUBEMAP_IN)0.f;

	Skinning tSkin = Skining(input.vPos, input.vNormal, input.vTangent, input.vBinormal, input.vBlendWeight, input.vBlendIndex);

	output.vPos = mul(float4(tSkin.vPos, 1.f), g_matWorld);

	return output;
}

GS_CUBEMAP_IN VS_CubeMap_Inst(VS_INPUT_INST_3D input, uint iId	:	SV_InstanceID)
{
	GS_CUBEMAP_IN output = (GS_CUBEMAP_IN)0.f;

	Skinning tSkin = Skining(input.vPos, input.vNormal, input.vTangent, input.vBinormal, input.vBlendWeight, input.vBlendIndex, iId);

	output.vPos = mul(float4(tSkin.vPos, 1.f), input.matWorld);

	return output;
}

struct PS_CUBEMAP_IN
{
	float4 vPos	:	SV_POSITION;
	uint iIndex	:	SV_RenderTargetArrayIndex;
};

[maxvertexcount(18)]
void GS_CubeMap(triangle GS_CUBEMAP_IN input[3],
	inout TriangleStream<PS_CUBEMAP_IN> CubeMapStream)
{
	for (int i = 0; i < 6; i++)
	{
		PS_CUBEMAP_IN output;

		output.iIndex = i;

		for (int j = 0; j < 3; j++)
		{
			output.vPos = mul(input[j].vPos, g_matLightViewProj[i]);
			CubeMapStream.Append(output);
		}

		CubeMapStream.RestartStrip();
	}
}