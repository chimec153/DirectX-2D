#include "Share.fx"

static const float	g_fDepthBias = 0.0005f;
Texture2D	g_Albedo	:	register(t10);
Texture2D	g_GBuffer1	:	register(t11);
Texture2D	g_GBuffer2	:	register(t12);
Texture2D	g_GBuffer3	:	register(t13);

Texture2D	g_LightDif	:	register(t14);
Texture2D	g_LightSpc	:	register(t15);
Texture2D	g_LightEmv	:	register(t16);

Texture2D	g_DecalAlbedo	:	register(t17);
Texture2D	g_DecalGBuffer1	:	register(t18);
Texture2D	g_DecalGBuffer2	:	register(t19);
Texture2D	g_Shadow	:	register(t20);

struct VS_INPUT
{
	float3	vPos	:	POSITION;
};

struct VS_OUTPUT
{
	float4	vPos	:	SV_POSITION;
	float4	vProjPos	:	POSITION;
	float2	vUV	:	TEXCOORD;
};

static const float4 g_NullPos[4] =
{
	float4(-1.f, 1.f, 0.f, 1.f),
	float4(1.f, 1.f, 0.f, 1.f),
	float4(-1.f, -1.f, 0.f, 1.f),
	float4(1.f, -1.f, 0.f, 1.f),
};

static const float2 g_NullUV[4] =
{
	float2(0.f, 0.f),
	float2(1.f, 0.f),
	float2(0.f, 1.f),
	float2(1.f, 1.f),
};

VS_OUTPUT LightAccVS(uint ID	:	SV_VertexID)
{
	VS_OUTPUT output = (VS_OUTPUT)0.f;

	output.vPos = g_NullPos[ID];
	output.vProjPos = output.vPos;
	output.vUV = g_NullUV[ID];

	return output;
}

PS_OUTPUT_MULTI LightAccPS(VS_OUTPUT input)
{
	PS_OUTPUT_MULTI output = (PS_OUTPUT_MULTI)0.f;

	float4 Albedo = 0.f;
	float4 GBuffer1 = 0.f;
	float4 GBuffer2 = g_GBuffer2.Sample(g_tPoint, input.vUV);
	float4 GBuffer3 = 0.f;

	clip(GBuffer2.w != 0.f);

	if (GBuffer2.z == 1.f)
	{
		GBuffer1 = g_DecalGBuffer1.Sample(g_tPoint, input.vUV);

		if (GBuffer1.z == 0.f)
		{
			GBuffer1 = g_GBuffer1.Sample(g_tPoint, input.vUV);
			GBuffer3 = g_GBuffer3.Sample(g_tPoint, input.vUV);
		}

		else
		{
			GBuffer3 = g_DecalGBuffer2.Sample(g_tPoint, input.vUV);
			GBuffer1.w = g_GBuffer1.Sample(g_tPoint, input.vUV).w;
		}
	}

	else
	{
		GBuffer1 = g_GBuffer1.Sample(g_tPoint, input.vUV);
		GBuffer3 = g_GBuffer3.Sample(g_tPoint, input.vUV);
	}

	float3 vNormal = GBuffer1.xyz;
	float4 ProjPos = 0.f;
	ProjPos.x = input.vUV.x * 2.f - 1.f;
	ProjPos.y = input.vUV.y * -2.f + 1.f;
	ProjPos.z = GBuffer2.x;
	ProjPos.w = 1.f;

	ProjPos *= GBuffer2.w;
	float3 ViewPos = mul(ProjPos, g_matProjInv).xyz;
	float3 WorldPos = mul(float4(ViewPos,1.f), g_matViewInv).xyz;
	float4 vShadowProj = mul(float4(WorldPos, 1.f), g_matShadowViewProj);
	float3 LightWorldPos = mul(float4(g_vLightPos, 1.f), g_matViewInv).xyz;

	float fSpecPower = GBuffer2.y;
	LightResult tResult = ComputeLightAcc(ViewPos, vNormal, input.vUV, GBuffer3, fSpecPower);

	float fShadow = 1.f;

	float fShadowDepth = GBuffer1.w;

	float2 vShadowUV = vShadowProj.xy / vShadowProj.w;
	vShadowUV.y *= -1.f;
	vShadowUV += 1.f;
	vShadowUV *= 0.5f;

	//fShadow = PointShadowPCF(WorldPos - LightWorldPos);
	fShadow = g_Shadow.SampleCmpLevelZero(g_tShadow, vShadowUV, fShadowDepth).r;
	/*float fDepth = g_Shadow.Sample(g_tPoint, vShadowUV).r;
	
	if (fShadowDepth > fDepth + g_fDepthBias)
	{
		fShadow = fShadowDepth - fDepth;
	}*/

	output.LightDif = fShadow *tResult.vDif + tResult.vAmb;
	output.LightSpc = fShadow *tResult.vSpc;
	output.LightEmv = tResult.vEmi;

	return output;
}

PS_OUTPUT_SINGLE LightCombinePS(VS_OUTPUT input)
{
	PS_OUTPUT_SINGLE output = (PS_OUTPUT_SINGLE)0.f;

	float4 vAlbedo = 0.f;
	float4 DecalGBuffer1 = g_DecalGBuffer1.Sample(g_tPoint, input.vUV);

	if (DecalGBuffer1.a == 0.f)
	{
		vAlbedo = g_Albedo.Sample(g_tPoint, input.vUV);
	}
	else
	{
		vAlbedo = g_DecalAlbedo.Sample(g_tPoint, input.vUV);
	}

	float4 vLightDif = g_LightDif.Sample(g_tPoint, input.vUV);
	float4 vLightSpc = g_LightSpc.Sample(g_tPoint, input.vUV);
	float4 vLightEmv = g_LightEmv.Sample(g_tPoint, input.vUV);

	output.vColor = vAlbedo *vLightDif + vLightSpc + vLightEmv;
	output.vColor.a = vAlbedo.a;

	if(output.vColor.a == 0.f)
		clip(-1);

	return output;
}