#include"MaterialPsh.hlsl"
Texture2D txDiffuse : register(t0);
SamplerState samLinear : register(s0);

float3 cSmoothMaterial::getAmbientColor(float2 vTexcoord)
{
	float4 vDiffuse = (float4)1.0f;
	vDiffuse = txDiffse.Sample(samLinear,vTexcoord); 
	return m_vColor * vDiffuse;
}

float3 