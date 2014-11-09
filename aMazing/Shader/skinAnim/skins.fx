Texture2D txDiffuse : register( t0 );
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

cbuffer light : register(b2)
{
    float4 lightDirection;
	float4 lightPosition;
	matrix viewMatrix;
    matrix projectionMatrix;
}

cbuffer skin : register(b3)
{
	matrix boneMatrix[550];
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

    output.Pos = input.Pos;
    output.Pos = mul( output.Pos, Rot);
    output.Pos = mul( output.Pos, Sca);
    output.Pos = mul( output.Pos, Pos);
    output.Pos = mul( output.Pos, World );
    output.Pos = mul( output.Pos, View );
    output.Pos = mul( output.Pos, Projection );
    output.Nor = normalize(input.Nor);
    output.Tex = input.Tex;
    return output;
}

float4 PSEntry(PS_INPUT input) : SV_Target
{
    float4 color = float4(0.3f, 0.5f, 0.6f, 1.0f);
    float4 diffuseColor = txDiffuse.Sample(samLinear, input.Tex);
    clip(diffuseColor.a == 0.0f ? -1 : 1);
	float3 lightDir = -lightDirection.xyz;
	lightDir = normalize(lightDir);
    float lightIntensity = dot(input.Nor.xyz, lightDir.xyz);
    if(lightIntensity > 0.0f)
        color += saturate(float4(1.0f,1.0f,1.0f,1.0f) *  lightIntensity);
	color.a = 1.0f;
	if (lightIntensity > 0.6f)
		color = diffuseColor;
	else
		color *= diffuseColor;
    return color;
}
