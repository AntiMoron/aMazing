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
