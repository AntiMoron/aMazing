Texture2D txDiffuse : register(t0);
Texture2D blurPic : register(t1);
Texture2D depthPic : register(t2);
SamplerState samLinear : register(s0);

cbuffer cameraMatrices : register(b0)
{
	matrix World;
	matrix View;
	matrix Projection;
};

struct VS_INPUT
{
	float4 Pos : POSITION;
	float2 Tex : TEXCOORD0;
};

struct PS_INPUT
{
	float4 Pos : SV_POSITION;
	float2 Tex : TEXCOORD0;
};

PS_INPUT VSEntry(VS_INPUT input)
{
	PS_INPUT output = (PS_INPUT)0;

	output.Pos = float4(input.Pos.xy, 0.0f, 1.0f);
	output.Tex = input.Tex;
	return output;
}

float4 PSEntry(PS_INPUT input) : SV_Target
{
    float4 color = txDiffuse.Sample(samLinear, input.Tex);
    float4 blurColor = blurPic.Sample(samLinear,input.Tex);
    float depth = depthPic.Sample(samLinear,input.Tex).r;
	float depthWeight = depthPic.Sample(samLinear, input.Tex).w;
	if (depth < 0.9993)
        return color;
    return blurColor;
}