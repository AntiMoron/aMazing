Texture2D txDiffuse : register(t0);
Texture2D glowPic : register(t1);
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
	float4 Nor : NORMAL;
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
    float baseAlpha = color.a;
    float4 glowColor = glowPic.Sample(samLinear,input.Tex);
    color = 0.4 * color + 0.6 * glowColor;
    if(glowColor.a == baseAlpha)
        color = color * 0.7f + glowColor * 0.3f;
    else
        color = glowColor;
    return color;
}
