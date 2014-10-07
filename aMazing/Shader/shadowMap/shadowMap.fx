Texture2D txDiffuse : register(t0);
Texture2D ProTexutre : register(t1);

SamplerState samWrap : register(s0);
SamplerState samClamp : register(s1);

cbuffer cameraMatrices : register( b0 )
{
    matrix World;
    matrix View;
    matrix Projection;
};

cbuffer PRS : register(b1)
{
    matrix Pos;
    matrix Rot;
    matrix Sca;
}

cbuffer LightMatrices : register(b2)
{
    matrix LightView;
    matrix LightProjection;
	float shadowBias;
}


struct VS_INPUT
{
    float4 Pos : POSITION;
    float2 Tex : TEXCOORD0;
};

struct PS_INPUT
{
	float4 Pos : SV_POSITION;
	float2 Tex : TEXCOORD0;
	float4 cameraDepth : CAMERA;
	float4 lightDepth : LIGHT;
};

PS_INPUT VSEntry( VS_INPUT input )
{
    PS_INPUT output = (PS_INPUT)0;

	output.Pos = input.Pos;
    output.Pos = mul( output.Pos, Rot);
    output.Pos = mul( output.Pos, Sca);
	output.Pos = mul(output.Pos, Pos);

	output.lightDepth = output.Pos;
    
	output.lightDepth = mul(output.lightDepth, LightView);
	output.lightDepth = mul(output.lightDepth, LightProjection);

	output.Pos = mul(output.Pos, View);
	output.Pos = mul(output.Pos, Projection);
    output.Tex = input.Tex;  

	output.cameraDepth = output.Pos;
	return output;
}

static const float SAMP_SIZE = 4096.0f;
static const float SAMP_DX = 1.0f / SAMP_SIZE;

float4 PSEntry(PS_INPUT input) : SV_Target
{
	float bias = 0.000003;
	float4 color = txDiffuse.Sample(samWrap, input.Tex);
	clip(color.a == 0.0f ? -1 : 1);

	float2 projectTexCoord;

	projectTexCoord.x = input.lightDepth.x / input.lightDepth.w / 2.0f + 0.5f;
	projectTexCoord.y = -input.lightDepth.y / input.lightDepth.w / 2.0f + 0.5f;

	if ((saturate(projectTexCoord.x) == projectTexCoord.x)
		&& (saturate(projectTexCoord.y) == projectTexCoord.y))
	{
		float lightdepth = saturate(input.lightDepth.z / input.lightDepth.w) - bias;
//		float dx = ddx(lightdepth);
//
		float shadowMapColor0 = ProTexutre.Sample(samClamp, projectTexCoord).r;
		float shadowMapColor1 = ProTexutre.Sample(samClamp, projectTexCoord + float2(SAMP_DX, 0)).r;
		float shadowMapColor2 = ProTexutre.Sample(samClamp, projectTexCoord + float2(0, SAMP_DX)).r;
		float shadowMapColor3 = ProTexutre.Sample(samClamp, projectTexCoord + float2(SAMP_DX, SAMP_DX)).r;

		float result0 = lightdepth <= shadowMapColor0;
		float result1 = lightdepth <= shadowMapColor1;
		float result2 = lightdepth <= shadowMapColor2;
		float result3 = lightdepth <= shadowMapColor3;

		float2 texelPos = frac(SAMP_SIZE * projectTexCoord.xy);
		float cameradepth = lerp(
			lerp(result0, result1, texelPos.x),
			lerp(result2, result3, texelPos.x),
			texelPos.y);
		if (lightdepth > cameradepth)
		{
			color = color *float4(0.44f,0.57f,0.74f,1.0f) * cameradepth;
			color.a = 1.0f;
		}
	}
	return color;
}
