Texture2D txDiffuse : register(t0);
Texture2D ProTexutre : register(t1);
Texture2D txEnvironmentMap : register(t2);

SamplerState samLinear : register(s0);
SamplerState samClamp : register(s1);

cbuffer cameraMatrices : register(b0)
{
	float3 eyeDirection;
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