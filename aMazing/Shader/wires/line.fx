Texture2D txDiffuse : register( t0 );
SamplerState samLinear : register( s0 );

cbuffer cameraMatrices : register( b0 )
{
    matrix World;
    matrix View;
    matrix Projection;
};

cbuffer color : register(b1)
{
    float4 lineColor;
}

struct VS_INPUT
{
    float4 Pos : POSITION;
    float2 Tex : TEXCOORD0;
};

struct PS_INPUT
{
    float4 Pos : SV_POSITION;
    float2 Tex : TEXCOORD0;
};

PS_INPUT VSEntry( VS_INPUT input )
{
    PS_INPUT output = (PS_INPUT)0;

    output.Pos = input.Pos;
    output.Pos = mul( output.Pos, World );
    output.Pos = mul( output.Pos, View );
    output.Pos = mul( output.Pos, Projection );
    output.Tex = input.Tex;    
    return output;
}

float4 PSEntry(PS_INPUT input) : SV_Target
{
    return lineColor;
}
