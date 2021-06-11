cbuffer cbSettings
{
};

static const float gWeights[11] =
{
	0.05f, 0.05f, 0.1f, 0.1f, 0.1f, 0.2f, 0.1f,0.1f,0.1f,0.05f,0.05f
	//-0.08f,-0.05f,-0.03f,-0.02f,-0.01f,0.01f,0.02f,0.03f,0.05f,0.08f, 0.08f
};

cbuffer cbFixed
{
	static const int gBlurRadius = 5;
};

Texture2D	gInput	:	register(t103);
RWTexture2D<float4> gOutput	:	register(u0);

#define N	256
#define	CacheSize	(N+2*gBlurRadius)
groupshared float4 gCache[CacheSize];

[numthreads(N,1,1)]
void HorzBlurCS(int3 groupThreadID	:	SV_GroupThreadID,
	int3 dispatchThreadID : SV_DispatchThreadID)
{
	if (groupThreadID.x < gBlurRadius)
	{
		int x = max(dispatchThreadID.x - gBlurRadius, 0);
		gCache[groupThreadID.x] = gInput[int2(x,
			dispatchThreadID.y)];
	}
	if (groupThreadID.x >= N - gBlurRadius)
	{
		int x = min(dispatchThreadID.x + gBlurRadius,
			gInput.Length.x - 1);
		gCache[groupThreadID.x + 2 * gBlurRadius] =
			gInput[int2(x, dispatchThreadID.y)];
	}

	gCache[groupThreadID.x + gBlurRadius] =
		gInput[min(dispatchThreadID.xy, gInput.Length.xy - 1)];

	GroupMemoryBarrierWithGroupSync();

	float4 blurColor = float4(0, 0, 0, 0);

	[unroll]
	for (int i = -gBlurRadius; i <= gBlurRadius; ++i)
	{
		int k = groupThreadID.x + gBlurRadius + i;

		blurColor += gWeights[i + gBlurRadius] * gCache[k];
	}

	gOutput[dispatchThreadID.xy] = blurColor;
}

[numthreads(1,N,1)]
void VertBlurCS(int3 groupThreadID	:	SV_GroupThreadID,
	int3 dispatchThreadID : SV_DispatchThreadID)
{
	if (groupThreadID.y < gBlurRadius)
	{
		int y = max(dispatchThreadID.y - gBlurRadius, 0);
		gCache[groupThreadID.y] = gInput[int2(dispatchThreadID.x, y)];
	}
	if (groupThreadID.y >= N - gBlurRadius)
	{
		int y = min(dispatchThreadID.y + gBlurRadius, gInput.Length.y - 1);
		gCache[groupThreadID.y + 2 * gBlurRadius] =
			gInput[int2(dispatchThreadID.x, y)];
	}
	gCache[groupThreadID.y + gBlurRadius] =
		gInput[min(dispatchThreadID.xy, gInput.Length.xy - 1)];

	GroupMemoryBarrierWithGroupSync();

	float4 blurColor = float4(0, 0, 0, 0);

	[unroll]
	for (int i = -gBlurRadius; i <=gBlurRadius; ++i)
	{
		int k = groupThreadID.y + gBlurRadius + i;

		blurColor += gWeights[i + gBlurRadius] * gCache[k];
	}

	gOutput[dispatchThreadID.xy] = blurColor;
}

[numthreads(1, N, 1)]
void GrayCS(int3 ID	:	SV_DispatchThreadID)
{
	float3 vKernel = float3(0.3f, 0.59f, 0.11f);
	float3 vGray = dot(gInput[ID.xy].xyz, vKernel);
	gOutput[ID.xy] = float4(vGray, gInput[ID.xy].w);
}

[numthreads(1, N, 1)]
void SepiaCS(int3 ID	:	SV_DispatchThreadID)
{
	float r = dot(gInput[ID.xy].xyz, float3(0.393f, 0.769f, 0.189f));
	float g = dot(gInput[ID.xy].xyz, float3(0.349f, 0.686f, 0.168f));
	float b = dot(gInput[ID.xy].xyz, float3(0.272f, 0.534f, 0.131f));
	gOutput[ID.xy] = float4(r,g,b, gInput[ID.xy].w);
}

