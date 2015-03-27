#ifndef MATERIALPSH_HLSL
#define MATERIALPSH_HLSL

#include"Buffers.hlsl"
interface iBasicMaterial
{
	//get the ambient color of material
	float3 getAmbientColor(float2 vTexcoord);
	//get the diffuse color of material
	float3 getDiffuseColor(float2 vTexcoord);
	//get the specular factor
	int getSpecularFactor();
};

class cBaseMaterial : iBasicMaterial
{
	float3 vColor;
	int iSpecFactor;
	float3 getAmbientColor(float2 vTexcoord)
	{
		return vColor;
	}
	float3 getDiffuseColor(float2 vTexcoord)
	{
		return (float3)vColor;
	}
	int getSpecularFactor()
	{
		return iSpecFactor;
	}
};

class cSmoothMaterial : cBaseMaterial{};

class cSmoothTexturedMaterial : cSmoothMaterial
{
	float3 getAmbientColor(float2 vTexcoord);
	float3 getDiffuseColor(float2 vTexcoord);
};

class cSmoothLightingOnlyMaterial : cBaseMaterial
{
	float3 getAmbientColor(float2 vTexcoord)
	{
		return (float3)1.0f;
	}
	float3 getDiffuseColor(float2 vTexcoord)
	{
		return (float3)1.0f;
	}
};


class cRoughMaterial : cBaseMaterial
{
	int GetSpecularPower()
	{
		return iSpecFactor;
	}
};

class cRoughTexturedMaterial : cRoughMaterial
{
	float3 getAmbientColor(float2 vTexcoord);

	float3 getDiffuseColor(float2 vTexcoord);
};
#endif