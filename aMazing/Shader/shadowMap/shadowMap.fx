Texture2D txDiffuse : register(t0);
Texture2D ProTexutre : register(t1);
SamplerState samLinear : register( s0 );

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

cbuffer PRS : register(b2)
{
    matrix LightView;
    matrix LightProjection;
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
	float4 viewPosition : TEXCOORED1;
};

PS_INPUT VSEntry( VS_INPUT input )
{
    PS_INPUT output = (PS_INPUT)0;

    output.Pos = input.Pos;
    output.Pos = mul( output.Pos, Rot);
    output.Pos = mul( output.Pos, Sca);
    output.Pos = mul( output.Pos, Pos);
    output.Pos = mul( output.Pos, World );

    output.viewPosition = output.Pos;
    output.viewPosition = mul(output.viewPosition, LightView);
    output.viewPosition = mul(output.viewPosition, LightProjection);

	output.Pos = mul(output.Pos, View);
	output.Pos = mul(output.Pos, Projection);

    output.Tex = input.Tex;    
    return output;
}

float4 PSEntry(PS_INPUT input) : SV_Target
{
	float bias = 0.00000055f;
	float4 color = txDiffuse.Sample(samLinear, input.Tex);
	clip(color.a == 0.0f ? -1 : 1);

	float2 projectTexCoord;

	projectTexCoord.x =  input.viewPosition.x / input.viewPosition.w / 2.0f + 0.5f;
    projectTexCoord.y = -input.viewPosition.y / input.viewPosition.w / 2.0f + 0.5f;
	if((saturate(projectTexCoord.x) == projectTexCoord.x) 
		&& (saturate(projectTexCoord.y) == projectTexCoord.y))
	{
		float4 depthColor = ProTexutre.Sample(samLinear, projectTexCoord);
		float lightdepth = input.viewPosition.z / input.viewPosition.w;
		lightdepth = lightdepth - bias;
		float cameradepth = input.Pos.z / input.Pos.w;
		if (lightdepth < cameradepth)
			color = depthColor;
	}
	return color;
}
