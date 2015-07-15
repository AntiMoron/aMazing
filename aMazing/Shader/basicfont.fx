#include"LightImpl.hlsl"
#include"MaterialImpl.hlsl"

cbuffer classInstance : register(b12)
{
	float4 fontColor;
}

struct VS_INPUT
{
    float4 Pos : POSITION;
    float4 Nor : NORMAL;
    float2 Tex : TEXCOORD0;
};

struct PS_INPUT
{
    float4 Pos : SV_POSITION;
    float4 Nor : NORMAL;
    float2 Tex : TEXCOORD0;
};

PS_INPUT VSEntry( VS_INPUT input )
{
    PS_INPUT output = (PS_INPUT)0;

    output.Pos = float4(input.Pos.xy,0.0f,1.0f);
    output.Nor = normalize(input.Nor);
    output.Tex = input.Tex;
    return output;
}

float4 PSEntry(PS_INPUT input) : SV_Target
{
	float4 color = txDiffuse.Sample(samplerLinear, input.Tex);
	color *= fontColor;
	clip(color.a == 0.0f ? -1 : 1);
	return color;
}
