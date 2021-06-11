#include "Share.fx"

struct VS_OUTPUT_DECAL
{
	float4	vPos	:	SV_POSITION;
	float4	vProjPos	:	POSITION;
};

struct PS_OUTPUT_DECAL
{
	float4	vAlbedo	:	SV_TARGET;
	float4	vGBuffer1	:	SV_TARGET1;
	float4	vGBuffer2	:	SV_TARGET2;
};

cbuffer Decal	:	register(b3)
{
	matrix	g_matInvWVP;
	float	g_fAlpha;
	float	g_fDecalRadius;
}

Texture2D	g_Albedo	:	register(t10);
Texture2D	g_GBuffer1	:	register(t11);
Texture2D	g_GBuffer2	:	register(t12);
Texture2D	g_GBuffer3	:	register(t13);
Texture2D	g_GBuffer4	:	register(t14);
Texture2D	g_GBuffer5	:	register(t15);

VS_OUTPUT_DECAL DecalVS(float3 vPos	:	POSITION)
{
	VS_OUTPUT_DECAL output = (VS_OUTPUT_DECAL)0.f;

	output.vPos = mul(float4(vPos, 1.f), g_matWVP);
	output.vProjPos = output.vPos;

	return output;
}

PS_OUTPUT_DECAL DecalPS(VS_OUTPUT_DECAL input)
{
	PS_OUTPUT_DECAL output = (PS_OUTPUT_DECAL)0.f;

	float4 vPos = input.vProjPos / input.vProjPos.w;
	float2 vUV = vPos.xy;

	vUV = vUV * float2(0.5f, -0.5f) + float2(0.5f, 0.5f);

	float4 vGBuffer2 = g_GBuffer2.Sample(g_tPoint, vUV);

	vPos.z = vGBuffer2.x;
	vPos.w = 1.f;

	vPos *= vGBuffer2.w;

	float3 vLocal = mul(vPos, g_matInvWVP).xyz;

	float2 vScreenUV = vLocal.xz + float2(0.5f, 0.5f);

	vScreenUV.y = 1.f - vScreenUV.y;

	float4 vColor = g_tTex.Sample(g_tAnisotropic, vScreenUV);

	if (vColor.w == 0.f || vGBuffer2.z == 0.f  || vGBuffer2.w == 0.f)
	{
		clip(-1);
	}

	clip(0.5f - abs(vLocal));

	float fAlpha = g_fAlpha * vColor.a;

	output.vAlbedo = vColor* fAlpha + g_Albedo.Sample(g_tPoint, vUV) * (1.f - fAlpha);

	float3 vTangent = g_GBuffer4.Sample(g_tPoint, vUV).xyz;
	float3 vBinormal = g_GBuffer5.Sample(g_tPoint, vUV).xyz;

	float3 vNormal = cross(vTangent, vBinormal);

	float3 vBumpedNormal = ComputeBumpNormalOrtho(vNormal, vTangent, vBinormal, vScreenUV);
	float4 vGBuffer1 = g_GBuffer1.Sample(g_tPoint, vUV);
	output.vGBuffer1.xyz = vBumpedNormal *fAlpha + vGBuffer1.xyz * (1.f - fAlpha);
	output.vGBuffer1.w = 1.f;

	float4 vOriginMtrl = g_GBuffer3.Sample(g_tPoint, vUV);

	float4 vMtrl = 0.f;

	vMtrl.r = ConvertColor(g_vDif * fAlpha + (1- fAlpha) * ConvertColor(vOriginMtrl.r));
	vMtrl.g = ConvertColor(g_vAmb * fAlpha + (1 - fAlpha) * ConvertColor(vOriginMtrl.g));
	
	float4 vSpc = g_vSpe;

	if (g_iSpecTex)
	{
		vSpc = g_SpcTex.Sample(g_tAnisotropic, vScreenUV);
	}
	vSpc.w = 1.f;

	vMtrl.b = ConvertColor(vSpc * fAlpha + (1 - fAlpha) * ConvertColor(vOriginMtrl.b));

	float4 vEmv = g_vEmi;

	if (g_iEmiTex)
	{
		vEmv = g_EmiTex.Sample(g_tAnisotropic, vScreenUV);
	}

	vMtrl.a = ConvertColor(vEmv *fAlpha + (1 - fAlpha) * ConvertColor(vOriginMtrl.a));

	output.vGBuffer2 = vMtrl;

	return output;
}


PS_OUTPUT_DECAL DecalAlphaMaskPS(VS_OUTPUT_DECAL input)
{
	PS_OUTPUT_DECAL output = (PS_OUTPUT_DECAL)0.f;

	float4 vPos = input.vProjPos / input.vProjPos.w;
	float2 vUV = vPos.xy;

	vUV = vUV * float2(0.5f, -0.5f) + float2(0.5f, 0.5f);

	float4 vGBuffer2 = g_GBuffer2.Sample(g_tPoint, vUV);

	vPos.z = vGBuffer2.x;
	vPos.w = 1.f;

	vPos *= vGBuffer2.w;

	float3 vLocal = mul(vPos, g_matInvWVP).xyz;

	float2 vScreenUV = vLocal.xz + float2(0.5f, 0.5f);

	vScreenUV.y = 1.f - vScreenUV.y;

	float4 vColor = g_vDif;
	
	if (g_iDifTex)
	{
		vColor = PaperBurn(g_tTex.Sample(g_tAnisotropic, vScreenUV), vScreenUV);
	}

	if (vColor.w == 0.f || vGBuffer2.z == 0.f || vGBuffer2.w == 0.f ||
		0.5f < vLocal.x || 0.5f < vLocal.y || 0.5f < vLocal.z ||
		-0.5f > vLocal.x || -0.5f > vLocal.y || -0.5f > vLocal.z)
	{
		clip(-1);
	}

	float4 vEmv = g_vEmi;

	if (g_iEmiTex)
	{
		vEmv = g_EmiTex.Sample(g_tAnisotropic, vScreenUV);
		clip(vEmv.r - 0.15f);
	}

	float fAlpha = g_fAlpha * vEmv.r;

	output.vAlbedo = vColor * fAlpha + g_Albedo.Sample(g_tPoint, vUV) * (1 - fAlpha);

	float3 vTangent = g_GBuffer4.Sample(g_tPoint, vUV).xyz;
	float3 vBinormal = g_GBuffer5.Sample(g_tPoint, vUV).xyz;
	float3 vNormal = cross(vTangent, vBinormal);

	float3 vBumpedNormal = ComputeBumpNormal(vNormal, vTangent, vBinormal, vScreenUV);
	output.vGBuffer1.xyz = vBumpedNormal * fAlpha + g_GBuffer1.Sample(g_tPoint, vUV).xyz * (1.f - fAlpha);
	output.vGBuffer1.w = 1.f;

	float4 vOriginMtrl = g_GBuffer3.Sample(g_tPoint, vUV);

	float4 vMtrl = 0.f;

	vMtrl.r = ConvertColor(g_vDif * fAlpha + (1 - fAlpha) * ConvertColor(vOriginMtrl.r));
	vMtrl.g = ConvertColor(g_vAmb * fAlpha + (1 - fAlpha) * ConvertColor(vOriginMtrl.g));

	float4 vSpc = g_vSpe;

	if (g_iSpecTex)
	{
		vSpc = g_SpcTex.Sample(g_tAnisotropic, vScreenUV);
	}
	vSpc.w = 1.f;

	vMtrl.b = ConvertColor(vSpc * fAlpha + (1 - fAlpha) * ConvertColor(vOriginMtrl.b));

	output.vGBuffer2 = vMtrl;

	return output;
}


PS_OUTPUT_DECAL DecalSpherePS(VS_OUTPUT_DECAL input)
{
	PS_OUTPUT_DECAL output = (PS_OUTPUT_DECAL)0.f;

	float4 vPos = input.vProjPos / input.vProjPos.w;
	float2 vUV = vPos.xy;

	vUV = vUV * float2(0.5f, -0.5f) + float2(0.5f, 0.5f);

	float4 vGBuffer2 = g_GBuffer2.Sample(g_tPoint, vUV);

	vPos.z = vGBuffer2.x;
	vPos.w = 1.f;

	vPos *= vGBuffer2.w;

	float3 vLocal = mul(vPos, g_matInvWVP).xyz;

	float2 vScreenUV = vLocal.xz + float2(0.5f, 0.5f);

	vScreenUV.y = 1.f - vScreenUV.y;

	float4 vColor = g_tTex.Sample(g_tAnisotropic, vUV + float2(g_fAccTime * 0.01f, g_fAccTime * 0.02f));

	float fDist = length(vLocal) * 2.f;

	if (vColor.w == 0.f || vGBuffer2.z == 0.f || vGBuffer2.w == 0.f ||
		 fDist > 1.f)
	{
		clip(-1);
	}

	float fAlpha = g_fAlpha * vColor.a * (1.f - fDist *  fDist);

	output.vAlbedo = vColor * fAlpha + g_Albedo.Sample(g_tPoint, vUV) * (1.f - fAlpha);

	float3 vTangent = g_GBuffer4.Sample(g_tPoint, vUV).xyz;
	float3 vBinormal = g_GBuffer5.Sample(g_tPoint, vUV).xyz;

	float3 vNormal = cross(vTangent, vBinormal);

	float3 vBumpedNormal = ComputeBumpNormalOrtho(vNormal, vTangent, vBinormal, vScreenUV);
	float4 vGBuffer1 = g_GBuffer1.Sample(g_tPoint, vUV);
	output.vGBuffer1.xyz = vBumpedNormal * fAlpha + vGBuffer1.xyz * (1.f - fAlpha);
	output.vGBuffer1.w = 1.f;

	float4 vOriginMtrl = g_GBuffer3.Sample(g_tPoint, vUV);

	float4 vMtrl = 0.f;

	vMtrl.r = ConvertColor(g_vDif * fAlpha + (1 - fAlpha) * ConvertColor(vOriginMtrl.r));
	vMtrl.g = ConvertColor(g_vAmb * fAlpha + (1 - fAlpha) * ConvertColor(vOriginMtrl.g));

	float4 vSpc = g_vSpe;

	if (g_iSpecTex)
	{
		vSpc = g_SpcTex.Sample(g_tAnisotropic, vScreenUV);
	}
	vSpc.w = 1.f;

	vMtrl.b = ConvertColor(vSpc * fAlpha + (1 - fAlpha) * ConvertColor(vOriginMtrl.b));

	float4 vEmv = g_vEmi;

	if (g_iEmiTex)
	{
		vEmv = g_EmiTex.Sample(g_tAnisotropic, vScreenUV);
	}

	vMtrl.a = ConvertColor(vEmv * fAlpha + (1 - fAlpha) * ConvertColor(vOriginMtrl.a));

	output.vGBuffer2 = vMtrl;

	return output;
}

PS_OUTPUT_DECAL DecalRadiusPS(VS_OUTPUT_DECAL input)
{
	PS_OUTPUT_DECAL output = (PS_OUTPUT_DECAL)0.f;

	float4 vPos = input.vProjPos / input.vProjPos.w;
	float2 vUV = vPos.xy;

	vUV = vUV * float2(0.5f, -0.5f) + float2(0.5f, 0.5f);

	float4 vGBuffer2 = g_GBuffer2.Sample(g_tPoint, vUV);

	vPos.z = vGBuffer2.x;
	vPos.w = 1.f;

	vPos *= vGBuffer2.w;

	float3 vLocal = mul(vPos, g_matInvWVP).xyz;

	float2 vScreenUV = vLocal.xz + float2(0.5f, 0.5f);

	vScreenUV.y = 1.f - vScreenUV.y;

	float4 vColor = g_tTex.Sample(g_tAnisotropic, vScreenUV);

	if (vGBuffer2.z == 0.f || vGBuffer2.w == 0.f)
	{
		clip(-1);
	}

	clip(0.5f - abs(vLocal));

	float fRadius = length(vLocal.xz * g_matWorld._11_33);
	float fAlpha = g_fAlpha * vColor.a;
	float fSize = g_matWorld._11 * 0.5f;

	if (fRadius < g_fDecalRadius)
	{
		vColor = float4(1.f, 0.f, 0.f, 1.f);		
		fAlpha = 0.5f;
	}
	else if (fRadius >= g_fDecalRadius - 1.f
		&& fRadius <= g_fDecalRadius)
	{
		vColor = float4(1.f, 1.f, 1.f, 1.f);
		fAlpha = 1.f;
	}

	output.vAlbedo = vColor * fAlpha + g_Albedo.Sample(g_tPoint, vUV) * (1.f - fAlpha);

	float3 vTangent = g_GBuffer4.Sample(g_tPoint, vUV).xyz;
	float3 vBinormal = g_GBuffer5.Sample(g_tPoint, vUV).xyz;

	float3 vNormal = cross(vTangent, vBinormal);

	float3 vBumpedNormal = ComputeBumpNormalOrtho(vNormal, vTangent, vBinormal, vScreenUV);
	float4 vGBuffer1 = g_GBuffer1.Sample(g_tPoint, vUV);
	output.vGBuffer1.xyz = vBumpedNormal * fAlpha + vGBuffer1.xyz * (1.f - fAlpha);
	output.vGBuffer1.w = 1.f;

	float4 vOriginMtrl = g_GBuffer3.Sample(g_tPoint, vUV);

	float4 vMtrl = 0.f;

	vMtrl.r = ConvertColor(g_vDif * fAlpha + (1 - fAlpha) * ConvertColor(vOriginMtrl.r));
	vMtrl.g = ConvertColor(g_vAmb * fAlpha + (1 - fAlpha) * ConvertColor(vOriginMtrl.g));

	float4 vSpc = g_vSpe;

	if (g_iSpecTex)
	{
		vSpc = g_SpcTex.Sample(g_tAnisotropic, vScreenUV);
	}
	vSpc.w = 1.f;

	vMtrl.b = ConvertColor(vSpc * fAlpha + (1 - fAlpha) * ConvertColor(vOriginMtrl.b));

	float4 vEmv = g_vEmi;

	if (g_iEmiTex)
	{
		vEmv = g_EmiTex.Sample(g_tAnisotropic, vScreenUV);
	}

	vMtrl.a = ConvertColor(vEmv * fAlpha + (1 - fAlpha) * ConvertColor(vOriginMtrl.a));

	output.vGBuffer2 = vMtrl;

	return output;
}