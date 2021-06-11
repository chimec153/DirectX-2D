#include "Share.fx"

struct Particle
{
	float4	vColor;
	float3	vPos;
	uint	iLive;
	float3	vSpeed;
	float	fTime;
	float3	vScale;
	float	fAngle;
	float	fAngleSpeed;
	float	fMaxTime;
	int		iNumber;
	int		iFrame;
};

struct ParticleShare
{
	int		iSpawnCount;
	float3	vMaxScale;
	int		iCurCount;
	float3	vMinScale;
	float4	vMaxColor;
	float4	vMinColor;
	int		iMaxFrame;
	float2	vImageSize;
	int		iCountX;
	int		iCountY;
};


struct VS_INPUT_PARTICLE
{
	float3 vPos	:	POSITION;
	float2 vUV	:	TEXCOORD;
	uint iInstID	:	SV_InstanceID;
};

struct VS_OUTPUT_PARTICLE
{
	uint iInstID	:	SV_InstanceID;
};

struct GS_OUTPUT_PARTICLE
{
	float4 vPos	:	SV_POSITION;
	float4 vProjPos	:	POSITION;
	float3 vPosV	:	POSITION1;
	float2	vUV	:	TEXCOORD;
	float4	vColor	:	COLOR;
	int		iNumber : NUMBER;
};

static const float4 gPos[4] =
{
	float4(-0.5f, 0.5f, 0.f, 0.f),
	float4(0.5f, 0.5f, 0.f, 0.f),
	float4(-0.5f, -0.5f, 0.f, 0.f),
	float4(0.5f, -0.5f, 0.f, 0.f)
};

static const float2 gUV[4] =
{
	float2(0.f, 0.f),
	float2(1.f, 0.f),
	float2(0.f, 1.f),
	float2(1.f, 1.f)
};


RWStructuredBuffer<Particle>		g_Particle	:	register(u0);
RWStructuredBuffer<ParticleShare>	g_ParticleShare	:	register(u1);


VS_OUTPUT_PARTICLE VS(VS_INPUT_PARTICLE input, uint iInstID	:	SV_InstanceID)
{
	VS_OUTPUT_PARTICLE output = (VS_OUTPUT_PARTICLE)0.f;

	output.iInstID = iInstID;

	return output;
}

StructuredBuffer<Particle> gParticle	:	register(t104);
StructuredBuffer<ParticleShare> gParticleShare	:	register(t105);

Texture2D g_DepthTexture	:	register(t12);

[maxvertexcount(6)]
void GS(in point VS_OUTPUT_PARTICLE Point[1],
	inout TriangleStream<GS_OUTPUT_PARTICLE> TriStream)
{
	int iInstID = Point[0].iInstID;

	if (gParticle[iInstID].iLive == 0)
	{
		return;
	}

	GS_OUTPUT_PARTICLE output[4] = {
		(GS_OUTPUT_PARTICLE)0.f,
		(GS_OUTPUT_PARTICLE)0.f,
		(GS_OUTPUT_PARTICLE)0.f,
		(GS_OUTPUT_PARTICLE)0.f
	};

	float4 vViewPos = mul(float4(gParticle[iInstID].vPos, 1.f), g_matView);
	float fRatio = gParticle[iInstID].fTime / gParticle[iInstID].fMaxTime;

	float4 vColor = lerp(gParticleShare[0].vMinColor, gParticleShare[0].vMaxColor, fRatio);
	float3 vScale = lerp(gParticleShare[0].vMinScale, gParticleShare[0].vMaxScale, fRatio);

	float2 vFrameSize =
		float2(gParticleShare[0].vImageSize.x / gParticleShare[0].iCountX,
			gParticleShare[0].vImageSize.y / gParticleShare[0].iCountY);

	int iY = gParticle[iInstID].iFrame / gParticleShare[0].iCountX;
	int iX = gParticle[iInstID].iFrame % gParticleShare[0].iCountX;

	float2 vStart = float2(vFrameSize.x * iX, vFrameSize.y* iY) / gParticleShare[0].vImageSize;
	float2 vEnd = float2(vFrameSize.x * (iX+1), vFrameSize.y* (iY+1)) / gParticleShare[0].vImageSize;

	float3x3 matRotationZ = float3x3(
		cos(gParticle[iInstID].fAngle), sin(gParticle[iInstID].fAngle), 0,
		-sin(gParticle[iInstID].fAngle), cos(gParticle[iInstID].fAngle), 0,
			0, 0, 1);

	const float2 vUV[4] =
	{
		float2(vStart.x, vStart.y),
		float2(vEnd.x, vStart.y),
		float2(vStart.x, vEnd.y),
		float2(vEnd.x, vEnd.y)
	};

	[unroll]
	for (int i = 0; i < 4; i++)
	{
		output[i].vPosV = mul(gPos[i].xyz * vScale, matRotationZ) + vViewPos.xyz;
		output[i].vPos = mul(float4(output[i].vPosV, 1.f), g_matProj);
		output[i].vProjPos = output[i].vPos;

		if (gParticleShare[0].iMaxFrame != 0)
		{

			output[i].vUV = vUV[i];
		}
		else
		{
			output[i].vUV = gUV[i];
		}
		output[i].vColor = vColor;
	}

	TriStream.Append(output[0]);
	TriStream.Append(output[1]);
	TriStream.Append(output[2]);
	TriStream.RestartStrip();

	TriStream.Append(output[1]);
	TriStream.Append(output[3]);
	TriStream.Append(output[2]);
	TriStream.RestartStrip();
}

PS_OUTPUT_SINGLE PS(GS_OUTPUT_PARTICLE input)
{
	PS_OUTPUT_SINGLE output = (PS_OUTPUT_SINGLE)0.f;

	output.vColor = g_tTex.Sample(g_tPoint, input.vUV) * input.vColor;

	clip(output.vColor.a - 0.15f);

	float fParticleDepth = input.vProjPos.w;

	input.vProjPos.xy /= fParticleDepth;

	input.vProjPos.xy = input.vProjPos.xy * float2(0.5f, -0.5f) + float2(0.5f, 0.5f);

	float fSceneDepth = g_DepthTexture.Sample(g_tPoint, input.vProjPos.xy).w;

	float fPower = 1.5f;

	float fAlpha = fSceneDepth - fParticleDepth;

	fAlpha = 1 - pow(2.f, -2.f * pow(2.f * saturate(fAlpha), fPower));

	if (fAlpha <= 0.f && fSceneDepth == 0.f)
	{
		fAlpha = 1.f;
	}

	output.vColor.a *= fAlpha;

	clip(output.vColor.a - 0.15f);

	return output;
}

PS_OUTPUT_SINGLE BlackClipPS(GS_OUTPUT_PARTICLE input)
{
	PS_OUTPUT_SINGLE output = (PS_OUTPUT_SINGLE)0.f;

	output.vColor = g_tTex.Sample(g_tPoint, input.vUV) * input.vColor;

	clip(output.vColor.a - 0.33f);

	clip(output.vColor.r + output.vColor.g + output.vColor.b - 0.1f);

	float fParticleDepth = input.vProjPos.w;

	input.vProjPos.xy /= fParticleDepth;

	input.vProjPos.xy = input.vProjPos.xy * float2(0.5f, -0.5f) + float2(0.5f, 0.5f);

	float fSceneDepth = g_DepthTexture.Sample(g_tPoint, input.vProjPos.xy).w;

	float fPower = 1.5f;

	float fAlpha = fSceneDepth - fParticleDepth;

	if (fAlpha < 0.f)
	{
		if (fSceneDepth == 0.f)
		{
			fAlpha = 1.f;
		}
		else
		{
			clip(-1);
		}
	}

	fAlpha = 1 - pow(2.f, -2.f * pow(2.f * saturate(fAlpha), fPower));


	output.vColor.a *= fAlpha;

	clip(output.vColor.a - 0.33f);

	return output;
}

PS_OUTPUT_SINGLE LightPS(GS_OUTPUT_PARTICLE input)
{
	PS_OUTPUT_SINGLE output = (PS_OUTPUT_SINGLE)0.f;

	output.vColor = g_tTex.Sample(g_tPoint, input.vUV) * input.vColor;

	clip(output.vColor.a - 0.15f);

	LightResult tResult = ComputeLight(input.vPosV, float3(0.f, 0.f, -1.f), 
		float3(1.f, 0.f, 0.f), float3(0.f, -1.f, 0.f), input.vUV);

	output.vColor = output.vColor * (tResult.vDif + output.vColor * tResult.vAmb)
		+ tResult.vSpc + tResult.vEmi;

	float fParticleDepth = input.vProjPos.z / input.vProjPos.w;

	input.vProjPos.xy /= input.vProjPos.w;

	input.vProjPos.xy = input.vProjPos.xy * float2(0.5f, -0.5f) + float2(0.5f, 0.5f);

	float fSceneDepth = g_DepthTexture.Sample(g_tPoint, input.vProjPos.xy).w;

	float fPower = 1.5f;

	float fAlpha = fSceneDepth - fParticleDepth;

	if (fSceneDepth != 0.f)
	{
		clip(fAlpha);
		fAlpha = 1 - pow(2.f, -2.f * pow(2.f * saturate(fAlpha), fPower));
	}
		
	else
	{
		fAlpha = 1.f;
	}

	output.vColor.a = fAlpha;

	return output;
}

PS_OUTPUT_SINGLE EmvAlphaPS(GS_OUTPUT_PARTICLE input)
{
	PS_OUTPUT_SINGLE output = (PS_OUTPUT_SINGLE)0.f;

	output.vColor = g_vDif;

	if (g_iDifTex)
	{
		output.vColor = g_tTex.Sample(g_tPoint, input.vUV) ;
	}

	output.vColor.a = g_EmiTex.Sample(g_tPoint, input.vUV).r;

	output.vColor *= input.vColor;

	float fParticleDepth = input.vProjPos.w;

	input.vProjPos.xy /= fParticleDepth;

	input.vProjPos.xy = input.vProjPos.xy * float2(0.5f, -0.5f) + float2(0.5f, 0.5f);

	float fSceneDepth = g_DepthTexture.Sample(g_tPoint, input.vProjPos.xy).w;

	float fPower = 1.5f;

	float fAlpha = fSceneDepth - fParticleDepth;

	if (fSceneDepth != 0.f)
	{
		clip(fAlpha);
		fAlpha = 1 - pow(2.f, -2.f * pow(2.f * saturate(fAlpha), fPower));
		output.vColor.a *= fAlpha;
	}

	output.vColor = PaperBurn(output.vColor, input.vUV);

	return output;
}

PS_OUTPUT_SINGLE BulletPS(GS_OUTPUT_PARTICLE input)
{
	PS_OUTPUT_SINGLE output = (PS_OUTPUT_SINGLE)0.f;

	output.vColor = g_tTex.Sample(g_tPoint, input.vUV) * input.vColor;

	clip(output.vColor.a - 0.15f);

	float fParticleDepth = input.vProjPos.w;

	input.vProjPos.xy /= fParticleDepth;

	input.vProjPos.xy = input.vProjPos.xy * float2(0.5f, -0.5f) + float2(0.5f, 0.5f);

	float fSceneDepth = g_DepthTexture.Sample(g_tPoint, input.vProjPos.xy).w;

	float fPower = 1.5f;

	float fAlpha = fSceneDepth - fParticleDepth;

	if (fSceneDepth == 0.f)
	{
		fAlpha = 1.f;
	}
	else
	{
		clip(fAlpha);
	}

	output.vColor.a = 1.f;

	return output;
}

[numthreads(64,1,1)]
void CS(uint3 vDispatchThreadID	:	SV_DispatchThreadID)
{
	if (vDispatchThreadID.x >= (uint)g_iMaxCount)
		return;

		g_ParticleShare[0].iSpawnCount = g_iSpawn;
		g_ParticleShare[0].vMaxScale = g_vMaxScale;
		g_ParticleShare[0].vMinScale = g_vMinScale;
		g_ParticleShare[0].vMaxColor = g_vMaxColor;
		g_ParticleShare[0].vMinColor = g_vMinColor;
		g_ParticleShare[0].iMaxFrame = g_iParticleMaxFrame;
		g_ParticleShare[0].vImageSize = g_vParticleImageSize;
		g_ParticleShare[0].iCountX = g_iParticleCountX;
		g_ParticleShare[0].iCountY = g_iParticleCountY;

	if (!g_Particle[vDispatchThreadID.x].iLive)
	{
		if (g_ParticleShare[0].iSpawnCount > 0)
		{
			int iPrev = g_ParticleShare[0].iSpawnCount;
			int iNew = g_ParticleShare[0].iSpawnCount - 1;
			int iExChange = 0;

			InterlockedExchange(g_ParticleShare[0].iSpawnCount, iNew, iExChange);

			if (iExChange != iPrev)
				return;

			iNew = g_ParticleShare[0].iCurCount + 1;
			InterlockedExchange(g_ParticleShare[0].iCurCount, iNew, iExChange);

			g_Particle[vDispatchThreadID.x].iLive = 1;

			float fKey = vDispatchThreadID.x * 10.f;// / g_iMaxCount;

			float3 vRange = float3(Rand(fKey), Rand(fKey * 2.f), Rand(fKey * 3.f));

			vRange -= 0.5f;

			g_Particle[vDispatchThreadID.x].fTime = 0.f;
			g_Particle[vDispatchThreadID.x].iFrame = 0;
			g_Particle[vDispatchThreadID.x].fMaxTime = g_fMinTime + (vRange.x  +0.5f)* (g_fMaxTime - g_fMinTime);
			g_Particle[vDispatchThreadID.x].vPos = g_vWorldPos + vRange * g_vRange;
			g_Particle[vDispatchThreadID.x].vSpeed = 0.f;
			g_Particle[vDispatchThreadID.x].fAngle = 0.f;
			g_Particle[vDispatchThreadID.x].fAngleSpeed = 0.f;
			if (g_ParticleShare[0].iMaxFrame < 0.f)
			{
				g_Particle[vDispatchThreadID.x].iFrame = (int)(-g_ParticleShare[0].iMaxFrame * Rand(g_fAccTime + g_Particle[vDispatchThreadID.x].vPos.x + (float)vDispatchThreadID.x));
			}

			if (g_bMove)
			{
				g_Particle[vDispatchThreadID.x].vSpeed = g_fMinSpeed * normalize(vRange);
				if (g_fAngleAccel > 0.f)
				{
					g_Particle[vDispatchThreadID.x].fAngle = 360.f * vRange.x;
					g_Particle[vDispatchThreadID.x].fAngleSpeed = g_fMinSpeed * vRange.y;
				}
			}
		}
	}

	else
	{
		g_Particle[vDispatchThreadID.x].fTime += g_fDeltaTime;
		g_Particle[vDispatchThreadID.x].fAngleSpeed += g_fDeltaTime * g_fAngleAccel * g_bMove;
		g_Particle[vDispatchThreadID.x].fAngle += g_fDeltaTime * g_Particle[vDispatchThreadID.x].fAngleSpeed * g_bMove;
		g_Particle[vDispatchThreadID.x].vSpeed += g_fDeltaTime * g_vAccel * g_bMove;
		g_Particle[vDispatchThreadID.x].vPos += g_fDeltaTime * g_Particle[vDispatchThreadID.x].vSpeed * g_bMove;

		if (g_ParticleShare[0].iMaxFrame > 0.f)
		{
			g_Particle[vDispatchThreadID.x].iFrame = (int)(g_Particle[vDispatchThreadID.x].fTime
				/ g_Particle[vDispatchThreadID.x].fMaxTime
				* g_ParticleShare[0].iMaxFrame);
		}

		if (g_Particle[vDispatchThreadID.x].fTime >= g_Particle[vDispatchThreadID.x].fMaxTime)
		{
			g_Particle[vDispatchThreadID.x].fAngleSpeed = 0;
			g_Particle[vDispatchThreadID.x].fAngle = 0;
			g_Particle[vDispatchThreadID.x].iFrame = 0;
			g_Particle[vDispatchThreadID.x].iLive = 0;
			g_Particle[vDispatchThreadID.x].fTime = 0.f;
			g_Particle[vDispatchThreadID.x].vSpeed = 0.f;
			g_Particle[vDispatchThreadID.x].vPos = float3(0.f,0.f, 0.f);
			int iExChange = 0;
			int iNew = g_ParticleShare[0].iCurCount - 1;
			InterlockedExchange(g_ParticleShare[0].iCurCount, iNew, iExChange);
		}
	}
}


[numthreads(16, 1, 1)]
void RockCS(uint3 vDispatchThreadID	:	SV_DispatchThreadID)
{
	if (vDispatchThreadID.x >= (uint)g_iMaxCount)
		return;

	g_ParticleShare[0].iSpawnCount = g_iSpawn;
	g_ParticleShare[0].vMaxScale = g_vMaxScale;
	g_ParticleShare[0].vMinScale = g_vMinScale;
	g_ParticleShare[0].vMaxColor = g_vMaxColor;
	g_ParticleShare[0].vMinColor = g_vMinColor;
	g_ParticleShare[0].iMaxFrame = g_iParticleMaxFrame;
	g_ParticleShare[0].vImageSize = g_vParticleImageSize;
	g_ParticleShare[0].iCountX = g_iParticleCountX;
	g_ParticleShare[0].iCountY = g_iParticleCountY;

	if (!g_Particle[vDispatchThreadID.x].iLive)
	{
		if (g_ParticleShare[0].iSpawnCount > 0)
		{
			int iPrev = g_ParticleShare[0].iSpawnCount;
			int iNew = g_ParticleShare[0].iSpawnCount - 1;
			int iExChange = 0;

			InterlockedExchange(g_ParticleShare[0].iSpawnCount, iNew, iExChange);

			if (iExChange != iPrev)
				return;

			iNew = g_ParticleShare[0].iCurCount + 1;
			InterlockedExchange(g_ParticleShare[0].iCurCount, iNew, iExChange);

			g_Particle[vDispatchThreadID.x].iLive = 1;

			float fKey = vDispatchThreadID.x * 10.f;// / g_iMaxCount;

			float3 vRange = float3(Rand(fKey), Rand(fKey * 2.f), Rand(fKey * 3.f));

			vRange -= 0.5f;

			g_Particle[vDispatchThreadID.x].fTime = 0.f;
			g_Particle[vDispatchThreadID.x].iFrame = 0;
			g_Particle[vDispatchThreadID.x].fMaxTime = g_fMinTime + (vRange.x + 0.5f) * (g_fMaxTime - g_fMinTime);
			g_Particle[vDispatchThreadID.x].vPos = g_vWorldPos + vRange * g_vRange;
			g_Particle[vDispatchThreadID.x].vSpeed = 0.f;
			g_Particle[vDispatchThreadID.x].fAngle = 0.f;
			g_Particle[vDispatchThreadID.x].fAngleSpeed = 0.f;
			if (g_ParticleShare[0].iMaxFrame < 0.f)
			{
				g_Particle[vDispatchThreadID.x].iFrame = (int)(-g_ParticleShare[0].iMaxFrame * Rand(g_fAccTime + g_Particle[vDispatchThreadID.x].vPos.x + (float)vDispatchThreadID.x));
			}

			if (g_bMove)
			{
				g_Particle[vDispatchThreadID.x].vSpeed = g_fMinSpeed * normalize(vRange);
				if (g_fAngleAccel > 0.f)
				{
					g_Particle[vDispatchThreadID.x].fAngle = 360.f * vRange.x;
					g_Particle[vDispatchThreadID.x].fAngleSpeed = g_fMinSpeed * vRange.y;
				}
			}
		}
	}

	else
	{
		g_Particle[vDispatchThreadID.x].fTime += g_fDeltaTime;
		g_Particle[vDispatchThreadID.x].fAngleSpeed += g_fDeltaTime * g_fAngleAccel * g_bMove;
		g_Particle[vDispatchThreadID.x].fAngle += g_fDeltaTime * g_Particle[vDispatchThreadID.x].fAngleSpeed * g_bMove;
		g_Particle[vDispatchThreadID.x].vSpeed += g_fDeltaTime * g_vAccel * g_bMove;
		g_Particle[vDispatchThreadID.x].vPos += g_fDeltaTime * g_Particle[vDispatchThreadID.x].vSpeed * g_bMove;

		if (g_Particle[vDispatchThreadID.x].vPos.y + g_ParticleShare[0].vMinScale.y / 2.f < 0.f)
		{
			g_Particle[vDispatchThreadID.x].vPos.y = g_ParticleShare[0].vMinScale.y / 2.f + 1.f;
			g_Particle[vDispatchThreadID.x].vSpeed.y *= -0.5f;
			g_Particle[vDispatchThreadID.x].fAngleSpeed *= -0.5f;
		}

		if (g_ParticleShare[0].iMaxFrame > 0.f)
		{
			g_Particle[vDispatchThreadID.x].iFrame = (int)(g_Particle[vDispatchThreadID.x].fTime
				/ g_Particle[vDispatchThreadID.x].fMaxTime
				* g_ParticleShare[0].iMaxFrame);
		}

		if (g_Particle[vDispatchThreadID.x].fTime >= g_Particle[vDispatchThreadID.x].fMaxTime)
		{
			g_Particle[vDispatchThreadID.x].fAngleSpeed = 0;
			g_Particle[vDispatchThreadID.x].fAngle = 0;
			g_Particle[vDispatchThreadID.x].iFrame = 0;
			g_Particle[vDispatchThreadID.x].iLive = 0;
			g_Particle[vDispatchThreadID.x].fTime = 0.f;
			g_Particle[vDispatchThreadID.x].vSpeed = 0.f;
			g_Particle[vDispatchThreadID.x].vPos = float3(0.f, 0.f, 0.f);
			int iExChange = 0;
			int iNew = g_ParticleShare[0].iCurCount - 1;
			InterlockedExchange(g_ParticleShare[0].iCurCount, iNew, iExChange);
		}
	}
}

static const float2 gNumberUV[4] =
{
	float2(0.283f, 0.098f),
	float2(0.317f, 0.098f),
	float2(0.283f, 0.188f),
	float2(0.317f, 0.188f)
};
static const float2 gCell[4] =
{
	float2(0.f, 0.f),
	float2(0.034f, 0.f),
	float2(0.f, 0.09f),
	float2(0.034f, 0.09f)
};

[maxvertexcount(6)]
void CharGS(in point VS_OUTPUT_PARTICLE Point[1],
	inout TriangleStream<GS_OUTPUT_PARTICLE> TriStream)
{
	int iInstID = Point[0].iInstID;

	if (gParticle[iInstID].iLive == 0)
	{
		return;
	}

	GS_OUTPUT_PARTICLE output[4] = {
		(GS_OUTPUT_PARTICLE)0.f,
		(GS_OUTPUT_PARTICLE)0.f,
		(GS_OUTPUT_PARTICLE)0.f,
		(GS_OUTPUT_PARTICLE)0.f
	};

	float4 vViewPos = mul(float4(gParticle[iInstID].vPos, 1.f), g_matView);
	float fRatio = saturate(gParticle[iInstID].fTime * gParticleShare[0].iMaxFrame / gParticle[iInstID].fMaxTime);

	float4 vColor = lerp(gParticleShare[0].vMinColor, gParticleShare[0].vMaxColor, fRatio);
	float3 vScale = lerp(gParticleShare[0].vMinScale, gParticleShare[0].vMaxScale, fRatio);

	[unroll]
	for (int i = 0; i < 4; i++)
	{
		output[i].vPosV = (gPos[i].xyz - float3(iInstID - gParticleShare[0].iSpawnCount /2.f, 0.f, 0.f)) *vScale + vViewPos.xyz;
		output[i].vPos = mul(float4(output[i].vPosV, 1.f), g_matProj);
		output[i].vProjPos = output[i].vPos;
		output[i].vUV = gNumberUV[0] + gCell[i];
		output[i].vUV.x += gParticle[iInstID].iFrame * 0.0375f;
		output[i].vColor = vColor;
	}

	TriStream.Append(output[0]);
	TriStream.Append(output[1]);
	TriStream.Append(output[2]);
	TriStream.RestartStrip();

	TriStream.Append(output[1]);
	TriStream.Append(output[3]);
	TriStream.Append(output[2]);
	TriStream.RestartStrip();
}

[numthreads(64, 1, 1)]
void CharCS(uint3 vDispatchThreadID	:	SV_DispatchThreadID)
{
	g_ParticleShare[0].vMaxScale = g_vMaxScale;
	g_ParticleShare[0].vMinScale = g_vMinScale;
	g_ParticleShare[0].vMaxColor = g_vMaxColor;
	g_ParticleShare[0].vMinColor = g_vMinColor;
	g_ParticleShare[0].iMaxFrame = g_iParticleMaxFrame;
	g_ParticleShare[0].iCountX = g_iParticleCountX;

	if (g_iSpawn > 0)
	{
		g_Particle[vDispatchThreadID.x].iLive = 1;
		g_Particle[vDispatchThreadID.x].fTime = 0.f;

		uint iId = vDispatchThreadID.x;
		int iFrame = 0;
		int iNumber = g_iParticleCountX;
		float fCounter = (float)g_iParticleCountX;

		int iDiv = 10;
		float fDiv = 1.f;

		unsigned int iCount = 0;

		while (true)
		{
			iCount++;
			fCounter *= 0.1f;

			if (fCounter < 1.f)
				break;

			if (vDispatchThreadID.x >= iCount)
			{
				iDiv *= 10;
				fDiv *= 0.1f;
			}
		}

		if (iCount <= vDispatchThreadID.x)
			return;

		g_ParticleShare[0].iSpawnCount = iCount;

		g_Particle[vDispatchThreadID.x].iFrame = (iNumber % iDiv - iNumber % ((int)(iDiv * 0.1f))) * fDiv;

		g_Particle[vDispatchThreadID.x].iFrame += 9;

		g_Particle[vDispatchThreadID.x].iFrame = g_Particle[vDispatchThreadID.x].iFrame % 10;

		g_Particle[vDispatchThreadID.x].vPos = g_vWorldPos;
		g_Particle[vDispatchThreadID.x].fMaxTime = g_fMaxTime;
		g_Particle[vDispatchThreadID.x].vSpeed = 0.f;
		g_Particle[vDispatchThreadID.x].fAngle = 0.f;
		g_Particle[vDispatchThreadID.x].fAngleSpeed = 0.f;

		if (g_bMove)
		{
			g_Particle[vDispatchThreadID.x].vSpeed = float3(0.f, g_fMaxSpeed, 0.f);
		}
	}

	if(g_Particle[vDispatchThreadID.x].iLive)
	{
		g_Particle[vDispatchThreadID.x].fTime += g_fDeltaTime;
		g_Particle[vDispatchThreadID.x].fAngleSpeed += g_fDeltaTime * g_fAngleAccel * g_bMove;
		g_Particle[vDispatchThreadID.x].fAngle += g_fDeltaTime * g_Particle[vDispatchThreadID.x].fAngleSpeed * g_bMove;
		g_Particle[vDispatchThreadID.x].vSpeed += g_fDeltaTime * g_vAccel * g_bMove;
		g_Particle[vDispatchThreadID.x].vPos += g_fDeltaTime * g_Particle[vDispatchThreadID.x].vSpeed * g_bMove;

		if (g_Particle[vDispatchThreadID.x].fTime >= g_Particle[vDispatchThreadID.x].fMaxTime)
		{
			g_Particle[vDispatchThreadID.x].fAngleSpeed = 0;
			g_Particle[vDispatchThreadID.x].fAngle = 0;
			g_Particle[vDispatchThreadID.x].iFrame = 0;
			g_Particle[vDispatchThreadID.x].iLive = 0;
			g_Particle[vDispatchThreadID.x].fTime = 0.f;
			g_Particle[vDispatchThreadID.x].vSpeed = 0.f;
			g_Particle[vDispatchThreadID.x].vPos = float3(0.f, 0.f, 0.f);
			int iExChange = 0;
			int iNew = g_ParticleShare[0].iCurCount - 1;
			InterlockedExchange(g_ParticleShare[0].iCurCount, iNew, iExChange);
		}
	}
}