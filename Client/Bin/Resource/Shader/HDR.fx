cbuffer DownScaleConstants	:	register(b0)
{
	uint2 Res	:	packoffset(c0);
	uint Domain	:	packoffset(c0.z);
	uint GroupSize	:	packoffset(c0.w);
}

Texture2D HDRTex	:	register(t0);
RWStructuredBuffer<float> AverageLum	:	register(u0);

groupshared float SharedPositions[1024];

static const float4 LUM_FACTOR = float4(0.299, 0.587, 0.114, 0);

float DownScale4x4(uint2 CurPixel, uint groupThreadId)
{
	float avgLum = 0.0;

	if (CurPixel.y < Res.y)
	{
		int3 nFullResPos = int3(CurPixel * 4, 0);

		float4 downScaled = float4(0.0, 0.0, 0.0, 0.0);
		[unroll]
		for (int i = 0; i < 4; i++)
		{
			[unroll]
			for (int j = 0; j < 4; j++)
			{
				downScaled += HDRTex.Load(nFullResPos, int2(j, i));
			}
		}
		downScaled /= 16.0;

		avgLum = dot(downScaled, LUM_FACTOR);

		SharedPositions[groupThreadId] = avgLum;
	}

	GroupMemoryBarrierWithGroupSync();

	return avgLum;
}

float DownScale1024to4(uint dispatchThreadId, uint groupThreadId, float avgLum)
{
	[unroll]
	for (uint groupSize = 4, step1 = 1, step2 = 2, step3 = 3;
		groupSize < 1024;
		groupSize *= 4, step1 *= 4, step2 *= 4, step3 *= 4)
	{
		if (groupThreadId % groupSize == 0)
		{
			float stepAvgLum = avgLum;
			stepAvgLum += dispatchThreadId + step1 < Domain ?
				SharedPositions[groupThreadId + step1] : avgLum;
			stepAvgLum += dispatchThreadId + step2 < Domain ?
				SharedPositions[groupThreadId + step2] : avgLum;
			stepAvgLum += dispatchThreadId + step3 < Domain ?
				SharedPositions[groupThreadId + step3] : avgLum;

			avgLum = stepAvgLum;
			SharedPositions[groupThreadId] = stepAvgLum;
			
		}

		GroupMemoryBarrierWithGroupSync();
	}

	return avgLum;
}

void DownScale4to1(uint dispatchThreadId, uint groupThreadId, uint groupId, float avgLum)
{
	if (groupThreadId == 0)
	{
		float fFinalAvgLum = avgLum;

		fFinalAvgLum += dispatchThreadId + 256 < Domain ?
			SharedPositions[groupThreadId + 256] : avgLum;
		fFinalAvgLum += dispatchThreadId + 512 < Domain ?
			SharedPositions[groupThreadId + 512] : avgLum;
		fFinalAvgLum += dispatchThreadId + 768 < Domain ?
			SharedPositions[groupThreadId + 768] : avgLum;
		fFinalAvgLum /= 1024.0;

		AverageLum[groupId] = fFinalAvgLum;
	}
}

[numthreads(1024,1,1)]
void DownScaleFirstPass(uint3 groupId : SV_GroupID,
	uint3 dispatchThreadId : SV_DispatchThreadID,
	uint3 groupThreadId : SV_GroupThreadID)
{
	uint2 CurPixel = uint2(dispatchThreadId.x % Res.x,
		dispatchThreadId.x / Res.x);

	float avgLum = DownScale4x4(CurPixel, groupThreadId.x);

	avgLum = DownScale1024to4(dispatchThreadId.x, groupThreadId.x, avgLum);

	DownScale4to1(dispatchThreadId.x, groupThreadId.x, groupId.x,
		avgLum);
}
#define MAX_GROUPS	64

groupshared float SharedAvgFinal[MAX_GROUPS];

[numthreads(MAX_GROUPS,1,1)]
void DownScaleSecondPass(uint3 groupId : SV_GroupID,
	uint3 groupThreadId : SV_GRoupThreadID,
	uint3 dispatchThreadId : SV_DispatchThreadID)
{
	float avgLum = 0.0;

	if (dispatchThreadId.x < GroupSize)
	{
		//avgLum = AverageValues1D[dispatchThreadId.x];
	}
	SharedAvgFinal[dispatchThreadId.x] = avgLum;

	GroupMemoryBarrierWithGroupSync();

	if (dispatchThreadId.x % 4 == 0)
	{
		float stepAvgLum = avgLum;
		stepAvgLum += dispatchThreadId.x + 1 < GroupSize ?
			SharedAvgFinal[dispatchThreadId.x + 1] : avgLum;
		stepAvgLum += dispatchThreadId.x + 2 < GroupSize ?
			SharedAvgFinal[dispatchThreadId.x + 2] : avgLum;
		stepAvgLum += dispatchThreadId.x + 3 < GroupSize ?
			SharedAvgFinal[dispatchThreadId.x + 3] : avgLum;

		avgLum = stepAvgLum;

		SharedAvgFinal[dispatchThreadId.x] = stepAvgLum;
	}

	GroupMemoryBarrierWithGroupSync();

	if (dispatchThreadId.x % 16 == 0)
	{
		float stepAvgLum = avgLum;
		stepAvgLum += dispatchThreadId.x + 4 < GroupSize ?
			SharedAvgFinal[dispatchThreadId.x + 4] : avgLum;
		stepAvgLum += dispatchThreadId.x + 8 < GroupSize ?
			SharedAvgFinal[dispatchThreadId.x + 8] : avgLum;
		stepAvgLum += dispatchThreadId.x + 12 < GroupSize ?
			SharedAvgFinal[dispatchThreadId.x + 12] : avgLum;

		avgLum = stepAvgLum;
		SharedAvgFinal[dispatchThreadId.x] = stepAvgLum;
	}

	GroupMemoryBarrierWithGroupSync();

	if (dispatchThreadId.x == 0)
	{
		float fFinalLumValue = avgLum;
		fFinalLumValue += dispatchThreadId.x + 16 < GroupSize ?
			SharedAvgFinal[dispatchThreadId.x + 16] : avgLum;
		fFinalLumValue += dispatchThreadId.x + 32 < GroupSize ?
			SharedAvgFinal[dispatchThreadId.x + 32] : avgLum;
		fFinalLumValue += dispatchThreadId.x + 48 < GroupSize ?
			SharedAvgFinal[dispatchThreadId.x + 48] : avgLum;
		fFinalLumValue /= 64.0;

		AverageLum[0] = fFinalLumValue;
	}
}

static const float2 arrBasePos[4] = {
	float2(-1.f,1.f),
	float2(1.f,1.f),
	float2(-1.f,-1.f),
	float2(1.f,-1.f),
};

static const float2 arrUV[4] = {
	float2(0.f, 0.f),
	float2(1.f,0.f),
	float2(0.f,1.f),
	float2(1.f,1.f),
};

struct VS_OUTPUT
{
	float4 Position : SV_Position;
	float2 UV	:	TEXCOORD;
};

VS_OUTPUT FullScreenQuadVS(uint VertexID:	SV_VertexID)
{
	VS_OUTPUT Output;

	Output.Position = float4(arrBasePos[VertexID].xy, 0.0, 1.0);
	Output.UV = arrUV[VertexID].xy;
	return Output;
}

Texture2D<float4> HDRTexture	:	register(t0);
StructuredBuffer<float> AvgLum	:	register(t1);

SamplerState PointSampler	:	register(s0);

cbuffer FinalPassConstants	:	register(b0)
{
	float MiddleGrey : packoffset(c0);
	float LumWhiteSqr : packoffset(c0.y);
}

float3 ToneMapping(float3 HDRColor)
{
	float LScale = dot(HDRColor, LUM_FACTOR);
	LScale *= MiddleGrey / AvgLum[0];
	LScale = (LScale + LScale * LScale / LumWhiteSqr) / (1.f + LScale);

	return HDRColor * LScale;
}

float4 FinalPassPS(VS_OUTPUT In) : SV_TARGET
{
	float3 color = HDRTex.Sample(PointSampler, In.UV.xy).xyz;
	color = ToneMapping(color);

	return float4(color, 1.f);
}