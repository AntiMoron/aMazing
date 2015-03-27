#include"LightImpl.hlsl"
#include"MaterialImpl.hlsl"

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

cbuffer lightBuffer : register(b4)
{
	cAmbientLight ambientLighting;
	cDirectionalLight directLighting;
	cEnvironmentLight environmentLighting;
	cSmoothTexturedMaterial material;
}


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
	// Compute the Ambient term
	float3 Ambient = (float3)0.0f;
	Ambient = material.getAmbientColor(input.Tex) * ambientLighting.illuminateAmbient(input.Nor);

	// Accumulate the Diffuse contribution  
	float3 Diffuse = (float3)0.0f;
	Diffuse += material.getDiffuseColor(input.Tex) * directLighting.illuminateDiffuse(input.Nor);
	// Compute the Specular contribution
	float3   Specular = (float3)0.0f;
	Specular += directLighting.illuminateSpecular(input.Nor, material.getSpecularFactor());
	Specular += environmentLighting.illuminateSpecular(input.Nor, material.getSpecularFactor());

	// Accumulate the lighting with saturation
	float3 Lighting = saturate(Ambient + Diffuse + Specular);
	return float4(Lighting, 1.0f);
}
