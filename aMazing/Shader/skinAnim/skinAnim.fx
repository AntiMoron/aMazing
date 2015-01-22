Texture2D txDiffuse : register(t0);
SamplerState samLinear : register(s0);

cbuffer cameraMatrices : register(b0)
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

cbuffer boneData : register(b3) 
{
	matrix bones[128];
}

struct VS_INPUT
{
	uint4 boneIndices : BONEINDICES;
	float4 Weights : WEIGHTS;
	float4 Pos : POSITION;
	float4 Nor : NORMAL;
	float4 Tex : TEXCOORD0;
};

struct PS_INPUT
{
	float4 Pos : SV_POSITION;
	float4 Nor : NORMAL;
	float2 Tex : TEXCOORD0;
};

PS_INPUT VSEntry(VS_INPUT input)
{
	PS_INPUT output = (PS_INPUT)0;
	//uint4 boneIndices = uint4(input.boneIndices & 0xff000000,
	//	input.boneIndices & 0xff0000, 
	//	input.boneIndices & 0xff00,
	//	input.boneIndices & 0xff);
	uint4 boneIndices = input.boneIndices;
	matrix boneTransform = (input.Weights.r * bones[boneIndices[0]])
		+ (input.Weights.g * bones[boneIndices[1]])
		+ (input.Weights.b * bones[boneIndices[2]])
		+ (input.Weights.a * bones[boneIndices[3]]);
	output.Pos = input.Pos;
	output.Pos = mul(output.Pos, boneTransform);
	output.Pos = mul(output.Pos, Rot);
	output.Pos = mul(output.Pos, Sca);
	output.Pos = mul(output.Pos, Pos);
	output.Pos = mul(output.Pos, World);
	output.Pos = mul(output.Pos, View);
	output.Pos = mul(output.Pos, Projection);
	output.Nor = input.Nor;
//	output.Nor = normalize(mul(input.Nor, boneTransform));
	output.Tex = input.Tex.xy;
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
	if (lightIntensity > 0.0f)
		color += saturate(float4(1.0f, 1.0f, 1.0f, 1.0f) *  lightIntensity);
	color.a = 1.0f;
	if (lightIntensity > 0.6f)
		color = diffuseColor;
	else
		color *= diffuseColor;
	return color;
}
