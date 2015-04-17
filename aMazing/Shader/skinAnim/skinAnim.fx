#include"../LightImpl.hlsl"
#include"../MaterialImpl.hlsl"

struct VS_INPUT
{
	uint boneIndices : BONEINDICES;
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

cbuffer classInstance : register(b11)
{
	cAmbientLight ambientLighting;
	cDirectionalLight directLighting;
	cEnvironmentLight environmentLighting;
	cSmoothTexturedMaterial material;
	/*iBasicLight ambientLighting;
	iBasicLight directLighting;
	iBasicLight environmentLighting;
	iBasicMaterial material;*/
}

PS_INPUT VSEntry(VS_INPUT input)
{
	PS_INPUT output = (PS_INPUT)0;
	uint4 boneIndices = uint4(input.boneIndices & 0x000000ff,
		(input.boneIndices & 0x0000ff00) >> 8, 
		(input.boneIndices & 0x00ff0000) >> 16,
		(input.boneIndices & 0xff000000) >> 24);
//	uint4 boneIndices = input.boneIndices;
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
	input.Nor.w = 0.0f;
	output.Nor = mul(input.Nor, boneTransform);
	output.Nor = normalize(output.Nor);
	output.Tex = input.Tex.xy;
	return output;
}

float4 PSEntry(PS_INPUT input) : SV_Target
{
	// Compute the Ambient term
	float3 Ambient = (float3)0.0f;
	Ambient = material.getAmbientColor(input.Tex) * ambientLighting.illuminateAmbient(input.Nor);

	// Accumulate the Diffuse contribution  
	float3 Diffuse = (float3)0.0f;
		Diffuse += material.getDiffuseColor(input.Tex) * directLighting.illuminateDiffuse(input.Nor);
	// Compute the Specular contribution
	float3   Specular = (float3)0.0f;
		Specular += directLighting.illuminateSpecular(input.Nor, 2);
	Specular += environmentLighting.illuminateSpecular(input.Nor, material.getSpecularFactor());

	// Accumulate the lighting with saturation
	float3 Lighting = saturate(Ambient + Diffuse);
		return float4(Lighting, 1.0f);
}
