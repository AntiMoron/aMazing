#include"Buffers.hlsl"

interface iBasicLight
{
	//to illuminate ambient color factory
	float3 illuminateAmbient(float3 vNormal);
	//to illuminate diffuse color factory
	float3 illuminateDiffuse(float3 vNormal);
	//to illuminate specular color factory according to specularFactory.
	float3 illuminateSpecular(float3 vNormal, int specularFactor);
};

class cAmbientLight : iBaseLight
{
	float3 vLightColor;
	bool isEnable;

	float3 illuminateAmbient(float3 vNormal);
	float3 illuminateDiffuse(float3 vNormal)
	{
		return (float3)0;
	}
	float3 illuminateSpecular(float3 vNormal, int specularFactor)
	{
		return (float3)0;
	}
};

class cHemiAmbientLight :cAmbientLight
{
	float4 groundColor;
	float4 vDirUp;

	float3 illuminateAmbient(float3 vNormal);
};

class cDirectionalLight : cAmbientLight
{
	float4 vLightDir;

	float3 illuminateDiffuse(float3 vNormal);
	float3 illuminateSpecular(float3 vNormal, int specularFactor);
};

class cOmniLight :cAmbientLight
{
	float3 vLightPosition;
	float radius;

	float3 illuminateDiffuse(float3 vNormal);
};

class cSpotLight : cAmbientLight
{
	float3 vLightPosition;
	float3 vLightDir;
};

class cEnvironmentLight : cAmbientLight
{
	float3 illuminateSpecular(float3 vNormal, int specularFactor);
};
