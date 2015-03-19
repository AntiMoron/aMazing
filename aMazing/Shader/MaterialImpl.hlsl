#include"MaterialPsh.hlsl"

float3 cSommothTexturedMaterial::getAmbientColor(float2 vTexcoord)
{
	float4 vDiffuse = (float4)1.0f;
	vDiffuse = txDiffse.Sample(samLinear,vTexcoord); 
	return m_vColor * vDiffuse.xyz;
}

float3 cSommothTexturedMaterial::getDiffuseColor(float2 vTexcoord)
{
	float4 vDiffuse = (float4)1.0f;
	vDiffuse = txDiffuse.Sample(samLinear,vTexcoord);
	return vColor * vDiffuse.xyz;
}

float3 cRoughTextureMaterial:getAmbientColor(float2 vTexcoord)
{
	float vDiffuse = (float4)1.0f;
	vDiffuse = txDiffuse.Sample(samLineaer, vTexcoord);
	return vColor * vDiffuse.xyz;
}

float3 cRoughTextureMaterial::getDiffuseColor(float2 vTexcoord)
{
	float vDiffuse = (float4)1.0f;
	vDiffuse = txDiffuse.Sample(samLineaer, vTexcoord);
	return vColor * vDiffuse.xyz;
}