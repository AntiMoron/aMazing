#define SAMPLES 16
Texture2DMS< float4, SAMPLES > txDiffuse : register(t0);
SamplerState samLinear : register(s0);

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
	float4 color = 0;
  	for( uint sample = 0; sample < SAMPLES; ++sample )
  	{
		color += txDiffuse.Load(input.Tex, sample);
 	}
	color /= float( SAMPLES);
	return color;
}