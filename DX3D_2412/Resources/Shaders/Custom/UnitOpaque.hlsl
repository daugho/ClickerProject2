#include "../VertexHeader.hlsli"
#include "../PixelHeader.hlsli"

struct PixelInput
{
    float4 pos : SV_POSITION;
    float2 uv : UV;
};

PixelInput VS(VertexUVNormalTangentBlend input)// : SV_POSITION//SV->RS
{
    PixelInput output;
    matrix transform = mul(SkinWorld(input.indices, input.weights), world);
    output.pos = mul(input.pos, transform);
    output.pos = mul(output.pos, view);
    output.pos = mul(output.pos, projection);
    output.uv = input.uv;
    
    return output;
}

float4 PS(PixelInput input) : SV_TARGET //->SV->OM
{
    float4 result = diffuseMap.Sample(samp, input.uv) * mDiffuse;
    
    return float4(result.rgb, 1.0);
}