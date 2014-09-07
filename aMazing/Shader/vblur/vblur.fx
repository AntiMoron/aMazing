Texture2D txDiffuse : register(t0);
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
	float2 blur1 :TEXCOORD1;
	float2 blur2 :TEXCOORD2;
	float2 blur3 :TEXCOORD3;
	float2 blur4 :TEXCOORD4;
	float2 blur5 :TEXCOORD5;
	float2 blur6 :TEXCOORD6;
	float2 blur7 :TEXCOORD7;
	float2 blur8 :TEXCOORD8;
	float2 blur9 :TEXCOORD9;
};

PS_INPUT VSEntry(VS_INPUT input)
{
	PS_INPUT output = (PS_INPUT)0;
	
	output.Pos = float4(input.Pos.xy, 0.0f, 1.0f);
	float texelSize = 1.0f / 480;
	output.Tex = input.Tex;
	output.blur1 = input.Tex + float2(0.0f,texelSize * -4.0f);
	output.blur2 = input.Tex + float2(0.0f,texelSize * -3.0f);
	output.blur3 = input.Tex + float2(0.0f,texelSize * -2.0f);
	output.blur4 = input.Tex + float2(0.0f,texelSize * -1.0f);
	output.blur5 = input.Tex + float2(0.0f,texelSize * 0.0f);
	output.blur6 = input.Tex + float2(0.0f,texelSize * 1.0f);
	output.blur7 = input.Tex + float2(0.0f,texelSize * 2.0f);
	output.blur8 = input.Tex + float2(0.0f,texelSize * 3.0f);
	output.blur9 = input.Tex + float2(0.0f,texelSize * 4.0f);
	return output;
}

float4 PSEntry(PS_INPUT input) : SV_Target
{
	float4 color = txDiffuse.Sample(samLinear, input.Tex);
	clip(color.a == 0.0f ? -1 : 1);

	float weight0,weight1,weight2,weight3,weight4;
	float normalization;
	weight0 = 1.0f;
	weight1 = 0.9f;
	weight2 = 0.56f;
	weight3 = 0.19f;
	weight4 = 0.1f;
	normalization = weight0 + (2.0f * (weight1 + weight2 + weight3 + weight4));
	weight0 = weight0 / normalization;
	weight1 = weight1 / normalization;
	weight2 = weight2 / normalization;
	weight3 = weight3 / normalization;
	weight4 = weight4 / normalization;
	
	color = float4(0.0f,0.0f,0.0f,0.0f);
	color += txDiffuse.Sample(samLinear, input.blur1) * weight4;
	color += txDiffuse.Sample(samLinear, input.blur2) * weight3;
	color += txDiffuse.Sample(samLinear, input.blur3) * weight2;
	color += txDiffuse.Sample(samLinear, input.blur4) * weight1;
	color += txDiffuse.Sample(samLinear, input.blur5) * weight0;
	color += txDiffuse.Sample(samLinear, input.blur6) * weight1;
	color += txDiffuse.Sample(samLinear, input.blur7) * weight2;
	color += txDiffuse.Sample(samLinear, input.blur8) * weight3;
	color += txDiffuse.Sample(samLinear, input.blur9) * weight4;
	color.a = 1.0f;
	return color;
}
