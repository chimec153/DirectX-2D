#include "Share.fx"

Texture2D	g_DepthTexture	:	register(t12);
Texture2D	g_Shadow	:	register(t20);

static const float	g_fDepthBias = 0.0005f;

VS_OUTPUT_3D StandardVS3D(VS_INPUT_3D input)
{
	VS_OUTPUT_3D output = (VS_OUTPUT_3D)0.f;
	Skinning tSkin = Skining(input.vPos, input.vNormal, input.vTangent, input.vBinormal, input.vBlendWeight, input.vBlendIndex);

	tSkin.vPos = tSkin.vPos - g_vPivot * g_vMeshSize;
	output.vUV = input.vUV;//mul(float4(input.vUV, 0.f, 1.f), g_matTexTransform).xy;
	output.vPos = mul(float4(tSkin.vPos, 1.f), g_matWVP);
	output.vProjPos = output.vPos;
	output.vPosV = mul(float4(tSkin.vPos, 1.f), g_matWV).xyz;
	output.vPosW = mul(float4(tSkin.vPos, 1.f), g_matWorld).xyz;
	output.vNormal = normalize(mul(float4(tSkin.vNormal,0.f), g_matWV).xyz);
	output.vTangent = normalize(mul(float4(tSkin.vTangent, 0.f), g_matWV).xyz);
	output.vBinormal = normalize(mul(float4(tSkin.vBinormal, 0.f), g_matWV).xyz);

	output.vShadowPos = mul(float4(tSkin.vPos, 1.f), g_matLightWorldViewProjTexture);
	output.vPosL = input.vPos;

	return output;
}

VS_OUTPUT_3D InstVS3D(VS_INPUT_INST_3D input, uint iInstID	:	SV_InstanceID)
{
	VS_OUTPUT_3D output = (VS_OUTPUT_3D)0.f;

	Skinning tSkin = Skining(input.vPos, input.vNormal, input.vTangent,
		input.vBinormal, input.vBlendWeight, input.vBlendIndex, iInstID);

	output.vUV = input.vUV;
	output.vPos = mul(float4(tSkin.vPos, 1.f), input.matWVP);
	output.vProjPos = output.vPos;
	output.vPosV = mul(float4(tSkin.vPos, 1.f), input.matWVP).xyz;
	output.vNormal = normalize(mul(float4(tSkin.vNormal, 0.f), input.matWV).xyz);
	output.vTangent = normalize(mul(float4(tSkin.vTangent, 0.f), input.matWV).xyz);
	output.vBinormal = normalize(mul(float4(tSkin.vBinormal, 0.f), input.matWV).xyz);
	output.vShadowPos = mul(float4(tSkin.vPos, 1.f), input.matShadowWVP);

	return output;
}

PS_OUTPUT_TARGET StandardPS3D(VS_OUTPUT_3D input)
{
	PS_OUTPUT_TARGET output = (PS_OUTPUT_TARGET)0.f;
	/*float2 vUV = input.vUV;
	LightResult tResult = 
		ComputeLight(input.vPosV, input.vNormal, input.vTangent,
	input.vBinormal, input.vUV);
	float4 vColor = g_vDif;
	if(g_iDifTex)
	{
		vColor = g_tTex.Sample(g_tAnisotropic, vUV);
	}

	vColor = vColor * (tResult.vDif + vColor * tResult.vAmb)
		+ tResult.vSpc + tResult.vEmi;*/

	//input.vProjTex.xyz /= input.vProjTex.w;

	//float depth = input.vProjTex.z;

	//float4 vDif = g_Proj.Sample(g_tAnisotropic, input.vProjTex.xy);

	float4 vDif = g_tTex.Sample(g_tAnisotropic, input.vUV);

	float fDepth = input.vShadowPos.z / input.vShadowPos.w;	

	clip(vDif.a - 0.15f);
	float2 vUV = input.vPosL.xy / g_vMeshSize.xy;
	output.Albedo = PaperBurn(vDif, vUV);

	output.GBuffer1.xyz = ComputeBumpNormal(input.vNormal, input.vTangent, input.vBinormal, input.vUV);
	output.GBuffer1.w = fDepth;
	output.GBuffer2.x = input.vProjPos.z / input.vProjPos.w;
	output.GBuffer2.w = input.vProjPos.w;
	output.GBuffer2.y = g_vSpe.w;
	output.GBuffer2.z = g_iDecal;
	output.GBuffer3.x = ConvertColor(g_vDif) ;
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
		vEmv =g_EmiTex.Sample(g_tAnisotropic, input.vUV);
	}
	output.GBuffer3.z = ConvertColor(vSpc);
	output.GBuffer3.w = ConvertColor(PaperBurn(vEmv, vUV));

	return output;
}

VS_OUTPUT_3D_COLLIDER InstColliderVS3D(VS_INPUT_INST_COLLIDER_3D input,
	uint iInstID	:	SV_InstanceID)
{
	VS_OUTPUT_3D_COLLIDER output = (VS_OUTPUT_3D_COLLIDER)0.f;

	float3 vPos = input.vPos - g_vPivot * g_vMeshSize;

	Skinning tSkin = Skining(vPos, input.vNormal, input.vTangent,
		input.vBinormal, input.vBlendWeight, input.vBlendIndex, iInstID);

	output.vUV = input.vUV;
	output.vPos = mul(float4(tSkin.vPos, 1.f), input.matWVP);
	output.vProjPos = output.vPos;
	output.vPosV = mul(float4(tSkin.vPos, 1.f), input.matWVP).xyz;
	output.vNormal = normalize(mul(float4(tSkin.vNormal, 0.f), input.matWV).xyz);
	output.vTangent = normalize(mul(float4(tSkin.vTangent, 0.f), input.matWV).xyz);
	output.vBinormal = normalize(mul(float4(tSkin.vBinormal, 0.f), input.matWV).xyz);
	output.vShadowPos = mul(float4(tSkin.vPos, 1.f), input.matShadowWVP);
	output.vColor = input.vColor;

	return output;
}

PS_OUTPUT_TARGET ColliderInstPS3D(VS_OUTPUT_3D_COLLIDER input)
{
	PS_OUTPUT_TARGET output = (PS_OUTPUT_TARGET)0.f;

	output.Albedo = input.vColor;


	output.GBuffer1.xyz = ComputeBumpNormal(input.vNormal, input.vTangent, input.vBinormal, input.vUV);
	float fDepth = input.vShadowPos.z / input.vShadowPos.w;
	output.GBuffer1.w = fDepth;
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

float4 PS3D(VS_OUTPUT_3D input) : SV_TARGET
{
	float3 toEyeW = g_vLightPos - input.vPosW;
	float distToEye = length(toEyeW);

	float fogLerp = saturate((distToEye - 10.f) / 500.f);

	return lerp(g_tTex.Sample(g_tAnisotropic, input.vUV), float4(0.2f, 0.2f, 0.2f,1.f ), fogLerp);
}

float4 Re3D(VS_OUTPUT_3D input) : SV_TARGET
{
	PS_OUTPUT_SINGLE output = (PS_OUTPUT_SINGLE)0.f;

	input.vProjPos.xyz = normalize(input.vProjPos.xyz);

	float3 toEyeW = g_vLightPos - input.vPosW;
	float distToEye = length(toEyeW);
	toEyeW /= distToEye;

	float4 ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 spec = float4(0.0f, 0.0f, 0.0f, 0.0f);

	float4 A, D, S;


	float4 texColor = float4(1.f, 1.f, 1.f, 1.f);
	texColor = g_tTex.Sample(g_tAnisotropic, input.vUV);

	float4 litColor = texColor * (ambient + diffuse) + spec;
	return litColor;
	if (true)
	{
		//float fogLerp = saturate((distToEye - gFogStart) / gFogRange);

		//litColor = lerp(litColor, gFogColor, fogLerp);
	}

	float3 incident = -toEyeW;
	float3 reflectionVector = reflect(incident, input.vNormal);
	float4 reflectionColor = gCubeMap.Sample(g_tAnisotropic, reflectionVector);

	litColor += g_vEmi * reflectionColor;

	return litColor;
}

VS_OUTPUT_3D ProjPointVS(VS_INPUT_3D input)
{
	VS_OUTPUT_3D output = (VS_OUTPUT_3D)0.f;

	Skinning tSkin = Skining(input.vPos, input.vNormal, input.vTangent, 
		input.vBinormal, input.vBlendWeight, input.vBlendIndex);

	output.vPos = mul(float4(tSkin.vPos, 1.f), g_matWVP);
	output.vNormal = normalize(mul(float4(tSkin.vNormal, 0.f), g_matWV).xyz);
	output.vTangent = normalize(mul(float4(tSkin.vTangent, 0.f), g_matWV).xyz);
	output.vBinormal = normalize(mul(float4(tSkin.vBinormal, 0.f), g_matWV).xyz);
	output.vUV = input.vUV;
	output.vPosW = mul(float4(tSkin.vPos, 1.f), g_matWorld).xyz;
	output.vPosV = mul(float4(tSkin.vPos, 1.f), g_matWV).xyz;

	return output;
}

//태양의 위치를 구한다. 
//하얀색 구로 표현한다
//포스트 프로세싱
// 오브젝트를 띄우는 방법이 있고 셰이더로만 처리를 하는 방법이 있다.
//모든 오브젝트를 랜더링하고 디퍼드 셰이딩이 끝나기 전이나 후에 처리를 하는 기법을 포스트 프로세싱이라고 한다.

float3 GetDirToLight(float3 WorldPosition)
{
	return mul(WorldPosition, (float3x3)g_matLightTransform);
}

float4 GetLightColor(float3 SampleDirection)
{
	return gCubeMap2.Sample(g_tLinear, SampleDirection);
}

float4 ProjPointPS(VS_OUTPUT_3D input, float4 vScreen	:	SV_POSITION) : SV_TARGET
{
	float3 vDir = GetDirToLight(g_vLightPos - input.vPosV);

	float4 vColor = GetLightColor(vDir);

	LightResult tResult = ComputeLight(input.vPosV, input.vNormal, input.vTangent, input.vBinormal, input.vUV);

	return vColor * (tResult.vDif + vColor * tResult.vAmb) + tResult.vEmi + tResult.vSpc;
}


PS_OUTPUT_TARGET TreePS(VS_OUTPUT_3D input)
{
	PS_OUTPUT_TARGET output = (PS_OUTPUT_TARGET)0.f;

	float4 vEmv = g_vEmi;
	if (g_iEmiTex)
	{
		vEmv = g_EmiTex.Sample(g_tAnisotropic, input.vUV);
	}

	if (vEmv.g == 0.f)
		clip(-1);


	float4 vDif = g_vDif;
	if (g_iDifTex)
	{
		vDif = g_tTex.Sample(g_tAnisotropic, input.vUV);
	}

	vDif.w = vEmv.g;
	output.Albedo = vDif;
	output.GBuffer1.xyz = ComputeBumpNormal(input.vNormal, input.vTangent, input.vBinormal, input.vUV).xyz;
	float fDepth = input.vShadowPos.z / input.vShadowPos.w;
	output.GBuffer1.w = fDepth;
	output.GBuffer2.x = input.vProjPos.z / input.vProjPos.w;
	output.GBuffer2.w = input.vProjPos.w;
	output.GBuffer2.y = g_vSpe.w;
	output.GBuffer2.z = g_iDecal;
	output.GBuffer3.x = ConvertColor(g_vDif);
	output.GBuffer3.y = ConvertColor(g_vAmb);
	output.GBuffer4.xyz = input.vTangent;
	output.GBuffer5.xyz = input.vBinormal;

	float4 vSpc = g_vSpe;
	if (g_iSpecTex)
	{
		vSpc = g_SpcTex.Sample(g_tAnisotropic, input.vUV);
	}
	vSpc.w = 1.f;

	output.GBuffer3.z = ConvertColor(vSpc);
	//output.GBuffer3.w = ConvertColor(vEmv);

	return output;
}


PS_OUTPUT_TARGET EmvRClipPaperPS(VS_OUTPUT_3D input)
{
	PS_OUTPUT_TARGET output = (PS_OUTPUT_TARGET)0.f;

	float4 vEmv = g_vEmi;
	if (g_iEmiTex)
	{
		vEmv = g_EmiTex.Sample(g_tAnisotropic, input.vUV);
	}

	if (vEmv.r < 0.9f)
		clip(-1);

	vEmv = 0.f;

	float4 vDif = g_vDif;
	if (g_iDifTex)
	{
		vDif = g_tTex.Sample(g_tAnisotropic, input.vUV);
	}

	float2 vUV = input.vPosL.xy / g_vMeshSize.xy;
	
	vDif = PaperBurn(vDif, vUV);
	output.Albedo = vDif;
	output.GBuffer1.xyz = ComputeBumpNormal(input.vNormal, input.vTangent, input.vBinormal, input.vUV).xyz;
	float fDepth = input.vShadowPos.z / input.vShadowPos.w;
	output.GBuffer1.w = fDepth;
	output.GBuffer2.x = input.vProjPos.z / input.vProjPos.w;
	output.GBuffer2.w = input.vProjPos.w;
	output.GBuffer2.y = g_vSpe.w;
	output.GBuffer2.z = g_iDecal;
	output.GBuffer3.x = ConvertColor(g_vDif);
	output.GBuffer3.y = ConvertColor(g_vAmb);
	output.GBuffer4.xyz = input.vTangent;
	output.GBuffer5.xyz = input.vBinormal;

	float4 vSpc = g_vSpe;
	if (g_iSpecTex)
	{
		vSpc = g_SpcTex.Sample(g_tAnisotropic, input.vUV);
	}
	vSpc.w = 1.f;

	output.GBuffer3.z = ConvertColor(vSpc);
	output.GBuffer3.w = ConvertColor(vEmv);

	return output;
}
PS_OUTPUT_TARGET SpecRPS(VS_OUTPUT_3D input)
{
	PS_OUTPUT_TARGET output = (PS_OUTPUT_TARGET)0.f;

	float4 vDif = g_tTex.Sample(g_tAnisotropic, input.vUV);
	float2 vUV = input.vPosL.xy / g_vMeshSize.xy;

	output.Albedo = PaperBurn(vDif, vUV);

	output.GBuffer1.xyz = ComputeBumpNormal(input.vNormal, input.vTangent, input.vBinormal, input.vUV);
	float fDepth = input.vShadowPos.z / input.vShadowPos.w;
	output.GBuffer1.w = fDepth;
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
		vSpc = g_SpcTex.Sample(g_tAnisotropic, input.vUV).rrrr;
	}

	float4 vEmv = g_vEmi;
	if (g_iEmiTex)
	{
		vEmv = g_EmiTex.Sample(g_tAnisotropic, input.vUV);

		if (vEmv.g < 0.9f)
			clip(-1);

		vEmv = g_vEmi;
	}
	output.GBuffer3.z = ConvertColor(vSpc);
	output.GBuffer3.w = ConvertColor(PaperBurn(vEmv, vUV));

	return output;
}


PS_OUTPUT_TARGET EmvRClipPS(VS_OUTPUT_3D input)
{
	PS_OUTPUT_TARGET output = (PS_OUTPUT_TARGET)0.f;

	float4 vEmv = g_vEmi;
	if (g_iEmiTex)
	{
		vEmv = g_EmiTex.Sample(g_tAnisotropic, input.vUV);
	}

	clip(vEmv.r);

	float4 vDif = g_vDif;
	if (g_iDifTex)
	{
		vDif = g_tTex.Sample(g_tAnisotropic, input.vUV);
	}

	vDif.a = vEmv.r;
	float2 vUV = input.vPosL.xy / g_vMeshSize.xy;
	vDif = PaperBurn(vDif, vUV);

	output.Albedo = vDif;
	output.GBuffer1.xyz = ComputeBumpNormal(input.vNormal, input.vTangent, input.vBinormal, input.vUV).xyz;
	float fDepth = input.vShadowPos.z / input.vShadowPos.w;
	output.GBuffer1.w = fDepth;
	output.GBuffer2.x = input.vProjPos.z / input.vProjPos.w;
	output.GBuffer2.w = input.vProjPos.w;
	output.GBuffer2.y = g_vSpe.w;
	output.GBuffer2.z = g_iDecal;
	output.GBuffer3.x = ConvertColor(g_vDif);
	output.GBuffer3.y = ConvertColor(g_vAmb);
	output.GBuffer4.xyz = input.vTangent;
	output.GBuffer5.xyz = input.vBinormal;

	float4 vSpc = g_vSpe;
	if (g_iSpecTex)
	{
		vSpc = g_SpcTex.Sample(g_tAnisotropic, input.vUV);
	}
	vSpc.w = 1.f;

	output.GBuffer3.z = ConvertColor(vSpc);

	return output;
}

PS_OUTPUT_SINGLE EmvRClipAlphaPS(VS_OUTPUT_3D input)
{
	PS_OUTPUT_SINGLE output = (PS_OUTPUT_SINGLE)0.f;

	float2 vUV = input.vProjPos.xy / input.vProjPos.w;
	vUV *= float2(0.5f, -0.5f);
	vUV += 0.5f;

	float fPrevDepth = g_DepthTexture.Sample(g_tPoint, vUV).r;

	if (fPrevDepth > 0.f)
	{
		float fDepth = input.vProjPos.z / input.vProjPos.w;

		clip(fPrevDepth - fDepth);
	}

	float4 vEmv = g_vEmi;
	if (g_iEmiTex)
	{
		vEmv = g_EmiTex.Sample(g_tAnisotropic, input.vUV);
	}

	clip(vEmv.r != 0.f);

	LightResult tResult = ComputeLight(input.vPosV, input.vNormal, input.vTangent,
		input.vBinormal, input.vUV);

	output.vColor = g_vDif;
	if (g_iDifTex)
	{
		output.vColor = g_tTex.Sample(g_tAnisotropic, input.vUV);
	}


	float fDepth = input.vShadowPos.z / input.vShadowPos.w;

	float2 vShadowUV = input.vShadowPos.xy / input.vShadowPos.w;

	vShadowUV.y *= -1.f;
	vShadowUV += 1.f;
	vShadowUV *= 0.5f;

	float fShadowDepth = g_ShadowTexture.Sample(g_tAnisotropic, vShadowUV).r;
	float fShadow = 1.f;

	if (fDepth > fShadowDepth + g_fDepthBias)
	{
		output.vColor.rgb *= 0.5f;
	}

	output.vColor = output.vColor * (tResult.vDif + output.vColor * tResult.vAmb)
		+ tResult.vSpc;

	output.vColor.w = vEmv.r;

	return output;
}
PS_OUTPUT_SINGLE NormalPS(VS_OUTPUT_3D input)
{
	PS_OUTPUT_SINGLE output = (PS_OUTPUT_SINGLE)0.f;

	output.vColor = g_vDif;

	return output;
}


PS_OUTPUT_TARGET EmvGClipPS(VS_OUTPUT_3D input)
{
	PS_OUTPUT_TARGET output = (PS_OUTPUT_TARGET)0.f;

	float4 vEmv = g_vEmi;
	if (g_iEmiTex)
	{
		vEmv = g_EmiTex.Sample(g_tAnisotropic, input.vUV);
	}

	if (vEmv.g < 0.9f)
		clip(-1);


	float4 vDif = g_vDif;
	if (g_iDifTex)
	{
		vDif = g_tTex.Sample(g_tAnisotropic, input.vUV);
	}

	vDif.w = vEmv.g;
	vEmv = 0.f;
	output.Albedo = vDif;
	output.GBuffer1.xyz = ComputeBumpNormal(input.vNormal, input.vTangent, input.vBinormal, input.vUV).xyz;
	float fDepth = input.vShadowPos.z / input.vShadowPos.w;
	output.GBuffer1.w = fDepth;
	output.GBuffer2.x = input.vProjPos.z / input.vProjPos.w;
	output.GBuffer2.w = input.vProjPos.w;
	output.GBuffer2.y = g_vSpe.w;
	output.GBuffer2.z = g_iDecal;
	output.GBuffer3.x = ConvertColor(g_vDif);
	output.GBuffer3.y = ConvertColor(g_vAmb);
	output.GBuffer4.xyz = input.vTangent;
	output.GBuffer5.xyz = input.vBinormal;

	float4 vSpc = g_vSpe;
	if (g_iSpecTex)
	{
		vSpc = g_SpcTex.Sample(g_tAnisotropic, input.vUV);
	}
	vSpc.w = 1.f;

	output.GBuffer3.z = ConvertColor(vSpc);
	output.GBuffer3.w = ConvertColor(vEmv);

	return output;
}

PS_OUTPUT_SINGLE EmvGClipAlphaPS(VS_OUTPUT_3D input)
{
	PS_OUTPUT_SINGLE output = (PS_OUTPUT_SINGLE)0.f;

	float2 vUV = input.vProjPos.xy / input.vProjPos.w;
	vUV *= float2(0.5f, -0.5f);
	vUV += 0.5f;

	float fPrevDepth = g_DepthTexture.Sample(g_tPoint, vUV).r;

	if (fPrevDepth > 0.f)
	{
		float fDepth = input.vProjPos.z / input.vProjPos.w;

		clip(fPrevDepth - fDepth);
	}

	float4 vEmv = g_vEmi;
	if (g_iEmiTex)
	{
		vEmv = g_EmiTex.Sample(g_tAnisotropic, input.vUV);
	}

	clip(vEmv.g != 0.f);

	LightResult tResult = ComputeLight(input.vPosV, input.vNormal, input.vTangent,
		input.vBinormal, input.vUV);

	output.vColor = g_vDif;
	if (g_iDifTex)
	{
		output.vColor = g_tTex.Sample(g_tAnisotropic, input.vUV);
	}

	float fDepth = input.vShadowPos.z / input.vShadowPos.w;

	float2 vShadowUV = input.vShadowPos.xy / input.vShadowPos.w;

	vShadowUV.y *= -1.f;
	vShadowUV += 1.f;
	vShadowUV *= 0.5f;

	float fShadowDepth = g_ShadowTexture.Sample(g_tAnisotropic, vShadowUV).r;
	float fShadow = 1.f;

	if (fDepth > fShadowDepth + g_fDepthBias)
	{
		output.vColor.rgb *= 0.5f;
	}

	output.vColor = output.vColor * (tResult.vDif + output.vColor * tResult.vAmb)
		+ tResult.vSpc;

	output.vColor.w = vEmv.g;

	return output;
}

PS_OUTPUT_SINGLE AlphaPS(VS_OUTPUT_3D input)
{
	PS_OUTPUT_SINGLE output = (PS_OUTPUT_SINGLE)0.f;

	float2 vUV = input.vProjPos.xy / input.vProjPos.w;
	vUV *= float2(0.5f, -0.5f);
	vUV += 0.5f;

	float fPrevDepth = g_DepthTexture.Sample(g_tPoint, vUV).r;

	if (fPrevDepth > 0.f)
	{
		float fDepth = input.vProjPos.z / input.vProjPos.w;

		if (fPrevDepth < fDepth)
		{
			output.vColor = float4(0.f, 0.f, 0.f, 0.f);
			return output;
		}
			
		//clip(fPrevDepth - fDepth);
	}

	LightResult tResult = ComputeLight(input.vPosV, input.vNormal, input.vTangent,
		input.vBinormal, input.vUV);

	output.vColor = g_vDif;
	if (g_iDifTex)
	{
		output.vColor = g_tTex.Sample(g_tAnisotropic, input.vUV);
	}

	float fDepth = input.vShadowPos.z / input.vShadowPos.w;

	float2 vShadowUV = input.vShadowPos.xy / input.vShadowPos.w;

	vShadowUV.y *= -1.f;
	vShadowUV += 1.f;
	vShadowUV *= 0.5f;

	float fShadow = g_Shadow.SampleCmpLevelZero(g_tShadow, vShadowUV, fDepth).r;

	output.vColor = PaperBurn(output.vColor, input.vUV);

	clip(output.vColor.a - g_vEmi.a);

	output.vColor = output.vColor * (tResult.vDif * fShadow + output.vColor * tResult.vAmb)
		+ tResult.vSpc * fShadow + tResult.vEmi;


	output.vColor.a = 1.f;

	return output;
}

PS_OUTPUT_SINGLE LimLightPS(VS_OUTPUT_3D input)
{
	PS_OUTPUT_SINGLE output = (PS_OUTPUT_SINGLE)0.f;

	float2 vUV = input.vProjPos.xy / input.vProjPos.w;
	vUV *= float2(0.5f, -0.5f);
	vUV += 0.5f;

	float fPrevDepth = g_DepthTexture.Sample(g_tPoint, vUV).r;

	if (fPrevDepth > 0.f)
	{
		float fDepth = input.vProjPos.z / input.vProjPos.w;

		clip(fPrevDepth - fDepth);
	}

	LightResult tResult = ComputeLight(input.vPosV, input.vNormal, input.vTangent,
		input.vBinormal, input.vUV);

	output.vColor = g_vDif;

	output.vColor = PaperBurn(output.vColor, input.vUV);

	float fDepth = input.vShadowPos.z / input.vShadowPos.w;

	float2 vShadowUV = input.vShadowPos.xy / input.vShadowPos.w;

	vShadowUV.y *= -1.f;
	vShadowUV += 1.f;
	vShadowUV *= 0.5f;

	float fShadowDepth = g_ShadowTexture.Sample(g_tAnisotropic, vShadowUV).r;
	float fShadow = 1.f;

	if (fDepth > fShadowDepth + g_fDepthBias)
	{
		output.vColor.rgb *= 0.5f;
	}
	output.vColor = output.vColor * (tResult.vDif + output.vColor * tResult.vAmb)
		+ tResult.vSpc + tResult.vEmi;

	return output;
}

const static float4 g_NullPos[4] =
{
	float4(-1.f, 1.f, 0.f, 1.f),
	float4(1.f, 1.f, 0.f, 1.f),
	float4(-1.f, -1.f, 0.f, 1.f),
	float4(1.f, -1.f, 0.f, 1.f)
};

VS_OUTPUT_3D NoDepthAlphaVS(VS_INPUT_3D input,
	uint iId	:	SV_VertexID)
{
	VS_OUTPUT_3D output = (VS_OUTPUT_3D)0.f;
	Skinning tSkin = Skining(input.vPos, input.vNormal, input.vTangent, input.vBinormal, input.vBlendWeight, input.vBlendIndex);
	output.vUV = input.vUV;//mul(float4(input.vUV, 0.f, 1.f), g_matTexTransform).xy;
	output.vPos = mul(float4(tSkin.vPos, 1.f), g_matWVP);
	output.vProjPos = output.vPos;
	output.vPosV = mul(float4(tSkin.vPos, 1.f), g_matWV).xyz;
	output.vPosW = mul(float4(tSkin.vPos, 1.f), g_matWorld).xyz;
	output.vNormal = normalize(mul(float4(tSkin.vNormal, 0.f), g_matWV).xyz);
	output.vTangent = normalize(mul(float4(tSkin.vTangent, 0.f), g_matWV).xyz);
	output.vBinormal = normalize(mul(float4(tSkin.vBinormal, 0.f), g_matWV).xyz);
	output.vPosL = input.vPos;

	return output;
}

PS_OUTPUT_SINGLE NoDepthAlphaPS(VS_OUTPUT_3D input)
{
	PS_OUTPUT_SINGLE output = (PS_OUTPUT_SINGLE)0.f;

	LightResult tResult = ComputeLightEmvRClip(input.vPosV, input.vNormal, input.vTangent,
		input.vBinormal, input.vUV);

	output.vColor = g_vDif;
	if (g_iDifTex)
	{
		output.vColor = g_tTex.Sample(g_tAnisotropic, input.vUV);
	}

//	clip(output.vColor.a - 0.15f);

	output.vColor = output.vColor * (tResult.vDif + output.vColor * tResult.vAmb)
		+ tResult.vSpc + tResult.vEmi;

	return output;
}

PS_OUTPUT_TARGET OutLinePS(VS_OUTPUT_3D input)
{
	PS_OUTPUT_TARGET output = (PS_OUTPUT_TARGET)0.f;

	output.Albedo = float4(0.59f, 0.208f, 0.87f, 1.f);
	////output.GBuffer1.xyz = ComputeBumpNormal(input.vNormal, input.vTangent, input.vBinormal, input.vUV);
	output.GBuffer2.x = input.vProjPos.z / input.vProjPos.w;
	output.GBuffer2.w = input.vProjPos.w;
	output.GBuffer2.y = g_vSpe.w;
	output.GBuffer2.z = g_iDecal;
	output.GBuffer3.x = ConvertColor(float4(0.59f, 0.208f, 0.87f, 1.f));
	output.GBuffer3.y = ConvertColor(float4(0.59f, 0.208f, 0.87f, 1.f));
	output.GBuffer4.xyz = input.vTangent;
	output.GBuffer5.xyz = input.vBinormal;

	output.GBuffer3.z = ConvertColor(float4(0.59f, 0.208f, 0.87f, 1.f));
	output.GBuffer3.w = ConvertColor(float4(0.59f, 0.208f, 0.87f, 1.f));

	return output;
}

float4 OutLineAlphaPS(VS_OUTPUT_3D input) : SV_TARGET
{
	return float4(0.59f, 0.208f, 0.87f, 1.f);
}

struct VS_OUTPUT_TRAIL
{
	float4	vPos		:	SV_Position;
	float2	vUV	:	TEXCOORD;
	float4	vProjPos	:	Position;
	float	fAlpha : ALPHA;
};

SamplerState	g_sMirror	:	register(s6);

VS_OUTPUT_TRAIL TrailVS(float3 vPos	:	POSITION,
	float2 vUV	:	TEXCOORD, float fAlpha	:	ALPHA)
{
	VS_OUTPUT_TRAIL output = (VS_OUTPUT_TRAIL)0.f;

	output.vPos = mul(float4(vPos, 1.f), g_matWVP);
	output.vUV = vUV;
	output.vProjPos = output.vPos;
	output.fAlpha = fAlpha;

	return output;
}

float4 TrailPS(VS_OUTPUT_TRAIL input) : SV_TARGET
{
	float2 vUV = 0.f;

	vUV.x = input.vProjPos.x / input.vProjPos.w * 0.5f + 0.5f;
	vUV.y = input.vProjPos.y / input.vProjPos.w * -0.5f + 0.5f;

	float fDepth = g_DepthTexture.Sample(g_tPoint, vUV).w;

	if (fDepth < input.vProjPos.w)
	{
		if (fDepth != 0.f)
			clip(-1);
	}

	float fAlpha = input.fAlpha;//g_tTex.Sample(g_sMirror, input.vUV).r;// +float2(g_fAccTime / 1.1f - 0.1f, 0.f)).r;

	return float4(g_vEmi.xyz, fAlpha);
}

float4 TrailTexturePS(VS_OUTPUT_TRAIL input) : SV_TARGET
{
	float2 vUV = 0.f;

	vUV.x = input.vProjPos.x / input.vProjPos.w * 0.5f + 0.5f;
	vUV.y = input.vProjPos.y / input.vProjPos.w * -0.5f + 0.5f;

	float fDepth = g_DepthTexture.Sample(g_tPoint, vUV).w;

	if (fDepth < input.vProjPos.w)
	{
		if (fDepth != 0.f)
			clip(-1);
	}

	float4 vColor = g_tTex.Sample(g_tAnisotropic, input.vUV);// +float2(g_fAccTime / 1.1f - 0.1f, 0.f)).r;
	vColor.a = g_EmiTex.Sample(g_tAnisotropic, input.vUV).r;

	vColor *= g_vDif;
	
	return vColor;
}