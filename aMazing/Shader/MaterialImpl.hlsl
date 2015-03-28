#ifndef MATERIALIMPL_HLSL
#define MATERIALIMPL_HLSL
#include"MaterialPsh.hlsl"

float3 cSmoothTexturedMaterial::getAmbientColor(float2 vTexcoord)
{
	float4 vDiffuse = (float4)1.0f;
	vDiffuse = txDiffuse.Sample(samplerLinear,vTexcoord); 
	return vDiffuse.xyz;
}

float3 cSmoothTexturedMaterial::getDiffuseColor(float2 vTexcoord)
{
	float4 vDiffuse = (float4)1.0f;
	vDiffuse = txDiffuse.Sample(samplerLinear, vTexcoord);
	return vColor * vDiffuse.xyz;
}

float3 cRoughTexturedMaterial::getAmbientColor(float2 vTexcoord)
{
	float4 vDiffuse = (float4)1.0f;
	vDiffuse = txDiffuse.Sample(samplerLinear, vTexcoord);
	return vColor * vDiffuse.xyz;
}

float3 cRoughTexturedMaterial::getDiffuseColor(float2 vTexcoord)
{
	float4 vDiffuse = (float4)1.0f;
	vDiffuse = txDiffuse.Sample(samplerLinear, vTexcoord);
	clip(vDiffuse.w == 0.0f ? -1 : 1);
	return vColor * vDiffuse.xyz;
}

#endif