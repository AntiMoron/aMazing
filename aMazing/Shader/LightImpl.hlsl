#include "LightPsh.hlsl"

float3 cAmbientLight::illuminateAmbient(float3 vNormal)
{
	return vLightColor * isEnable;
}

float3 cHemiAmbientLight::illuminateAmbient(float3 vNormal)
{
	float theta = (dot(vNormal, vDirUp.xyz) + 1.0f) / 2.0f;
	return lerp(groundColor, vLightColor, theta) * isEnable;
}

float3 cDirectionalLight::illuminateDiffuse(float3 vNormal)
{
	float lambert = saturate(dot(vNormal, vLightDir.xyz));
	return ((float3)lambert * vLightColor * isEnable);
}

float3 cDirectionalLight::illuminateSpecular(float3 vNormal, int specularFactor)
{
	float3 highLights = -normalize(vEyeDir.xyz) + m_vLightDir.xyz;
	float3 halfAngle = normalize(H);
	float specular = pow(max(0, dot(halfAngle, normalize(vNormal) )), specularFactor);

	return ((float3)specular * vLightColor * isEnable);
}

float3 cOmniLight::illuminateDiffuse(float3 vNormal)
{
	return (float3)0.0f;
}

float3 cEnvironmentLight::illuminateSpecular(float3 vNormal,int specularFactor)
{
	float3 N = normallize(vNormal);
	float3 E = normalize(eyeDir.xyz);
	float3 R = reflect(E, N);
	float fresnel = 1 - dot(-E, N);
	fresnel = (fresnel * fresnel * fresnel);
	float3 specular = txEnvironmentMap.Sample(sampleLinear, R).xyz * fresnel;
	return (specular*(float3)isEnable);
}