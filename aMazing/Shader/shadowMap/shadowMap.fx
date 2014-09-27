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

float4 PSEntry(PS_INPUT input) : SV_Target
{
	float bias = 0.00000028f;
	float4 color = txDiffuse.Sample(samWrap, input.Tex);
	clip(color.a == 0.0f ? -1 : 1);

	float2 projectTexCoord;

	projectTexCoord.x = input.lightDepth.x / input.lightDepth.w / 2.0f + 0.5f;
	projectTexCoord.y = -input.lightDepth.y / input.lightDepth.w / 2.0f + 0.5f;

	float4 shadowMapColor = ProTexutre.Sample(samClamp, projectTexCoord);
	float cameradepth = saturate(shadowMapColor.z/shadowMapColor.w );
	float lightdepth = saturate(input.lightDepth.z / input.lightDepth.w);
	if ((saturate(projectTexCoord.x) == projectTexCoord.x)
		&& (saturate(projectTexCoord.y) == projectTexCoord.y))
	{
//		return float4(cameradepth, cameradepth, cameradepth,1.0f);
		lightdepth = lightdepth - bias;
		if (lightdepth > cameradepth)
			color = float4(0.1f,0.1f,0.1f,1.0f) * color;
	}
	return color;
}
